CREATE TABLE equipos
(
    codE char(3) constraint codE_no_nulo not null constraint codE_clave_primaria primary key,
    nombreE varchar2(30) constraint nombreE_no_nulo not null constraint nombreE_unico UNIQUE,
    entrenador varchar2(30) constraint entrenador_no_nulo not null,
    fecha_crea date constraint fecha_crea_no_nulo not null
);

CREATE TABLE jugadores
(
    codJ char(3) constraint codJ_no_nulo not null constraint codJ_clave_primaria primary key,
    codE char(3) constraint codE_no_nulo not null,
    nombreJ varchar2(30) constraint nombreJ_no_nulo not null
);

CREATE TABLE encuentros
(
    ELocal char(3) constraint ELocal_clave_externa references equipos(codE),
    EVisitante char(3) constraint EVisitante_clave_externa references equipos(codE),
    fecha date constraint fecha_no_nulo not null,
    PLocal int constraint PLocal_default DEFAULT 0,
    PVisitante int constraint PVisitante_default DEFAULT 0
);

CREATE TABLE faltas
(
    codJ char(3) constraint codJ_clave_externa references jugadores(codJ)
    ELocal char
    (3) constraint ELocal_no_nulo not null,
    EVisitante char
    (3) constraint EVisitante_no_nulo not null,
    constraint clave_externa references encuentros
    (ELocal, EVisitante)
);