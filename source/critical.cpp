#include "game.h"
#include "iostream"

using namespace std;
vector <string> answer;


string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj){
    string output;
    output.push_back((char) fromj + 'a');
    output.push_back((char) (7 - fromi) + '1');
    output += pieceName;
    output.push_back((char) toj + 'a');
    output.push_back((char) (7 - toi) + '1');
    return output;
}

bool case2(Game& game, char attacker, int depth = 0){
    char defender = (attacker == 'B') ? 'W' : 'B';
    vector <pair<int, int>> validmoves;

    if (depth == 3)
        return game.isCheckMate(defender);

    if (game.isCheckMate(defender)) return false;
    if (game.isCheckMate(attacker)) return false;

    int mvno = 0;

    for (Piece p : game.gameBoard.getPieces(game.playerTurn)) {
        for (int i = 0; i < validmoves.size(); i++) cout << validmoves[i].first << ' ' << validmoves[i].second << " -- " << endl;
        //validmoves.clear();
        //cout << " name: " << p.getName();
        vector<pair<int, int> > mv = game.gameBoard.getValidMoves(p);
        
        for (int i = 0; i < mv.size(); i++) {
            bool x = game.move(p, mv[i]);
            //cout <<"***" << " depth: "<< depth << ' ' << p.getName() << ' ' << mv[i].first << ' ' << mv[i].second << endl;

            if (x) {
                mvno++;
                //cout << " ACCEPTED " << endl;
                bool res = case2(game, attacker, depth + 1);

                if (res && game.playerTurn != attacker) {
                    if (depth == 0)
                        answer.push_back(standardOutput(p.x, p.y, p.getName(), mv[i].first, mv[i].second));
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

bool case1(Game& game, char defender, int depth = 0){
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
                bool res = case1(game, defender, depth + 1);
                if (depth == 0 && res) {
                    answer.push_back(standardOutput(p.x, p.y, p.getName(), mv[i].first, mv[i].second));
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