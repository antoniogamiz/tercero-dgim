#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player
{
public:
  Player(int jug);
  Environment::ActionType Think();
  void Perceive(const Environment &env);
  double Poda(Environment env, int jug, int profundidad, int COTA_PROF, Environment::ActionType accion, double alpha, double beta);

private:
  int jugador_;
  Environment actual_;
};
#endif

/**
 * Poda(act, jug, profundidad, COTA, acc, alpha, beta) {
 *  //nodo en el que nos encontramos
 *  nodo actual
 *  // generamos los hijos
 *  vector hijos = generarHijos()
 *  
 *  // descendemos en el arbol hasta llegar a los nodos
 *  // hoja
 *  if profundidad < COTA - 1 {
 *     actual = Poda(... profundidad+2 ) 
 *     for hijo+1 in hijos { 
 *        if ( nodoAdesarrollar(actual) ) {
 *          actual = Poda(... profundidad+2 )
 *        }   
 *     }
 *  }
 *  
 *  // estamos en los nodos hoja, no tienen valor alpha
 *  // beta por lo que hay que darles un valor directamente
 *  if profundidad == COTA - 1 {
 *     for hijo in hijos { hijo.value = Valoracion() } 
 *  }
 *  
 *  // ahora tenemos que calcular el valor del nodo en el que
 *  // nos encontramos
 * 
 *  // esto se ejecutara lo ultimo, cuando todas las llamadas recursivas
 *  // hayan sido finalizadas
 *  if profundidad == 0 {return actual.value}
 *  else {return actual}
 * }
 */