## Desconecta 4

**Nota:** No sé si gana contra el ninja 1 porque ha estado caído durante el domingo.

A continuación voy a describir una función heurística cuya función es, dado un tablero y
un determinador jugador, asignarle un número indicando cómo de bueno es ese tablero para el
jugador dado. Entendemos que un buen tablero es aquel con más posibilidades de hacer cualquier
linea de 4 fichas del juguador pasado.

Una vez tengamos esa función, la aplicamos de la siguiente forma:

```C++
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
      return (Puntuacion2(estado, jugador) / 2 + Puntuacion2(estado, jugador == 1 ? 2 : 1) * 2);
}
```

Lo único que hay que comentar es el último `return`: considerando la definición de buen tablero anterior,
tenemos más en cuenta la valoración del tablero respecto al jugador 2 (cuanto más puntos tenga, mayor
probabilidad tendrá de perder).

### Función heurística: Puntuacion2

La primera aproximación es en tener en cuenta las agrupaciones de dos o tres bloques, como es lógico. Las
considero en diagonal, vertical y horizontal. A las agrupaciones de dos les doy un valor (8) y a las de
tres otro (12).

```C++
double Puntuacion2(const Environment &estado, int jugador)
{
   double suma = 0;
   int dos = 8;
   int tres = 12;
   // dos seguidas horizontalmente
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 6; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i, j + 1) == jugador)
         {
            suma += dos;
         }
      }

   {...}

   // tres seguidas horizontalmente
   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 5; j++)
      {
         if (estado.See_Casilla(i, j) == jugador and estado.See_Casilla(i, j + 1) == jugador and estado.See_Casilla(i, j + 2) == jugador)
         {
            suma += tres;
         }
      }

   {...}

   return suma:
}
```

Lo siguiente que he hecho ha sido tener en cuenta los espacios alredeador de cada una de mis fichas,
para intentar dispersarlas. Para busca una de mis fichas, y por cada hueco que tenga alrededor, sumo 1.
En caso de que haya una de mi mismo color debajo, no hago nada ya que me podría llevar a hacer una
torre de 4. Eso está hecho con el siguiente código:

```C++
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
                        suma += 1;
                     }
                  }
               }
      }
```

Por último, he considerado las bombas: favorezco explotarlas cuando hay más fichas mias en la misma fila donde
está la bomba. Eso es hecho por:

```C++
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
```
