use Ejercicios;

-- crear esquema
create SCHEMA EJ2;


-- crear tablas
create table EJ2.proveedor
(
	nroProv int not null,
	nomProv varchar(50) not null,
	categoria int not null,
	ciudadProv varchar(50) not null,
	primary key(nroProv)
);

create table EJ2.articulo
(
	nroArt int not null,
	descripcion varchar(50) not null,
	ciudadArt varchar(50) not null,
	precio float not null,
	primary key(nroArt)
);

create table EJ2.cliente
(
	nroCli int not null,
	nomCli varchar(50) not null,
	ciudadCli varchar(50) not null,
	primary key(nroCli)
);

create table EJ2.pedido
(
	nroPed int not null,
	nroArt int not null,
	nroCli int not null,
	nroProv int not null,
	fechaPedido date not null,
	cantidad int not null,
	precioTotal float not null,
	primary key(nroPed),
	foreign key(nroArt) references articulo(nroArt),
	foreign key(nroCli) references cliente(nroCli),
	foreign key(nroProv) references proveedor(nroProv)
);

create table EJ2.stock
(
	nroArt int not null,
	fecha date not null,
	cantidad int not null,
	primary key(nroArt)
);


--Datos de ejemplo
INSERT INTO EJ2.proveedor (nroProv, nomProv, categoria, ciudadProv) VALUES
(17, 'Proveedor P017', 1, 'Ciudad A'),
(18, 'Proveedor B', 4, 'Ciudad B'),
(19, 'Proveedor C', 5, 'Ciudad C');

INSERT INTO EJ2.articulo (nroArt, descripcion, ciudadArt, precio) VALUES
(140, 'Artículo A140', 'Mendoza', 10.0),
(150, 'Artículo B150', 'Ciudad B', 20.0),
(10, 'Artículo C10', 'Mendoza', 30.0);

INSERT INTO EJ2.cliente (nroCli, nomCli, ciudadCli) VALUES
(4, 'Cliente z', 'Rosario'),
(5, 'Cliente D', 'Mendoza'),
(6, 'Cliente E', 'Rosario'),
(7, 'Cliente F', 'Mendoza');

INSERT INTO EJ2.pedido (nroPed, nroArt, nroCli, nroProv, fechaPedido, cantidad, precioTotal) VALUES
(13,140, 4, 19,'2023-10-01', 5, 50.0),
(14, 10, 5, 19, '2023-10-03', 2, 90.0);

INSERT INTO EJ2.stock (nroArt, fecha, cantidad) VALUES
(1, '2023-10-01', 100),
(2, '2023-10-02', 200),
(3, '2023-10-03', 300);



-- Ejercicios
---- 1. Hallar el código (nroProv) de los proveedores que proveen el artículo a146
SELECT p.nroProv from EJ2.proveedor p where p.nroProv in (
    SELECT pe.nroProv FROM EJ2.pedido pe
    INNER JOIN EJ2.articulo a ON pe.nroArt = a.nroArt
    WHERE a.nroArt = 146
);

---- 2. Hallar los clientes (nomCli) que solicitan artículos provistos por p015.
select c.nomCli from EJ2.cliente c where c.nroCli in (
    SELECT pe.nroCli FROM EJ2.pedido pe
    INNER JOIN EJ2.proveedor p ON pe.nroProv = p.nroProv
    WHERE p.nroProv = 15
);

---- 3. Hallar los clientes que solicitan algún item provisto por proveedores con categoría mayor que 4
select * from EJ2.cliente c where c.nroCli in (
    SELECT pe.nroCli FROM EJ2.pedido pe
    INNER JOIN EJ2.proveedor p ON pe.nroProv = p.nroProv
    WHERE p.categoria > 4
);

---- 4. Hallar los pedidos en los que un cliente de Rosario solicita artículos producidos en la ciudad de Mendoza.
select * from EJ2.pedido pe where pe.nroPed in (
    SELECT pe.nroPed FROM EJ2.pedido pe
    INNER JOIN EJ2.cliente c ON pe.nroCli = c.nroCli
    INNER JOIN EJ2.articulo a ON pe.nroArt = a.nroArt
    WHERE c.ciudadCli = 'Rosario' AND a.ciudadArt = 'Mendoza'
);

---- 5. Hallar los pedidos en los que el cliente c23 solicita artículos solicitados por el cliente c30.
select * from EJ2.cliente c where c.nroCli in (
    SELECT pe.nroCli FROM EJ2.pedido pe
    WHERE pe.nroCli = 23 AND pe.nroCli IN (
	SELECT pe2.nroCli FROM EJ2.pedido pe2
	WHERE pe2.nroCli = 30
    )
);

---- 6. Hallar los proveedores que suministran todos los artículos cuyo precio es superior al precio promedio de los artículos que se producen en La Plata.
select * from EJ2.proveedor p where p.nroProv in(
	select p.nroProv from EJ2.proveedor p
	where not exists (
		select 1 from EJ2.articulo a
		where a.precio > (select avg(precio) from EJ2.articulo where ciudadArt = 'La Plata')
		and a.nroArt not in (
			select pe.nroArt from EJ2.pedido pe where pe.nroProv = p.nroProv
		)
	)
		
)
---- 7. Hallar la cantidad de artículos diferentes provistos por cada proveedor que provee a todos los clientes de Junín.
select p.nroProv, count(distinct a.nroArt) as CantidadArticulos from EJ2.proveedor p
where p.nroProv in (
	select pe.nroProv from EJ2.pedido pe
	inner join EJ2.cliente c on pe.nroCli = c.nroCli
	where c.ciudadCli = 'Junín'
	group by pe.nroProv
	having count(distinct pe.nroCli) = (select count(*) from EJ2.cliente where ciudadCli = 'Junín')
)

---- 8. Hallar los nombres de los proveedores cuya categoría sea mayor que la de todos los proveedores que proveen el artículo “cuaderno”.
select p.nomProv from EJ2.proveedor p where p.categoria > (
	select max(p2.categoria) from EJ2.proveedor p2 where p2.nroProv in (
		select pe.nroProv from EJ2.pedido pe inner join EJ2.articulo a on pe.nroArt = a.nroArt
		where a.descripcion = 'cuaderno'
	)
);

---- 9. Hallar los proveedores que han provisto más de 1000 unidades entre los artículos A001 y A100.
select * from EJ2.proveedor p where p.nroProv in (
	select pe.nroProv from EJ2.pedido pe
	inner join EJ2.articulo a on pe.nroArt = a.nroArt
	where a.nroArt between 1 and 100
	group by pe.nroProv
	having sum(pe.cantidad) > 1000
);

---- 10 Listar la cantidad y el precio total de cada artículo que han pedido los Clientes a sus proveedores entre las fechas 01-01-2004 y 31-03-2004 (se requiere visualizar Cliente, Articulo, Proveedor, Cantidad y Precio)
SELECT c.nomCli AS Cliente, a.descripcion AS Articulo, p.nomProv AS Proveedor, COUNT(*) AS Cantidad, SUM(a.precio) AS PrecioTotal FROM  EJ2.pedido pe INNER JOIN EJ2.cliente c ON pe.nroCli = c.nroCli
	INNER JOIN EJ2.articulo a ON pe.nroArt = a.nroArt
	INNER JOIN EJ2.proveedor p ON pe.nroProv = p.nroProv
	WHERE pe.fechaPedido BETWEEN '2004-01-01' AND '2004-03-31'
	GROUP BY c.nomCli, a.descripcion, p.nomProv
	ORDER BY c.nomCli, a.descripcion;

---- 11. Idem anterior y que además la Cantidad sea mayor o igual a 1000 o el Precio sea mayor a $ 1000.
SELECT c.nomCli AS Cliente, a.descripcion AS Articulo, p.nomProv AS Proveedor, COUNT(*) AS Cantidad, SUM(a.precio) AS PrecioTotal FROM  EJ2.pedido pe 
	INNER JOIN EJ2.cliente c ON pe.nroCli = c.nroCli
	INNER JOIN EJ2.articulo a ON pe.nroArt = a.nroArt
	INNER JOIN EJ2.proveedor p ON pe.nroProv = p.nroProv
	WHERE (pe.fechaPedido BETWEEN '2004-01-01' AND '2004-03-31')
	GROUP BY c.nomCli, a.descripcion, p.nomProv
	HAVING sum(a.precio) >1000 or count(*) >= 1000
	ORDER BY c.nomCli, a.descripcion;

---- 12. Listar la descripción de los artículos en donde se hayan pedido en el día más del stock existente para ese mismo día.
select a.descripcion from EJ2.articulo a where a.NroArt in(
	select pe.NroArt from EJ2.pedido pe
	inner join EJ2.stock s on pe.NroArt = s.NroArt and date(pe.fechaPedido) = date(s.fecha)
	group by pe.NroArt, date(pe.fechaPedido)
	having sum(pe.cantidad) > max(s.cantidad)
)

---- 13. Listar los datos de los proveedores que hayan pedido de todos los artículos en un mismo día. Verificar sólo en el último mes de pedidos.
select * from EJ2.proveedores p where p.nroProv in (
	select pe.nroProv from EJ2.pedido pe 
	where pe.fechaPedido >=(
		select date_sub(max(fechaPedido), interval 1 month)
		from EJ2.pedido
	)
	group by pe.nroProv, date(pe.fechaPedido)
	having count(distinct pe.nroArt) = (select count(*)from EJ2.articulo)
)

---- 14. Listar los proveedores a los cuales no se les haya solicitado ningún artículo en el último mes, pero sí se les haya pedido en el mismo mes del año anterior.
select * from EJ2.proveedor p where p.nroProv in (
	select pe.nroProv from EJ2.pedido pe
	where pe.fechaPedido >= date_sub(getdate(), interval 1 month)
	group by pe.nroProv
	having count(*) = 0
) and p.nroProv in (
	select pe2.nroProv from EJ2.pedido pe2
	where pe2.fechaPedido >= date_sub(getdate(), interval 1 year) and pe2.fechaPedido < date_sub(gedate(), interval 1 month)
	group by pe2.nroProv
	having count(*) > 0
);
