#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf = 9999999999.0, menosinf = -9999999999.0;

// Constructor
Player::Player(int jug)
{
   jugador_ = jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment &env)
{
   actual_ = env;
}

double Puntuacion(int jugador, const Environment &estado)
{
   double suma = 0;

   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador)
         {
            if (j < 3)
               suma += j;
            else
               suma += (6 - j);
         }
      }

   return suma;
}

// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador)
{
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres() == 0)
      return 0; // Hay un empate global y se ha rellenado completamente el tablero
   else
      return Puntuacion(jugador, estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

// 0 => vacio
// 1 => jugador 1
// 2 => jugador 2
// 0110
double situacion1(const Environment &estado, int jugador)
{
   double suma = 0;
   for (int i = 0; i < 7; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         if (estado.See_Casilla(i, j) == 0 && estado.See_Casilla(i, j + 1) == jugador && estado.See_Casilla(i, j + 2) == jugador && estado.See_Casilla(i, j + 3) == 0)
         {
            suma += 100;
         }
      }
   }
   return suma;
}

// 11
double situacion2(const Environment &estado, int jugador)
{
   double suma = 0;
   for (int i = 0; i < 7; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         if (estado.See_Casilla(i, j) == jugador && estado.See_Casilla(i, j + 1) == jugador)
         {
            suma += 5;
         }
         if (estado.See_Casilla(j, i) == jugador && estado.See_Casilla(j + 1, i) == jugador)
         {
            suma += 5;
         }
      }
   }
   return suma;
}

// 111
double situacion3(const Environment &estado, int jugador)
{
   double suma = 0;
   for (int i = 0; i < 7; i++)
   {
      for (int j = 0; j < 5; j++)
      {
         if (estado.See_Casilla(i, j) == jugador && estado.See_Casilla(i, j + 1) == jugador && estado.See_Casilla(i, j + 2) == jugador)
         {
            suma += 10;
         }
         if (estado.See_Casilla(j, i) == jugador && estado.See_Casilla(j + 1, i) == jugador && estado.See_Casilla(j + 2, i) == jugador)
         {
            suma += 10;
         }
      }
   }
   return suma;
}

double Puntuacion2(const Environment &estado, int jugador)
{
   return situacion1(estado, jugador) + situacion2(estado, jugador) + situacion3(estado, jugador);
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador)
{
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres() == 0)
      return 0; // Hay un empate global y se ha rellenado completamente el tablero
   else
      return Puntuacion2(estado, jugador);
}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j)
{
   j = 0;
   for (int i = 0; i < 8; i++)
   {
      if (aplicables[i])
      {
         opciones[j] = i;
         j++;
      }
   }
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think()
{
   const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
   const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

   Environment::ActionType accion; // acci�n que se va a devolver
   bool aplicables[8];             // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                                   // aplicables[0]==true si PUT1 es aplicable
                                   // aplicables[1]==true si PUT2 es aplicable
                                   // aplicables[2]==true si PUT3 es aplicable
                                   // aplicables[3]==true si PUT4 es aplicable
                                   // aplicables[4]==true si PUT5 es aplicable
                                   // aplicables[5]==true si PUT6 es aplicable
                                   // aplicables[6]==true si PUT7 es aplicable
                                   // aplicables[7]==true si BOOM es aplicable

   double valor;       // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
   double alpha, beta; // Cotas de la poda AlfaBeta

   int n_act; //Acciones posibles en el estado actual

   n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
   int opciones[10];

   // Muestra por la consola las acciones aplicable para el jugador activo
   //actual_.PintaTablero();
   cout << " Acciones aplicables ";
   (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
   for (int t = 0; t < 8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr(static_cast<Environment::ActionType>(t));
   cout << endl;

   // --------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------
   double values[8];
   for (int i = 0; i < 8; i++)
   {
      values[i] = 0;
   };
   for (int i = 0; i < 8; i++)
   {
      if (aplicables[i])
         values[i] = Poda(actual_, actual_.JugadorActivo(), 0, PROFUNDIDAD_ALFABETA, static_cast<Environment::ActionType>(i), -99999, 99999);
   }
   double max = -9999;
   int index = 0;
   for (int i = 0; i < 8; i++)
   {
      if (values[i] > max && aplicables[i])
      {
         index = i;
         max = values[i];
      }
   };

   accion = static_cast<Environment::ActionType>(index);

   return accion;
}

double Player::Poda(const Environment envi, int jug, int profundidad, int COTA_PROF, Environment::ActionType accion, double alpha, double beta)
{
   // si es un nodo hoja
   if (profundidad == COTA_PROF)
      return Valoracion(envi, jug);

   Environment enviroments[8];
   int n_act = envi.GenerateAllMoves(enviroments);

   // contamos cuantas acciones podemos hacer

   // dependiendo del jugador
   int nextPlayer = (jug == 1) ? 2 : 1;
   if (jug == 1)
   { // verde 1
      // calculamos alpha
      for (int i = 0; i < n_act; i++)
         if (alpha < beta) // podamos si se puede
            alpha = max(alpha, Poda(enviroments[i], enviroments[i].JugadorActivo(), profundidad + 1, COTA_PROF, static_cast<Environment::ActionType>(enviroments[i].Last_Action(jug)), alpha, beta));
      return alpha;
   }
   else
   { // azul 2
      // calculamos alpha
      for (int i = 0; i < n_act; i++)
         if (alpha < beta) // podamos si se puede
            beta = min(beta, Poda(enviroments[i], enviroments[i].JugadorActivo(), profundidad + 1, COTA_PROF, static_cast<Environment::ActionType>(enviroments[i].Last_Action(jug)), alpha, beta));
      return beta;
   }
}

//--------------------- COMENTAR Desde aqui
// cout << "\n\t";
// int n_opciones = 0;
// JuegoAleatorio(aplicables, opciones, n_opciones);

// if (n_act == 0)
// {
//    (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
//    cout << " No puede realizar ninguna accion!!!\n";
//    //accion = Environment::actIDLE;
// }
// else if (n_act == 1)
// {
//    (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
//    cout << " Solo se puede realizar la accion "
//         << actual_.ActionStr(static_cast<Environment::ActionType>(opciones[0])) << endl;
//    accion = static_cast<Environment::ActionType>(opciones[0]);
// }
// else
// { // Hay que elegir entre varias posibles acciones
//    int aleatorio = rand() % n_opciones;
//    cout << " -> " << actual_.ActionStr(static_cast<Environment::ActionType>(opciones[aleatorio])) << endl;
//    accion = static_cast<Environment::ActionType>(opciones[aleatorio]);
// }

//--------------------- COMENTAR Hasta aqui