#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char** initBoard(int n);
void displayBoard(char** board, int n);
bool isValidMove(char** board, int n, int row, int col);
void makeMove(char** board, int row, int col, char symbol);
bool checkWin(char** board, int n, char symbol);
bool checkDraw(char** board, int n);
void logGame(FILE *logFile, char** board, int n);

int main() {
    int n;
    printf("Enter grid size (3 - 10): ");
    scanf("%d", &n);

    if (n < 3 || n > 10) {
        printf("Invalid grid size! Exiting...\n");
        return 1;
    }

    // Open log file
    FILE *logFile = fopen("tic_tac_toe_log.txt", "w");
    if (!logFile) {
        printf("Error opening log file!\n");
        return 1;
    }

    // Initialize game
    char** board = initBoard(n);
    int totalMoves = n * n;
    char currentPlayer = 'X';
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board, n);

        int row, col;
        printf("Player %c, enter your move (row and column): ", currentPlayer);
        scanf("%d %d", &row, &col);

        if (row < 1 || row > n || col < 1 || col > n) {
            printf("Invalid position! Try again.\n");
            continue;
        }

        if (!isValidMove(board, n, row - 1, col - 1)) {
            printf("Cell already occupied! Try again.\n");
            continue;
        }

        // Make move
        makeMove(board, row - 1, col - 1, currentPlayer);

        // Log board
        logGame(logFile, board, n);

        // win check
        if (checkWin(board, n, currentPlayer)) {
            displayBoard(board, n);
            printf("Player %c wins!\n", currentPlayer);
            fprintf(logFile, "Player %c wins!\n", currentPlayer);
            gameOver = true;
            break;
        }

        // draw chek
        if (checkDraw(board, n)) {
            displayBoard(board, n);
            printf("It's a draw!\n");
            fprintf(logFile, "It's a draw!\n");
            gameOver = true;
            break;
        }

        // Switching player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Free memory
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
    fclose(logFile);

    return 0;
}

// Initialize board with empty spaces
char** initBoard(int n) {
    char** board = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

// Display the game board
void displayBoard(char** board, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf(" %c ", board[i][j]);
            if (j < n - 1) printf("|");
        }
        printf("\n");
        if (i < n - 1) {
            for (int j = 0; j < n; j++) {
                printf("---");
                if (j < n - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Check if a move is valid
bool isValidMove(char** board, int n, int row, int col) {
    return board[row][col] == ' ';
}

// Place player's symbol
void makeMove(char** board, int row, int col, char symbol) {
    board[row][col] = symbol;
}

// Check win condition
bool checkWin(char** board, int n, char symbol) {
    // Check rows
    for (int i = 0; i < n; i++) {
        bool win = true;
        for (int j = 0; j < n; j++) {
            if (board[i][j] != symbol) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check columns
    for (int j = 0; j < n; j++) {
        bool win = true;
        for (int i = 0; i < n; i++) {
            if (board[i][j] != symbol) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check main diagonal
    bool win = true;
    for (int i = 0; i < n; i++) {
        if (board[i][i] != symbol) {
            win = false;
            break;
        }
    }
    if (win) return true;

    // Check anti-diagonal
    win = true;
    for (int i = 0; i < n; i++) {
        if (board[i][n - 1 - i] != symbol) {
            win = false;
            break;
        }
    }
    if (win) return true;

    return false;
}

// Check for draw
bool checkDraw(char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

// Log board to file
void logGame(FILE *logFile, char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(logFile, " %c ", board[i][j]);
            if (j < n - 1) fprintf(logFile, "|");
        }
        fprintf(logFile, "\n");
        if (i < n - 1) {
            for (int j = 0; j < n; j++) {
                fprintf(logFile, "---");
                if (j < n - 1) fprintf(logFile, "+");
            }
            fprintf(logFile, "\n");
        }
    }
    fprintf(logFile, "\n");
}
