### 1. BFS Traversal (Breadth-First Search)
*The "Layer by Layer" Approach*
**Data Structure:** Queue (FIFO)

1.  **Start:** Put the starting node in the **Queue** and mark it as **Visited**.
2.  **Loop:** While the Queue is not empty:
    * **Dequeue** the front node and write it to your **Output**.
    * **Check Neighbors:** Look at all direct neighbors of this node.
    * **Enqueue:** If a neighbor has **not** been visited yet, add it to the Queue and mark it as Visited immediately.
3.  **Repeat:** Keep going until the Queue is empty.

> **Visual Tip:** Imagine dropping a stone in water. The ripples move out in perfect circles. That is BFS.

---

### 2. DFS Traversal (Depth-First Search)
*The "Maze Runner" Approach*
**Data Structure:** Stack (LIFO)

1.  **Start:** Push the starting node into the **Stack**.
2.  **Loop:** While the Stack is not empty:
    * **Pop** the top node.
    * **Process:** If this node has **not** been visited yet:
        * Mark it as **Visited** and write to **Output**.
        * **Push Neighbors:** Push all **unvisited** neighbors into the Stack.
        * *Important:* Push neighbors in **Reverse Order** (e.g., if you want to visit Left then Right, push Right first then Left).
3.  **Repeat:** Keep going until the Stack is empty.

> **Visual Tip:** Imagine solving a maze. You go down one path until you hit a dead end, then you backtrack to the last junction and try the next path.

---

### 3. Topological Sorting (DFS Method)
*The "Dead-End First" Approach*
**Data Structure:** Stack (for the final result)

1.  **Pick a Node:** Select any unvisited node to start.
2.  **Go Deep (DFS):** Travel to its unvisited neighbors recursively. Keep going until you cannot go any further (a "Dead End" or a node whose neighbors are all visited).
3.  **The "Dead End" Rule:** Once a node has no more unvisited paths coming out of it:
    * **Push that node into the Stack.**
    * **Backtrack** to the previous node.
4.  **Repeat:** Continue this process until the start node is pushed. Then pick the next unvisited node in the graph and repeat steps 1-3.
5.  **Final Result:** Once all nodes are processed, **Pop everything from the Stack**. That sequence is your Topological Sort.

> **Visual Tip:** You are listing tasks in reverse. The task that relies on nothing else (the "last" thing you do) gets put in the stack first (at the bottom).
