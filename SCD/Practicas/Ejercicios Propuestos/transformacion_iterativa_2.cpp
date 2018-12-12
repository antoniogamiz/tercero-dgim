#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int p  = 3;
const int num_procesos = p + 1;
const int n = 9;
const int m = 2;

// ---------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}
// ---------------------------------------------------------------------

void Coordinador() {
   MPI_Status estado;
   
   float inicial[n],
         resultado[n],
         bloque[n/p];

   // Inicializamos
   for( int i = 0; i < n; i++ )
      inicial[i] = aleatorio<10,100>()*1.1;
   
   // Mandamos los bloques
   for( int j = 0; j<p; j++ ) {
      for( int k = 0; k<n/p; k++ ) bloque[k] = inicial[j*(n/p) + k];
      MPI_Ssend ( &bloque, n/p, MPI_FLOAT, j, 0, MPI_COMM_WORLD );
   }

   //Recoemos los resultados
   for( int j = 0; j<p; j++ ) {
      MPI_Recv ( &bloque, n/p, MPI_FLOAT, j, 0, MPI_COMM_WORLD, &estado );
      for( int k = 0; k<n/p; k++ ) resultado[j*(n/p)+k] = bloque[k];
   }

   // Imprimimos los resultados
   for( int i = 0; i<n; i++ )
      cout << "r[" << i << "] == " << resultado[i] << endl << flush;
}
// ---------------------------------------------------------------------

void Tarea( int id )
{
    float entrada[n/p+2],
          salida[n/p+2];
    MPI_Status estado;

    float tmp = 0;
    MPI_Recv ( &entrada, n/p, MPI_FLOAT, p, 0, MPI_COMM_WORLD, &estado );
    for( int k = 0; k<m; k++) {
      if( id == 0 ) {
         MPI_Ssend ( &entrada[0], 1, MPI_FLOAT, (id-1+p)%p, 0, MPI_COMM_WORLD );
         MPI_Recv ( &entrada[n/p+1], 1, MPI_FLOAT, (id+1)%p, 0, MPI_COMM_WORLD, &estado );
         MPI_Ssend ( &entrada[n/p - 1], 1, MPI_FLOAT, (id+1)%p, 0, MPI_COMM_WORLD );
         MPI_Recv ( &entrada[0], 1, MPI_FLOAT, (id-1+p)%p, 0, MPI_COMM_WORLD, &estado );
      } else {
         MPI_Recv ( &entrada[n/p+1], 1, MPI_FLOAT, (id+1)%p, 0, MPI_COMM_WORLD, &estado );
         MPI_Ssend ( &entrada[1], 1, MPI_FLOAT, (id-1+p)%p, 0, MPI_COMM_WORLD );
         MPI_Recv ( &entrada[0], 1, MPI_FLOAT, (id-1+p)%p, 0, MPI_COMM_WORLD, &estado );
         MPI_Ssend ( &entrada[n/p], 1, MPI_FLOAT, (id+1)%p, 0, MPI_COMM_WORLD );
      }

      for( int j = 1; j<(n/p + 1); j++ ) {
        salida[j]=(entrada[j-1] - entrada[j] + entrada[j+1])/2;
      }
      
      //Swapping de los vectores
      for(int i = 1; i < n/p+2; i++) {
        tmp = entrada[i];
        entrada[i] = salida[i];
        salida[i-1] = tmp;
      }
    }
    MPI_Ssend ( &entrada, n/p, MPI_FLOAT, p, 0, MPI_COMM_WORLD );
}
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
      if( id_propio == p ) {
         Coordinador();
      }
      else
         Tarea( id_propio );
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
