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
void computerMove(char** board, int n, char symbol);

int main() {
    int n;
    printf("Enter grid size (3 - 10): ");
    scanf("%d", &n);

    if (n < 3 || n > 10) {
        printf("Invalid grid size! Exiting...\n");
        return 1;
    }

    // Seed random generator
    srand(time(0));

    // Open log file
    FILE *logFile = fopen("tic_tac_toe_log.txt", "w");
    if (!logFile) {
        printf("Error opening log file!\n");
        return 1;
    }

    // Role setup
    // 1 = player, 0 = computer
    int role[3];  
    printf("\nConfigure players:\n");
    printf("Player X (1=player, 0=Computer): ");
    scanf("%d", &role[0]);
    printf("Player O (1=player, 0=Computer): ");
    scanf("%d", &role[1]);
    printf("Player Z (1=player, 0=Computer): ");
    scanf("%d", &role[2]);

    // Ensure at least one player
    if (role[0] == 0 && role[1] == 0 && role[2] == 0) {
        printf("At least one human player is required!\n");
        return 1;
    }

    // Initialize game
    char** board = initBoard(n);
    char players[3] = {'X', 'O', 'Z'};
    int currentPlayer = 0; // 0 = X, 1 = O, 2 = Z
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board, n);
        char symbol = players[currentPlayer];

        if (role[currentPlayer] == 1) {  
            // Human turn
            int row, col;
            printf("Player %c , enter your move (row col): ", symbol);
            scanf("%d %d", &row, &col);

            if (row < 1 || row > n || col < 1 || col > n) {
                printf("Invalid position! Try again.\n");
                continue;
            }

            if (!isValidMove(board, row - 1, col - 1)) {
                printf("Cell already occupied! Try again.\n");
                continue;
            }

            makeMove(board, row - 1, col - 1, symbol);
        } else {  
            // Computer turn
            printf("Player %c (Computer) is making a move....\n", symbol);
            computerMove(board, n, symbol);
        }

        // Log current state
        logGame(logFile, board, n);

        // Check win
        if (checkWin(board, n, symbol)) {
            displayBoard(board, n);
            if (role[currentPlayer] == 1)
                printf("Player %c  wins!\n", symbol);
            else
                printf("Player %c (Computer) wins!\n", symbol);

            fprintf(logFile, "Player %c wins!\n", symbol);
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

        // Switch player cyclically
        currentPlayer = (currentPlayer + 1) % 3;
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
bool isValidMove(char** board, int row, int col) {
    return board[row][col] == ' ';
}

// Place player's symbol
void makeMove(char** board, int row, int col, char symbol) {
    board[row][col] = symbol;
}

// Check win condition
bool checkWin(char** board, int n, char symbol) {
    // Rows
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

    // Columns
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

    // Main diagonal
    bool win = true;
    for (int i = 0; i < n; i++) {
        if (board[i][i] != symbol) {
            win = false;
            break;
        }
    }
    if (win) return true;

    // Anti-diagonal
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
void computerMove(char** board, int n, char symbol) {
    int row, col;
    do {
        row = rand() % n;
        col = rand() % n;
    } while (!isValidMove(board, row, col));

    makeMove(board, row, col, symbol);
}
