# Seminario III

## Básico:

* Hay que hacer 
~~~C
#include <mpi.h>
~~~

* Si las funciones devuelven MPI_SUCCESS => ejecución correcta.
* MPI_Status=> estructura con los metadatos de los mensajes.
~~~C
status.MPI_SOURCE //proceso fuente.
status.MPI_TAG //etiqueta del mensaje.
~~~

* Constantes:
~~~C
MPI_CHAR, MPI_INT, MPI_LONG, etc.
~~~

* Comunicador: es tanto un grupo de procesos como un contexto de comunicación. Todas las funciones de comunicación necesitan como argumento un comunicador. De tipo MPI_Comm. El comunicador es un subconjunto de MPI_COMM_WORLD.

## Compilación:

* *mpicxx*: para compilar y/o enlazar programas C++ que hagan uso de MPI.
* *mpirun*: para ejecutar programas MPI.

`mpicxx -std=c++11 -c ejemplo.cpp`
`mpicxx -std=c++11 -o ejemplo ejemplo.o`
`mpirun -np 4 ./ejemplo`

## Funciones MPI básicas:

* *MPI_Init*: inicializa el entorno de ejecución MPI.
  `int MPI_Init( int *argc, char ***argv )`
* *MPI_Finalize*: finaliza el entorno de ejecución MPI.
  `int MPI_Finalize()`
* *MPI_Comm_size*: determina el número de procesos de un comunicador.
  `int MPI_Comm_size( MPI_Comm comm, int *size )`
  ~~~C++
  int num_procesos ; //contendrá el total de procesos de la aplic.
  MPI_Comm_size( MPI_COMM_WORLD, &num_procesos );
  cout << "El número total de procesos es: " << num_procesos << endl;
  ~~~
* *MPI_Comm_rank*: determina el identificador del proceso en **un comunicador**.
  `int MPI_Comm_Rank( MPI_Comm comm, int *rank )`
  ~~~C++
  int id_propio ; //contendrá el número de proceso que llama
  MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
  cout << "Mi número de proceso es: " << id_propio << endl;
  ~~~
* *MPI_Send*: operación básica para envío de un mensaje.
  `int MPI_Send( void *buf_emi, int num, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )`
    * *num* => número de elementos almacenados de tipo *datatype* a partir de *buf_emi*.
    * *tag* => se transfiere junto con el mensaje, y suele usarse para clasificar los mensajes en distintas categorías o tipos, en función de sus etiquetas. Es no negativo.
    * Consideración: *MPI_Send* realiza **envío asíncrono seguro** por lo que podemos volver a escribir sobre *buf_emi*.
* *MPI_Recv*: operación básica para recepción de un mensaje.
  `int MPI_Recv( void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status )`
    * Espera hasta recibir un mensaje del proceso *source* dentro del comunicador *comm* con la etiquete *tag* y escribe los datos en posiciones contiguas desde *buf_rec*.
    * Es una recepción síncrona (espera a que el emisor envíe) => recepción segura.
    * Valores especiales: 
        * Si *source* es *MPI_ANY_SOURCE*, se puede recibir un mensaje de cualquier proceso en el comunicador.
        * Si *tag* es *MPI_ANY_TAG*, se puede recibir un mensaje con cualquier etiqueta.
* *MPI_Get_Count*: para obtener la cuenta de valores recibidos, usamos *status*:
  `int MPI_Get_count( MPI_Status *status, MPI_Datatype dtype, int *num )`
* *MPI_Ssend*: inicia un envío, lee datos y espera el inicio d la recepción con los mismos argumentos que *MPI_Send* => envío síncrono.
  `int MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )`

### Sondeo de mensajes:

MPI incorpora dos operaciones que permiten a un proceso receptor averiguar si hay algún mensaje pendiente de recibir (en un comunicador), y en ese caso obtener los metadatos de dicho mensaje.

* *MPI_Iprobe*: consultar si hay o no algún mensaje pendiente en este momento (no bloqueante).

`int MPI_Iprobe( int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status )`

Al terminar el entero apuntado por *flag* será mayor que 0 solo si hay algún mensaje enviado al proceso que llama, y que encaje con los argumentos (en el comunicador *comm*). Si no hay mensajes dicho entero es 0.
* *MPI_Probe*: esperar bloqueado hasta que haya al menos un mensaje.

`int MPI_Probe( int source, int tag, MPI_Comm comm, MPI_Status *status )`

### Operaciones inseguras (asíncronas).

* *MPI_Isend*: inicia envío pero retorna antes de leer el buffer.

`int MPI_Isend( void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )`
* *MPI_Irecv*: inicia recepción pero retorna antes de recibir.

`int MPI_Irecv( void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request )`

*request* => es un **ticket** que permitirá después identificar la operación cuyo estado se pretende consultar o se espera que finalice. 

Cuando ya no se va a usar una variable *MPI_Request*, se puede liberar la memoria que usa con *MPI_Request_free*, declarada así:

`int MPI_Request( MPI_Request *request )`

En algún momento posterior se puede comprobar si la operación ha terminado o no, se puede hacer de dos formas:

* *MPI_Wait*: espera bloqueado hasta que acabe el envío o recepción. En ambas funciones, una vez terminada la operación referenciada por el ticket podemos usar el objeto *status* y no hace falta liberar la memoria usada por *request* ya que lo hace MPI.

`int MPI_Wait( MPI_Request *request, MPI_Status *status )`
* *MPI_Test*: comprueba si el envio o recepción ha finalizado o no. No supone espera bloqueante. Escribe en *flag* 0 si no ha acabado o un número positivo si sí.

` int MPI_Test( MPI_Request *request, int *flag, MPI_Status *status ) `