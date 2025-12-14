#include <iostream>
using namespace std;

#define N 4 
int board[N][N]; // Global board initialized to 0

// Check if placing a queen at board[row][col] is safe
bool isSafe(int row, int col) {
    int i, j;

    // 1. Check Vertical Column (Straight Up)
    for (i = 0; i < row; i++) {
        if (board[i][col] == 1) return false;
    }

    // 2. Check Top-Left Diagonal
    i = row; j = col;
    while (i >= 0 && j >= 0) {
        if (board[i][j] == 1) return false;
        i--; // Move Up
        j--; // Move Left
    }

    // 3. Check Top-Right Diagonal
    i = row; j = col;
    while (i >= 0 && j < N) {
        if (board[i][j] == 1) return false;
        i--; // Move Up
        j++; // Move Right
    }

    return true; // No attacks found
}

// Main Backtracking Function
bool solveNQ(int row) {
    // Base Case: All queens are placed
    if (row >= N) return true;

    // Try all columns for the current row
    for (int col = 0; col < N; col++) {
        if (isSafe(row, col)) {
            board[row][col] = 1; // Place Queen

            if (solveNQ(row + 1)) // Recur to place next queen
                return true;

            board[row][col] = 0; // BACKTRACK: Remove Queen (Undo)
        }
    }
    return false; // No solution found for this row
}

int main() {
    if (solveNQ(0)) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) cout << board[i][j] << " ";
            cout << endl;
        }
    } else cout << "No solution";
    return 0;
}
