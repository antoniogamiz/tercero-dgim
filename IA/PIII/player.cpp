#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
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

double Puntuacion2(const Environment &estado, int jugador)
{
   double suma = 0;
   int dos = 6;
   int tres = 14;
   // dos seguidas horizontalmente
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 6; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i, j + 1) == jugador)
         {
            suma += dos;
         }
      }

   // dos seguidas verticalmente
   for (int i = 0; i < 6; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i + 1, j) == jugador)
         {
            suma += dos;
         }
      }

   /* dos seguidas diagonalmente \ */
   for (int i = 0; i < 6; i++)
      for (int j = 0; j < 6; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i + 1, j + 1) == jugador)
         {
            suma += dos;
         }
      }

   /* dos seguidas diagonalmente / */
   for (int i = 1; i < 7; i++)
      for (int j = 0; j < 6; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i - 1, j + 1) == jugador)
         {
            suma += dos;
         }
      }

   // tres seguidas horizontalmente
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 5; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i, j + 1) == jugador and estado.See_Casilla(i, j + 2) == jugador)
         {
            suma += tres;
         }
      }

   // tres seguidas verticalmente
   for (int i = 0; i < 5; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i + 1, j) == jugador and estado.See_Casilla(i + 2, j) == jugador)
         {
            suma += tres;
         }
      }

   /* tres seguidas diagonalmente  \ */
   for (int i = 0; i < 5; i++)
      for (int j = 0; j < 5; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i + 1, j + 1) == jugador and estado.See_Casilla(i + 2, j + 2) == jugador)
         {
            suma += tres;
         }
      }

   /* tres seguidas diagonalmente  / */
   for (int i = 2; i < 5; i++)
      for (int j = 0; j < 5; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i - 1, j + 1) == jugador and estado.See_Casilla(i - 2, j + 2) == jugador)
         {
            suma += tres;
         }
      }

   // heuristica bomba
   bool bombas[7];
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador + 3)
         {
            bombas[i] = true;
         }
      }

   for (int i = 0; i < 7; i++)
   {
      if (bombas[i])
      {
         for (int j = 0; j < 7; j++)
         {

            if (estado.See_Casilla(i, j) == jugador)
            {
               suma += 2;
            }
         }
      }
   }

   // considerando huecos
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 7; j++)
      {
         if (i - 1 >= 0 and estado.See_Casilla(i - 1, j) != jugador and estado.See_Casilla(i, j) == jugador)
            for (int k = i - 1; k < i + 2; k++)
               for (int l = j - 1; l < j + 2; l++)
               {
                  if (k >= 0 and l >= 0 and k < 7 and l < 7)
                  {
                     if (estado.See_Casilla(k, l) == 0)
                     {
                        suma += 1.5;
                     }
                  }
               }
      }

   return suma;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador)
{
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
   {
      return -99999999.0; // Pierde el jugador que pide la valoracion
   }
   else if (estado.Get_Casillas_Libres() == 0)
      return 0; // Hay un empate global y se ha rellenado completamente el tablero
   else
      return Puntuacion2(estado, jugador) * 0.5 + Puntuacion2(estado, jugador == 1 ? 2 : 1) * 2;
} // Puntuacion(jugador, estado)

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

Environment::ActionType Player::Think()
{
   const int PROFUNDIDAD_ALFABETA = 6;

   Environment::ActionType accion;
   bool aplicables[8];

   actual_.possible_actions(aplicables);

   double values[8];
   for (int i = 0; i < 8; i++)
   {
      values[i] = 0;
   };

   Environment enviroments[8];
   int n_act = actual_.GenerateAllMoves(enviroments);

   for (int i = 0; i < 8; i++)
   {
      if (aplicables[i])
      {
         values[i] = Poda(enviroments[i], jugador_, 0, PROFUNDIDAD_ALFABETA, -99999999.0, 99999999.0);
      }
   }

   double max = -999999999.0;
   int index = -1;
   for (int i = 0; i < 8; i++)
   {
      if (values[i] > max && aplicables[i])
      {
         index = i;
         max = values[i];
      }
   };

   return static_cast<Environment::ActionType>(index);
}

double Player::Poda(const Environment envi, const int jug, int profundidad, int COTA_PROF, double alpha, double beta)
{
   if (profundidad == COTA_PROF || envi.JuegoTerminado())
      return Valoracion(envi, jug);

   Environment enviroments[8];
   int n_act = envi.GenerateAllMoves(enviroments);

   double value;
   bool maxmin = envi.JugadorActivo() == jug;
   if (maxmin)
   {
      value = -99999999.0;
      for (int i = 0; i < n_act; i++)
      {
         value = max(value, Poda(enviroments[i], jug, profundidad + 1, COTA_PROF, alpha, beta));
         alpha = max(value, alpha);
         if (alpha > beta)
            return value;
      }
      return value;
   }
   else
   {
      value = 99999999.0;
      for (int i = 0; i < n_act; i++)
      {
         value = min(value, Poda(enviroments[i], jug, profundidad + 1, COTA_PROF, alpha, beta));
         beta = min(value, beta);
         if (alpha > beta)
            return value;
      }
      return value;
   }
}
