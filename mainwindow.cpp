#include "mainwindow.h"
#include <QApplication>

/**
 * @brief MainWindow::initActions
 * 初始化菜单动作
 */
void MainWindow::initActions()
{
    manAction = new QAction(tr("人人模式"),this);
    computerAction = new QAction(tr("人机模式"),this);
    internetAction = new QAction(tr("网络对战"),this);
    easyAction = new QAction(tr("简单"),this);
    normalAction = new QAction(tr("普通"),this);
    difficultAction = new QAction(tr("困难"),this);
    whiteAction = new QAction(tr("白色"),this);
    blackAction = new QAction(tr("黑色"),this);
    firstAction = new QAction(tr("先手"),this);
    lastAction = new QAction(tr("后手"),this);
    aboutmeAction = new QAction(tr("有关作者"),this);
    helpAction = new QAction(tr("帮助"),this);
}

/**
 * @brief MainWindow::initMenus
 * 初始化菜单
 */
void MainWindow::initMenus()
{
    modeMenu = menuBar()->addMenu(tr("游戏模式"));
    optionMenu = menuBar()->addMenu(tr("游戏设置"));
    helpMenu = menuBar()->addMenu(tr("帮助"));
    difficultMenu = optionMenu->addMenu(tr("游戏难度"));
    colorMenu = optionMenu->addMenu(tr("本方棋色"));
    priorityMenu = optionMenu->addMenu(tr("先后手"));

    modeMenu->addAction(manAction);
    modeMenu->addAction(computerAction);
    modeMenu->addAction(internetAction);

    difficultMenu->addAction(easyAction);
    difficultMenu->addAction(normalAction);
    difficultMenu->addAction(difficultAction);

    colorMenu->addAction(whiteAction);
    colorMenu->addAction(blackAction);

    priorityMenu->addAction(firstAction);
    priorityMenu->addAction(lastAction);

    helpMenu->addAction(aboutmeAction);
    helpMenu->addAction(helpAction);
}

/**
 * @brief MainWindow::initButtons
 * 初始化按钮
 */
void MainWindow::initButtons()
{
    withdrawButton = new QPushButton(tr("悔棋"),this);
    renewButton = new QPushButton(tr("重新开始游戏"),this);
    quitButton = new QPushButton(tr("退出"),this);

    withdrawButton->setGeometry(625,90,100,50);
    renewButton->setGeometry(625,190,100,50);
    quitButton->setGeometry(625,290,100,50);
}

/**
 * @brief MainWindow::initWindow
 * 初始化界面
 */
void MainWindow::initWindow()
{

    message = new QMessageBox(this);
    message->setAutoFillBackground(true);
    message->setWindowTitle(tr("提示"));
    message->setWindowIcon(QIcon(":/icon.jpg"));

    setFixedSize(800,600);
    setWindowTitle(tr("五子棋"));
    setWindowIcon(QIcon(":/icon.jpg"));
    QPixmap pixmap = QPixmap(":/gg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    setPalette(palette);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            chessboard[i][j] = EMPTY;
        }
    }

    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            for (int a=0;a<8;a++){
                playerChessboard[i][j][a] = EMPTY;
                computerChessboard[i][j][a] = EMPTY;
            }
        }
    }

    tool = new Tool;
    ai = new Ai;
    message = new QMessageBox(this);
    socket = new QTcpSocket(this);

    position.setX(0);
    position.setY(0);

    step = 0;
    isFirstHand = true;//本方先手
    colorFlag = BLACK;//1为本方黑棋，2为本方白棋
    termFlag = colorFlag;//1为轮到黑下棋，2为轮到白下棋
    isWin = false;
    isEasy = false;
    isNormal = false;
    isManMode = false;
    isComputerMode = false;
    isInternetMode = false;
    isComputerTerm = false;

    initActions();
    initMenus();
    initButtons();
    initWindow();

    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(renewButton,SIGNAL(clicked(bool)),this,SLOT(renew()));
    connect(withdrawButton,SIGNAL(clicked(bool)),this,SLOT(withdraw()));
    connect(firstAction,SIGNAL(triggered(bool)),this,SLOT(selectFirstHand()));
    connect(lastAction,SIGNAL(triggered(bool)),this,SLOT(selectLastHand()));
    connect(whiteAction,SIGNAL(triggered(bool)),this,SLOT(selectWhiteColor()));
    connect(blackAction,SIGNAL(triggered(bool)),this,SLOT(selectBlackColor()));
    connect(manAction,SIGNAL(triggered(bool)),this,SLOT(selectManMode()));
    connect(computerAction,SIGNAL(triggered(bool)),this,SLOT(selectComputerMode()));
    connect(internetAction,SIGNAL(triggered(bool)),this,SLOT(selectInternetMode()));
    connect(easyAction,SIGNAL(triggered(bool)),this,SLOT(selectEasyMode()));
    connect(normalAction,SIGNAL(triggered(bool)),this,SLOT(selectNormalMode()));
    connect(difficultAction,SIGNAL(triggered(bool)),this,SLOT(selectDifficultMode()));
    connect(helpAction,SIGNAL(triggered(bool)),this,SLOT(selectHelp()));
    connect(aboutmeAction,SIGNAL(triggered(bool)),this,SLOT(selectAboutMe()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

/**
 * @brief MainWindow::mousePressEvent
 * @param event
 * 鼠标移动事件，处理点击事件
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if ((isManMode||isInternetMode|| isEasy||isNormal) &&event->button()==Qt::LeftButton &&event->pos().x()>=20 && event->pos().x()<=515 &&event->pos().y()>=20&&event->pos().y()<=515&&isWin==false){

        //捕捉下棋点
        if (chessboard[tool->findPosition(event->pos()).x()][tool->findPosition(event->pos()).y()] ==0){
            if(termFlag==BLACK){
                position = tool->findPosition(event->pos());
                pointStack.append(position);
                chessboard[position.x()][position.y()] = BLACK;
            }
            else if(termFlag==WHITE){
                position = tool->findPosition(event->pos());
                pointStack.append(position);
                chessboard[position.x()][position.y()] = WHITE;
            }

            step++;
            isWin = tool->checkWin(chessboard);//检测是否胜利
            if (isWin ==true && isComputerMode==true){
                message->setText(tr("你赢了！"));
                message->setGeometry(550,350,200,200);
                message->show();
            }
            else if(isWin ==true && isManMode==true){
                if (termFlag ==BLACK)
                    message->setText(tr("黑棋赢！"));
                else
                    message->setText(tr("白棋赢！"));
                message->setGeometry(550,350,200,200);
                message->show();
            }


            //轮换下棋方
            if (termFlag ==BLACK)
                termFlag = WHITE;
            else
                termFlag = BLACK;
            isComputerTerm= true;

            if ((isEasy == true || isNormal==true) && isWin ==false){
                computerMode();
                isComputerTerm= false;
                isWin = tool->checkWin(chessboard);
                if (isWin ==true){
                    message->setText(tr("你输了！"));
                    message->setGeometry(550,350,200,200);
                    message->show();
                }
            }
        }
    }
    else if((!isManMode&&!isComputerMode&&!isInternetMode)){
        message->setText(tr("请先选择模式！"));
        message->setGeometry(550,350,200,200);
        message->show();
    }
    else if(!isEasy&&!isNormal&&!isManMode){
        message->setText(tr("请先选择难度！"));
        message->setGeometry(550,350,200,200);
        message->show();
    }
}

/**
 * @brief MainWindow::paintEvent
 * 绘图事件，绘制棋盘棋子
 */
void MainWindow::paintEvent(QPaintEvent *)
{

    if (step !=0){
        whiteAction->setEnabled(false);
        blackAction->setEnabled(false);
        firstAction->setEnabled(false);
        lastAction->setEnabled(false);
    }
    else{
        whiteAction->setEnabled(true);
        blackAction->setEnabled(true);
        firstAction->setEnabled(true);
        lastAction->setEnabled(true);
    }

    //绘制棋盘
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,5));
    painter.drawRect(50,50,465,465);
    painter.setBrush(Qt::white);
    painter.drawRect(50,50,465,465);
    painter.setPen(QPen(Qt::black,2));
    for(int i = 0;i<15;i++){
        painter.drawLine(i*33+50,50,i*33+50,517);
    }
    for(int j = 0;j<15;j++){
        painter.drawLine(50,j*33+50,517,j*33+50);
    }
    painter.setBrush(Qt::black);
    painter.drawEllipse(7*33+42,7*33+42,15,15);


    //绘制棋子
    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            if (chessboard[i][j] == BLACK){
                painter.setBrush(Qt::black);
                painter.drawEllipse(i*33+35,j*33+35,30,30);
                painter.setBrush(Qt::red);
                painter.drawEllipse(pointStack.top().x()*33+45,pointStack.top().y()*33+45,10,10);
            }
            else if(chessboard[i][j] == WHITE){
                painter.setBrush(Qt::white);
                painter.drawEllipse(i*33+35,j*33+35,30,30);
                painter.setBrush(Qt::red);
                painter.drawEllipse(pointStack.top().x()*33+45,pointStack.top().y()*33+45,10,10);
            }
        }
    }


    //胜利连线
    static int whiteCountV,whiteCountH,whiteCountLeftup,whiteCountRightup;
    static int blackCountV,blackCountH,blackCountLeftup,blackCountRightup;
    painter.setBrush(Qt::red);
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
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i+a)*33+45,j*33+45,10,10);
                    }
                }
                //纵向
                whiteCountV =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i][j+check] ==WHITE&&j<12){
                        whiteCountV ++;
                    }
                }

                if (whiteCountV ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse(i*33+45,(j+a)*33+45,10,10);
                    }
                }

                //左斜//
                whiteCountLeftup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i-check][j-check] ==WHITE&&i>3&&j>3){
                        whiteCountLeftup ++;
                    }
                }

                if (whiteCountLeftup ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i-a)*33+45,(j-a)*33+45,10,10);
                    }
                }
                //右斜//
                whiteCountRightup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j-check] ==WHITE&&i<12&&j>3){
                        whiteCountRightup ++;
                    }
                }
                if (whiteCountRightup ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i+a)*33+45,(j-a)*33+45,10,10);
                    }
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
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i+a)*33+45,j*33+45,10,10);
                    }
                }

                //纵向
                blackCountV =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i][j+check] ==BLACK&&j<12){
                        blackCountV ++;
                    }
                }

                if (blackCountV ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse(i*33+45,(j+a)*33+45,10,10);
                    }
                }

                //左斜//
                blackCountLeftup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i-check][j-check] ==BLACK&&i>3&&j>3){
                        blackCountLeftup ++;
                    }
                }

                if (blackCountLeftup ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i-a)*33+45,(j-a)*33+45,10,10);
                    }
                }

                //右斜//
                blackCountRightup =0;
                for (int check=0;check<=4;check++){
                    if(chessboard[i+check][j-check] ==BLACK&&i<12&&j>3){
                        blackCountRightup ++;
                    }
                }

                if (blackCountRightup ==5){
                    for (int a=0;a<5;a++){
                        painter.drawEllipse((i+a)*33+45,(j-a)*33+45,10,10);
                    }
                }
            }
        }
    }
    update();
}

/**
 * @brief MainWindow::selectManMode
 * 选择人人模式
 */
void MainWindow::selectManMode()
{
    isManMode = true;
    isComputerMode = false;
    isInternetMode = false;
    isEasy =false;
    isNormal = false;
    withdrawButton->setEnabled(true);

    renew();

    message->setText(tr("人人模式开始！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectComputerMode
 * 选择人机模式
 */
void MainWindow::selectComputerMode()
{
    isComputerMode = true;
    isManMode = false;
    isInternetMode = false;
    withdrawButton->setEnabled(true);

    renew();

    message->setText(tr("人机模式开始！请选择难度！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectInternetMode
 * 选择网络对战
 */
void MainWindow::selectInternetMode()
{
    isInternetMode = true;
    isComputerMode = false;
    isManMode = false;
    isEasy =false;
    isNormal = false;
    withdrawButton->setEnabled(false);

    renew();

    //得到地址与端口,并发送连接请求
    address = QInputDialog::getText(this,tr("设置"),tr("地址"));
    port = QInputDialog::getText(this,tr("设置"),tr("端口")).toInt();
    newConnection();
}


/**
 * @brief MainWindow::selectEasyMode
 * 选择简单难度
 */
void MainWindow::selectEasyMode()
{
    isComputerMode = true;
    isEasy =true;
    isManMode = false;
    isNormal = false;
    isInternetMode = false;
    withdrawButton->setEnabled(true);

    renew();

    message->setText(tr("简单难度已选择！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectNormalMode
 * 选择普通难度
 */
void MainWindow::selectNormalMode()
{
    isComputerMode = true;
    isNormal =true;
    isEasy = false;
    isManMode = false;
    isInternetMode = false;
    withdrawButton->setEnabled(true);

    renew();

    message->setText(tr("普通难度已选择！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectDifficultMode
 * 选择困难难度
 */
void MainWindow::selectDifficultMode()
{
    isComputerMode = true;
    isManMode = false;
    isInternetMode = false;
    isNormal =false;
    isEasy = false;
    withdrawButton->setEnabled(true);

    renew();

    message->setText(tr("该功能尚未开放，只需999元，立刻解锁困难模式！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectHelp
 * 选择游戏帮助
 */
void MainWindow::selectHelp()
{
    message->setText(tr("五子棋，先连成五子者为胜"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectAboutMe
 * 选择有关作者
 */
void MainWindow::selectAboutMe()
{
    message->setText(tr("制作人郑志昭,如有bug,可联系qq:329121948"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::withdraw
 * 悔棋
 */
void MainWindow::withdraw()
{
    if (!pointStack.empty() && isWin != true){
        if (isManMode == true || (isFirstHand ==false&&step==1)){
            chessboard[pointStack.top().x()][pointStack.top().y()] = EMPTY;
            pointStack.pop();

            if (termFlag ==BLACK)
                termFlag =WHITE;
            else
                termFlag = BLACK;
            step--;
        }
        else if(isComputerMode == true){
            chessboard[pointStack.top().x()][pointStack.top().y()] = EMPTY;
            pointStack.pop();
            chessboard[pointStack.top().x()][pointStack.top().y()] = EMPTY;
            pointStack.pop();
            step -=2;
        }
    }
}

/**
 * @brief MainWindow::selectFirstHand
 * 选择先手
 */
void MainWindow::selectFirstHand()
{
    isFirstHand = true;
    message->setText(tr("本方先手！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectLastHand
 * 选择后手
 */
void MainWindow::selectLastHand()
{
    if (isEasy == true || isNormal == true){
        if (step ==0){
            if (colorFlag ==BLACK)
                chessboard[7][7] = WHITE;
            else
                chessboard[7][7] = BLACK;
            position.setX(7);
            position.setY(7);
            pointStack.append(position);
            step++;
        }
        isFirstHand = false;
        message->setText(tr("本方后手！"));
        message->setGeometry(550,350,200,200);
        message->show();
    }
    else{
        message->setText(tr("请先选择难度！"));
        message->setGeometry(550,350,200,200);
        message->show();
    }
}

/**
 * @brief MainWindow::selectWhiteColor
 * 选择白色
 */
void MainWindow::selectWhiteColor()
{
    colorFlag = WHITE;
    termFlag= colorFlag;
    message->setText(tr("本方为白棋！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::selectBlackColor
 * 选择黑色
 */
void MainWindow::selectBlackColor()
{
    colorFlag = BLACK;
    termFlag= colorFlag;
    message->setText(tr("本方为黑棋！"));
    message->setGeometry(550,350,200,200);
    message->show();
}

/**
 * @brief MainWindow::renew
 * 重新开始游戏
 */
void MainWindow::renew()
{
    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            chessboard[i][j] = EMPTY;
        }
    }

    step = 0;
    position.setX(0);
    position.setY(0);
    isComputerTerm= false;
    isWin = false;
    isFirstHand = true;
    termFlag = colorFlag;
    if(!isInternetMode){
        message->setText(tr("游戏已重新开始！"));
        message->setGeometry(550,350,200,200);
        message->show();
    }
}

/**
 * @brief MainWindow::computerMode
 * 人机模式
 */
void MainWindow::computerMode()
{

    //贪心算法
    if (isEasy == true)
        position = ai->greedAlgorithm(chessboard,playerChessboard,computerChessboard,colorFlag);
    //极大极小
    else if(isNormal==true)
        position = ai->minMaxAlgorithm(2,chessboard,playerChessboard,computerChessboard,isComputerTerm,colorFlag);

    pointStack.append(position);
    chessboard[position.x()][position.y()] = termFlag;
    step++;

    if (termFlag ==BLACK)
        termFlag = WHITE;
    else
        termFlag = BLACK;
}

/**
 * @brief MainWindow::newConnection
 * 客户端请求连接
 */
void MainWindow::newConnection()
{
    socket->connectToHost(address,port);
}

/**
 * @brief MainWindow::displayError
 * 显示客服端连接错误
 */
void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<"Socket Error!";
}


MainWindow::~MainWindow()
{
    delete manAction;
    delete computerAction;
    delete internetAction;
    delete easyAction;
    delete normalAction;
    delete difficultAction;
    delete whiteAction;
    delete blackAction;
    delete firstAction;
    delete lastAction;
    delete modeMenu;
    delete optionMenu;
    delete difficultMenu;
    delete colorMenu;
    delete priorityMenu;
    delete withdrawButton;
    delete renewButton;
    delete quitButton;
    delete message;
    delete tool;
    delete ai;
}
