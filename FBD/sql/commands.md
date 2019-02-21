### Commands

- `describe nombre-tabla;`
- `DBA_TABLES`,`ALL_TABLES`,`USER_TABLES`.
- `SYSDATE` => la fecha en oracla se representa en segundos desde una determinada fecha, pero podemos sumar y restar valores a sysdate en numero de dias. => `TO_DATE('date', 'format');` y `TO_CHAR('date', 'format');`.
- Ejemplo: `SELECT TO_CHAR(fechaalta, 'dd-mon-yyy') FROM plantilla`.
- Creación de tablas:

```
CREATE TABLE nombre-tabla(
    nombre-atributo1 tipo-atributo1 [DEFAULT expr],
    nombre-atributo2 tipo-atributo2 [DEFAULT expr],
    [PRIMARY KEY (nombre-atributo1,...),]
    [UNIQUE (nombre-atributo1,...),]
    [FOREIGN KEY (nombre-atributo1,...) REFERENCES nombre-tabla(nombre-atributo,...),]
    [CHECK(condicion)]

);
```

Si la clave primaria, candidata o externa está formada por un solo atributo, las palabras reservadas _PRIMARY KEY_, _UNIQUE_ y _REFERENCES_, se podrán incluir a continuación de la definición del atributo correspondiente (donde pone _[DEFAULT expr]_).

```
CREATE TABLE serjefe (
    dnijefe REFERENCES plantilla(dni),
    dnitrabajador REFERENCES plantilla(dni),
    PRIMARY KEY (dnitrabajador)
);
```

- `ALTER TABLE nombre-tabla ADD ( attr tipo [constraint f not null] );`
- `CHECK (element IN ('element1', 'element2', 'element3'))`
- Operadores: AND, OR, NOT, BETWEEN (attr1 BETWEEN 5 AND 10), IN.
- `DROP TABLE nombre-tabla`
- `INSERT INTO nombre-tabla (column1, column2, ...) VALUES(valor1, valor2, ...);`
- `INSERT INTO nombre-tabla (c1, ...) VALUES (SELECT c1, ... FROM nombre-tabla2);`
- `SELECT * FROM nombre-tabla;`
- `SELECT campo1, campo2, ... FROM nombre-tabla;`
- `SELECT table_name FROM user_tables;`
- `UPDATE nombre_tabla SET attr1= 'nuevo valor', ... [WHERE <condition>];`

```
UPDATE plantilla
SET estadocivil = 'divorciado'
WHERE nombre = 'Juan';
```

- `DELETE [FROM] nombre_tabla [WHERE <condition>];`
- `DELETE FROM tabla2` => borrar todas las tuplas de tabla2.

#### Select

```
SELECT [ DISTINCT | ALL]
    expresion [alias_columna_expresion]
    {,expresion [alias_columna_expresion]}
FROM [esquema.]tabla|vista [alias_tabla_vista]
[WHERE <condicion>]
[GROUP BY expresion {,expresion}]
[{UNION | UNION ALL | INTERSECT | MINUS} <SELECT instruccion>]
[HAVING <condicion>]
[ORDER BY {expresion} [ASC | DESC]]
```

- Operadores: `_` y `%`. (attr like 'algo'), `attr IS [NOT] NULL`
- `pi_ciudad(proyecto)` => `SELECT [distinct] ciudad FROM proyecto`
- seleccion => `SELECT codpro FROM ventas where codpj='J1'`

- Ejercicio 3.3: `select codpie from pieza where (ciudad='Madrid' and (color='Gris' OR color='Rojo'));`
- Ejercicio 3.4: `select codpro, codpie, codpj from ventas where (cantidad>=200 AND cantidad<=300);`

```
<SELECT instruccion>
    UNION | UNION ALL | INTERSECT | MINUS
<SELECT instruccion>
```

Ejemplo:

```
(select distinct ciudad from proveedor where status>2)
    MINUS
(select distinct ciudad from pieza where codpie=’P1’);
```

- Importante: los tipos deben de coincidir antes de aplicar estos operandos.
- Todos los operandos menos `UNION ALL` eliminan las tuplas repetidas.

- Ejercicio 3.8:

```
(select DISTINCT codpro from ventas where codpro='S1')
MINUS
(select DISTINCT codpro from ventas where codpro='S1');
```

##### Producto cartesiano:

`from` lo que hace es el producto cartesiano y deja las tuplas que cumplen la condicion `where`.

```
select codpro, codpie, codpj
from proveedor, proyecto, pieza
where Proveedor.ciudad=Proyecto.ciudad
and Proyecto.ciudad=Pieza.ciudad;


select codpro,codpie,codpj
from proveedor, proyecto, pieza
where Proveedor.ciudad=’Londres’ and Proyecto.ciudad=’Londres’
and Pieza.ciudad=’Londres’;
```

- Ejercicio 3.12:

```
(select codpro, codpie codpj from proveedor, proyecto, pieza where Proveedor.ciudad=Proyecto.ciudad and Proyecto.ciudad=Pieza.ciudad)
minus
(select codpro, codpie, codpj from ventas);
```

##### El renombramiento o alias en SQL

Los alias se definen asociándolos a aquellas tablas o consultas presentes en la cláusula FROM.

```
SELECT codpro, codpie, codpj
FROM proveedor S, ciudad Y, pieza P
where S.ciudad=Y.ciudad and Y.ciudad=P.ciudad;
```

- Ejercicio 3.13:

```
SELECT proveedor.codpro, pro.codpro
FROM proveedor, proveedor pro
where proveedor.ciudad!=pro.ciudad;
```

- Ejercicio 3.14:

```
SELECT codpie
FROM pieza, pieza p
where pieza.peso > p.peso;
```

##### Selección natural

```
select nompro, cantidad
from proveedor natural join (select * from ventas where cantidad>800);
```

- Equireunion:

  ```
  select nompro, cantidad
  from proveedor s JOIN (select * from ventas where cantidad>800) v
  ON (s.codpro=v.codpro);
  ```

- Ejercicio 3.15:

```
select codpie
from (select codpro from proveedor where ciudad='Madrid') natural join (select codpie, codpro from ventas);
```

- Ejercicio 3.16:

```
select codpie, ciudad
from ((select codpro, codpj from proveedor, proyecto where proveedor.ciudad=proyecto.ciudad) natural join ventas) natural join pieza
```

##### Ordenación de resultados

Clasulas ORDER BY. Su valor por defecto es ASC.

```
SELECT [DISTINCT | ALL] expresion [alias_columna_expresion]
{,expresion [alias_columna_expresion]}
FROM [esquema.]tabla|vista [alias_tabla_vista]
[WHERE <condicion>]
ORDER BY expresion [ASC | DESC]{,expresion [ASC | DESC]}
```

Ejemplo:

```
select nompro
from proveedor
order by nompro;
```

- Ejercicio 3.18: (no sé si está bien):

```
:/
```

##### Subconsultas en SQL

Existen en SQL distintos operadores que permiten operar sobre el resutlado de una consulta, esto se hace incorporando una subconsulta en la cláusula WHERE de la consulta principal. La razón de proceder de esta forma es que se fragmenta la consulta original en varias subconsultas más sencillas, evitando en muchas ocasiones numerosas reuniones.

```
select <expresion>
from tabla
where <expresion> OPERADOR <select instruccion>
```

###### Operadores para subconsultas:

- Operador IN:

```
select codpie
from ventas
where codpro IN (select codpro from proveedor where ciudad='Londres');
```

- Ejercicio 3.19:

```
select codpie
from ventas
where codpro IN (select codpro from proveedor where ciudad='Madrid');
```

- Ejercicio 3.20:

```
select codpj
from proyecto
where ciudad IN (select ciudad from proveedor);
```

- Ejercicio 3.21:

```
(select codpj from proyecto)
MINUS
(select codpj from venta
where codpie IN (select codpie from pieza where color='Roja')
and codpro IN (select codpro from proveedor where ciudad='Londres+'))
```

##### EXISTS, el operador de comprobación de existencia.

```
select codpro
from proveedor
where EXISTS (select * from ventas
                where ventas.codpro = proveedor.codpro
                        AND ventas.codpie='P1');
```

##### Otros operadores, los operadores sobre conjuntos.

- Operadores: <, <=, >, >=, <> más los cuantificadores ANY y ALL.

```
select codpro
from proveedor
where status = ( select status from proveedor where codpro='S3' );
```

```
select codpie
from pieza
where peso > ANY ( select peso from pieza where nompie like 'Tornillo%' );
```

- Ejercicio 3.23:

```
select codpie
from pieza
where peso > ANY (select peso from pieza);
```

##### La división AR en SQL.

Calcular: `π cod pro,cod pie (ventas) ÷ π cod pie (pieza)`

- Primera aproximación:
  `relacion1 ÷ relacion2 = π A (Relacion1) − π A ((π A (Relacion1) × Relacion2) − Relacion1) siendo A = {AtributosRelacion1} − {AtributosRelacion2}`

```
(select codpro from ventas)
MINUS
(select codpro
        from (
            select v.codpro, p.codpie from
                (select distinct codpro from ventas) v,
                (select codpie from pieza) p
        )
        MINUS
        (select codpro, codpie from ventas)
    );
```

- Segunda aproximacion: calculo orientado a tuplas:

```
select codpro
from proveedor
where not exists (
    (select * from pieza
    where not exists (select * from ventas
            where pieza.codpie=ventas.codpie
            and proveedor.codpro=ventas.codpro))
);
```

- Tercera aproximación: mezcla de las dos anteriores: seleccionar proveedores tal que (el conjunto de todas las piezas) menos (el conjunto de las piezas suministradas por ese proveedor) sea vacío.

```
select codpro
from proveedor
where not exists (
    (select distinct codpie from pieza)
    minus
    (select distinct codpie from ventas where proveedor.codpro=ventas.codpro)
);
```

##### Funciones de agregación

```
select MAX(cantidad), MIN(cantidad), SUM(cantidad) from ventas;
select MAX(DISTINCT cantidad), MIN(DISTINCT cantidad), SUM(DISTINCT cantidad) from ventas;
```

##### Formando grupos GROUP BY

```
select codpro, count(*), max(cantidad)
from ventas
group by (codpro);
```

##### Seleccionando grupos HAVING

```
select codpro, AVG(cantidad)
from ventas
group by (codpro)
having COUNT(*) > 3;
```

```
select codpro, codpie, avg(cantidad)
from ventas
where codpie='P1'
group by (codpro, codpie) having count(*) between 2 and 10;
```

```
select v.codpro, v.codpj, j.nompj, AVG(v.cantidad)
from ventas v, proyecto j
where v.codpj=j.codpj
group by (v.codpj, j.nompj, v.codpro)
```

##### Usando to_date to_numbre

- Ejemplo importante:

```
(select distinct codpie from pieza)
MINUS
(select distinct codpie from ventas
where to_number(to_char(fecha, 'YYYY')) > 2001);
```

```
select to_char(fecha, 'YYYY'), SUM(cantidad)
from ventas
group by to_char(fecha,'YYYY');
```

##### Dictionary

`describe dictionary` para consultar el catálogo.

##### El esquema externo en un SGBD: vistas.

```
CREATE VIEW VentasParis (codpro, codpie, codpj, cantidad, fecha) AS
    SELECT codpro, codpie, codpj, cantidad, fecha
    FROM ventas
    WHERE (codpro, codpie, codpj) IN
    (
        SELECT codpro, codpie, codpj
        FROM proveedor, pieza, proyecto
        WHERE proveedor.ciudad='Paris' and
              pieza.ciudad='Paris' and
              proyecto.ciudad='Paris'
    );
```

Las vistas disponibles se pueden consultar en _all_views_.

```
all_views(owner, view_name, text)
```

- Consulta de vistas:

  ```
  select distinct codpro
  from VentasParis
  where codpj='J4';
  ```

- Condiciones para la modificación de vistas:

  - no podrá incluir cláusulas de agrupamiento (group by, etc) ni funciones de agregación (add, etc).
  - no puede aparecer la cláusula _DISTINCT_.
  - la definición de la vista no podrá incluir operaciones de reunión ni de conjuntos, esto es, deberá construirse sobre una única tabla base.
  - todos los valores que deban tomar siempre valor (not null y primary key) han de estar incluidos necesariamente en la definición de la vista.

- Ejemplo importante:

  ```
  create view PiezasLondres AS
  select codpie, nompie, color, peso from pieza
  where pieza.ciudad='Londres';

  insert into PiezasLondres values ('P9', 'Pieza 9', 'rojo', 90);
  ```

  Como vemos, la vista PiezasLondres cumple las condiciones para la modificación, en el campo _ciudad_ que falta, pondrá un valor _null_.

- Usar `DROP VIEW <vista>` para eliminar vistas.
- Usar `CREATE OR REPLACE VIEW <vista>` para cambiar la definición de una vista existente (para no perder los privilegios otorgados sobre la misma).
