#### Ejercicios sección 3:

- Ejercicio 3.1:
  No, porque en el AR no hay tuplas iguales, son todas únicas. Para conseguir el mismo resultado en SQL, debemos añadir la cláusula `DISTINCT` a la consulta:

  ```
  select distinct ciudad from proyecto;
  ```

- Ejercicio 3.2: sí, es necesario utilizar `distinct`, ya que aunque `(codpie, codpro, codpj)` sea único, el atributo `fecha` nos indica que la misma venta puede haberse realizo en distintas fechas.

  ```
  select codpie, codpro, codpj from ventas;
  ```

- Ejercicio 3.3:

  ```
  select codpie from pieza where (color='Gris' OR color='Rojo');
  ```

- Ejercicio 3.4:
  ```
  select distinct codpie from ventas where (cantidad >= 200 AND cantidad <= 300);
  ```
- Ejercicio 3.5:

  ```
  select codpie from pieza where (nompie like '%tornillo%' OR nompie like '%Tornillo%');
  ```

- Ejercicio 3.6: sólo se puede hacer en los ordenadores del aula.

- Ejercicio 3.7:

  ```
  (select distinct ciudad from proveedor where status > 2)
  MINUS
  (select distinct ciudad from pieza where codpie='P1');

  (select distinct ciudad from proveedor where status > 2)
  INTERSECT
  (select distinct ciudad from pieza where codpie <> 'P1');
  ```

- Ejercicio 3.8:

  ```
  (select codpj from ventas where codpro='S1')
  MINUS
  (select codpj from ventas where codpro <> 'S1');
  ```

- Ejercicio 3.9:

  ```
  (select ciudad from pieza)
  UNION
  (select ciudad from proyecto)
  UNION
  (select ciudad from proveedor);
  ```

- Ejercicio 3.10:

  ```
  select distinct * from (
    (select ciudad from pieza)
    UNION ALL
    (select ciudad from proyecto)
    UNION ALL
    (select ciudad from proveedor)
  );
  ```

- Ejercicio 3.11:

  ```
  select count(*) from ventas;
  select count(*) from proveedor;
  select count(*) from ventas, proveedor;
  ```

- Ejercicio 3.12:

  ```
  (select codpro, codpie, codpj
  from proveedor, proyecto, pieza
  where proveedor.ciudad=proyecto.ciudad and proyecto.ciudad=pieza.ciudad)
  INTERSECT
  (select codpro, codpie, codpj from ventas);
  ```

- Ejercicio 3.13:

  ```
  select s.codpro, p.codpro
  from proveedor s, proveedor p
  where s.ciudad<>p.ciudad;
  ```

- Ejercicio 3.14:

  ```
  (select codpie from pieza)
  minus
  (select s.codpie from pieza s, pieza p
  where s.peso > p.peso);
  ```

- Ejercicio 3.15:

  ```
  select codpie
  from (select codpro, codpie from ventas)
  natural join
  (select codpro from proveedor where ciudad='Madrid');
  ```

- Ejercicio 3.16:

  ```
  select ciudad codpie
  from
  (select p.ciudad, p.codpro, j.codpj from proveedor p, proyecto j where p.ciudad=j.ciudad) natural join ventas
  ```

- Ejercicio 3.17:

  ```
  select nompro
  from proveedor
  order by nompro;
  ```

- Ejercicio 3.18:

  ```
  select *
  from ventas
  order by cantidad ASC, fecha DESC;
  ```

- Ejercicio 3.19:

  ```
  select codpro from ventas
  where codpro IN
  (select codpro from proveedor where ciudad='Madrid');
  ```

- Ejercicio 3.20:

  ```
  select codpj from proyecto
  where proyecto.ciudad IN
  (select ciudad from pieza);
  ```

- Ejercicio 3.21:

  ```
  select codpj from proyecto
  MINUS
  (select codpj from ventas
  where codpie IN (select codpie from pieza where color='Rojo')
  AND codpro IN (select codpro from proveedor where ciudad='Londres'));
  ```

- Ejercicio 3.22:

  ```
  select codpie
  from pieza
  where peso > ANY (select peso from pieza where nompie like '%tornillo%);
  ```

- Ejercicio 3.23:

  ```
  (select codpie from pieza)
  minus
  (select codpie from pieza
  where peso > ANY (select peso from pieza));
  ```

- Ejercicio 3.24:

  ```
  select codpie
  from pieza
  where not exists (
      (select codpj from proyecto where ciudad='Londres')
      minus
      (select codpj from ventas
      where codpj IN (select codpj from proyecto where ciudad='Londres'))
  );
  ```

- Ejercicio 3.25:

  ```
  select codpj
  from proyecto j
  where not exists(
    (select codpie from (select ciudad from proyecto) natural join (select ciudad, codpie from pieza))
    minus
    (select codpie from ventas where codpj=j.codpj)
  );
  ```

- Ejercicio 3.26:

  ```
  select count(*) from ventas where cantidad > 100;
  ```

- Ejercicio 3.27:

  ```
  select MAX(peso) from pieza;
  ```

- Ejercicio 3.28:

  ```
  select codpie from pieza
  where peso = (select MAX(peso) from pieza);
  ```

- Ejercicio 3.29: la sentencia

  ```
  select codpie, MAX(peso)
  from pieza;
  ```

  da error al ejecutarse ya que estamos usando una función de agrupación junto a un select (codpie) que no es de agrupación.

- Ejercicio 3.30:

  ```
  select codpro from proveedor p
  where (select COUNT(*) from ventas where ventas.codpro=p.codpro) > 3;
  ```

- Ejercicio 3.31:

  ```
  select codpie, nompie, AVG(cantidad)
  from ventas natural join pieza
  group by codpie, nompie;
  ```

- Ejercicio 3.32:

  ```
  select codpro, AVG(cantidad)
  from ventas
  where codpie='P1'
  group by codpro;
  ```

- Ejercicio 3.33:

  ```
  select codpj, codpie, SUM(cantidad)
  from ventas
  group by codpj, codpie
  order by codpj, codpie;
  ```

- Ejercicio 3.34: creo que la solución es correcta.

- Ejercicio 3.35: **IMPORTANTE**: todos los campos que usemos en el _select_, deben aparecer en el _group by_, en caso contrario, dará error.

  ```
  select p.nompro
  from proveedor p, ventas v
  where p.codpro=v.codpro
  group by (p.codpro, p.nompro)
  having SUM(v.cantidad) > 1000;
  ```

- Ejercicio 3.36:

  ```
  select codpie, MAX(cantidad)
  from ventas
  group by codpie;
  ```

- Ejercicio 3.37: efectivamente, las comparaciones de fechas con cadenas no funcionan correctamente.

- Ejercicio 3.38:

  ```
  select to_char(fecha, 'MM'), AVG(cantidad)
  from ventas
  group by to_char(fecha, 'MM');
  ```

- Ejercicio 3.42:

  ```
  select codpro
  from ventas
  group by codpro
  having SUM(cantidad) >
  (select SUM(cantidad) from ventas where codpro='S1');
  ```

- Ejercicio 3.43:

  ```
  select codpro, SUM(cantidad)
  from ventas
  group by codpro
  order by SUM(cantidad) DESC;
  ```

- Ejercicio 3.44:

  ```
  select codpro
  from proveedor p
  where not exists(
    (select j.ciudad
    from (proyectos j natural join ventas)
    where codpro='S3')
    minus
    (select j.ciudad
    from (proyectos j natural join ventas)
    where codpro=p.codpro)
  )
  minus
  (select codpro from proveedor where codpro='S3');
  ```

- Ejercicio 3.45:

  ```
  select codpro
  from ventas
  group by codpro
  having count(*) >= 10;
  ```

- Ejercicio 3.46:

  ```
  select codpro
  from proveedor p
  where not exists(
    (select codpie from ventas where codpro='S1')
    minus
    (select codpie from ventas where codpro=p.codpro)
  );
  ```

- Ejercicio 3.47:

  ```
  select codpro, SUM(cantidad) from ventas
  where codpro IN
  (select codpro
  from proveedor p
  where not exists(
    (select codpie from ventas where codpro='S1')
    minus
    (select codpie from ventas where codpro=p.codpro)
  ))
  group by codpro;
  ```

- Ejercicio 3.48:

  ```
  select codpj
  from proyecto j
  where not exists (
    (select distinct codpro from ventas where codpie='P3')
    minus
    (select distinct codpro from ventas where codpj=j.codpj)
  );
  ```

- Ejercicio 3.49:

  ```
  select distinct codpro, AVG(cantidad)
  from ventas
  where codpie='P3'
  group by codpro;
  ```

- Ejercicio 3.50: no sé hacerlo :(.

- Ejercicio 3.51: no sé hacerlo :(.

- Ejercicio 3.52:

  ```
  select codpro, AVG(cantidad)
  from ventas
  group by to_date(fecha, 'YYYY');
  ```

- Ejercicio 3.53:

  ```
  select distinct codpro
  from ventas v, pieza p
  where v.codpie = p.codpie and color='Roja'
  group by codpro
  having SUM(cantidad) = 1;
  ```

- Ejercicio 3.54:

  ```
  select distinct p.codpro
  from proveedor p
  where not exists (
    (select codpie from pieza where color='Roja')
    minus
    (select distinct codpie
    from ventas v1, pieza p1
    where v1.codpie=p1.codpie and codpro=p.codpro and color='Roja');
  );
  ```

- Ejercicio 3.55:

  ```
  select codpro from proveedor p
  where not exists(
    (select distinct codpie
    from ventas where codpro=p.codpro)
    minus
    (select distinct v1.codpie
    from ventas v1, pieza p1
    where v1.codpie=p1.codpie and p1.color='Rojo' v1.codpro=p.codpro)
  );
  ```

- Ejercicio 3.56:

  ```
    select distinct v1.codpro
    from ventas v1, pieza p1
    where v1.codpie=p1.codpie and p1.color='Rojo'
    group by v1.codpro
    having SUM(cantidad) > 1;

  ```

- Ejercicio 3.57:

  ```
  select codpro
  from proveedor pp
  where not exists ((select *
  from ventas
  where pp.codpro IN
  (
    select codpro from proveedor p
    where not exists(
      (select distinct codpie
      from ventas where ventas.codpro=p.codpro)
      minus
      (select distinct v1.codpie
      from ventas v1, pieza p1
      where v1.codpie=p1.codpie and p1.color='Rojo' v1.codpro=p.codpro)
    )
  ))
  minus
  (select * from ventas
  where cantidad > 10 and ventas.codpro=pp.codpro));
  ```

- Ejercicio 3.58:

  ```
  update proveedor p
  set status = 1
  where p.codpro in (
    select codpro from proveedor p1
    where not exists (
      (select distinct codpie from ventas where p1.codpro=p.codpro)
      minus
      (select distinct codpie from pieza where codpie='S1')
    )
  );
  ```

### El esquema externo en un SGBD.

- Ejercicio 4.1: al insertar una nueva tupla que tenga el campo ciudad con valor _Granada_ estaríamos insertando una tupla errónea en la tupla, para asegurar la consistencia de la vist usamos la cláusula `WITH CHECK OPTION`.

  ```
  create view ProveedoresLondres (codpro, nompro, statusm, ciudad) AS
      select codpro, nompro, status, ciudad
      from proveedor
      where ciudad='Londres' with check option;
  ```

- Ejercicio 4.2: el problema es que no cumplimos con las condiciones de modificación de vistas, ya que el campo _nompro_ tiene la condición `NOT NULL`, por lo que debería estar incluido en la vista para poder modificarla.

  ```
  create view Proveedores (codpro, ciudad) AS
  select codpro, ciudad
  from proveedor;

  insert into Proveedores values ('w', 'ccc');
  ```

- Ejercicio 4.3: no se pueden insertar tuplas ya que estamos usando operaciones sobre conjuntos y estamos dejados campos con condiciones `not null` y `primary key` sin especificar en la vista.

  ```
  create view Ejercicio (codpro, nompro, codpj) AS
  select codpro, nompro, codpj
  from ventas natural join pieza natural join proveedor
  where color='Gris';
  ```

##### Privilegios.

Dos tipos de privilegios: sobre el sistema y sobre los objetos. Hay dos comandos que se encargan de controlar los privilegios, `GRANT` y `REVOKE`. Estos permiten otorgar y retirar privilegios a un usuario o a un "role".

```
grant
```

##### Indices

- Importante: no crear índices sobre una clave primaria ya que Oracle siempre crea índices para ellas, por lo que siempre empeoraría el rendimiento al tener que mantener un índice más.

```
CREATE INDEX nombre_del_indice
  ON tabla (campo [ASC|DESC], ... );
```

Es mejor crear la tabla primero, insertar los registros, y luego crear el índice. En caso contario, Oracle tendrá que actualizar el índice cada vez que insertemos un registro.

```
SELECT * FROM user_indexes WHERE index_name like '%INDICE_PROVEEDORES%';
```

El orden de las columnas usado en la definición del índice es importante: generalmente, los campos por los que se accede con mayor frecuencia se colocan antes en la creación del índice.

```
create index indicelibros on libros(genero,titulo,editorial);
```

Este índice solo acelerará consultas cuya condiciçon tenga referencias a género o a género y titulo o a género, título y editorial.

```
DROP INDEX nombre_del_indice;
```

- Indice invertido:
  ```
  create index nombre_del_indice on tabla(campo [ASC|DESC], ...) REVERSE;
  ```

Pseudo-código importante

```
BEGIN
  FOR i IN 1..50000 LOOP
    INSERT INTO Prueba_bit (
    select decode(round(dbms_random.value(1,4)),1,’Rojo’,2,’Verde’,
    3,’Amarillo’,4,’Azul’) from dual);
  END LOOP;
END;
```

```
CREATE BITMAP INDEX indice on tabla (campo [ASC|DESC], ...);
```

##### Tablas organizadas por IOT

Este tipo de tablas organizan sus tuplas según el valor de la clave utilizando una estructura de árbol B\*. La diferencia respecto a un índice normal estriba en que en las hojas están las tuplasm no los RID que apuntan las tuplas.

```
CREATE TABLE prueba (id number primary key) ORGANIZATION INDEX;
```
