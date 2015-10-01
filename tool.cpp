#include "tool.h"

/**
 * @brief Tool::findPosition
 * @param position
 * @return 下棋点
 * 捕捉下棋位置
 */
QPoint Tool::findPosition(QPoint position)
{
    QPoint targetPosition;
    targetPosition.setX(0);
    targetPosition.setY(0);
    int differentX=10000;
    int differentY=10000;
    for (int i =0;i<15;i++){
        int temp = abs(i*33+50 -position.x());
        if (differentX >= temp){
            differentX = temp;
            targetPosition.setX(i);
        }
    }
    for (int i =0;i<15;i++){
        int temp = abs(i*33+50 -position.y());
        if (differentY >= temp){
            differentY = temp;
            targetPosition.setY(i);
        }
    }
    return targetPosition;
}

/**
 * @brief Tool::checkWin
 * @param chessboard
 * @return
 */
bool Tool::checkWin(const int chessboard[15][15])
{
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            /*判断白棋是否赢*/
            if (chessboard[i][j] == WHITE){
                //横向
                whiteCountH =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j] ==WHITE && i<12){
                        whiteCountH ++;
                    }
                }

                if (whiteCountH ==5){
                    return true;
                }
                //纵向
                whiteCountV =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i][j+check] ==WHITE&&j<12){
                        whiteCountV ++;
                    }
                }

                if (whiteCountV ==5){
                    return true;
                }

                //左斜//
                whiteCountLeftup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i-check][j-check] ==WHITE&&i>3&&j>3){
                        whiteCountLeftup ++;
                    }
                }

                if (whiteCountLeftup ==5){
                    return true;
                }
                //右斜//
                whiteCountRightup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j-check] ==WHITE&&i<12&&j>3){
                        whiteCountRightup ++;
                    }
                }
                if (whiteCountRightup ==5){
                    return true;
                }
           }

            /*判断黑棋是否赢*/
            if (chessboard[i][j] == BLACK){
                //横向
                blackCountH =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j] ==BLACK&&i<12){
                        blackCountH ++;
                    }
                }

                if (blackCountH ==5){
                    return true;
                }

                //纵向
                blackCountV =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i][j+check] ==BLACK&&j<12){
                        blackCountV ++;
                    }
                }

                if (blackCountV ==5){
                    return true;
                }

                //左斜//
                blackCountLeftup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i-check][j-check] ==BLACK&&i>3&&j>3){
                        blackCountLeftup ++;
                    }
                }

                if (blackCountLeftup ==5){
                    return true;
                }

                //右斜//
                blackCountRightup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j-check] ==BLACK&&i<12&&j>3){
                        blackCountRightup ++;
                    }
                }

                if (blackCountRightup ==5){
                    return true;
                }
            }
        }
    }
    return false;
}

