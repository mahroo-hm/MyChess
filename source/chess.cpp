#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
string inputString;
vector <string> answer;


class Piece {
public:
    char color;
    char type;
    int x, y;
    Piece() {}
    Piece(char color, char type, int x, int y) {
        this->color = color;
        this->type = type;
        this->x = x;
        this->y= y;
    }

    string getName() {
        string s;
        s.push_back(type);
        s.push_back(color);
        return s;
    }

    Piece* clone() {
        return new Piece(color, type, x, y);
    }
};

class Board {
public:
    Piece board[8][8];
    
    void setBoard(string** inpBoard) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) 
                board[i][j] = *(new Piece(inpBoard[i][j][1], inpBoard[i][j][0], i, j));
    }

    Board () {}
    Board (string** board) {
        this->setBoard(board);
    }

    Piece getPiece(int x, int y) {
        return board[x][y];
    }

    bool isPointValid(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8; 
    }

    vector<pair<int, int> > getValidMoves(Piece p) {
        switch (p.type) {
        case 'K':
            return getKingMoves(p);
            break;
        case 'Q':
            return getQueenMoves(p);
            break;
        case 'B':
            return getBishopMoves(p);
            break;
        case 'R':
            return getRookMoves(p);
            break;
        case 'N':
            return getKnightMoves(p);
            break;
        case 'P':
            return getPawnMoves(p);
            break;
        default:
            pair<int, int> nullpair(0, 0);
            vector<pair<int, int> > nullvect;
            nullvect.push_back(nullpair);
            return nullvect;
            break;
        }
    }

    vector<Piece> getPieces(char color) {
        vector <Piece> playerPieces;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (board[i][j].color == color)
                    playerPieces.push_back(board[i][j]);
            }
        }
        return playerPieces;
    }

    vector<pair<int, int> > getKingMoves(Piece p) {
        vector<pair<int, int> > mv;
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++) {
                int nx = p.x + i, ny = p.y + j;
                if ((i == 0 && j == 0) || !isPointValid(nx, ny) || board[nx][ny].color == p.color || board[nx][ny].type == 'K')
                    continue;
                bool flag = true;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int nxx = nx + i, nyy = ny + j;
                        if (isPointValid(nxx, nyy) && board[nxx][nyy].type == 'K' && board[nxx][nyy].color != p.color) {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag)
                        break;
                }
                if (flag)
                    mv.push_back({nx, ny});
            }
        }
        return mv;
    }

    vector<pair<int, int> > getMovesByD(vector<int> dx, vector<int> dy, Piece p) {
        vector<pair<int, int> > mv;
        for (int k = 0; k < dx.size(); k++)
            for (int i = 1; i <= 8; i++) {
                int nx = p.x + dx[k] * i, ny = p.y + dy[k] * i;
                if (!isPointValid(nx, ny)) continue;
                if (board[nx][ny].color == p.color || board[nx][ny].type == 'K'){
                    break;
                }
                mv.push_back({nx, ny});
                if (board[nx][ny].color != '-'){
                    break;
                }
            }
        return mv;
    }

    vector<pair<int, int> > getBishopMoves(Piece p) {
        vector<int> dx = {1, 1, -1, -1};
        vector<int> dy = {-1, 1, 1, -1};
        return getMovesByD(dx, dy, p);
    }

    vector<pair<int, int> > getRookMoves(Piece p) {
        vector<int> dx = {0, 0, 1, -1};
        vector<int> dy = {-1, 1, 0, 0};
        return getMovesByD(dx, dy, p);
    }

    vector<pair<int, int> > getQueenMoves(Piece p) {
        vector<pair<int, int>> rookMv = getRookMoves(p);
        vector<pair<int, int>> bishopMv = getBishopMoves(p);
        for (auto i : bishopMv){
            rookMv.push_back(i);
        }
        return rookMv;
    }

    vector<pair<int, int> > getKnightMoves(Piece p) {
        vector<pair<int, int> > mv;
        int dx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
        int dy[8] = {2, -2, 1, -1, 2, -2, 1, -1};
        for (int k = 0; k < 8; k++) {
            int nx = p.x + dx[k], ny = p.y + dy[k];
            if (!isPointValid(nx, ny) || board[nx][ny].type == 'K' || board[nx][ny].color == p.color)
                continue;
            mv.push_back({nx, ny});
        }
        return mv;
    }

    vector<pair<int, int> > getPawnMoves(Piece p) {
        vector<pair<int, int> > mv;
        for (int j = -1; j <= 1; j++) {
            int nx = p.x + 1 + (p.color == 'W') * -2, ny = p.y + j;
            if (isPointValid(nx, ny) && j == 0 && board[nx][ny].type == '-'){
                if (p.x == 1 && p.color == 'B' && board[nx + 1][ny].type == '-')
                    mv.push_back({nx + 1, ny});
                else if (p.x == 6 && p.color == 'W' && board[nx - 1][ny].type == '-')
                    mv.push_back({nx - 1, ny});
                mv.push_back({nx, ny});
            }
            if (isPointValid(nx, ny) && j != 0 && board[nx][ny].color != p.color && board[nx][ny].color != '-' && board[nx][ny].type != 'K')
                mv.push_back({nx, ny});
        }
        return mv;
    }
};

class Game{
friend class Board;
private:
    vector<Piece> store;
    bool isPointValid(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8; 
    }
public:
    Board gameBoard;
    char playerTurn;

    Game(char turn, string** board) {
        playerTurn = turn;
        this->gameBoard.setBoard(board);
    }

    int* findKing(char KingColor){
        int* LocKing = new int[2];
        LocKing[0] = -1;
        LocKing[1] = -1;
        bool KingIsFound = false;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (this->gameBoard.board[i][j].color == KingColor && this->gameBoard.board[i][j].type == 'K') {
                    LocKing[0] = i;
                    LocKing[1] = j;
                    KingIsFound = true;
                    break;
                }
            }
            if (KingIsFound) break;
        }
        return LocKing;
    }

    bool isCheck(char color){
        int x, y;
        int* locKing = new int[2];
        locKing = findKing(color);
        x = locKing[0];
        y = locKing[1];
        delete[] locKing;

        int dxN[8] = {1, 1, 2, 2, -1, -1, -2, -2};
        int dyN[8] = {2, -2, 1, -1, 2, -2, 1, -1};
        for (int k = 0; k < 8; k++) {
            int nx = x + dxN[k], ny = y + dyN[k];
            if (isPointValid(nx, ny) && gameBoard.board[nx][ny].type == 'N' && gameBoard.board[nx][ny].color != color)
                return true;
        }

        vector<int> dx = {1, 1, -1, -1};
        vector<int> dy = {-1, 1, 1, -1};
        for (int k = 0; k < 4; k++){
            for (int i = 1; i <= 8; i++) {
                int nx = x + dx[k] * i, ny = y + dy[k] * i;
                if (isPointValid(nx, ny) && gameBoard.board[nx][ny].color != color){
                    if (i == 1){
                        if (gameBoard.board[nx][ny].type == 'P' && gameBoard.board[nx][ny].color == 'W' && nx > x){
                            return true;
                        }
                        if (gameBoard.board[nx][ny].type == 'P' && gameBoard.board[nx][ny].color == 'B' && nx < x){
                            return true;
                        }
                        if (gameBoard.board[nx][ny].type == 'K'){
                            return true;
                        }
                    }
                    if (gameBoard.board[nx][ny].type  == 'B' || gameBoard.board[nx][ny].type  == 'Q')
                        return true;
                }

                if (gameBoard.board[nx][ny].color != '-')
                    break;
            }
        }

        vector<int> dxR = {0, 0, 1, -1};
        vector<int> dyR = {-1, 1, 0, 0};
        for (int k = 0; k < 4; k++){
            for (int i = 1; i <= 8; i++) {
                int nx = x + dxR[k] * i, ny = y + dyR[k] * i;
                if (isPointValid(nx, ny) && gameBoard.board[nx][ny].color != color){
                    if (i == 1)
                        if (gameBoard.board[nx][ny].type == 'K')
                            return true;
                    if (gameBoard.board[nx][ny].type == 'R' || gameBoard.board[nx][ny].type == 'Q')
                        return true;
                }
                if (gameBoard.board[nx][ny].color != '-')
                    break;
            }

        }
        return false;
    }
 
    bool isCheckMate(char color){
        if (!isCheck(color)) return false;
        else {
            for (Piece p : gameBoard.getPieces(color)) {
                vector<pair<int, int> > mv = gameBoard.getValidMoves(p);
                for (int i = 0; i < mv.size(); i++) {
                    bool x = move(p, mv[i]);
                    if(x && !isCheck(color)) {
                        reverseMove();
                        return false;
                    }
                    reverseMove();
                }
            }
        }
        return true;
    }

    void changeTurns(){
        playerTurn = (playerTurn == 'W') ? 'B' : 'W';
    }

    void reverseMove(){
        for (int i = 0; i < 2; i++) {
            Piece a = store.back();
            store.pop_back();
            gameBoard.board[a.x][a.y] = a;
        }
        changeTurns();
    }
    
    bool move(Piece p, pair<int, int> mv){
        store.push_back(*p.clone());
        store.push_back(*gameBoard.board[mv.first][mv.second].clone());
        
        gameBoard.board[p.x][p.y].color = '-'; 
        gameBoard.board[p.x][p.y].type = '-';
        p.x = mv.first, p.y = mv.second;
        gameBoard.board[mv.first][mv.second] = p;

        changeTurns();

        if(isCheck(p.color)){
            return false;
        }
        else{
            return true;
        }
    }

};

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

    /*cout << endl;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8;j++){
            cout << game.gameBoard.board[i][j].getName() << ' ';
        }
        cout << endl;
    }*/

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

int main(){
    cin >> inputString;
    string** curBoard = new string*[8];
    for (int i = 0; i < 8; i++) curBoard[i] = new string[8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            cin >> curBoard[i][j];
    
    Game game = Game(inputString[0], curBoard);
    //cout << game.isCheck('B') << endl << endl;

    if (inputString[1] == 'M')
        case2(game, inputString[0]);
    else
        case1(game, inputString[0]);
    

    vector<string> res = abreviateMoves(answer, game);
    if (res.size() > 0){
        for (int i = 0; i < res.size(); i++) 
            cout << res[i] << endl;
    }
    else{
        cout << "No Answer!" << endl;
    } 
    return 0;
}