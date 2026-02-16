### 1. The Anatomy of a Search Problem

A search problem is a formal way to represent an agent's decision-making process. Think of it as mapping out a road trip.

* **State Space:** Every possible situation the environment can be in. *(Example: Every city in Romania.)*
* **Initial State:** Where you start. *(Example: Currently in Arad.)*
* **Goal State(s):** Where you want to end up. This can be one specific state or a condition that must be met. *(Example: Arriving in Bucharest, or a vacuum robot ensuring a room is 100% clean.)*
* **Actions:** The choices available at a given state. *(Example: Drive to Sibiu or Timisoara.)*
* **Transition Model:** The result of taking an action. *(Example: If you are in Arad and take the action "Drive to Sibiu", the resulting state is "Sibiu".)*
* **Action Cost Function:** The numerical penalty for taking an action. Costs are additive. *(Example: The miles driven, fuel used, or time spent getting from Arad to Sibiu.)*

**Key Outcomes:**

* **Path:** A sequence of actions linking the initial state to a goal state.
* **Optimal Solution:** The path that has the absolute lowest total cost.

---

### 2. Problem Formulation & Abstraction

To make problems solvable for AI, we must simplify reality. This is called **abstraction**.

* **What it is:** Stripping away irrelevant real-world details to focus strictly on what matters for the goal.
* **Why it matters:** If an AI calculating a route to Bucharest had to factor in the radio station playing or the exact angle of the steering wheel, the problem would be infinitely complex. Good abstraction removes the noise so the core problem is easy to compute.

---

### 3. State Space vs. Search Tree

It is critical not to confuse these two concepts:

* **State Space (The Map):** The actual, physical layout of the world (e.g., the map of cities and roads). It exists independently of the search.
* **Search Tree (The Exploration):** The pathways the algorithm *builds* as it explores the state space looking for the goal. A single city (state) might appear multiple times in a search tree if the algorithm finds different routes to get there.

---

### 4. Uninformed Search Strategies

"Uninformed" or "blind" searches have no extra clues about how far away the goal is. They only know the current state and the available actions.

To keep this concise, we will define the core variables used to calculate costs and complexity:

*  = **Branching factor** (the number of successors/options a node has).
*  = **Depth** of the shallowest goal node.
*  = **Maximum depth** of the search tree.
*  = **Cost** of the optimal solution.
*  = The minimum cost of any given action.

---

### Uninformed Search Algorithms: Quick Comparison

| Algorithm | Space (Fringe/Memory) | Complete? | Optimal? | Time Complexity |
| --- | --- | --- | --- | --- |
| **Breadth-First (BFS)** |  | Yes | Only if all step costs are equal |  |
| **Uniform-Cost (UCS)** |  | Yes (if cost ) | Yes |  |
| **Depth-First (DFS)** |  | No (can loop) | No |  (worst case) |
| **Backtracking** |  | No | No | Similar to DFS |

---

### 1. Breadth-First Search (BFS)

**How it works:** It searches level by level. It explores the root, then all nodes 1 step away, then all nodes 2 steps away, etc.

* **Space (Fringe):** . This is **exponential** and represents a massive memory flaw. At depth 10, a tree could require terabytes of RAM because the fringe holds the entire widest layer of the search tree. Memory will run out long before time does.
* **Is it Complete?:** **Yes**. Because it searches systematically level by level, it will always find a solution if one exists, even in an infinite space.
* **Is it Optimal?:** **Yes, but with a catch**. It is only optimal if every action has the exact same cost. It finds the path with the *fewest steps*, not necessarily the *cheapest cost*.
* **Time Complexity & Costs:** . It evaluates every single node down to depth .

### 2. Uniform-Cost Search (UCS) / Dijkstra’s Algorithm

**How it works:** Instead of expanding wave by wave based on *depth* (like BFS), UCS expands wave by wave based on *path cost*. It always expands the cheapest available node on the fringe.

* **Space (Fringe):** . This can actually be much worse than BFS if there are many small-cost steps, because the fringe will hold many low-cost paths before ever looking at a higher-cost (but potentially shorter) path.
* **Is it Complete?:** **Yes**, assuming every action has a cost greater than zero (no free infinite loops).
* **Is it Optimal?:** **Yes**. It specifically targets the lowest total cost and expands nodes strictly in order of increasing cost.
* **Time Complexity & Costs:** . It can be very slow because it explores all cheap paths in every direction before committing to a more expensive, direct path.

### 3. Depth-First Search (DFS)

**How it works:** Dives straight down a single path to the very bottom of the tree. If it hits a dead end, it backs up (backtracks) to the next closest unexplored branch.

* **Space (Fringe):** . **Highly efficient.** Unlike BFS, it only needs to keep a single path from the root to a leaf node in memory, plus the immediate sibling nodes along that path. This turns exabytes of memory into kilobytes.
* **Is it Complete?:** **No**. It can easily get trapped in infinite loops (cyclic spaces) or wander forever down an infinitely long path that never reaches a goal. (It is *only* complete in finite, acyclic trees).
* **Is it Optimal?:** **No**. It returns the very first solution it stumbles across, even if a vastly cheaper or shorter solution exists on another branch.
* **Time Complexity & Costs:** Time is proportional to the number of states, worst case . However, in dense trees with many solutions, it can actually be faster than BFS because it might strike lucky and find a deep solution immediately.

### 4. Backtracking Search (DFS Variant)

**How it works:** A highly optimized version of DFS. Instead of generating all successors of a node at once and storing them, it generates exactly *one* successor at a time.

* **Space (Fringe):**  actions and just ** state description**. It modifies the current state directly instead of copying it into a new memory block. This is the absolute most parsimonious use of memory possible.
* **Is it Complete?:** **No** (same flaws as standard DFS).
* **Is it Optimal?:** **No**.
* **Time Complexity & Costs:** Similar time constraints to DFS, but significantly cheaper in terms of overhead and hardware requirements. It is a workhorse for huge robotic/assembly problems.

---

Would you like me to create a scenario question where you must choose the correct algorithm based on these four factors?
