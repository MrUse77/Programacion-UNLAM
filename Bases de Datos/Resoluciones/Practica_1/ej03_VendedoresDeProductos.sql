 
-- consultas

USE ej3_vendedoresDeProductos;
-- 1
select count(*) as cantidadProductos from producto;

-- 2
select count(*) as cantidadProductosEnStock from producto p
where p.estado = 'stock';

-- 3
select p.id_producto as productosNoVendidos from producto p
where not exists(
	select distinct dv.id_producto from detalle_venta dv
);

-- en mysql la diferencia debe hacerse con left outer join
select p.id_producto as productosNoVendidos from producto p
left outer join detalle_venta dv
on p.id_producto = dv.id_producto
where dv.nro_factura is null;

-- 4
select dv.id_producto, sum(dv.cantidad) as unidadesVendidas
from detalle_venta dv
group by dv.id_producto;

-- 5
select dv.id_producto, avg(dv.cantidad) as promedioUnidadesVendidas
from detalle_venta dv
group by dv.id_producto;

-- 6
create view ventasPorVendedor
as
	select v.id_vendedor, count(*) as cantidadVentas
    from venta v
    group by v.id_vendedor;
    
select vv.id_vendedor as vendedorConMasVentas from ventasPorVendedor vv
where vv.cantidadVentas = (
	select max(v.cantidadVentas) from ventasPorVendedor v
);

-- 7
select dv.id_producto
from detalle_venta dv
group by dv.id_producto
having sum(dv.cantidad) > 15000;

-- 8
create view unidadesVendidasPorVendedor
as
	select v.id_vendedor, sum(dv.cantidad) as unidadesVendidas
    from venta v
    inner join detalle_venta dv on v.nro_factura = dv.nro_factura
    group by v.id_vendedor;
    
select vv.id_vendedor as vendedorConMasUnidadesVendidas
from unidadesVendidasPorVendedor vv
where vv.unidadesVendidas = (
	select max(v.unidadesVendidas) from unidadesVendidasPorVendedor v
);

