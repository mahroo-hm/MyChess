#include "critical.h"
#include "iostream"

using namespace std;


/*bool mate(Game game, char attacker,vector < pair<Piece, pair<int, int>> > &answer, int depth){
    char defender = (attacker == 'B') ? 'W' : 'B';
    vector <pair<int, int>> validmoves;

    if (depth == 3)
        return game.isCheckMate(defender);

    if (game.isCheckMate(defender)) return false;
    if (game.isCheckMate(attacker)) return false;

    int mvno = 0;

    for (Piece p : game.gameBoard.getPieces(game.playerTurn)) {
        vector<pair<int, int> > mv = game.gameBoard.getValidMoves(p);
        
        for (int i = 0; i < mv.size(); i++) {
            bool x = game.move(p, mv[i]);

            if (x) {
                mvno++;
                bool res = mate(game, attacker,answer, depth + 1);

                if (res && game.playerTurn != attacker) {
                    if (depth == 0)
                        answer.push_back(make_pair(p, mv[i]));
                    if (depth != 0) {
                        game.reverseMove();
                        return true;
                    }
                }
                if (!res && game.playerTurn == attacker) {
                    game.reverseMove();
                    return false;
                }
            }
            game.reverseMove();
        }
    }

    if (!mvno)
        return false;

    if (game.playerTurn == attacker)
        return false;
    else
        return true;
}

bool defend(Game game, char defender,vector < pair<Piece, pair<int, int>> > &answer, int depth = 0){
    char attacker = (defender == 'B') ? 'W' : 'B';
    if (game.isCheckMate(defender)) return true;
    if (game.isCheckMate(attacker)) return false;
    if (depth == 4)
        return false;

    int mvno = 0;

    for (Piece p : game.gameBoard.getPieces(game.playerTurn)) {
        vector<pair<int, int> > mv = game.gameBoard.getValidMoves(p);
        
        for (int i = 0; i < mv.size(); i++) {
            bool x = game.move(p, mv[i]);

            if (x) {
                mvno++;
                bool res = defend(game, defender, answer,  depth + 1);
                if (depth == 0 && res) {
                    answer.push_back(make_pair(p, mv[i]));
                }
                if (depth != 0) {
                    if (res && game.playerTurn != attacker) {
                        game.reverseMove();
                        return true;
                    }
                    if (!res && game.playerTurn == attacker) {
                        game.reverseMove();
                        return false;
                    }
                }
            }
            game.reverseMove();
        }
    }

    if (!mvno)
        return false;

    if (game.playerTurn == attacker)
        return false;
    else
        return true;
}

vector<string> abreviateMoves(vector<string> moves, Game game) {
    sort(moves.begin(), moves.end());

    vector<string> res;
    for (int i = 0; i < moves.size(); ) {
        int cnt = 0, j = i;
        while (j < moves.size() && moves[j][0] == moves[i][0] && moves[j][1] == moves[i][1])
            j++, cnt++;
        int y = (moves[i][0] - 'a');
        int x = 7 - (moves[i][1] - '1');
        Piece p = game.gameBoard.board[x][y];
        vector<pair<int, int> > mv = game.gameBoard.getValidMoves(p);
        int rcnt = 0;
        for (pair<int, int> m : mv) {
            rcnt += game.move(p, m);
            game.reverseMove();
        }
        if (cnt == rcnt)
            res.push_back(moves[i].substr(0, 4)), i = j;
        else { 
            for (i; i < j; i++)
                res.push_back(moves[i]);
        }
    }
    return res;
}
*/