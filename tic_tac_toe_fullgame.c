
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOG_FILE "tic_tac_toe_log.txt"

// Function Prototypes
char **initializeBoard(int size);
void displayBoard(char **board, int size);
int isValidMove(char **board, int size, int row, int col);
int checkWin(char **board, int size, char symbol);
int checkDraw(char **board, int size);
void freeBoard(char **board, int size);
void logMove(char **board, int size, int player, char symbol);
void userMove(char **board, int size, char symbol);
void computerMove(char **board, int size, char symbol);

// Main Program
int main() {
    int size, mode;
    srand(time(NULL));

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("Choose mode:\n");
    printf("1. Two Player (User vs User)\n");
    printf("2. User vs Computer\n");
    printf("3. Three Player Mode\n");
    printf("Enter choice: ");
    scanf("%d", &mode);

    printf("Enter board size (3-10): ");
    scanf("%d", &size);
    if (size < 3 || size > 10) {
        printf("Invalid size. Exiting.\n");
        return 1;
    }

    char **board = initializeBoard(size);
    int moves = 0;
    int winner = 0;

    // Player symbols
    char symbols[3] = {'X', 'O', 'Z'};
    int players = (mode == 3) ? 3 : 2;

    FILE *file = fopen(LOG_FILE, "w");
    if (!file) {
        printf("Error opening log file.\n");
        return 1;
    }
    fclose(file);

    while (1) {
        int currentPlayer = moves % players;
        char symbol = symbols[currentPlayer];

        displayBoard(board, size);
        printf("Player %d (%c)'s turn\n", currentPlayer + 1, symbol);

        if (mode == 2 && currentPlayer == 1) {
            computerMove(board, size, symbol);
        } else {
            userMove(board, size, symbol);
        }

        logMove(board, size, currentPlayer + 1, symbol);

        if (checkWin(board, size, symbol)) {
            displayBoard(board, size);
            printf("Player %d (%c) wins!\n", currentPlayer + 1, symbol);
            winner = 1;
            break;
        }

        if (checkDraw(board, size)) {
            displayBoard(board, size);
            printf("It's a draw!\n");
            break;
        }

        moves++;
    }

    freeBoard(board, size);
    return 0;
}

// Function Implementations

char **initializeBoard(int size) {
    char **board = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        board[i] = (char *)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

void displayBoard(char **board, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            for (int k = 0; k < size; k++) {
                printf("---");
                if (k < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int isValidMove(char **board, int size, int row, int col) {
    return (row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ' ');
}

int checkWin(char **board, int size, char symbol) {
    // Check rows and columns
    for (int i = 0; i < size; i++) {
        int rowWin = 1, colWin = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) rowWin = 0;
            if (board[j][i] != symbol) colWin = 0;
        }
        if (rowWin || colWin) return 1;
    }

    // Check diagonals
    int diag1 = 1, diag2 = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) diag1 = 0;
        if (board[i][size - i - 1] != symbol) diag2 = 0;
    }
    return (diag1 || diag2);
}

int checkDraw(char **board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

void freeBoard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

void logMove(char **board, int size, int player, char symbol) {
    FILE *file = fopen(LOG_FILE, "a");
    if (!file) return;

    fprintf(file, "Player %d (%c) made a move\n", player, symbol);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, " %c ", board[i][j]);
            if (j < size - 1) fprintf(file, "|");
        }
        fprintf(file, "\n");
        if (i < size - 1) {
            for (int k = 0; k < size; k++) {
                fprintf(file, "---");
                if (k < size - 1) fprintf(file, "+");
            }
            fprintf(file, "\n");
        }
    }
    fprintf(file, "\n");
    fclose(file);
}

void userMove(char **board, int size, char symbol) {
    int row, col;
    while (1) {
        printf("Enter row and column (0-%d): ", size - 1);
        scanf("%d %d", &row, &col);
        if (isValidMove(board, size, row, col)) {
            board[row][col] = symbol;
            break;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

void computerMove(char **board, int size, char symbol) {
    int row, col;
    do {
        row = rand() % size;
        col = rand() % size;
    } while (!isValidMove(board, size, row, col));
    board[row][col] = symbol;
    printf("Computer placed at (%d, %d)\n", row, col);
}
