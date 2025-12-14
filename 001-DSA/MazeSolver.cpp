#include <iostream>
using namespace std;

#define N 4
// 1 = Open, 0 = Blocked
int maze[N][N] = { {1, 0, 0, 0},
                   {1, 1, 0, 1},
                   {0, 1, 0, 0},
                   {1, 1, 1, 1} };

int sol[N][N]; // To store the solution path

// Check if x, y is valid index and open path
bool isSafe(int x, int y) {
    // 1. Check Boundaries (Are we off the board?)
    if (x < 0 || x >= N || y < 0 || y >= N) 
        return false;

    // 2. Check Walls (Is this spot blocked?)
    if (maze[x][y] == 0) 
        return false;
        
    // 3. Check Previous Visits (Prevent infinite loops)
    // (Optional logic if you modify maze directly, usually needed)
    if (sol[x][y] == 1) 
        return false;

    // If we passed all checks, it's safe!
    return true;
}
// Main Backtracking Function
bool solveMaze(int x, int y) {
    // Base Case: Reached destination (bottom-right)
    if (x == N - 1 && y == N - 1) {
        sol[x][y] = 1;
        return true;
    }

    if (isSafe(x, y)) {
        sol[x][y] = 1; // Mark as part of solution path

        // Move Forward (Right)
        if (solveMaze(x, y + 1)) return true;

        // Move Down
        if (solveMaze(x + 1, y)) return true;

        // BACKTRACK: Unmark if neither way works
        sol[x][y] = 0; 
        return false;
    }
    return false;
}

int main() {
    if (solveMaze(0, 0)) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) cout << sol[i][j] << " ";
            cout << endl;
        }
    } else cout << "No path found";
    return 0;
}
