#include "../includes/protocol.h"
#include <ctype.h>

// Declaración forward de función utilitaria
void trim_whitespace(char* str);

// Función para enviar comando
int send_command(int socket_fd, command_message_t* cmd)
{
	char buffer[MAX_BUFFER_SIZE];
	int offset = 0;

	// Serializar comando
	offset += sprintf(buffer + offset, "%d|", cmd->type);
	offset += sprintf(buffer + offset, "%s|", cmd->query);
	offset += sprintf(buffer + offset, "%d|", cmd->param_count);

	for (int i = 0; i < cmd->param_count && i < 10; i++) {
		offset += sprintf(buffer + offset, "%s|", cmd->params[i]);
	}

	buffer[offset] = '\n';
	buffer[offset + 1] = '\0';

	int sent = send(socket_fd, buffer, strlen(buffer), 0);
	return sent;
}

// Función para recibir comando
int receive_command(int socket_fd, command_message_t* cmd)
{
	char buffer[MAX_BUFFER_SIZE];
	int received = recv(socket_fd, buffer, MAX_BUFFER_SIZE - 1, 0);

	if (received <= 0) {
		return received;
	}

	buffer[received] = '\0';

	// Deserializar comando
	char* token = strtok(buffer, "|");
	if (token) {
		cmd->type = atoi(token);
	}
	else {
		return -1;
	}

	token = strtok(NULL, "|");
	if (token) {
		strncpy(cmd->query, token, MAX_BUFFER_SIZE - 1);
	}

	token = strtok(NULL, "|");
	if (token) {
		cmd->param_count = atoi(token);
	}

	for (int i = 0; i < cmd->param_count && i < 10; i++) {
		token = strtok(NULL, "|");
		if (token) {
			strncpy(cmd->params[i], token, MAX_FIELD_SIZE - 1);
		}
	}

	return received;
}

// Función para enviar respuesta
int send_response(int socket_fd, response_message_t* resp)
{
	char buffer[MAX_BUFFER_SIZE * 2];
	int offset = 0;

	offset += sprintf(buffer + offset, "%d|", resp->code);
	offset += sprintf(buffer + offset, "%s|", resp->message);
	offset += sprintf(buffer + offset, "%d|", resp->row_count);
	offset += sprintf(buffer + offset, "%s", resp->data);

	buffer[offset] = '\0';
	strcat(buffer, "\r\n\r\n"); // Usar doble CRLF como terminador

	int sent = send(socket_fd, buffer, strlen(buffer), 0);
	return sent;
}

// Función para recibir respuesta
int receive_response(int socket_fd, response_message_t* resp)
{
	char buffer[MAX_BUFFER_SIZE * 2];
	int total_received = 0;
	int received = 0;
	puts("esperando respuesta...");

	// Leer hasta encontrar '\n' o llenar el buffer
	while (total_received < sizeof(buffer) - 4) {
		received = recv(socket_fd, buffer + total_received, 1, 0);
		if (received <= 0)
			break;
		total_received++;
		if (total_received >= 4 &&
				strncmp(&buffer[total_received - 4], "\r\n\r\n", 4) == 0) {
			total_received -= 4; // Remover el terminador
			break;
		}

	}
	if (total_received == 0) {
		return -1;
	}

	buffer[total_received] = '\0';

	// Deserializar respuesta
	char* token = strtok(buffer, "|");
	if (token) {
		resp->code = atoi(token);
	}
	else {
		return -1;
	}

	token = strtok(NULL, "|");
	if (token) {
		strncpy(resp->message, token, MAX_BUFFER_SIZE - 1);
		resp->message[MAX_BUFFER_SIZE - 1] = '\0';
	}

	token = strtok(NULL, "|");
	if (token) {
		resp->row_count = atoi(token);
	}

	token = strtok(NULL, "");
	if (token) {
		strncpy(resp->data, token, MAX_BUFFER_SIZE - 1);
		resp->data[MAX_BUFFER_SIZE - 1] = '\0';
	}

	return total_received;
}

// Función para parsear consulta SELECT
int parse_select_query(const char* query, char* table, char* conditions,
											 char* fields)
{
	char query_copy[MAX_BUFFER_SIZE];
	strncpy(query_copy, query, MAX_BUFFER_SIZE - 1);

	// Convertir a mayúsculas para parsing
	char* upper_query = query_copy;
	for (int i = 0; upper_query[i]; i++) {
		upper_query[i] = toupper(upper_query[i]);
	}

	// Buscar SELECT
	char* select_pos = strstr(upper_query, "SELECT");
	if (!select_pos)
		return -1;

	// Buscar campos (después de SELECT)
	char* fields_start = select_pos + 6;
	while (*fields_start == ' ')
		fields_start++;

	char* where_pos = strstr(upper_query, "WHERE");
	char* fields_end;

	if (where_pos) {
		fields_end = where_pos - 1;
		while (*fields_end == ' ')
			fields_end--;
		fields_end++;
	}
	else {
		fields_end = upper_query + strlen(upper_query);
	}

	// Extraer campos
	int fields_len = fields_end - fields_start;
	if (fields_len > 0) {
		strncpy(fields, fields_start, fields_len);
		fields[fields_len] = '\0';
		trim_whitespace(fields);
	}
	else {
		strcpy(fields, "*");
	}

	// Extraer condiciones (después de WHERE)
	if (where_pos) {
		char* conditions_start = where_pos + 5;
		while (*conditions_start == ' ')
			conditions_start++;
		strcpy(conditions, conditions_start);
		trim_whitespace(conditions);
	}
	else {
		conditions[0] = '\0';
	}

	// Para este ejemplo, asumimos una sola tabla
	strcpy(table, "main");

	return 0;
}

// Función para parsear consulta INSERT
int parse_insert_query(const char* query, char* table,
											 char values[][MAX_FIELD_SIZE], int* value_count)
{
	char query_copy[MAX_BUFFER_SIZE];
	strncpy(query_copy, query, MAX_BUFFER_SIZE - 1);

	// Buscar VALUES
	char* values_pos = strstr(query_copy, "VALUES");
	if (!values_pos) {
		values_pos = strstr(query_copy, "values");
	}
	if (!values_pos)
		return -1;

	// Buscar paréntesis
	char* open_paren = strchr(values_pos, '(');
	if (!open_paren)
		return -1;

	char* close_paren = strchr(open_paren, ')');
	if (!close_paren)
		return -1;

	// Extraer valores entre paréntesis
	char values_str[MAX_BUFFER_SIZE];
	int len = close_paren - open_paren - 1;
	strncpy(values_str, open_paren + 1, len);
	values_str[len] = '\0';

	// Parsear valores separados por coma
	*value_count = 0;
	char* token = strtok(values_str, ",");

	while (token && *value_count < MAX_FIELDS) {
		trim_whitespace(token);

		// Remover comillas si las hay
		if (token[0] == '"' || token[0] == '\'') {
			int token_len = strlen(token);
			if (token[token_len - 1] == token[0]) {
				token[token_len - 1] = '\0';
				token++;
			}
		}

		strncpy(values[*value_count], token, MAX_FIELD_SIZE - 1);
		(*value_count)++;
		token = strtok(NULL, ",");
	}

	strcpy(table, "main");
	return 0;
}

// Función para parsear consulta UPDATE
int parse_update_query(const char* query, char* table, char* set_clause,
											 char* conditions)
{
	char query_copy[MAX_BUFFER_SIZE];
	strncpy(query_copy, query, MAX_BUFFER_SIZE - 1);

	// Convertir a mayúsculas para buscar keywords
	char upper_query[MAX_BUFFER_SIZE];
	strcpy(upper_query, query_copy);
	for (int i = 0; upper_query[i]; i++) {
		upper_query[i] = toupper(upper_query[i]);
	}

	// Buscar SET
	char* set_pos = strstr(upper_query, "SET");
	if (!set_pos)
		return -1;

	// Buscar WHERE
	char* where_pos = strstr(upper_query, "WHERE");

	// Calcular posición en query original
	int set_offset = set_pos - upper_query;
	char* set_start = query_copy + set_offset + 3;
	while (*set_start == ' ')
		set_start++;

	if (where_pos) {
		int where_offset = where_pos - upper_query;
		char* set_end = query_copy + where_offset - 1;
		while (*set_end == ' ')
			set_end--;
		set_end++;

		int set_len = set_end - set_start;
		strncpy(set_clause, set_start, set_len);
		set_clause[set_len] = '\0';

		char* conditions_start = query_copy + where_offset + 5;
		while (*conditions_start == ' ')
			conditions_start++;
		strcpy(conditions, conditions_start);
	}
	else {
		strcpy(set_clause, set_start);
		conditions[0] = '\0';
	}

	trim_whitespace(set_clause);
	trim_whitespace(conditions);

	strcpy(table, "main");
	return 0;
}

// Función para parsear consulta DELETE
int parse_delete_query(const char* query, char* table, char* conditions)
{
	char query_copy[MAX_BUFFER_SIZE];
	strncpy(query_copy, query, MAX_BUFFER_SIZE - 1);

	// Convertir a mayúsculas para buscar WHERE
	char upper_query[MAX_BUFFER_SIZE];
	strcpy(upper_query, query_copy);
	for (int i = 0; upper_query[i]; i++) {
		upper_query[i] = toupper(upper_query[i]);
	}

	char* where_pos = strstr(upper_query, "WHERE");
	if (where_pos) {
		int where_offset = where_pos - upper_query;
		char* conditions_start = query_copy + where_offset + 5;
		while (*conditions_start == ' ')
			conditions_start++;
		strcpy(conditions, conditions_start);
		trim_whitespace(conditions);
	}
	else {
		conditions[0] = '\0';
	}

	strcpy(table, "main");
	return 0;
}

// Función para validar comando
int validate_command(command_message_t* cmd)
{
	if (cmd->type < CMD_SELECT || cmd->type > CMD_PING) {
		return -1;
	}

	switch (cmd->type) {
	case CMD_SELECT:
	case CMD_INSERT:
	case CMD_UPDATE:
	case CMD_DELETE:
		if (strlen(cmd->query) == 0) {
			return -1;
		}
		break;
	default:
		break;
	}

	return 0;
}

// Función para formatear salida de registro
void format_record_output(db_record_t* record, database_t* db, char* output,
													int max_len)
{
	int offset = 0;

	for (int i = 0; i < db->num_fields && offset < max_len - 50; i++) {
		if (i > 0) {
			offset += snprintf(output + offset, max_len - offset,
												 " | ");
		}
		offset += snprintf(output + offset, max_len - offset, "%-15s",
											 record->fields[i]);
	}

	if (offset < max_len - 2) {
		strcat(output + offset, "\n");
	}
}

// Función para formatear encabezado de tabla
void format_table_header(database_t* db, char* output, int max_len)
{
	int offset = 0;

	for (int i = 0; i < db->num_fields && offset < max_len - 50; i++) {
		if (i > 0) {
			offset += snprintf(output + offset, max_len - offset,
												 " | ");
		}
		offset += snprintf(output + offset, max_len - offset, "%-15s",
											 db->headers[i]);
	}

	if (offset < max_len - 2) {
		strcat(output + offset, "\n");
		offset = strlen(output);
	}

	// Agregar línea separadora
	for (int i = 0; i < db->num_fields && offset < max_len - 50; i++) {
		if (i > 0) {
			offset += snprintf(output + offset, max_len - offset,
												 "-+-");
		}
		offset += snprintf(output + offset, max_len - offset,
											 "---------------");
	}

	if (offset < max_len - 2) {
		strcat(output + offset, "\n");
	}
}

// Función para obtener nombre de comando
const char* get_command_name(command_type_t type)
{
	switch (type) {
	case CMD_SELECT:
		return "SELECT";
	case CMD_INSERT:
		return "INSERT";
	case CMD_UPDATE:
		return "UPDATE";
	case CMD_DELETE:
		return "DELETE";
	case CMD_BEGIN_TRANSACTION:
		return "BEGIN TRANSACTION";
	case CMD_COMMIT_TRANSACTION:
		return "COMMIT TRANSACTION";
	case CMD_ROLLBACK_TRANSACTION:
		return "ROLLBACK TRANSACTION";
	case CMD_DESCRIBE:
		return "DESCRIBE";
	case CMD_HELP:
		return "HELP";
	case CMD_QUIT:
		return "QUIT";
	case CMD_PING:
		return "PING";
	default:
		return "UNKNOWN";
	}
}

// Función para obtener mensaje de respuesta
const char* get_response_message(response_code_t code)
{
	switch (code) {
	case RESP_OK:
		return "OK";
	case RESP_ERROR:
		return "Error";
	case RESP_NOT_FOUND:
		return "Not Found";
	case RESP_CONFLICT:
		return "Conflict";
	case RESP_LOCKED:
		return "Locked";
	case RESP_SERVER_ERROR:
		return "Server Error";
	default:
		return "Unknown";
	}
}

// Función utilitaria para limpiar espacios
void trim_whitespace(char* str)
{
	if (!str)
		return;

	// Limpiar espacios al inicio
	char* start = str;
	while (*start == ' ' || *start == '\t' || *start == '\n' ||
				 *start == '\r') {
		start++;
	}

	// Mover el contenido al inicio
	if (start != str) {
		memmove(str, start, strlen(start) + 1);
	}

	// Limpiar espacios al final
	int len = strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' ||
										 str[len - 1] == '\n' || str[len - 1] == '\r')) {
		str[len - 1] = '\0';
		len--;
	}
}
