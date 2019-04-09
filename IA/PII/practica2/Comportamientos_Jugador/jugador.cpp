#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores)
{
	Action sigAccion = actIDLE;

	// Estoy en el nivel 1
	if (sensores.nivel != 4)
	{
		//Capturar los valores de filas y columnas
		if (sensores.mensajeF != -1)
		{
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			ultimaAccion = actIDLE;
		}

		// Actualizar el efecto de la ultima accion
		switch (ultimaAccion)
		{
		case actTURN_R:
			brujula = (brujula + 1) % 4;
			break;
		case actTURN_L:
			brujula = (brujula + 3) % 4;
			break;
		case actFORWARD:
			switch (brujula)
			{
			case 0:
				fil--;
				break;
			case 1:
				col++;
				break;
			case 2:
				fil++;
				break;
			case 3:
				col--;
				break;
			}
			break;
		}

		// Mirar si ha cambiado el destino
		if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)
		{
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		//Calcular un camino hasta el destino
		if (!hayPlan)
		{
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
		}

		// Si hay plan, se sigue. En caso contrario comportamiento reactivo
		if (hayPlan and plan.size() > 0)
		{
			sigAccion = plan.front();
			plan.erase(plan.begin());
		}
		else
		{
			sigAccion = movimientoReactivo(sensores);
		}
	}
	else
	{
		//no sabemos destino todavia
		if (destino.fila == -1)
		{
			sigAccion = randomMove(sensores);
		}

		if (sensores.mensajeF != -1)
		{
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			cout << "Punto de referencia encontrado!" << endl;
			mapaResultado[fil][col] = 'K';
		}
	}

	//Recordar la ultima accion
	ultimaAccion = sigAccion;

	return sigAccion;
}

// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan)
{
	switch (level)
	{
	case 1:
		cout << "Busqueda en profundad\n";
		return pathFinding_Profundidad(origen, destino, plan);
		break;
	case 2:
		cout << "Busqueda en Anchura\n";
		return pathFinding_Anchura(origen, destino, plan);
		break;
	case 3:
		cout << "Busqueda Costo Uniforme\n";
		return pathFinding_Peso(origen, destino, plan);
		break;
	case 4:
		cout << "Busqueda para el reto\n";
		// Incluir aqui la llamada al algoritmo de búsqueda usado en el nivel 2
		break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M' or casilla == 'D' or casilla == 'a')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.

bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		col++;
		break;
	case 2:
		fil++;
		break;
	case 3:
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct nodo
{
	estado st;
	list<Action> secuencia;
	int peso = INT_MAX;

	bool operator()(const nodo &lhs, const nodo &rhs)
	{
		return lhs.peso > rhs.peso;
	}
};

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};

// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;					   // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

	while (!pila.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		if (generados.find(hijoTurnR.st) == generados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		if (generados.find(hijoTurnL.st) == generados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (generados.find(hijoForward.st) == generados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty())
		{
			current = pila.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> generados; // Lista de Cerrados
	queue<nodo> cola;					   // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

	while (!cola.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		cola.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		if (generados.find(hijoTurnR.st) == generados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			cola.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		if (generados.find(hijoTurnL.st) == generados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (generados.find(hijoForward.st) == generados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!cola.empty())
		{
			current = cola.front();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

bool ComportamientoJugador::pathFinding_Peso(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> generados;		   // Lista de Cerrados
	priority_queue<nodo, vector<nodo>, nodo> cola; // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

	while (!cola.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		cola.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		if (generados.find(hijoTurnR.st) == generados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			hijoTurnR.peso = calcularPeso(hijoTurnR.secuencia);
			cola.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		if (generados.find(hijoTurnL.st) == generados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			hijoTurnL.peso = calcularPeso(hijoTurnL.secuencia);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (generados.find(hijoForward.st) == generados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				hijoForward.peso = calcularPeso(hijoForward.secuencia);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!cola.empty())
		{
			current = cola.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.columna++;
				break;
			case 2:
				cst.fila++;
				break;
			case 3:
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 4;
		}
		else
		{
			cst.orientacion = (cst.orientacion + 3) % 4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

Action ComportamientoJugador::movimientoReactivo(Sensores sensores)
{
	Action sigAccion = actIDLE;
	unsigned char casilla;
	switch (brujula)
	{
	case 0:
		casilla = mapaResultado[fil - 1][col];
		break;
	case 1:
		casilla = mapaResultado[fil][col + 1];
		break;
	case 2:
		casilla = mapaResultado[fil + 1][col];
		break;
	case 3:
		casilla = mapaResultado[fil][col - 1];
		break;
	}

	if (casilla == 'P' or casilla == 'M' or
		casilla == 'D' or sensores.superficie[2] == 'a')
	{
		sigAccion = actTURN_R;
	}
	else
	{
		sigAccion = actFORWARD;
	}

	return sigAccion;
}

int ComportamientoJugador::calcularPeso(list<Action> &plan)
{
	estado current = actual;
	int peso = 0;
	int distance;
	for (list<Action>::iterator it = plan.begin(); it != plan.end(); ++it)
	{
		distance = sqrt((destino.fila - current.fila) ^ 2 + (destino.columna - current.columna) ^ 2);
		// prettier-ignore
		switch (*it)
		{
		case actTURN_R:
			current.orientacion = (current.orientacion + 1) % 4;
			peso += 1;
			break;
		case actTURN_L:
			current.orientacion = (current.orientacion + 3) % 4;
			peso += 1;
			break;
		case actFORWARD:
			peso += getPeso(current);
			switch (current.orientacion)
			{
			case 0:
				current.fila--;
				break;
			case 1:
				current.columna++;
				break;
			case 2:
				current.fila++;
				break;
			case 3:
				current.columna--;
				break;
			}
			break;
		}
	}
	return peso + distance;
}

int ComportamientoJugador::getPeso(const estado &casilla)
{
	switch (mapaResultado[casilla.fila][casilla.columna])
	{
	case 'A':
		return 10;
		break;
	case 'B':
		return 5;
		break;
	case 'S':
		return 2;
		break;
	default:
		return 1;
	}
}

// devuelve un movimiento random valido
Action ComportamientoJugador::randomMove(Sensores sensores)
{
	Action sigAccion = actIDLE;
	srand(time(NULL));
	float num = (1 + rand() % 100) / 100.0;
	if (num < 0.1)
	{
		sigAccion = actTURN_L;
	}
	else if (0.1 <= num and num < 0.2)
	{
		sigAccion = actTURN_R;
	}
	else
	{
		sigAccion = actFORWARD;
	}
	if (HayObstaculoDelante2(sensores) and sigAccion == actFORWARD)
	{
		sigAccion = actTURN_R;
	}

	return sigAccion;
}

estado ComportamientoJugador::move(const estado &st, Action accion)
{
	estado nst = st;
	switch (st.orientacion)
	{
	case 0:
		switch (accion)
		{
		case actFORWARD:
			nst.fila--;
			break;
		case actTURN_R:
			nst.orientacion = (nst.orientacion + 1) % 4;
			break;
		case actTURN_L:
			nst.orientacion = (nst.orientacion + 3) % 4;
			break;
		}
		break;
	case 1:
		switch (accion)
		{
		case actFORWARD:
			nst.columna++;
			break;
		case actTURN_R:
			nst.orientacion = (nst.orientacion + 1) % 4;
			break;
		case actTURN_L:
			nst.orientacion = (nst.orientacion + 3) % 4;
			break;
		}
		break;
	case 2:
		switch (accion)
		{
		case actFORWARD:
			nst.fila++;
			break;
		case actTURN_R:
			nst.orientacion = (nst.orientacion + 1) % 4;
			break;
		case actTURN_L:
			nst.orientacion = (nst.orientacion + 3) % 4;
			break;
		}
		break;
	case 3:
		switch (accion)
		{
		case actFORWARD:
			nst.columna--;
			break;
		case actTURN_R:
			nst.orientacion = (nst.orientacion + 1) % 4;
			break;
		case actTURN_L:
			nst.orientacion = (nst.orientacion + 3) % 4;
			break;
		}
		break;
	}
	return nst;
}

bool ComportamientoJugador::HayObstaculoDelante2(Sensores sensores)
{
	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(sensores.terreno[2]) and !EsObstaculo(sensores.superficie[2]))
	{
		return false;
	}
	else
	{
		return true;
	}
}