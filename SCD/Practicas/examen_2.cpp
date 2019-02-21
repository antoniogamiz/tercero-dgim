// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: prodcons2.cpp
// Implementación del problema del productor-consumidor con
// un proceso intermedio que gestiona un buffer finito y recibe peticiones
// en orden arbitrario
// (versión con un único productor y un único consumidor)
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
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
   nProds                = 1,
   nCons                 = 5,
   id_buffer             = nProds,
   num_procesos_esperado = 1 + nProds + nCons ,
   num_items             = 20,
   tam_vector            = 5,
   num_items_prod        = num_items / nProds,
   num_items_cons        = num_items / nCons,
   etiq_impar             = 0,
   etiq_par              = 1,
   etiq_prod             = 2,
   etiq_cons             = 3;

const int etiq_buf = 67;

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}
// ---------------------------------------------------------------------
// ptoducir produce los numeros en secuencia (1,2,3,....)
// y lleva espera aleatorio
int producir( int id_propio )
{
   static int contador = id_propio * num_items_prod;
   sleep_for( milliseconds( aleatorio<10,100>()) );
   contador++ ;
   cout << "Productor ha producido valor " << contador << endl << flush;
   return contador ;
}
// ---------------------------------------------------------------------

void funcion_productor( int id_propio )
{
   for ( unsigned int i= 0 ; i < num_items_prod ; i++ )
   {
      // producir valor
      int valor_prod = producir( id_propio );
      // enviar valor
      cout << "Productor va a enviar valor " << valor_prod << endl << flush;
      MPI_Ssend( &valor_prod, 1, MPI_INT, id_buffer, etiq_prod, MPI_COMM_WORLD );
   }
}
// ---------------------------------------------------------------------

void consumir( int valor_cons, int id_propio )
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<110,200>()) );
   cout << "Consumidor " << id_propio << " ha consumido valor " << valor_cons << endl << flush ;
}
// ---------------------------------------------------------------------

void funcion_consumidor( int id_propio )
{
   int         peticion,
               valor_rec = 1 ;
   MPI_Status  estado ;

    int tag; 
    if( id_propio % 2 == 0 )
        tag = etiq_par;
    else 
        tag = etiq_impar;
   for( unsigned int i=0 ; i < num_items_cons; i++ )
   {
      MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD);
      MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_buf, MPI_COMM_WORLD,&estado );
      cout << "Consumidor " << id_propio << "ha recibido valor " << valor_rec << endl << flush ;
      consumir( valor_rec, id_propio );
   }
}
// ---------------------------------------------------------------------

void funcion_buffer()
{
   int        buffer[tam_vector],      // buffer con celdas ocupadas y vacías
              valor,                   // valor recibido o enviado
              primera_libre       = 0, // índice de primera celda libre
              primera_ocupada     = 0, // índice de primera celda ocupada
              num_celdas_ocupadas = 0, // número de celdas ocupadas
              tag = 0;
   MPI_Status estado ;                 // metadatos del mensaje recibido

   int i = 0;
   while ( i < num_items)
   {
       sleep_for( milliseconds(100) );
      MPI_Iprobe( MPI_ANY_SOURCE, etiq_prod, MPI_COMM_WORLD, &tag, &estado );
      if( tag ) {
          if( num_celdas_ocupadas < tam_vector ) {
            MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_prod, MPI_COMM_WORLD, &estado );
            buffer[primera_libre] = valor ;
            primera_libre = (primera_libre+1) % tam_vector ;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor " << valor << endl ;
          }
      }

      MPI_Iprobe( MPI_ANY_SOURCE, etiq_cons, MPI_COMM_WORLD, &tag, &estado );
      if( tag ) {
          valor = buffer[primera_ocupada];
          if( ((valor % 2 == 0) && (estado.MPI_SOURCE % 2 == 0)) || ((valor % 2 != 0) && (estado.MPI_SOURCE % 2 != 0)) ) {
            i++;
            MPI_Recv( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_cons, MPI_COMM_WORLD, &estado );
            valor = buffer[primera_ocupada];
            primera_ocupada = (primera_ocupada+1) % tam_vector ;
            num_celdas_ocupadas-- ;
            cout << "Buffer va a enviar valor " << valor << endl ;
            MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_buf, MPI_COMM_WORLD);
          }
      cout << i << endl;
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

   if ( num_procesos_esperado == num_procesos_actual )
   {
      // ejecutar la operación apropiada a 'id_propio'
      if ( id_propio == id_buffer )
         funcion_buffer();
      else if ( id_propio < id_buffer )
         funcion_productor( id_propio );
      else
         funcion_consumidor( id_propio );
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
