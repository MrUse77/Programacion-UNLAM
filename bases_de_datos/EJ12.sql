use Ejercicios;

create schema EJ12;


-- crear tablas
create table EJ12.proveedor
(
	codProv int not null,
	razonSocial varchar(20) not null,
	fechaInicio date not null,
	primary key(codProv)
);

create table EJ12.producto
(
	codProv int not null,
	descripcion varchar(30) not null,
	codProd int not null,
	stockActual int not null,
	primary key(codProd),
	foreign key(codProv) references EJ12.proveedor(codProv)
);

create table EJ12.stock
(
	nro int not null,
	fecha date not null,
	codProd int not null,
	cantidad int not null,
	primary key(nro, fecha, codProd),
	foreign key(codProd) references EJ12.producto(codProd)
);

---- 1. p_EliminaSinstock(): Realizar un procedimiento que elimine los productos que no poseen stock.
create procedure EJ12.p_ElimiarSinStock
AS
BEGIN TRY
	BEGIN TRANSACTION
		delete from EJ12.producto where stockActual = 0
	COMMIT
END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 2.  p_ActualizaStock(): Para los casos que se presenten inconvenientes en los datos, se necesita realizar un procedimiento que permita actualizar todos los
----     Stock_Actual de los productos, tomando los datos de la entidad Stock. Para ello, se utilizará como stock válido la última fecha en la cual se haya cargado el stock
create view EJ12.utltimaActualizacion
as 
	select s.codProd, max(s.fecha) as ultimaFecha
	from EJ12.stock s
	group by s.codProd

create procedure EJ12.p_ActualizaStock
AS
BEGIN TRY
	BEGIN TRANSACTION
		update EJ12.prodcuto p 
		set p.stockActual = s.cantidad	
		from EJ12.producto p
		inner join EJ12.stock s
		on p.codProd = s.codProd
		inner join EJ12.utltimaActualizacion ua on s.codProd = ua.codProd and s.fecha = ua.ultimaFecha
	COMMIT

END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 3. p_DepuraProveedor(): Realizar un procedimiento que permita depurar todos los proveedores de los cuales no se posea stock de ningún producto provisto desde hace más de 1 año.

create procedure EJ12.p_DepuraProveedor
AS
BEGIN TRY
	BEGIN TRANSACTION
		select distinct prov.codProv into #depurar
		from EJ12.proveedor prov
		inner join EJ12.producto p on prov.codProv = p.codProv
		inner join EJ12.stock s on p.codProd = s.codProd
		inner join EJ12.utltimaActualizacion ua on s.codProd = ua.codProd and s.fecha = ua.ultimaFecha
		where s.cantidad = 0 and s.fecha < dateAdd(year,-1,getDate());

		delete from EJ12.producto where codProv exists(
			select pad.codProv from #depurar pad
		);
		delete from EJ12.proveedor where codProv exists(
			select pad.codProv from #depurar pad
		);
	COMMIT

END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 4. p_InsertStock(nro,fecha,prod,cantidad): Realizar un procedimiento que permita agregar stocks de productos. Al realizar la inserción se deberá validar que:
---- a. El producto debe ser un producto existente
---- b. La cantidad de stock del producto debe ser cualquier número entero mayor a cero.
---- c. El número de stock será un valor correlativo que se irá agregando por cada nuevo stock de producto
create procedure EJ12.p_InsertStock(@nro in, @fecha date, @codProd int, @cantidad int)
AS
BEGIN TRY
	BEGIN TRANSACTION
		if exists(select p.codProd from EJ12.producto p where p.codProd = @codProd)
			if @cantidad > 0
				if @nro = ((select max(s.nro) from EJ12.stock s where s.stock = @codProd)+1)
					insert int EJ12.stock(nro, fecha, codProd, cantidad) values(@nro,@fecha,@codProd,@cantidad)
				else
					print 'Error'
			else print 'Error'
		else print 'Error'

	COMMIT
END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 5. tg_CrearStock: Realizar un trigger que permita automáticamente agregar un registro en la entidad Stock, cada vez que se inserte un nuevo producto. El stock inicial a tomar, será el valor del campo Stock_Actual.
create trigger E12.tg_CrearStock
on E12.producto after insert
	as
	begin 
		declare @nro int 
		set @nro = (select p.codProd from EJ12.producto p where p.codProd = @codProd) +1,
		insert into EJ12.stock(nro,fecha,codProd,cantidad) values(@nro,getDate(),inserted.codProd,inserted.stockActual)
	end;

---- 6. p_ListaSinStock(): Crear un procedimiento que permita listar los productos que no posean stock en este momento y que no haya ingresado ninguno en este último mes. De estos productos, listar el código y nombre del producto, razón social del proveedor y stock que se tenía al mes anterior
create procedure EJ12.p_ListaSinStock
AS
BEGIN TRY
	BEGIN TRANSACTION
		select s.codProd, prod.descripcion, prov.razonSocial, s.cantidad
		from EJ12.stock s
		inner join(
			select distinct s.codProd, max(s.fecha) as ultimaFecha
			from EJ12.stock s
			inner join (
				select distinct s.codProd
				from EJ12.stock
				where s.cantidad=0 and s.fecha<dateAdd(month,-1,getDate())
			)ss
			on s.codProd = ss.codProd
			where s.cantidad > 0
			group by s.codProd
		)t
		on s.codProd = t.codProd and s.fecha = t.ultimaFecha
		inner join EJ12.producto prod
		on s.codProd = prod.codProd
		inner join EJ12.proveedor prov
		on s.codProv = prov.codProv
	COMMIT

END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 7. p_ListaStock(): Realizar un procedimiento que permita generar el siguiente reporte:
/*
 * En este listado se observa que se contará la cantidad de productos que posean a una 
 * determinada fecha más de 1000 unidades, menos de 1000 unidades o que no 
 * existan unidades de ese producto.
 * Según el ejemplo, el 01/08/2009 existen 100 productos que poseen más de 1000 
 * unidades, en cambio el 03/08/2009 sólo hubo 53 productos con más de 1000 
 * unidades 
 */
create procedure EJ12.p_ListaStock
AS
BEGIN TRY
	BEGIN TRANSACTION
		select	s.fecha, 
			(select count(distinct s.codProd) from EJ12.stock ss where ss.cantidad > 1000) as '>1000'	
			(select count(distinct s.codProd) from EJ12.stock ss where ss.cantidad < 1000) as '<1000'
			(select count(distinct s.codProd) from EJ12.stock ss where ss.cantidad = 0) as '=0'
		from EJ12.stock s
		group by s.fecha
	COMMIT
END TRY
BEGIN CATCH
	ROLLBACK
END CATCH

---- 8. El siguiente requerimiento consiste en actualizar el campo stock actual de la entidad producto, 
----    cada vez que se altere una cantidad (positiva o negativa) de ese
----    producto. El stock actual reflejará el stock que exista del producto, sabiendo que
----    en la entidad Stock se almacenará la cantidad que ingrese o egrese. Además, se
----    debe impedir que el campo “Stock actual” pueda ser actualizado manualmente. Si
----    esto sucede, se deberá dar marcha atrás a la operación indicando que no está
----    permitido.
create trigger tg_ActualizarStockActual
on EJ12.stock after insert
	as
	begin
		execute p_ActualizaStockActual inserted.codProd,inserted.cantidad;
	end;

create procedure EJ12.p_ActualizaStockActual(@codProd int, @cantidad int)
AS
BEGIN TRY
	BEGIN TRANSACTION
		update EJ12.producto
		set stockActual = stockActual + @cantidad 
		where codProd = @codProd

	COMMIT
END TRY
BEGIN CATCH
	ROLLBACK
END CATCH
