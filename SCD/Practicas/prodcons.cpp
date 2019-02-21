// -----------------------------------------------------------------------------
// Yabir García Benchakhtir - 3º DGIIM
// Examen MPI 
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
	id_buffer             = 0 ,
	num_productores       = 3 ,
	num_consumidores      = 4 ,
	id_consumidor         = 2 ,
	num_procesos_esperado = num_productores + num_consumidores +1 ,
	num_items             = 20,
	tam_vector            = 3,
	etiq_prod             = 1,
	etiq_cons             = 2,
	etiq_buff             = 0;
  
int tag_no_produccion   = 5; // Tag que se usa si no hay un elemento que le sirva  

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio(){
	static default_random_engine generador( (random_device())() );
	static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
	return distribucion_uniforme( generador );
}
// ---------------------------------------------------------------------
// ptoducir produce los numeros en secuencia (1,2,3,....)
// y lleva espera aleatorio
int producir(int id){
	static int contador = (id-1)*num_productores ;
	sleep_for( milliseconds( aleatorio<10,100>()) );
	contador++ ;
	//cout << "Productor " << id << " ha producido valor " << contador << endl << flush;
	return contador ;
}
// ---------------------------------------------------------------------

void funcion_productor(int id)
{
	for ( unsigned int i= 0 ; i < num_items/num_productores  ; i++ ){

		// producir valor
      	int valor_prod = producir(id );
      	// enviar valor
      	MPI_Ssend( &valor_prod, 1, MPI_INT, id_buffer, etiq_prod, MPI_COMM_WORLD );
    }
}
// ---------------------------------------------------------------------

void consumir( int valor_cons, int id ){
  	// espera bloqueada
	sleep_for( milliseconds( aleatorio<110,200>()) );
	cout << "\tConsumidor " << id << " ha consumido valor " << valor_cons << endl << flush ;
}
// ---------------------------------------------------------------------

void funcion_consumidor(int id)
{
	int         peticion,
    valor_rec = 1 ;
  	MPI_Status  estado ;

	for( unsigned int i=0 ; i < num_items/num_consumidores; i++ ){

		// Mandamos una petición al buffer
		MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD);
		
		// Recibimos las respuesta del buffer
		MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, MPI_ANY_TAG, MPI_COMM_WORLD, &estado );
		// Si manda el mensaje con la etiqueta del buffer
		if (estado.MPI_TAG != tag_no_produccion){
			// Consumimos el dato
			consumir( valor_rec , id);
		}else{
			// Si la manda con la etiqueta tag_no_produccion no contamos
			// la iteracion como iteracion valida. Si no hacemos esto
			// el programa no acaba correctamente leyendo todos los mensajes
			i--;
		}
	}
}
// ---------------------------------------------------------------------

void funcion_buffer()
{
  	int	buffer[tam_vector],      // buffer con celdas ocupadas y vacías
    valor,                   // valor recibido o enviado
    primera_libre       = 0, // índice de primera celda libre
    primera_ocupada     = 0, // índice de primera celda ocupada
    num_celdas_ocupadas = 0, // número de celdas ocupadas
    tag_emisor_aceptable ;    // identificador de emisor aceptable
  	MPI_Status estado ;                 // metadatos del mensaje recibido

	for( unsigned int i=0 ; i < num_items*2 ; i++ ){
      // 1. determinar si puede enviar solo prod., solo cons, o todos

		if ( num_celdas_ocupadas == 0 )               // si buffer vacío
			tag_emisor_aceptable = etiq_prod ;       // $~~~$ solo prod.
	  	else if ( num_celdas_ocupadas == tam_vector ) // si buffer lleno
			tag_emisor_aceptable = etiq_cons ;      // $~~~$ solo cons.
	  	else                                          // si no vacío ni lleno
			tag_emisor_aceptable = MPI_ANY_TAG ;     // $~~~$ cualquiera

		// 2. recibir un mensaje del emisor o emisores aceptables

	  	MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, tag_emisor_aceptable, MPI_COMM_WORLD, &estado );

		// 3. procesar el mensaje recibido

	  	switch( estado.MPI_TAG ){ // leer emisor del mensaje en metadatos{
			case etiq_prod: // si ha sido el productor: insertar en buffer
				buffer[primera_libre] = valor ;
				primera_libre = (primera_libre+1) % tam_vector ;
				num_celdas_ocupadas++ ;
				//cout << "\t\tBuffer ha recibido valor " << valor << endl ;
				break;

			case etiq_cons: // si ha sido el consumidor: extraer y enviarle
				valor = buffer[primera_ocupada] ;
				
				// Si el proceso tiene id par y además el valor que mandamos
				// es par o el proceso y el valor son impares
				if ( (estado.MPI_SOURCE % 2 == 0 && valor % 2 == 0 ) ||
					 (estado.MPI_SOURCE % 2 != 0 && valor % 2 != 0) ){
					//cout << "\t\tBuffer va a enviar valor " << valor << " al proceso " <<  estado.MPI_SOURCE << endl ;
					MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_buff, MPI_COMM_WORLD);
					primera_ocupada = (primera_ocupada+1) % tam_vector ;
					num_celdas_ocupadas-- ;
				}else{
					// En caso contrario mandamos un mensaje indicando que no hay produccion valida
					MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, tag_no_produccion, MPI_COMM_WORLD);
					// Si la iteracion no es valida no la contamos en el bucle
					i--;
				}	
				break;
		}
    }
}

// ---------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	int id_propio, num_procesos_actual;

  	// inicializar MPI, leer identif. de proceso y número de procesos
 	MPI_Init( &argc, &argv );
  	MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
	MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

  	if ( num_procesos_esperado == num_procesos_actual ){
		// ejecutar la operación apropiada a 'id_propio'
      	if (id_propio == id_buffer){
			cout << "Se ha creado buffer" << endl;
			funcion_buffer();
      	}
		else if ( id_propio <= num_productores ){
			cout << "Se ha creado un productor" << endl;
			funcion_productor(id_propio);
      	}
      	else{
			cout << "Se ha creado un consumidor" << endl;
			funcion_consumidor(id_propio);
      	}
    }
  	else{
      if ( id_propio == 0 ){ // solo el primero escribe error, indep. del rol
		 cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
				<< "el número de procesos en ejecución es: " << num_procesos_actual << endl
	      		<< "(programa abortado)" << endl ;
		}
    }

  	// al terminar el proceso, finalizar MPI
  	MPI_Finalize( );
  	return 0;
}
