#include <algorithm>
#include <iostream>
#include <vector>
#include "game.h"
#include "critical.h"
#include "iostream"

using namespace std;
string inputString;
vector <string> answer;


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