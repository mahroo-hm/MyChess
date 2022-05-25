#ifndef CRITICAL_H
#define CRITICAL_H

#include "game.h"
#include "vector"
using namespace std;


string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj);
bool case2(Game& game, char attacker, int depth = 0);
bool case1(Game& game, char defender, int depth = 0);
vector<string> abreviateMoves(vector<string> moves, Game game);

#endif