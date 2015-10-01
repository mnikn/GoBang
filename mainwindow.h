#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QMenuBar>
#include <QStack>
#include <QPoint>
#include <QVector>
#include "tool.h"
#include "ai.h"
#include "constants.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    QStack <QPoint> pointStack;
    QPoint position;
    int chessboard[15][15];
    int playerChessboard[15][15][8];
    int computerChessboard[15][15][8];
    int step;
    int termFlag;
    int colorFlag;
    bool isFirstHand;
    bool isEasy;
    bool isNormal;
    bool isWin;
    bool isManMode;
    bool isComputerMode;
    bool isInternetMode;
    bool isComputerTerm;
    ~MainWindow();

private:
    class Tool *tool;
    class Ai *ai;
    QMessageBox *message;
    QAction *manAction;
    QAction *computerAction;
    QAction *internetAction;
    QAction *easyAction;
    QAction *normalAction;
    QAction *difficultAction;
    QAction *whiteAction;
    QAction *blackAction;
    QAction *firstAction;
    QAction *lastAction;
    QAction *aboutmeAction;
    QAction *helpAction;
    QMenu *modeMenu;
    QMenu *optionMenu;
    QMenu *difficultMenu;
    QMenu *colorMenu;
    QMenu *priorityMenu;
    QMenu *helpMenu;
    QPushButton *withdrawButton;
    QPushButton *quitButton;
    QPushButton *renewButton;
    void initActions();
    void initMenus();
    void initButtons();
    void initWindow();
    void computerMode();

private slots:
    void renew();
    void withdraw();
    void selectFirstHand();
    void selectLastHand();
    void selectWhiteColor();
    void selectBlackColor();
    void selectManMode();
    void selectComputerMode();
    void selectInternetMode();
    void selectEasyMode();
    void selectNormalMode();
    void selectDifficultMode();
    void selectHelp();
    void selectAboutMe();
};

#endif // MAINWINDOW_H
