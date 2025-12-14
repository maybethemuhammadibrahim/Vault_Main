### 1. BFS Traversal (Breadth-First Search)
*The "Layer by Layer" Approach*
**Data Structure:** Queue (FIFO)

1.  **Start:** Put the starting node in the **Queue** and mark it as **Visited**.
2.  **Loop:** While the Queue is not empty:
    * **Dequeue** the front node and write it to your **Output**.
    * **Check Neighbors:** Look at all direct neighbors of this node.
    * **Enqueue:** If a neighbor has **not** been visited yet, add it to the Queue and mark it as Visited immediately.
3.  **Repeat:** Keep going until the Queue is empty.

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

To show cycle detection on paper for an exam, you need to track the **Recursion Stack** (current path) separately from the **Visited** set.

**The Rule:** A cycle exists if you encounter a neighbor that is currently in the **Recursion Stack**.

Here is the step-by-step dry run.

### Key
* **Visiting (Stack):** Node is currently active in recursion (Gray).
* **Visited (Done):** Node and all children fully processed (Black).

---

### Step 1: Start DFS from Node 1
**Stack:** `[1]`

1.  Current: **1**. Neighbor: **2**.
    * Push 2. **Stack:** `[1, 2]`
2.  Current: **2**. Neighbor: **3**.
    * Push 3. **Stack:** `[1, 2, 3]`
3.  Current: **3**. Neighbors: **4, 7**.
    * Pick 4. **Stack:** `[1, 2, 3, 4]`
4.  Current: **4**. Neighbor: **5**.
    * Push 5. **Stack:** `[1, 2, 3, 4, 5]`
5.  Current: **5**. Neighbor: **6**.
    * Push 6. **Stack:** `[1, 2, 3, 4, 5, 6]`
6.  Current: **6**. Neighbors: **None**.
    * Pop 6. Mark 6 as **Visited (Done)**.
    * **Stack:** `[1, 2, 3, 4, 5]`
7.  Back to **5**. No more neighbors.
    * Pop 5. Mark 5 as **Visited (Done)**.
    * **Stack:** `[1, 2, 3, 4]`
8.  Back to **4**. No more neighbors.
    * Pop 4. Mark 4 as **Visited (Done)**.
    * **Stack:** `[1, 2, 3]`
9.  Back to **3**. Remaining Neighbor: **7**.
    * Push 7. **Stack:** `[1, 2, 3, 7]`
10. Current: **7**. Neighbor: **5**.
    * Check 5: Is it in the Stack? **No**. Is it Visited? **Yes**.
    * *Result:* Cross Edge (safe). **No Cycle**.
    * Pop 7. Mark 7 as **Visited**.
    * **Stack:** `[1, 2, 3]`
11. Backtrack 3 $\to$ 2 $\to$ 1. All marked Visited. Stack Empty.

---

### Step 2: Continue to next unvisited Node (8)
**Stack:** `[8]`

1.  Current: **8**. Neighbor: **9**.
    * Push 9. **Stack:** `[8, 9]`
2.  Current: **9**. Neighbor: **10**.
    * Push 10. **Stack:** `[8, 9, 10]`
3.  Current: **10**. Neighbor: **8**.
    * Check 8: **Is it in the Stack?**
    * **YES.** (8 is the ancestor of 10).

### Conclusion for Exam
**Cycle Detected.**
The edge $10 \to 8$ is a **Back Edge** because node 8 is currently in the recursion stack (Visiting state).

**Cycle Path:** $8 \to 9 \to 10 \to 8$

Would you like me to write the pseudocode for this logic?
