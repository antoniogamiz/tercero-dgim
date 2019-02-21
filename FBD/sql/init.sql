DROP TABLE ventas;
DROP TABLE proveedor;
DROP TABLE pieza;
DROP TABLE proyecto;

CREATE TABLE proveedor
(
    codpro char(3) constraint codpro_no_nulo not null constraint codpro_clave_primaria primary key,
    nompro varchar2(30) constraint nompro_no_nulo not null,
    status number constraint status_entre_1_y_10 check(status>=1 AND status<=10),
    ciudad varchar2(15)
);

CREATE TABLE pieza
(
    codpie char(3) constraint codpie_clave_primaria primary key,
    nompie varchar2(10) constraint nompie_no_nulo not null,
    color varchar2(10),
    peso number(5,2) constraint peso_entre_0_y_100 check(peso>0 and peso<=100),
    ciudad varchar2(15)
);

CREATE TABLE proyecto
(
    codpj char(3) constraint codpj_clave_primaria primary key,
    nompj varchar2(20) constraint nompj_no_nulo not null,
    ciudad varchar2(15)
);

CREATE TABLE ventas
(
    codpro constraint codpro_clave_externa_proveedor references proveedor(codpro),
    codpie constraint codpie_clave_externa_pieza references pieza(codpie),
    codpj constraint codpj_clave_externa_proyecto references proyecto(codpj),
    cantidad number(4),
    constraint clave_primaria primary key (codpro, codpie, codpj)
);

ALTER TABLE ventas ADD fecha date;

INSERT INTO proveedor
    (codpro, nompro, status, ciudad)
VALUES
    ('S1', 'Jose Fernandez', 2, 'Madrid');
INSERT INTO proveedor
    (codpro, nompro, status, ciudad)
VALUES
    ('S2', 'Manuel Vidal', 1, 'Londres');
INSERT INTO proveedor
    (codpro, nompro, status, ciudad)
VALUES
    ('S3', 'Luisa Gomez', 3, 'Lisboa');
INSERT INTO proveedor
    (codpro, nompro, status, ciudad)
VALUES
    ('S4', 'Pedro Sanchez', 4, 'Paris');
INSERT INTO proveedor
    (codpro, nompro, status, ciudad)
VALUES
    ('S5', 'Maria Reyes', 5, 'Roma');

INSERT INTO pieza
    (codpie, nompie, color, peso, ciudad)
VALUES
    ('P1', 'Tuerca', 'Gris', 2.5, 'Madrid');
INSERT INTO pieza
    (codpie, nompie, color, peso, ciudad)
VALUES
    ('P2', 'Tornillo', 'Rojo', 1.25, 'Paris');
INSERT INTO pieza
    (codpie, nompie, color, peso, ciudad)
VALUES
    ('P3', 'Arandela', 'Blanco', 3, 'Londres');
INSERT INTO pieza
    (codpie, nompie, color, peso, ciudad)
VALUES
    ('P4', 'Clavo', 'Gris', 5.5, 'Lisboa');
INSERT INTO pieza
    (codpie, nompie, color, peso, ciudad)
VALUES
    ('P5', 'Alcayata', 'Blanco', 10, 'Roma');

INSERT INTO proyecto
    (codpj, nompj, ciudad)
VALUES
    ('J1', 'Proyecto 1', 'Londres');
INSERT INTO proyecto
    (codpj, nompj, ciudad)
VALUES
    ('J2', 'Proyecto 2', 'Londres');
INSERT INTO proyecto
    (codpj, nompj, ciudad)
VALUES
    ('J3', 'Proyecto 3', 'Paris');
INSERT INTO proyecto
    (codpj, nompj, ciudad)
VALUES
    ('J4', 'Proyecto 4', 'Roma');

INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P1', 'J1', 150, to_date('18/09/97','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P1', 'J2', 100, to_date('06/05/96','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P1', 'J3', 500, to_date('06/05/96','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P2', 'J1', 200, to_date('22/07/95','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S2', 'P2', 'J2', 15, to_date('23/11/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S4', 'P2', 'J3', 1700, to_date('28/11/2000','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P3', 'J1', 800, to_date('22/07/95','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S5', 'P3', 'J2', 30, to_date('21/01/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P4', 'J1', 10, to_date('22/07/95','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P4', 'J3', 250, to_date('09/03/94','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S2', 'P5', 'J2', 300, to_date('23/11/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S2', 'P2', 'J1', 4500, to_date('15/08/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S3', 'P1', 'J1', 90, to_date('09/06/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S3', 'P2', 'J1', 190, to_date('12/04/02','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S3', 'P5', 'J3', 20, to_date('28/11/2000','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S4', 'P5', 'J1', 15, to_date('12/04/02','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S4', 'P3', 'J1', 100, to_date('12/04/02','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S4', 'P1', 'J3', 1500, to_date('26/01/03','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P4', 'J4', 290, to_date('09/03/94','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S1', 'P2', 'J4', 175, to_date('09/03/94','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S5', 'P1', 'J4', 400, to_date('21/01/04','dd/mm/yyyy'));
INSERT INTO ventas
    (codpro, codpie, codpj, cantidad, fecha)
VALUES
    ('S5', 'P3', 'J3', 400, to_date('21/01/04','dd/mm/yyyy'));

select *
from ventas
where cantidad = (select min(cantidad) from ventas);

select s.codpro
from proveedor s
where not exists (
    (select distinct codpie, codpj from pieza, proyecto)
    minus
    (select distinct codpie, codpj from ventas
    where codpro=s.codpro)
);