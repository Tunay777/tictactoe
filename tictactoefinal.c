#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 5
#define MAX_HISTORY 25
char board[MAX_SIZE][MAX_SIZE];
char history[MAX_HISTORY][MAX_SIZE][MAX_SIZE];
int historyIndex = 0;
int currentPlayer;
int boardSize;
void initializeBoard();
void printBoard();
void printMarks(char c);
int checkWin();
int isBoardFull();
void saveState();
void undo();
void redo();
void play();

int main() {
    srand(time(NULL));
    currentPlayer = rand() % 2;
    printf("Welcome to Tic-Tac-Toe!\n");
    printf("Select board size (3 for 3x3,4 for 4x4,5 for 5x5): ");
    scanf("%d", &boardSize);
    if (boardSize < 3 || boardSize > 5) {
        printf("Invalid size. Starting Tic-Tac-Toe with default 3x3.\n");
        boardSize = 3;
    }
    initializeBoard();
    saveState();
    if (currentPlayer == 1) {
    printf("Player X starts.\n");
    } else {
    printf("Player O starts.\n");
    }
    while (1) {
        printBoard();
        play();
        if (checkWin()) {
            printBoard();
            if (currentPlayer == 1) {
            printf("Winner is player X!\n");
            } else {
            printf("Winner is player O!\n");
            }
            break;
        }
        if (isBoardFull()) {
            printBoard();
            printf("It's a draw!\n");
            break;
        }
        currentPlayer = 1 - currentPlayer;
    }
    return 0;
}
void initializeBoard() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = ' ';
        }
    }
}
void printMarks(char c) {
    if (c == 'X') {
        printf("\033[31mX\033[0m");
    } else if (c == 'O') {
        printf("\033[34mO\033[0m");
    } else {
        printf(" ");
    }
}

/// 

void printBoard() {
    printf("\n");
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            printf(" ");
            printMarks(board[i][j]);
            printf(" ");
            if (j < boardSize - 1) printf("|");
        }
        printf("\n");
        if (i < boardSize - 1) {
            for (int k = 0; k < boardSize; k++) {
                printf("---");
                if (k < boardSize - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}
void saveState() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            history[historyIndex][i][j] = board[i][j];
        }
    }
    historyIndex++;
}
void undo() {
    if (historyIndex <= 1) {
        printf("No moves to undo!\n");
        return;
    }
    historyIndex--;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = history[historyIndex - 1][i][j];
        }
    }
}
void redo() {
    if (historyIndex >= MAX_HISTORY || history[historyIndex][0][0] == '\0') {
        printf("No moves to redo!\n");
        return;
    }
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = history[historyIndex][i][j];
        }
    }
    historyIndex++;
}

//

int checkWin() {
    char mark;
    if (currentPlayer == 1) {
        mark = 'X';
    } else {
        mark = 'O';
    }
    for (int i = 0; i < boardSize; i++) {
        int rowCheck = 1, colCheck = 1;
        for (int j = 1; j < boardSize; j++) {
            if (board[i][j] != mark || board[i][j - 1] != mark) rowCheck = 0;
            if (board[j][i] != mark || board[j - 1][i] != mark) colCheck = 0;
        }
        if (rowCheck || colCheck) return 1;
    }
    int diag1Check = 1, diag2Check = 1;
    for (int i = 1; i < boardSize; i++) {
        if (board[i][i] != mark || board[i - 1][i - 1] != mark) diag1Check = 0;
        if (board[i][boardSize - i - 1] != mark || board[i - 1][boardSize - i] != mark) diag2Check = 0;
    }
    return diag1Check || diag2Check;
}
int isBoardFull() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == ' ') return 0;
        }
    }
    return 1;
}
void play() {
    int row, col, choice;
    while (1) {
        if (currentPlayer == 1) {
            printf("Player X, enter row and column (1-based index): ");
        } else {
            printf("Player O, enter row and column (1-based index): ");
        }
        scanf("%d %d", &row, &col);
        row--, col--;
        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize && board[row][col] == ' ') {
            if (currentPlayer == 1) {
                board[row][col] = 'X';
            } else {
                board[row][col] = 'O';
            }
            saveState();
            printBoard();
            break;
        } else {
            printf("Invalid row and column.\n");
        }
    }
    while (1) {
        printf("Undo (-1) or Continue (0): ");
        scanf("%d", &choice);
        if (choice == -1) {
            undo();
            printBoard();
            printf("Redo (1) or New Move (0): ");
            scanf("%d", &choice);
            if (choice == 1) {
                redo();
                printBoard();
            } else if (choice == 0) {
                play();
                return;
            }
        } else if (choice == 0) {
            break;
        } else {
            printf("Invalid input.\n");
        }
    }
}