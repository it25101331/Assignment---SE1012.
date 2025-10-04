#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Function prototypes
char** initBoard(int n);
void displayBoard(char** board, int n);
bool isValidMove(char** board, int row, int col);
void makeMove(char** board, int row, int col, char symbol);
bool checkWin(char** board, int n, char symbol);
bool checkDraw(char** board, int n);
void logGame(FILE *logFile, char** board, int n);
void computerMove(char** board, int n);

int main() {
    int n;
    printf("Enter grid size (3 - 10): ");
    scanf("%d", &n);

    if (n < 3 || n > 10) {
        printf("Invalid grid size! Exiting...\n");
        return 1;
    }

    // seed random generator for computer moves
    srand(time(0));

    // open log file
    FILE *logFile = fopen("tic_tac_toe_log.txt", "w");
    if (!logFile) {
        printf("Error opening log file!\n");
        return 1;
    }

    // initialize game
    char** board = initBoard(n);
    char currentPlayer = 'X'; // User starts first
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board, n);

        if (currentPlayer == 'X') {  
            // user's turn
            int row, col;
            printf("Player %c, enter your move (row and column): ", currentPlayer);
            scanf("%d %d", &row, &col);

            if (row < 1 || row > n || col < 1 || col > n) {
                printf("Invalid position! Try again.\n");
                continue;
            }

            if (!isValidMove(board, row - 1, col - 1)) {
                printf("Cell already occupied! Try again.\n");
                continue;
            }

            makeMove(board, row - 1, col - 1, currentPlayer);
        } 
        else {  
            // computer's turn
            printf("Computer is making a move...\n");
            computerMove(board, n);
        }

        // Log game state
        logGame(logFile, board, n);

        // Check win
        if (checkWin(board, n, currentPlayer)) {
            displayBoard(board, n);
            if (currentPlayer == 'X')
                printf("Congratulations! Player %c (User) wins!\n", currentPlayer);
            else
                printf("Computer wins!\n");

            fprintf(logFile, "Player %c wins!\n", currentPlayer);
            gameOver = true;
            break;
        }

        // Check draw
        if (checkDraw(board, n)) {
            displayBoard(board, n);
            printf("It's a draw!\n");
            fprintf(logFile, "It's a draw!\n");
            gameOver = true;
            break;
        }

        // Switch player
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

// Check if a move is valid (empty cell)
bool isValidMove(char** board, int row, int col) {
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

// Generate random computer move
void computerMove(char** board, int n) {
    int row, col;
    do {
        row = rand() % n;
        col = rand() % n;
    } while (!isValidMove(board, row, col));

    makeMove(board, row, col, 'O');
}
