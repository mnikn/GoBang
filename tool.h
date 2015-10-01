#ifndef TOOL_H
#define TOOL_H

#include "mainwindow.h"
#include "constants.h"

class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject *parent = 0):QObject(parent){}
    QPoint findPosition(QPoint position);
    bool checkWin(const int chessboard[15][15]);
private:
    int whiteCountV,whiteCountH,whiteCountLeftup,whiteCountRightup;
    int blackCountV,blackCountH,blackCountLeftup,blackCountRightup;
};

#endif // TOOL_H
