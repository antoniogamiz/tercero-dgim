#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int num_procesos  = 10;
const int m = 10;
const int primes[] = {2,3,5,7,11,13,17,23,29,31};

// ---------------------------------------------------------------------

int leer() {
   int x = 0;
   cout << "Introduce un numero para ver si es primo: " << endl;
   cin >> x;
   return x;
}
// ---------------------------------------------------------------------

void Etapa( int id )
{
   int izquierda = 0;
   MPI_Status estado;

   // while ( 0 <= izquierda ) {
      if( id == 0 ) {
         izquierda = leer();
      }
      else {
        MPI_Recv ( &izquierda, 1, MPI_INT, id - 1, 0, MPI_COMM_WORLD, &estado );
      }

      if ( izquierda % primes[id] != 0 ) {
         if ( id != m - 1 )
            MPI_Ssend ( &izquierda, 1, MPI_INT, id + 1, 0, MPI_COMM_WORLD );
         else
            cout << "Numero: " << izquierda << " es primo." << endl;
      } else {
         cout << "El número no es primo" << endl;
      }
   // }
}
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
      Etapa( id_propio );
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
