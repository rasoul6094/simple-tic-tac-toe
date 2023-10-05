#ifndef BOARD_H
#define BOARD_H
#include <QPushButton>
#include "mythread.h"
#include <QDialog>
#include <QTcpSocket>


namespace Ui {
class Board;
}

class Board : public QDialog
{
    Q_OBJECT

public:
    //    void on_but0_clicked();
    Board(QTcpSocket* test, QWidget *parent = nullptr);
    ~Board();
    void setLabale(QString str);
    bool turn;
    void newMove(QString str);


private slots:
    void click();
    void printBoard(int** board, int v = 0);
    void setMode0();
    void setMode1();


private:
    QTcpSocket* socket;
    Ui::Board *ui;
    MyThread* helperThread;
    void initGame(int mode);
    void setMode(int mode);
};

#endif // BOARD_H
