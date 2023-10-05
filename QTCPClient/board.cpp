#include "board.h"
#include "ui_board.h"


#include <QSemaphore>
#include <QThread>
#include <QPushButton>
#include <QDebug>

int v = 0;

QSemaphore semaphore;

Board::Board(QTcpSocket*test, QWidget *parent) :
    QDialog(parent),
    socket(test),
    ui(new Ui::Board)
{
    ui->setupUi(this);
    turn = false;



    connect(ui->but0, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but1, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but2, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but3, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but4, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but5, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but6, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but7, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but8, SIGNAL(clicked()), this, SLOT(click()));

    //    connect(ui->h_vs_h, SIGNAL(clicked()), this, SLOT(setMode0()));
    //    connect(ui->Al_vs_Al, SIGNAL(clicked()), this, SLOT(setMode1()));

    initGame(0);
}

Board::~Board()
{
    delete ui;
    semaphore.release();
    helperThread->terminate();
    helperThread->wait();
    delete helperThread;
}

void Board::setLabale(QString str)
{
    ui->winner_label->setText(str);
    if(ui->winner_label->text()=="your turn")
        turn=true;
    else if(ui->winner_label->text()=="not your turn")
        turn=false;
}

void Board::newMove(QString str)
{
    QStringList words = str.split(' ');
    QString btn = words[1];
    if(btn==ui->but0->objectName() )
    {
        ui->but0->setDisabled(true);
        helperThread->p= ui->but0->objectName().back().unicode()-48;
        semaphore.release();
    }

    else if(btn==ui->but1->objectName() )
    {
        ui->but1->setDisabled(true);
        helperThread->p= ui->but1->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but2->objectName() )
    {
        ui->but2->setDisabled(true);
        helperThread->p= ui->but2->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but3->objectName() )
    {
        ui->but3->setDisabled(true);
        helperThread->p= ui->but3->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but4->objectName() )
    {
        ui->but4->setDisabled(true);
        helperThread->p= ui->but4->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but5->objectName() )
    {
        ui->but5->setDisabled(true);
        helperThread->p= ui->but5->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but6->objectName() )
    {
        ui->but6->setDisabled(true);
        helperThread->p= ui->but6->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but7->objectName())
    {
        ui->but7->setDisabled(true);
        helperThread->p= ui->but7->objectName().back().unicode()-48;
        semaphore.release();
    }
    else if(btn==ui->but8->objectName() )
    {
        ui->but8->setDisabled(true);
        helperThread->p= ui->but8->objectName().back().unicode()-48;
        semaphore.release();
    }
}



void Board::click()
{
    if(turn)
    {
        QPushButton* pPushButton = qobject_cast<QPushButton*>(sender());
        QByteArray moveBtn = pPushButton->objectName().toUtf8();
        if (!pPushButton) // safety check
            return;
        if(pPushButton->isEnabled())
        {
         pPushButton->setDisabled(true);
        helperThread->p = pPushButton->objectName().back().unicode() - 48;
        semaphore.release();
        setLabale("not your turn");
        socket->write("move "+moveBtn);
        }
    }

}

void Board::printBoard(int **board, int v)
{
    if (board == nullptr)
        return;

    QString* vals = new QString[9];

    for (int i = 0; i < 9; i++) {
        if (board[i / 3][i % 3] == 1) {
            vals[i] = "X";
        } else if (board[i / 3][i % 3] == -1) {
            vals[i] = "O";
        } else {
            vals[i] = " ";
        }
    }

    ui->but0->setText(vals[0]);
    ui->but1->setText(vals[1]);
    ui->but2->setText(vals[2]);
    ui->but3->setText(vals[3]);
    ui->but4->setText(vals[4]);
    ui->but5->setText(vals[5]);
    ui->but6->setText(vals[6]);
    ui->but7->setText(vals[7]);
    ui->but8->setText(vals[8]);

    if (v != 0) {
        if (v == 1) {
            ui->winner_label->setText("X wins!");
        } else if (v == -1) {
            ui->winner_label->setText("O wins!");
        } else {
            ui->winner_label->setText("Tie!");
        }
        socket->waitForBytesWritten(3000);
        socket->write("finish");
    }

    delete [] vals;
}

void Board::setMode0()
{
    setMode(0);
}

void Board::setMode1()
{
    setMode(1);
}



void Board::initGame(int mode)
{
    ui->winner_label->setText("");
    ui->but0->setText("");
    ui->but1->setText("");
    ui->but2->setText("");
    ui->but3->setText("");
    ui->but4->setText("");
    ui->but5->setText("");
    ui->but6->setText("");
    ui->but7->setText("");
    ui->but8->setText("");

    helperThread = new MyThread;
    helperThread->mode = mode;
    connect(helperThread, &MyThread::updateBoardSignal, this, &Board::printBoard);
    helperThread->start();
}

void Board::setMode(int mode)
{
    semaphore.release();
    helperThread->requestInterruption();
    helperThread->wait();
    delete helperThread;

    initGame(mode);
}
