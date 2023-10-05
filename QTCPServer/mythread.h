#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread {
    Q_OBJECT

public:
    int p = -1;
    int mode = 1;

    ~MyThread();

signals:
    void updateBoardSignal(int** board, int v = 0);

private:
    void run();
    void updateBoard(int** board, int v = 0);
    void playerMove(int** board, int curr);
    int minimax(int player, int** test_board, int depth);
    void computerMove(int** board, int curr);
    int isFull(int** board);
    void initBoard(int** board);
    int checkBoard(int** board);

    int pcMove = -1;
    int** board;
};

#endif // MYTHREAD_H
