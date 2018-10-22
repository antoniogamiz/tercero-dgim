#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

//**********************************************************************
// variables compartidas

const int num_items = 20,   // número de items
	       tam_vec   = 5 ;   // tamaño del buffer
unsigned  cont_prod[num_items] = {0}, // contadores de verificación: producidos
          cont_cons[num_items] = {0}; // contadores de verificación: consumidos

int consumidos = 0;
int producidos = 0;

int buffer[tam_vec];

Semaphore producir( tam_vec );
Semaphore consumir( 0 );
Semaphore alguien_consumiendo( 1 ); // a 1 ya que al principio no hay nadie consumiendo.
Semaphore alguien_produciendo( 1 ); // a 1 ya que al principio no hay nadie consumiendo.
Semaphore mod_consumidos( 1 );  // para modificar consumidos en exclusion mutua.
Semaphore mod_producidos( 1 );  // para modificar producidos en exclusion mutua.

int counter_read = 0;
int counter_write = 0;

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

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato()
{
  static int contador = 0 ;
  this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

  cout << "producido: " << contador << endl << flush ;

  cont_prod[contador] ++ ;
  return contador++ ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;
   
}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora(  )
{
   sem_wait( mod_producidos );
   while( producidos < num_items )
   {
      producidos++;
      sem_signal( mod_producidos );
      int dato = producir_dato() ;
      
      sem_wait( producir );
      sem_wait( alguien_produciendo );
      buffer[ counter_write ]=dato;
      counter_write = (counter_write + 1) % tam_vec;
      sem_signal( alguien_produciendo );
      sem_signal( consumir );

   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora(  )
{
   sem_wait( mod_consumidos ); // lo ponemos antes del bucle para evitar lecturas incorrectas
   while ( consumidos < num_items )
   {
      consumidos++;
      sem_signal( mod_consumidos );
      
      int dato ;

      sem_wait( consumir );
      sem_wait( alguien_consumiendo );
      

      dato=buffer[counter_read];
      counter_read = (counter_read + 1) % tam_vec;

      sem_signal( alguien_consumiendo );
      sem_signal( producir );
      
      consumir_dato( dato ) ;
    

    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "--------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución FIFO)." << endl
        << "--------------------------------------------------------" << endl
        << flush ;

    int n_consumidores = 10;
    int n_productores = 10;

   thread productores[n_productores];
   thread consumidores[n_consumidores];

   for( int i = 0; i < n_productores; i++ ){ productores[i] = thread( funcion_hebra_productora ); }
   for( int i = 0; i < n_consumidores; i++ ){ consumidores[i] = thread( funcion_hebra_consumidora ); }

   for( int i = 0; i < n_productores; i++ ){ productores[i].join(); }
   for( int i = 0; i < n_consumidores; i++ ){ consumidores[i].join(); }


   test_contadores();
}
