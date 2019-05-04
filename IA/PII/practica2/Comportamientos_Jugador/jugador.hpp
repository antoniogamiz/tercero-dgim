#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <stack>

struct estado
{
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento
{
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Inicializar Variables de Estado
    fil = col = 99;
    brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
    destino.fila = -1;
    destino.columna = -1;
    destino.orientacion = -1;
    ultimaAccion = actIDLE;
    hayPlan = false;
    pkencontrado = false;
    buf = stack<pair<Action, vector<unsigned char>>>();
    plan = list<Action>();
  }
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializar Variables de Estado
    fil = col = 99;
    brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
    destino.fila = -1;
    destino.columna = -1;
    destino.orientacion = -1;
    ultimaAccion = actIDLE;
    hayPlan = false;
    pkencontrado = false;
    buf = stack<pair<Action, vector<unsigned char>>>();
    plan = list<Action>();
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void VisualizaPlan(const estado &st, const list<Action> &plan);
  ComportamientoJugador *clone() { return new ComportamientoJugador(*this); }

private:
  // Declarar Variables de Estado
  int fil, col, brujula;
  estado actual, destino;
  list<Action> plan;

  // Nuevas variable de estado
  Action ultimaAccion;
  bool hayPlan;
  bool pkencontrado;
  stack<pair<Action, vector<unsigned char>>> buf;

  // Métodos privados de la clase
  bool
  pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_Peso(const estado &origen, const estado &destino, list<Action> &plan);

  void PintaPlan(list<Action> plan);
  bool HayObstaculoDelante(estado &st);

  // nuevos metodos
  Action movimientoReactivo(Sensores sensores);
  int calcularPeso(list<Action> &plan);
  int getPeso(const estado &casilla);
  Action randomMove(Sensores sensores);
  estado move(const estado &st, Action accion);
  bool HayObstaculoDelante2(Sensores sensores);
  void updateView(const estado &pos, vector<unsigned char> &terreno);
  void updateViewAldeanos(const estado &pos, vector<unsigned char> &terreno);
  int updateBrujula(int current, Action accion);
  estado undoMove(const estado &st, Action accion);
  void Pintar(const estado &st);
};

#endif
