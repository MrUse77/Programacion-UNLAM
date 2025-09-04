#include <stddef.h>
#include <vector.h>

void vectorCrear(Vector *v)
{
	v->cantElem = 0;
}

int vectorInsertar(Vector *v, int elem)
{
	size_t ce = v->cantElem;
	if (v->cantElem == TAM) {
		return ERR_ESPACIO;
	}
	v->vec[ce] = elem;
	v->cantElem++;
	return OK;
}

int vectorOrdInsertar(Vector *v, int elem)
{
	size_t ce = v->cantElem;
	if (ce == TAM) {
		return ERR_ESPACIO;
	}
	int *i = v->vec;
	int *ult = v->vec + (v->cantElem - 1);
	while (elem > *i && i <= ult) {
		i++;
	}
	if (elem == *i) {
		return ERR_REP;
	}
	for (int *j = ult; i <= j; j--) {
		*(j + 1) = *j;
	}

	v->cantElem++;
	*i = elem;
	return OK;
}

void vectorMostrar(const Vector *v)
{
	for (int i = 0; i < v->cantElem; i++) {
		printf("%d\n", v->vec[i]);
	}
}

int vectorOrdBuscar(const Vector *v, int elem)
{
	int *li = (int *)v->vec;
	int *ls = (int *)v->vec + (v->cantElem - 1);
	int *m;
	int comp;
	bool encontrado = false;
	int pos;

	while (li <= ls && !encontrado) {
		m = li + (ls - li) / 2;
		comp = elem - *m;
		if (comp == 0) {
			encontrado = true;
			pos = m - v->vec;
		}
		if (elem > *m) {
			li = m + 1;
		} else {
			ls = m - 1;
		}
	}
	return pos;
}

bool vectorEliminarDePosicion(Vector *v, size_t pos)
{
	if (pos < 0 || pos > v->cantElem) {
		return false;
	}
	int *i = v->vec + pos + 1;
	int *ult = v->vec + v->cantElem - 1;
	//puede no tener inicializacion
	for (; i <= ult; i++) {
		*(i - 1) = *i;
		ult--;
	}
	return true;
}

bool vectorOrdEliminar(Vector *v, int elem)
{
	int pos = vectorOrdBuscar(v, elem);
	if (!pos) {
		return false;
	}
	vectorEliminarDePosicion(v, pos);
	return true;
}
bool vectorEliminar(Vector *v, int elem)
{
	int *ult = v->vec + v->cantElem - 1;
	int *i = v->vec;
	bool eliminado = false;
	while (i < ult && !eliminado) {
		if (*i == elem) {
			for (int *j = i + 1; j <= ult; j++) {
				*(j - 1) = *j;
				ult--;
			}
			eliminado = true;
		}
		i++;
	}
	return eliminado;
}
