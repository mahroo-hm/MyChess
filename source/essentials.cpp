#include "essentials.h"
#include "iostream"




string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj){
    string output;
    output.push_back((char) fromj + 'a');
    output.push_back((char) (7 - fromi) + '1');
    output += pieceName;
    output.push_back((char) toj + 'a');
    output.push_back((char) (7 - toi) + '1');
    return output;
}

Vector2f getCellPosition(int row,int column) {
    return Vector2f(
        cellOffset + column * cellSize,
        cellOffset + row * cellSize);
}

int getCellIndex(int x) {
    x -= cellOffset;
    if (x < 0)
        return -1;
    int index = x / cellSize;
    if (index > 2 || x > index * cellSize + cellSize)
        return -1;
    return index;
}

string** restart(){
    string** firstBoard = new string*[8];
    for (int i = 0; i < 8; i++) firstBoard[i] = new string[8];
    firstBoard[0][0] = "RB";
    firstBoard[0][1] = "NB";
    firstBoard[0][2] = "BB";
    firstBoard[0][3] = "QB";
    firstBoard[0][4] = "KB";
    firstBoard[0][5] = "BB";
    firstBoard[0][6] = "NB";
    firstBoard[0][7] = "RB";
    for (int i = 0; i < 8; i++)
        firstBoard[1][i] = "PB";
    
    firstBoard[7][0] = "RW";
    firstBoard[7][1] = "NW";
    firstBoard[7][2] = "BW";
    firstBoard[7][3] = "QW";
    firstBoard[7][4] = "KW";
    firstBoard[7][5] = "BW";
    firstBoard[7][6] = "NW";
    firstBoard[7][7] = "RW";
    for (int i = 0; i < 8; i++)
        firstBoard[6][i] = "PW";

    return firstBoard;    
}

/*string getPiecePath(Piece p, string type) {
    string path = "resources/pieces/"; 
    path += p.getName();
    path += type;
    return path + ".png";
}*/
