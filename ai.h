#ifndef AI_H
#define AI_H

#include <mainwindow.h>
#include "tool.h"
#include "constants.h"

class Ai
{
public:
    explicit Ai();
    ~Ai();
    const QPoint greedAlgorithm(int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int colorFlag);
    const QPoint minMaxAlgorithm(const int &depth,int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int isComputerTerm,int colorFlag);
private:
    QPoint position;
    class Tool *tool;
    int score;
    int alpha;
    int beta;
    int getScore(int chessboard[15][15], int player_chessboard[15][15][8], int computer_chessboard[15][15][8], int color_flag);
    int minMax(const int &depth,int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int isComputerTerm,int colorFlag);
};

#endif // AI_H
