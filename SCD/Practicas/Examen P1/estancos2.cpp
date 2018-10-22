
// g++ -std=c++11 -pthread -I. -o exe fumadores-plantilla.cpp Semaphore.cpp

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;


// Variables compartidas y valores iniciales

Semaphore mostr_vacio = 1; //1 si mostrador vacío, 0 si ocupado
Semaphore fumadores[4] = {0, 0, 0, 0};

int cigarros_fumados[4] = {0, 0, 0, 0};


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

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
  int i=0;
  int j=0; // para evitar ejecucion infinita
  while ( j < 10) {
    i = aleatorio<0,3>();
    sem_wait( mostr_vacio );
    cout << "Estanquero produce ingrediente:  " << i << endl << flush ;
    j++;
    sem_signal( fumadores[i] );
  }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl << flush;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl << flush;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
   while( true )
   {
     sem_wait( fumadores[num_fumador] );
     cout << "Fumador " << num_fumador << " retira ingrediente:  " << num_fumador << endl << flush ;

    if( cigarros_fumados[num_fumador] % 2 == 0 ){
        fumar( num_fumador );
        sem_signal( mostr_vacio );
    } 
    else {
        sem_signal( mostr_vacio );
        fumar( num_fumador );
    }

     cigarros_fumados[num_fumador]++; // no hace falta poner semáforo ya que cada hebra accede solamente a una única posicion del array
   }
}

//----------------------------------------------------------------------

int main()
{
   cout << "--------------------------------------------------------" << endl
        << "Problema de los fumadores." << endl
        << "--------------------------------------------------------" << endl
        << flush ;

   thread estanquero ( funcion_hebra_estanquero ),
          fumador0( funcion_hebra_fumador, 0 ),
          fumador1( funcion_hebra_fumador, 1 ),
          fumador2( funcion_hebra_fumador, 2 ),
          fumador3( funcion_hebra_fumador, 3 );
   thread fumadores[4];
   for( int i = 0; i < 4; i++ ) { fumadores[i] = thread( funcion_hebra_fumador, i); }

   estanquero.join() ;
   for( int i = 0; i < 4; i++ ) { fumadores[i].join(); }


}
