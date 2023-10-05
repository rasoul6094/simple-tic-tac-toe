#include "mythread.h"

#include <QDebug>
#include <QSemaphore>

extern QSemaphore semaphore;
extern int v;

void MyThread::run() {
    board = new int*[3];
    for (int i = 0; i < 3; i++) {
        board[i] = new int[3];
    }
    int curr = 1;

    initBoard(board);
    updateBoard(board);

    while (!checkBoard(board) && !isFull(board) && !MyThread::isInterruptionRequested()) {

        if (mode < 2)
            playerMove(board, curr);
        else
            computerMove(board, curr);

        updateBoard(board);

        curr *= -1;

        if (MyThread::isInterruptionRequested())
            break;

        if (checkBoard(board) || isFull(board))
            break;

        if (mode != 0)
            computerMove(board, curr);
        else
            playerMove(board, curr);

        curr *= -1;

        updateBoard(board);
    }

    if (checkBoard(board)) {
        updateBoard(board, v);
    } else if (isFull(board)) {
        updateBoard(board, 2);
    } else {
        updateBoard(board);
    }
}

void MyThread::updateBoard(int** board, int v) {
    emit updateBoardSignal(board, v);
}

void MyThread::playerMove(int** board, int curr) {
    semaphore.acquire();
    while ((!(p >= 0 && p <= 8) || board[p / 3][p % 3] != 0) && !MyThread::isInterruptionRequested()) {
        semaphore.acquire();
    }

    if (p >= 0)
        board[p / 3][p % 3] = curr;
}

int MyThread::minimax(int player, int** test_board, int depth) {
    int i, j, c = -1, d = -1;
    int** scores = new int*[3];
    for (int k = 0; k < 3; k++) {
        scores[k] = new int[3];
    }

    initBoard(scores);

    if (checkBoard(test_board)) {
        return v * -10 + (player * depth);
    }

    if (isFull(test_board)) {
        return 0;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (test_board[i][j] != 0) {
                continue;
            }

            test_board[i][j] = player;
            scores[i][j] += minimax(player * -1, test_board, depth + 1);
            test_board[i][j] = 0;
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (c == -1 && test_board[i][j] == 0) {
                c = i;
                d = j;
            }
            if (player == -1) {
                if (test_board[i][j] == 0 && scores[i][j] > scores[c][d]) {
                    c = i;
                    d = j;
                }
            } else {
                if (test_board[i][j] == 0 && scores[i][j] < scores[c][d]) {
                    c = i;
                    d = j;
                }
            }
        }
    }

    pcMove = c * 3 + d;

    return scores[c][d];
}

void MyThread::computerMove(int** board, int curr) {
    int** pc_board = new int*[3];
    for (int i = 0; i < 3; i++) {
        pc_board[i] = new int[3];
    }
    int i, j;

    initBoard(pc_board);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            pc_board[i][j] = board[i][j];
        }
    }

    minimax(curr, pc_board, 0);

    board[pcMove / 3][pcMove % 3] = curr;
}

int MyThread::isFull(int** board) {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j= 0; j < 3; j++) {
            if (board[i][j] == 0) {
                return 0;
            }
        }
    }

    return 1;
}

void MyThread::initBoard(int** board) {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
}

int MyThread::checkBoard(int** board) {
    int i, j;

    if ((v = board[0][0]) != 0 && board[1][1] == v && board[2][2] == v) {
        return 1;
    }

    if ((v = board[2][0]) != 0 && board[1][1] == v && board[0][2] == v) {
        return 1;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == 0 && (v = board[0][j]) != 0 && board[1][j] == v && board[2][j] == v) {
                return 1;
            }
            if (j == 0 && (v = board[i][0]) != 0 && board[i][1] == v && board[i][2] == v) {
                return 1;
            }
        }
    }

    return 0;
}

MyThread::~MyThread() {
    for (int i = 0; i < 3; i++) {
        delete [] board[i];
    }
    delete [] board;
}

