-- a
select distinct codpro
from ventas
where codpj='J1';

-- b
select *
from ventas
where cantidad > 100;

-- c
select p.nompie, j.nompj, s.nompro
from pieza p, proyecto j, proveedor s
where p.ciudad=j.ciudad and j.ciudad = s.ciudad;

-- d
select p.nompie
from
    (select distinct v.codpie
    from proveedor s join ventas v on (s.ciudad='Londres'
            and s.codpro=v.codpro)) natural join pieza p;

-- e
select distinct * from
(select ciudad 
from ventas natural join proveedor),
(select ciudad
from ventas natural join proyecto);

-- f
select distinct v.codpie from
(select s.codpro, j.codpj
from proveedor s natural join proyecto j)
natural join
ventas v;

-- g
select distinct codpj
from proyecto j
where exists (
    select *
    from proveedor s natural join ventas v
    where j.codpj=v.codpj and s.ciudad!=j.ciudad
);

-- h
select distinct * from
(select distinct ciudad from pieza),
(select distinct ciudad from proyecto);

-- i
(select distinct ciudad
from proveedor)
minus
(select distinct ciudad
from pieza);

-- j
(select distinct ciudad
from proveedor)
intersect
(select distinct ciudad
from pieza);

-- k
select distinct codpj
from ventas
where codpie in (
    select distinct codpie
    from ventas
    where codpro='S1'
);

-- l
select *
from ventas
where cantidad = (select min(cantidad) from ventas);

-- m
(select codpj
from  proyecto)
minus
select distinct codpj from
(select distinct codpie, codpro
from pieza, proveedor s
where color='Rojo' and s.ciudad='Londres') natural join ventas;

-- n
(select codpj
from proyecto)
minus
(select codpj
from ventas
where codpro!='S1');

-- ñ
select p.codpie
from pieza p
where not exists (
    (select j.codpj
    from proyecto j
    where j.ciudad='Paris')
    minus
    (select v.codpj
    from ventas v
    where v.codpie=p.codpie)
);

-- o
select s.codpro
from proveedor s
where exists (
    select p.codpie
    from pieza p
    where not exists (
        (select codpj from ventas)
        minus
        (select v.codpj
        from ventas v
        where v.codpie=p.codpie and v.codpro=s.codpro)
    )
);

-- p
select j.codpj
from proyecto j
where not exists (
    (select p.codpie from pieza p)
    minus
    (select v.codpie
    from ventas v
    where v.codpro='S1' and v.codpj=j.codpj)
);

-- q
select s.codpro
from proveedor s
where not exists (
    (select distinct codpie, codpj from pieza, proyecto)
    minus
    (select distinct codpie, codpj from ventas
    where codpro=s.codpro)
);