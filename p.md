Here is a comprehensive breakdown of the `PuzzleGenerator` class.

### **1. High-Level Structure & Structs**

This class generates a "NetWalk" or "Pipe" style puzzle. The goal is to create a random spanning tree (a maze without loops) connecting a central Server to multiple Computers, determine the correct shape of the pipes based on that tree, and then scramble them.

#### **The Helper Structs**

1.  **`struct Connection`**:

      * **Purpose:** Represents an "Edge" in graph theory terms.
      * **Usage:** It stores a link between two adjacent grid cells (`from` and `to`). If a connection exists between $(0,0)$ and $(0,1)$, a pipe connects them. This list is essentially the "skeleton" of the puzzle before shapes are assigned.

2.  **`struct PointCount`**:

      * **Purpose:** Used to calculate the "Degree" of a node (how many connections it has).
      * **Usage:** Used exclusively in the `placeComputers` function.
      * **Logic:**
          * If a point has `count = 1`, it is a **Leaf Node** (a dead end). In this game, dead ends become **Computers**.
          * If a point has `count > 1`, it is a pipe or intersection.

-----

### **2. Code Breakdown**

#### **Class Variables & Constructor**

**Logic:** Sets up movement deltas and seeds the random number generator.

```cpp
class PuzzleGenerator {
private:
    // Direction Arrays: Up, Down, Left, Right
    // Used to calculate neighbor coordinates easily in loops
    const int dr[4] = { -1, 1, 0, 0 }; // Change in Row
    const int dc[4] = { 0, 0, -1, 1 }; // Change in Column

    struct Connection {
        Point from;
        Point to;
    };

    struct PointCount {
        Point position;
        int count; // Number of connections attached to this point
    };

public:
    // Constructor
    PuzzleGenerator() {
        // Seed the random generator with current time to ensure
        // a different puzzle is generated every time the program runs.
        srand(static_cast<unsigned int>(time(0)));
    }
```

-----

#### **Function: `generatePuzzle` (The Manager)**

**Algorithm:**

1.  **Initialize:** Clear the grid and fill with empty space.
2.  **Setup Server:** Place the 'N' (Server) in the exact middle.
3.  **Create Skeleton:** Use `generateMazeRecursive` (Randomized DFS) to fill the grid with a maze.
4.  **Identify Endpoints:** Use `placeComputers` to find dead ends and turn them into 'C'.
5.  **Prune:** Use `removeDeadEnds` to delete any pipe branches that don't lead to a computer (clean up the puzzle).
6.  **Shape:** Use `assignPipeShapes` to turn abstract connections into visual pipe characters (L, -, +).
7.  **Scramble:** Rotate tiles randomly to create the gameplay element.

<!-- end list -->

```cpp
    void generatePuzzle(vector<vector<Tile>>& grid, int rows, int cols,
        Point& serverPos, vector<Point>& computerPos) {
        
        computerPos.clear(); // Reset computer list
        
        // 1. Initialize Grid
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) 
                grid[i][j].setType(' '); // Start with empty tiles
        }
        
        // 2. Place Server in center
        serverPos.r = rows / 2;
        serverPos.c = cols / 2;
        grid[serverPos.r][serverPos.c].setType('N'); // 'N' = Network Server
        
        // Setup for Maze Generation
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        vector<Connection> connections; // Will store the spanning tree edges

        // 3. Generate the logical maze structure
        generateMazeRecursive(grid, serverPos, rows, cols, visited, connections);
        
        // 4. Turn maze dead-ends into Computers
        placeComputers(grid, computerPos, connections, serverPos);

        // 5. Remove "false branches" (pipes that don't lead to computers)
        removeDeadEnds(grid, rows, cols, serverPos, computerPos);

        // 6. specific pipe shapes (curved, straight, T-shape) based on connections
        assignPipeShapes(grid, connections, rows, cols);
        
        // 7. Rotate tiles randomly to make it a puzzle
        scramblePuzzle(grid, rows, cols);
    }
```

-----

#### **Function: `generateMazeRecursive`**

**Algorithm (Randomized DFS):**

1.  Mark current cell as visited.
2.  Create a list of 4 directions and shuffle them randomly.
3.  Iterate through the shuffled directions:
      * Calculate neighbor coordinates.
      * If neighbor is valid (inside bounds) and **not visited**:
          * Mark current cell as part of the maze ('X').
          * Record the connection (current -\> neighbor).
          * Recursively call the function on the neighbor.

<!-- end list -->

```cpp
private:
    void generateMazeRecursive(vector<vector<Tile>>& grid, Point curr, int rows, int cols,
        vector<vector<bool>>& visited,
        vector<Connection>& connections) {
        
        visited[curr.r][curr.c] = true; // Mark current node as visited
        vector<int> dirs = {0, 1, 2, 3}; // Indices for dr/dc arrays

        // Simple shuffle algorithm (Fisher-Yates style simplified)
        // Randomizes the order we check neighbors to create unpredictable mazes
        for (int i = 0; i < 10; i++) {
            int idx1 = rand() % 4;
            int idx2 = rand() % 4;
            int temp = dirs[idx1];
            dirs[idx1] = dirs[idx2];
            dirs[idx2] = temp;
        }

        // Iterate through randomized directions
        for (int i = 0; i < dirs.size(); i++) {
            int dir = dirs[i];
            int nr = curr.r + dr[dir]; // New Row
            int nc = curr.c + dc[dir]; // New Col

            // Check bounds and if already visited
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                grid[nr][nc].setType('X'); // Mark as a generic path for now

                // Store this connection (Edge)
                Connection conn;
                conn.from = curr;
                conn.to.r = nr;
                conn.to.c = nc;
                connections.push_back(conn);

                // Prepare next point for recursion
                Point nextPoint;
                nextPoint.r = nr;
                nextPoint.c = nc;
                
                // Recurse (Deep Dive)
                generateMazeRecursive(grid, nextPoint, rows, cols, visited, connections);
            }
        }
    }
```

-----

#### **Function: `placeComputers`**

**Algorithm:**

1.  Iterate through the `connections` list.
2.  Count how many times every specific coordinate appears.
3.  If a coordinate appears exactly **once**, it means it is an endpoint (Leaf Node).
4.  If that endpoint is not the Server, mark it as a Computer ('C').

<!-- end list -->

```cpp
    void placeComputers(vector<vector<Tile>>& grid, vector<Point>& computerPos,
        const vector<Connection>& connections, Point serverPos) {

        vector<PointCount> counts; // Map-like structure to store degrees

        // 1. Count connections for each point
        for (int i = 0; i < connections.size(); i++) {
            Point p1 = connections[i].from;
            Point p2 = connections[i].to;

            // Logic to update count for p1:
            // Check if p1 already exists in our 'counts' vector
            bool found1 = false;
            for (int j = 0; j < counts.size(); j++) {
                if (counts[j].position.r == p1.r && counts[j].position.c == p1.c) {
                    counts[j].count++; // Increment degree
                    found1 = true;
                    break;
                }
            }
            // If not found, add it with count 1
            if (!found1) {
                PointCount pc;
                pc.position = p1;
                pc.count = 1;
                counts.push_back(pc);
            }

            // Repeat exact same logic for p2 (the other end of the connection)
            bool found2 = false;
            for (int j = 0; j < counts.size(); j++) {
                if (counts[j].position.r == p2.r && counts[j].position.c == p2.c) {
                    counts[j].count++;
                    found2 = true;
                    break;
                }
            }
            if (!found2) {
                PointCount pc;
                pc.position = p2;
                pc.count = 1;
                counts.push_back(pc);
            }
        }

        // 2. Identify Leaf Nodes
        for (int i = 0; i < counts.size(); i++) {
            Point pos = counts[i].position;
            
            // Skip the server (it might have 1 connection but it's not a computer)
            if (pos.r == serverPos.r && pos.c == serverPos.c) continue;
            
            // If degree is 1, it's a dead end -> Place Computer
            if (counts[i].count == 1) {
                grid[pos.r][pos.c].setType('C');
                computerPos.push_back(pos); // Add to list of computers
            }
        }
    }
```

-----

#### **Function: `removeDeadEnds`**

**Algorithm (Path Pruning):**
*Why? To make the puzzle cleaner. We only want pipes that actually carry data.*

1.  **Forward BFS:** Start at Server. Mark every tile reachable via pipes.
2.  **Backward BFS:** Start at *every* Computer. Traverse back towards the Server (only using tiles marked in step 1). Mark these tiles as `isUseful`.
3.  **Cleanup:** Loop through grid. If a tile is a pipe but NOT marked `isUseful`, delete it.

<!-- end list -->

```cpp
    void removeDeadEnds(vector<vector<Tile>>& grid, int rows, int cols,
        Point serverPos, const vector<Point>& computerPos) {
        
        // isUseful will mark pipes that are strictly necessary to solve the puzzle
        vector<vector<bool>> isUseful(rows, vector<bool>(cols, false));

        // Always mark Server and Computers as useful
        isUseful[serverPos.r][serverPos.c] = true;
        for (int i = 0; i < computerPos.size(); i++) {
            isUseful[computerPos[i].r][computerPos[i].c] = true;
        }

        // --- STEP 1: Find all pipes connected to Server ---
        vector<vector<bool>> reachableFromServer(rows, vector<bool>(cols, false));
        vector<Point> queue;
        
        // BFS Setup
        queue.push_back(serverPos);
        reachableFromServer[serverPos.r][serverPos.c] = true;

        int queueStart = 0;
        while (queueStart < queue.size()) {
            Point curr = queue[queueStart];
            queueStart++;

            // Check 4 neighbors
            for (int d = 0; d < 4; d++) {
                int nr = curr.r + dr[d];
                int nc = curr.c + dc[d];

                // If neighbor is valid, not visited, and IS A PIPE/COMPONENT
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                    !reachableFromServer[nr][nc] && grid[nr][nc].type != ' ') {
                    reachableFromServer[nr][nc] = true;
                    Point next; next.r = nr; next.c = nc;
                    queue.push_back(next);
                }
            }
        }

        // --- STEP 2: Find paths from Computers back to Server ---
        for (int comp = 0; comp < computerPos.size(); comp++) {
            Point compPos = computerPos[comp];
            vector<vector<bool>> visited(rows, vector<bool>(cols, false));
            vector<Point> bfsQueue;
            
            bfsQueue.push_back(compPos);
            visited[compPos.r][compPos.c] = true;

            int start = 0;
            while (start < bfsQueue.size()) {
                Point curr = bfsQueue[start];
                start++;

                // If this tile connects to server, it's a vital part of the solution
                if (reachableFromServer[curr.r][curr.c]) {
                    isUseful[curr.r][curr.c] = true;
                }

                // Explore neighbors
                for (int d = 0; d < 4; d++) {
                    int nr = curr.r + dr[d];
                    int nc = curr.c + dc[d];

                    // Standard BFS logic + check if neighbor is reachable from server
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                        !visited[nr][nc] && grid[nr][nc].type != ' ' &&
                        reachableFromServer[nr][nc]) {
                        visited[nr][nc] = true;
                        isUseful[nr][nc] = true; // Mark as useful path
                        Point next; next.r = nr; next.c = nc;
                        bfsQueue.push_back(next);
                    }
                }
            }
        }

        // --- STEP 3: Delete useless pipes ---
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                // If it's a pipe component but NOT on a useful path, delete it
                if (grid[r][c].type != ' ' && grid[r][c].type != 'N' &&
                    grid[r][c].type != 'C' && !isUseful[r][c]) {
                    grid[r][c].setType(' ');
                }
            }
        }
    }
```

-----

#### **Function: `assignPipeShapes`**

**Algorithm:**

1.  **Build Adjacency List:** Convert the list of `connections` into a grid-based lookup table so we can instantly ask "Who is connected to coordinate (x,y)?".
2.  **Iterate Grid:** For every 'X' (generic pipe):
      * Check neighbors (Up, Down, Left, Right) using the Adjacency List.
      * **Straight (-):** If connected Up+Down OR Left+Right.
      * **Elbow (L):** If connected (Up OR Down) AND (Left OR Right).
      * **T-Junction (+):** If connected to 3 neighbors.
      * **Default (-):** Fallback.

<!-- end list -->

```cpp
    void assignPipeShapes(vector<vector<Tile>>& grid,
        const vector<Connection>& connections,
        int rows, int cols) {
        
        // 1. Build Adjacency List (Flattened 2D to 1D index)
        // adj[i] contains a list of Points connected to tile i
        vector<vector<Point>> adj(rows * cols);
        
        for (int i = 0; i < connections.size(); i++) {
            Point p1 = connections[i].from;
            Point p2 = connections[i].to;
            // Calculate 1D array index: row * width + col
            int idx1 = p1.r * cols + p1.c;
            int idx2 = p2.r * cols + p2.c;
            
            // Add bi-directional connection
            adj[idx1].push_back(p2);
            adj[idx2].push_back(p1);
        }

        // 2. Determine shapes
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c].type == 'X') { // Only process generic pipes
                    bool up = false, down = false, left = false, right = false;
                    int idx = r * cols + c;
                    
                    // Check all connections for this specific tile
                    for (int i = 0; i < adj[idx].size(); i++) {
                        Point p = adj[idx][i];
                        if (p.r == r - 1) up = true;
                        if (p.r == r + 1) down = true;
                        if (p.c == c - 1) left = true;
                        if (p.c == c + 1) right = true;
                    }

                    // Logic for shapes
                    // Straight pipe (Vertical or Horizontal)
                    if ((up && down) || (left && right)) grid[r][c].setType('-');
                    // Corner/Elbow (Vertical + Horizontal connection)
                    else if ((up || down) && (left || right)) grid[r][c].setType('L');
                    // 3-way intersection
                    else if (adj[idx].size() == 3) grid[r][c].setType('+');
                    // Fallback
                    else grid[r][c].setType('-');
                }
            }
        }
    }
```

-----

#### **Function: `scramblePuzzle`**

**Algorithm:**

1.  Iterate through the entire grid.
2.  If the tile is a Pipe, Corner, or T-Junction (not empty, not Server):
      * Generate a random number of rotations (1 to 3).
      * Call the tile's `.rotate()` method that many times.

<!-- end list -->

```cpp
    void scramblePuzzle(vector<vector<Tile>>& grid, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Don't rotate empty space or the Server (Server usually fixed)
                if (grid[i][j].type != ' ' && grid[i][j].type != 'N') {
                    
                    // Random number between 1 and 3
                    // (0 would mean it's already solved, 4 is full circle)
                    int rotations = (rand() % 3) + 1;
                    
                    // Apply rotations
                    for (int k = 0; k < rotations; k++) {
                        grid[i][j].rotate();
                    }
                }
            }
        }
    }
};
```
