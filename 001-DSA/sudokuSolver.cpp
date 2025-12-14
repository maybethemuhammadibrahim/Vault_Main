#include <iostream>
using namespace std;

#define N 9
// 0 represents an empty cell
int board[N][N] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

// Check if placing 'num' at board[row][col] is valid
bool isSafe(int row, int col, int num) {
    
    // 1. Check Row & Column in one loop
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num) return false; // Check Row
        if (board[i][col] == num) return false; // Check Col
    }

    // 2. Check 3x3 Subgrid
    // Calculate the top-left corner of the 3x3 box
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) 
                return false;
        }
    }

    return true; // Safe to place
}

// Main Backtracking Function
bool solveSudoku() {
    int row, col;
    bool isEmpty = false;

    // STEP 1: Find an empty cell (marked with 0)
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                isEmpty = true;
                break;
            }
        }
        if (isEmpty) break; // Found one, break outer loop
    }

    // Base Case: No empty cells left? We are done!
    if (!isEmpty) return true;

    // STEP 2: Try numbers 1 to 9
    for (int num = 1; num <= 9; num++) {
        
        if (isSafe(row, col, num)) {
            board[row][col] = num; // Place Number

            if (solveSudoku()) // Recur to fill the rest
                return true;

            board[row][col] = 0; // BACKTRACK: Reset to 0 (Undo)
        }
    }
    
    return false; // Trigger backtracking
}

int main() {
    if (solveSudoku()) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) cout << board[i][j] << " ";
            cout << endl;
        }
    } else cout << "No solution exists";
    
    return 0;
}
