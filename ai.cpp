#include "ai.h"
#include <QDebug>

Ai::Ai()
    :tool(new Tool),
     alpha(-INFINITY),
     beta(INFINITY),
     score(0)
{
}

Ai::~Ai()
{
    delete tool;
}

/**
 * @brief Ai::greedAlgorithm
 * @param chessboard
 * @param playerChessboard
 * @param computerChessboard
 * @param colorFlag 当前玩家棋色
 * @return 下棋点
 */
const QPoint Ai::greedAlgorithm(int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int colorFlag)
{

    getScore(chessboard,playerChessboard,computerChessboard,colorFlag);

    int max_score = 0;
    QPoint targetPoint;
    targetPoint.setX(0);
    targetPoint.setY(0);

    //遍历棋型表找最佳点
    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            for (int a=0;a<8;a++){
                if (max_score < computerChessboard[i][j][a] && chessboard[i][j] ==0){
                    max_score = computerChessboard[i][j][a];
                    targetPoint.setX(i);
                    targetPoint.setY(j);
                }
                else if(max_score < playerChessboard[i][j][a]&& chessboard[i][j] ==0){
                    max_score = playerChessboard[i][j][a];
                    targetPoint.setX(i);
                    targetPoint.setY(j);
                }
            }
        }
    }

    return targetPoint;
}

/**
 * @brief Ai::minMax
 * @param depth
 * @param chessboard
 * @param playerChessboard
 * @param computerChessboard
 * @param isComputerTerm
 * @param colorFlag
 * @return
 */
int Ai::minMax(const int &depth,int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int isComputerTerm,int colorFlag)
{

    int best=0,val=0;

    if(depth==0||tool->checkWin(chessboard)==true){
        return this->getScore(chessboard,playerChessboard,computerChessboard,colorFlag);
    }
    else{
        //极大节点
        if (isComputerTerm ==true){
            best = -INFINITY;
            for (int i=0;i<15;i++){
                for (int j=0;j<15;j++){
                    if (chessboard[i][j]==EMPTY){
                        if (colorFlag ==BLACK)
                            chessboard[i][j] = WHITE;
                        else
                            chessboard[i][j] = BLACK;
                        isComputerTerm = false;

                        val = minMax(depth-1,chessboard,playerChessboard,computerChessboard,isComputerTerm,colorFlag);
                        chessboard [i][j] = EMPTY;

                        if(best<val){
                            best = val;
                            position.setX(i);
                            position.setY(j);
                        }
                    }
                }
            }
        }
        //极小节点
        else if(isComputerTerm == false){
            best = INFINITY;
            for (int i=0;i<15;i++){
                for (int j=0;j<15;j++){
                    if (chessboard[i][j]==EMPTY){
                        chessboard [i][j] = colorFlag;
                        isComputerTerm = true;
                        val = minMax(depth-1,chessboard,playerChessboard,computerChessboard,isComputerTerm,colorFlag);
                        chessboard [i][j] = EMPTY;
                        if(best>val){
                            best = val;
                        }
                    }
                }
            }
        }
    }
    return best;
}

/**
 * @brief Ai::minMaxAlgorithm
 * @param depth
 * @param chessboard
 * @param playerChessboard
 * @param computerChessboard
 * @param isComputerTerm
 * @param colorFlag
 * @return 下棋点
 */
const QPoint Ai::minMaxAlgorithm(const int &depth,int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int isComputerTerm,int colorFlag)
{
    this->minMax(depth,chessboard,playerChessboard,computerChessboard,isComputerTerm,colorFlag);
    return position;
}

/**
 * @brief Ai::getScore
 * @param chessboard
 * @param playerChessboard
 * @param computerChessboard
 * @param colorFlag
 * @return 分数
 */
int Ai::getScore(int chessboard[15][15], int playerChessboard[15][15][8], int computerChessboard[15][15][8],int colorFlag)
{
    int playerColoer = colorFlag;
    int computerColor;
    if (colorFlag == BLACK)
        computerColor = WHITE;
    else
        computerColor = BLACK;

    //置零
    int count =0;
    score = 0;
    bool dead_flag= false;
    for (int i =0;i<15;i++){
        for(int j=0;j<15;j++){
            for(int a=0;a<8;a++){
                playerChessboard[i][j][a] = EMPTY;
                computerChessboard[i][j][a] = EMPTY;
            }
        }
    }


    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            if (chessboard[i][j] ==EMPTY){

                /***玩家棋盘估分***/

                //横向
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j] == playerColoer&&i>=2)
                        count ++;
                    else if(chessboard[i-check][j] == computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j] == playerColoer&&i<=13)
                        count ++;
                    else if(chessboard[i+check][j] == computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag == false)
                        playerChessboard[i][j][0] = 5;
                    else
                        playerChessboard[i][j][0] = 3;
                }
                else if(count ==2){
                    if (dead_flag == false)
                        playerChessboard[i][j][0] = 50;
                    else
                        playerChessboard[i][j][0] = 30;
                }
                else if(count == 3){
                    if (dead_flag == false)
                        playerChessboard[i][j][0] = 500;
                    else
                        playerChessboard[i][j][0] = 300;
                }
                else if (count == 4){
                    if (dead_flag == false)
                        playerChessboard[i][j][0] = 5000;
                    else
                        playerChessboard[i][j][0] = 3000;
                }
                else if (count >=5){
                    playerChessboard[i][j][0] = 50000;
                }

                //纵向
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i][j-check] == playerColoer&&j>=2)
                        count ++;
                    else if(chessboard[i][j-check]==computerColor){
                        dead_flag= true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i][j+check] == playerColoer&&j<=13)
                        count ++;
                    else if(chessboard[i][j+check]==computerColor){
                        dead_flag= true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag ==false)
                        playerChessboard[i][j][1] = 5;
                    else
                        playerChessboard[i][j][1] = 3;
                }
                else if(count ==2){
                    if (dead_flag ==false)
                        playerChessboard[i][j][1] = 50;
                    else
                        playerChessboard[i][j][1] = 30;
                }
                else if(count == 3){
                    if (dead_flag ==false)
                        playerChessboard[i][j][1] = 500;
                    else
                        playerChessboard[i][j][1] = 300;
                }
                else if (count == 4){
                    if (dead_flag ==false)
                        playerChessboard[i][j][1] = 5000;
                    else
                        playerChessboard[i][j][1] = 3000;
                }
                else if (count >=5){
                    playerChessboard[i][j][1] = 50000;
                }

                //左斜线
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j-check] == playerColoer&&i>=2&&j>=2)
                        count ++;
                    else if(chessboard[i-check][j-check]==computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j+check] == playerColoer&&i<=13&&j<=13)
                        count ++;
                    else if(chessboard[i+check][j+check]==computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag == false)
                        playerChessboard[i][j][2] = 5;
                    else
                        playerChessboard[i][j][2] = 3;
                }
                else if(count ==2){
                    if (dead_flag == false)
                        playerChessboard[i][j][2] = 50;
                    else
                        playerChessboard[i][j][2] = 30;
                }
                else if(count == 3){
                    if (dead_flag == false)
                        playerChessboard[i][j][2] = 500;
                    else
                        playerChessboard[i][j][2] = 300;
                }
                else if (count == 4){
                    if (dead_flag == false)
                        playerChessboard[i][j][2] = 5000;
                    else
                        playerChessboard[i][j][2] = 3000;
                }
                else if (count >=5){
                    playerChessboard[i][j][2] = 50000;
                }

                //右斜线
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j-check] == playerColoer&&i<=13&&j>=2)
                        count ++;
                    else if(chessboard[i+check][j-check]==computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j+check] == playerColoer&&i>=2&&j<=13)
                        count ++;
                    else if(chessboard[i-check][j+check]==computerColor){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag == false)
                        playerChessboard[i][j][3] = 5;
                    else
                        playerChessboard[i][j][3] = 3;
                }
                else if(count ==2){
                    if (dead_flag == false)
                        playerChessboard[i][j][3] = 50;
                    else
                        playerChessboard[i][j][3] = 30;
                }
                else if(count == 3){
                    if (dead_flag == false)
                        playerChessboard[i][j][3] = 500;
                    else
                        playerChessboard[i][j][3] = 300;
                }
                else if (count == 4){
                    if (dead_flag == false)
                        playerChessboard[i][j][3] = 5000;
                    else
                        playerChessboard[i][j][3] = 3000;
                }
                else if (count >=5){
                    playerChessboard[i][j][3] = 50000;
                }

                /***电脑棋盘估分***/

                //横向
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j] == computerColor&&i>=2)
                        count ++;
                    else if(chessboard[i-check][j] == playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j] == computerColor&&i<=13)
                        count ++;
                    else if(chessboard[i+check][j] == playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag ==false)
                        computerChessboard[i][j][0] = 5;
                    else
                        computerChessboard[i][j][0] = 3;
                }
                else if(count ==2){
                    if (dead_flag ==false)
                        computerChessboard[i][j][0] = 50;
                    else
                        computerChessboard[i][j][0] = 30;
                }
                else if(count == 3){
                    if (dead_flag ==false)
                        computerChessboard[i][j][0] = 500;
                    else
                        computerChessboard[i][j][0] = 300;
                }
                else if (count == 4){
                    if (dead_flag ==false)
                        computerChessboard[i][j][0] = 5000;
                    else
                        computerChessboard[i][j][0] = 3000;
                }
                else if (count >=5){
                    computerChessboard[i][j][0] = 50000;
                }

                //纵向
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i][j-check] == computerColor&&j>=2)
                        count ++;
                    else if(chessboard[i][j-check]==playerColoer){
                        dead_flag= true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i][j+check] == computerColor&&j<=13)
                        count ++;
                    else if(chessboard[i][j+check]==playerColoer){
                        dead_flag= true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag == false)
                        computerChessboard[i][j][1] = 5;
                    else
                        computerChessboard[i][j][1] = 3;
                }
                else if(count ==2){
                    if (dead_flag == false)
                        computerChessboard[i][j][1] = 50;
                    else
                        computerChessboard[i][j][1] = 30;
                }
                else if(count == 3){
                    if (dead_flag == false)
                        computerChessboard[i][j][1] = 500;
                    else
                        computerChessboard[i][j][1] = 300;
                }
                else if (count == 4){
                    if (dead_flag == false)
                        computerChessboard[i][j][1] = 5000;
                    else
                        computerChessboard[i][j][1] = 3000;
                }
                else if (count >=5){
                    computerChessboard[i][j][1] = 50000;
                }

                //左斜线
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j-check] == computerColor&&i>=2&&j>=2)
                        count ++;
                    else if(chessboard[i-check][j-check]==playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j+check] == computerColor&&i<=13&&j<=13)
                        count ++;
                    else if(chessboard[i+check][j+check]==playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag == false)
                        computerChessboard[i][j][2] = 5;
                    else
                        computerChessboard[i][j][2] = 3;
                }
                else if(count ==2){
                    if (dead_flag == false)
                        computerChessboard[i][j][2] = 50;
                    else
                        computerChessboard[i][j][2] = 30;
                }
                else if(count == 3){
                    if (dead_flag == false)
                        computerChessboard[i][j][2] = 500;
                    else
                        computerChessboard[i][j][2] = 300;
                }
                else if (count == 4){
                    if (dead_flag == false)
                        computerChessboard[i][j][2] = 5000;
                    else
                        computerChessboard[i][j][2] = 3000;
                }
                else if (count >=5){
                    computerChessboard[i][j][2] = 50000;
                }

                //右斜线
                count = 0;
                dead_flag = false;
                for (int check =1;check<=5;check++){
                    if (chessboard[i+check][j-check] == computerColor&&i<=13&&j>=2)
                        count ++;
                    else if(chessboard[i+check][j-check]==playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                for (int check =1;check<=5;check++){
                    if (chessboard[i-check][j+check] == computerColor&&i>=2&&j<=13)
                        count ++;
                    else if(chessboard[i-check][j+check]==playerColoer){
                        dead_flag = true;
                        break;
                    }
                    else
                        break;
                }
                if (count ==1){
                    if (dead_flag ==false)
                        computerChessboard[i][j][3] = 5;
                    else
                        computerChessboard[i][j][3] = 3;
                }
                else if(count ==2){
                    if (dead_flag ==false)
                        computerChessboard[i][j][3] = 50;
                    else
                        computerChessboard[i][j][3] = 30;
                }
                else if(count == 3){
                    if (dead_flag ==false)
                        computerChessboard[i][j][3] = 500;
                    else
                        computerChessboard[i][j][3] = 300;
                }
                else if (count == 4){
                    if (dead_flag ==false)
                        computerChessboard[i][j][3] = 5000;
                    else
                        computerChessboard[i][j][3] = 3000;
                }
                else if (count >=5){
                    computerChessboard[i][j][3] = 50000;
                }


                /***棋型判断***/

                //死四活四
                if(playerChessboard[i][j][0]==300&&(playerChessboard[i][j][1]==500||playerChessboard[i][j][2]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][4] = 8000;
                if(playerChessboard[i][j][1]==300&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][2]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][4] = 8000;
                if(playerChessboard[i][j][2]==300&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][1]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][4] = 8000;
                if(playerChessboard[i][j][3]==300&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][1]==500||playerChessboard[i][j][2]==500))
                    playerChessboard[i][j][4] = 8000;

                if(computerChessboard[i][j][0]==300&&(computerChessboard[i][j][1]==500||computerChessboard[i][j][2]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][4] = 8000;
                if(computerChessboard[i][j][1]==300&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][2]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][4] = 8000;
                if(computerChessboard[i][j][2]==300&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][1]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][4] = 8000;
                if(computerChessboard[i][j][3]==300&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][1]==500||computerChessboard[i][j][2]==500))
                    computerChessboard[i][j][4] = 8000;

                //死四活三
                if(playerChessboard[i][j][0]==300&&(playerChessboard[i][j][1]==50||playerChessboard[i][j][2]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][5] = 3500;
                if(playerChessboard[i][j][1]==300&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][2]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][5] = 3500;
                if(playerChessboard[i][j][2]==300&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][1]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][5] = 3500;
                if(playerChessboard[i][j][3]==300&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][1]==50||playerChessboard[i][j][2]==50))
                    playerChessboard[i][j][5] = 3500;

                if(computerChessboard[i][j][0]==300&&(computerChessboard[i][j][1]==50||computerChessboard[i][j][2]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][5] = 3500;
                if(computerChessboard[i][j][1]==300&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][2]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][5] = 3500;
                if(computerChessboard[i][j][2]==300&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][1]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][5] = 3500;
                if(computerChessboard[i][j][3]==300&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][1]==50||computerChessboard[i][j][2]==50))
                    computerChessboard[i][j][5] = 3500;

                //双活三
                if(playerChessboard[i][j][0]==50&&(playerChessboard[i][j][1]==50||playerChessboard[i][j][2]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][6] = 1000;
                if(playerChessboard[i][j][1]==50&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][2]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][6] = 1000;
                if(playerChessboard[i][j][2]==50&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][1]==50||playerChessboard[i][j][3]==50))
                    playerChessboard[i][j][6] = 1000;
                if(playerChessboard[i][j][3]==50&&(playerChessboard[i][j][0]==50||playerChessboard[i][j][1]==50||playerChessboard[i][j][2]==50))
                    playerChessboard[i][j][6] = 1000;

                if(computerChessboard[i][j][0]==50&&(computerChessboard[i][j][1]==50||computerChessboard[i][j][2]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][6] = 1000;
                if(computerChessboard[i][j][1]==50&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][2]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][6] = 1000;
                if(computerChessboard[i][j][2]==50&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][1]==50||computerChessboard[i][j][3]==50))
                    computerChessboard[i][j][6] = 1000;
                if(computerChessboard[i][j][3]==50&&(computerChessboard[i][j][0]==50||computerChessboard[i][j][1]==50||computerChessboard[i][j][2]==50))
                    computerChessboard[i][j][6] = 1000;

                //活四活三
                if(playerChessboard[i][j][0]==500&&(playerChessboard[i][j][1]==500||playerChessboard[i][j][2]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][7] = 5500;
                if(playerChessboard[i][j][1]==500&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][2]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][7] = 5500;
                if(playerChessboard[i][j][2]==500&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][1]==500||playerChessboard[i][j][3]==500))
                    playerChessboard[i][j][7] = 5500;
                if(playerChessboard[i][j][3]==500&&(playerChessboard[i][j][0]==500||playerChessboard[i][j][1]==500||playerChessboard[i][j][2]==500))
                    playerChessboard[i][j][7] = 5500;

                if(computerChessboard[i][j][0]==500&&(computerChessboard[i][j][1]==500||computerChessboard[i][j][2]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][7] = 5500;
                if(computerChessboard[i][j][1]==500&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][2]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][7] = 5500;
                if(computerChessboard[i][j][2]==500&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][1]==500||computerChessboard[i][j][3]==500))
                    computerChessboard[i][j][7] = 5500;
                if(computerChessboard[i][j][3]==500&&(computerChessboard[i][j][0]==500||computerChessboard[i][j][1]==500||computerChessboard[i][j][2]==500))
                    computerChessboard[i][j][7] = 5500;

            }
        }
    }



    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            for (int a=0;a<4;a++){
                score += computerChessboard[i][j][a];
                score -= playerChessboard[i][j][a];
            }
        }
    }
    return score;
}
