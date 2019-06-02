#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player
{
public:
  Player(int jug);
  Environment::ActionType Think();
  void Perceive(const Environment &env);
  double Poda(const Environment envi, const int jug, int profundidad, int COTA_PROF, double alpha, double beta);

private:
  int jugador_;
  Environment actual_;
};
#endif
