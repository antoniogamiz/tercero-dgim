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

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Player::Valoracion(const Environment &estado, int jugador)
{
    int ganador = estado.RevisarTablero();
    double resultado;
    int contrincante = 1;
    if (jugador == 1)
        contrincante = 2;

    if (ganador == jugador)
        return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador != 0)
        return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres() == 0)
        return 0; // Hay un empate global y se ha rellenado completamente el tablero
    else
    {
        resultado -= contarTablero(estado, jugador) + contarTablero(estado, contrincante) * 2;
    }
    return resultado;
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

    cout << "Puntuación actual del tablero para el jugador " << jugador_ << " es de " << contarTablero(actual_, jugador_);

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

    //--------------------- COMENTAR Desde aqui
    /*

    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
         */
    //--------------------- COMENTAR Hasta aqui

    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------
    Environment estados_siguientes[8];

    int n_estados_siguientes;

    //Calcular la lista de estados siguientes
    n_estados_siguientes = actual_.GenerateAllMoves(estados_siguientes);
    bool hayJugadaGanadora = false;
    //Iterar por la lista de estados siguientes
    for (int i = 0; i < n_estados_siguientes && !hayJugadaGanadora; i++)
    {
        Environment e = estados_siguientes[i];
        //Vamos a ver si se trata de un estado ganador
        //Revisra tablero devuelve 1, 0, 2 depende si gana jugador 1, hay empate o gana el jugador 2 respectivamente. Por lo que si se cumple lo de abajo el jugador actual habrá ganado
        if (e.RevisarTablero() == jugador_)
        {
            int n_action = e.Last_Action(jugador_);
            accion = static_cast<Environment::ActionType>(n_action);
            hayJugadaGanadora = true;
        }
    }

    if (!hayJugadaGanadora)
    {
        //Como no hay estado siguiente ganador, seleccionamos uno aleatoriamente
        int cero = 0;
        valor = miniMax(actual_, jugador_, 0, PROFUNDIDAD_MINIMAX, accion);
        /*  int seleccion_aleatoria = rand() % n_estados_siguientes;

      Environment estado_aleatorio;
      estado_aleatorio = estados_siguientes[seleccion_aleatoria];
      int n_action = estado_aleatorio.Last_Action(jugador_);
      accion = static_cast< Environment::ActionType > (n_action);*/
    }

    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    // valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}

//Devuelve la puntuacion asignada al contar la fila de esa posición (a menor valor, mejor es el tablero)

int Player::contarFichaFila(const Environment &estado, int fila, int columna, int jugador)
{
    int resultado = 0;
    if (columna != 6)
    {
        if (columna < 6)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
                if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1))
                    resultado = 1;
        }
        //Mirar si hay dos espacio una, o una espacio dos
        if (columna < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if (((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1)) and (estado.See_Casilla(fila, columna + 1) % 3 == estado.See_Casilla(fila, columna + 3))) or ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 2)) and (estado.See_Casilla(fila, columna + 2) % 3 == estado.See_Casilla(fila, columna + 3))))
                {
                    resultado = 2;
                }
            }
        }

        // Miramos si hay tres juntos
        if (columna < 5)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1)) and (estado.See_Casilla(fila, columna + 1) % 3 == estado.See_Casilla(fila, columna + 2) % 3))
                {
                    resultado = 3;
                }
            }
        }

        //Miramos Si hay Cuatro dosJuntas

        if (columna < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1)) and (estado.See_Casilla(fila, columna + 1) % 3 == estado.See_Casilla(fila, columna + 2) % 3) and
                    (estado.See_Casilla(fila, columna + 2) % 3 == estado.See_Casilla(fila, columna + 3) % 3))
                {
                    resultado = 4;
                }
            }
        }

        if (columna < 5)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
                if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1))
                    resultado = 5;
        }
    }

    switch (resultado)
    {
    case 0:
        return CERO;
    case 1:
        return UNO;
    case 2:
        return DOSUNO;
    case 3:
        return TRES;
    case 4:
        return CUATRO;
    case 5:
        return CINCO;
    }
}

int Player::contarFichaColumna(const Environment &estado, int fila, int columna, int jugador)
{
    int resultado = 0;
    if (fila != 6)
    {
        if (fila < 6)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
                if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna))
                    resultado = 1;
        }
        //Mirar si hay dos espacio una, o una espacio dos
        if (fila < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if (((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna)) and (estado.See_Casilla(fila + 1, columna) % 3 == estado.See_Casilla(fila + 3, columna))) or ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 2, columna)) and (estado.See_Casilla(fila + 2, columna) % 3 == estado.See_Casilla(fila + 3, columna))))
                {
                    resultado = 2;
                }
            }
        }

        // Miramos si hay tres juntos
        if (fila < 5)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna)) and (estado.See_Casilla(fila + 1, columna) % 3 == estado.See_Casilla(fila + 2, columna) % 3))
                {
                    resultado = 3;
                }
            }
        }

        //Miramos Si hay Cuatro dosJuntas

        if (fila < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna)) and (estado.See_Casilla(fila + 1, columna) % 3 == estado.See_Casilla(fila + 2, columna) % 3) and
                    (estado.See_Casilla(fila + 2, columna) % 3 == estado.See_Casilla(fila + 3, columna) % 3))
                {
                    resultado = 4;
                }
            }
        }
        if (fila < 5)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
                if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila, columna + 1))
                    resultado = 5;
        }
    }

    switch (resultado)
    {
    case 0:
        return CERO;
    case 1:
        return UNO;
    case 2:
        return DOSUNO;
    case 3:
        return TRES;
    case 4:
        return CUATRO;
    case 5:
        return CINCO;
    }
}

int Player::contarFichaDiagonalAscendente(const Environment &estado, int fila, int columna, int jugador)
{
    int resultado = 0;

    if (fila != 6 && columna != 6)
    {

        //Vamos a contar si hay cuatro seguidas
        if (fila < 4 && columna < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna + 1)) and (estado.See_Casilla(fila + 1, columna + 1) % 3 == estado.See_Casilla(fila + 2, columna + 2) % 3) and
                    (estado.See_Casilla(fila + 2, columna + 2) % 3 == estado.See_Casilla(fila + 3, columna + 3) % 3))
                {
                    resultado = 4;
                }
            }
        }

        if (resultado != 4)
        {
            if (fila < 5 && columna < 5)
            {
                if (estado.See_Casilla(fila, columna) % 3 == jugador)
                {
                    if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna + 1)) and (estado.See_Casilla(fila + 1, columna + 1) % 3 == estado.See_Casilla(fila + 2, columna + 2) % 3))
                    {
                        resultado = 3;
                    }
                }
            }
            if (resultado != 3)
            {
                if (fila < 4 && columna < 4)
                {
                    if (estado.See_Casilla(fila, columna) % 3 == jugador)
                    {
                        if (((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna + 1)) and (estado.See_Casilla(fila + 1, columna + 1) % 3 == estado.See_Casilla(fila + 3, columna + 3))) or ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 2, columna + 2)) and (estado.See_Casilla(fila + 2, columna + 2) % 3 == estado.See_Casilla(fila + 3, columna + 3))))
                        {
                            resultado = 2;
                        }
                    }
                }
                if (resultado != 2)
                {
                    if (fila < 6 && columna < 6)
                    {
                        if (estado.See_Casilla(fila, columna) % 3 == jugador)
                            if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila + 1, columna + 1))
                                resultado = 1;
                    }
                }
            }
        }
    }
    switch (resultado)
    {
    case 0:
        return CERO;
    case 1:
        return UNO;
    case 2:
        return DOSUNO;
    case 3:
        return TRES;
    case 4:
        return CUATRO;
    }
}

int Player::contarFichaDiagonalDescendente(const Environment &estado, int fila, int columna, int jugador)
{
    int resultado = 0;

    if (fila != 0 && columna != 6)
    {

        //Vamos a contar si hay cuatro seguidas
        if (fila > 2 && columna < 4)
        {
            if (estado.See_Casilla(fila, columna) % 3 == jugador)
            {
                if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila - 1, columna + 1)) and (estado.See_Casilla(fila - 1, columna + 1) % 3 == estado.See_Casilla(fila - 2, columna + 2) % 3) and
                    (estado.See_Casilla(fila - 2, columna + 2) % 3 == estado.See_Casilla(fila - 3, columna + 3) % 3))
                {
                    resultado = 4;
                }
            }
        }

        if (resultado != 4)
        {
            if (fila > 3 && columna < 5)
            {
                if (estado.See_Casilla(fila, columna) % 3 == jugador)
                {
                    if ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila - 1, columna + 1)) and (estado.See_Casilla(fila - 1, columna + 1) % 3 == estado.See_Casilla(fila - 2, columna + 2) % 3))
                    {
                        resultado = 3;
                    }
                }
            }
            if (resultado != 3)
            {
                if (fila > 2 && columna < 4)
                {
                    if (estado.See_Casilla(fila, columna) % 3 == jugador)
                    {
                        if (((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila - 1, columna + 1)) and (estado.See_Casilla(fila - 1, columna + 1) % 3 == estado.See_Casilla(fila - 3, columna + 3))) or ((estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila - 2, columna + 2)) and (estado.See_Casilla(fila - 2, columna + 2) % 3 == estado.See_Casilla(fila - 3, columna + 3))))
                        {
                            resultado = 2;
                        }
                    }
                }
                if (resultado != 2)
                {
                    if (fila > 0 && columna < 6)
                    {
                        if (estado.See_Casilla(fila, columna) % 3 == jugador)
                            if (estado.See_Casilla(fila, columna) % 3 == estado.See_Casilla(fila - 1, columna + 1))
                                resultado = 1;
                    }
                }
            }
        }
    }
    switch (resultado)
    {
    case 0:
        return CERO;
    case 1:
        return UNO;
    case 2:
        return DOSUNO;
    case 3:
        return TRES;
    case 4:
        return CUATRO;
    }
}

int Player::contarTablero(const Environment &estado, int jugador)
{
    int puntTablero = 0;
    for (int fil = 0; fil < 7; fil++)
    {
        for (int col = 0; col < 7; col++)
        {
            if (estado.See_Casilla(fil, col) % 3 == jugador)
            {
                puntTablero += contarFichaFila(estado, fil, col, jugador) +
                               contarFichaColumna(estado, fil, col, jugador) +
                               contarFichaDiagonalAscendente(estado, fil, col, jugador) +
                               contarFichaDiagonalDescendente(estado, fil, col, jugador);
            }
        }
    }
    return puntTablero;
}

double Player::miniMax(const Environment &tab, int jugador, int profundidad, const int maxProf, Environment::ActionType &best)
{
    // Condición de parada de la recursividad

    if (tab.JuegoTerminado() or profundidad == maxProf)
    {
        double val = Valoracion(tab, jugador);
        cout << " Valoración " << val << endl;
        return val;
    }
    else
    {

        double mejor, posible;
        Environment::ActionType mejorAccion;
        Environment estados_siguientes[8];
        int n_mov_pos = tab.GenerateAllMoves(estados_siguientes);

        // Estoy en un nodo MAX
        if (profundidad % 2 == 0)
        {
            cout << "Estoy en profundidad mejor caso para mi" << endl;
            mejor = MININT;
            for (int i = 0; i < n_mov_pos; i++)
            {
                posible = miniMax(estados_siguientes[i], jugador, profundidad + 1, maxProf, best);
                cout << "Posible " << profundidad << " es: " << posible << endl;
                if (posible > mejor)
                {
                    mejor = posible;
                    cout << " \neo: " << mejor << endl;
                    cout << "Antes de casting";
                    if (profundidad == 0)
                        best = static_cast<Environment::ActionType>(estados_siguientes[i].Last_Action(jugador));
                    cout << "Despues de casting";
                }
            }

            return mejor;
        }
        else
        {
            mejor = MAXINT;
            for (int i = 0; i < n_mov_pos; i++)
            {
                posible = miniMax(estados_siguientes[i], jugador, profundidad + 1, maxProf, best);
                if (posible < mejor)
                {
                    mejor = posible;
                    cout << "/neo: " << mejor << endl;
                }
            }
            return mejor;
        }
    }
}