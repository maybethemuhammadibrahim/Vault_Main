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

## 3.4 Uninformed Search Strategies

Uninformed search algorithms (also called blind search) are given no clues about how close a state is to the goal. They only know how to generate successors and distinguish a goal state from a non-goal state.

### 3.4.1 Breadth-First Search (BFS)

BFS explores the state space outward from the root in uniform waves—expanding the root, then all nodes 1 step away, then all nodes 2 steps away, and so on.

* **Implementation Mechanics:** It can be implemented as a generic `BEST-FIRST-SEARCH` where the evaluation function is the depth of the node ().
* **The Data Structure:** FIFO Queue (First-In, First-Out)
**How it works on a Tree/Graph:**
1. You start at the root node.
2. You look at all of the root's immediate child nodes and push them into the back of your queue.
3. You then pop the first node off the *front* of the queue to explore it.
4. You look at this new node's children and push them to the *very back* of the queue.
5. You repeat this process: always pulling the next node to explore from the front, and adding newly discovered nodes to the back.

**Why this works:** Because newly discovered, deeper nodes are always sent to the back of the line, the algorithm is forced to finish popping and checking every single node at Level 1 before it ever reaches a node at Level 2. It naturally searches the graph layer by layer, moving outward in uniform waves.

#### **Efficiency Tricks (Enhancements):**
* **FIFO Queue:** Instead of a complex priority queue, a simple First-In-First-Out (FIFO) queue ensures the correct expansion order. New nodes (deeper) go to the back; old nodes (shallower) get expanded first.
* **`Reached` as a Set:** The `reached` memory can be a simple set of states rather than a mapping structure. Why? Because the very first time BFS reaches a state, it is guaranteed to be the shortest path to that state.
* **Early Goal Test:** Because the first time a node is generated it is via the shortest path, BFS checks if a node is the goal *as soon as it is generated*, rather than waiting for it to be popped off the queue (a "late goal test").


#### **Completeness & Optimality:**
* **Complete:** Yes. It systematically searches every level, so it will always find a solution even in infinite state spaces.
* **Optimal:** It always finds the solution with the *minimal number of actions*. However, it is only **cost-optimal** if all actions share the exact same cost.


* **Complexity ( = branching factor,  = depth of solution):**
* Nodes generated: 1+b+b^2+b^3+⋯+b^d=O(b^d). .
* **Time & Space Complexity:** Both are O(b^d) because every generated node must remain in memory.
* **The Memory Problem (Example):** If b=10, speed is 1 million nodes/sec, and size is 1 KB/node: searching to d=10 takes under 3 hours but requires 10 terabytes of memory. At d=14, it would take 3.5 years. Memory runs out long before time does, meaning uninformed exponential searches can only handle very small instances.


### 3.4.2 Dijkstra’s Algorithm / Uniform-Cost Search (UCS)

When actions have different costs, BFS fails to be cost-optimal. Uniform-Cost Search solves this by spreading out in waves of uniform *path-cost* rather than uniform *depth*.

* **Implementation Mechanics:** Implemented as a `BEST-FIRST-SEARCH` where the evaluation function is the total cost from the root to the current node ().
* **The Data Structure:** Priority Queue (Lowest-Cost Out)

**How it works on a Tree/Graph:**
1. You start at the root node.
2. You look at the edges connecting the root to its child nodes. Each edge has a specific cost (a number, like distance).
3. You calculate the *total accumulated cost* from the root to each child, and push these nodes into your priority queue alongside their total cost.
4. Instead of popping the oldest node (like BFS), you search the queue and always pop the node that has the **lowest total cost** attached to it, regardless of how deep it is in the graph.
5. You explore that cheapest node, calculate the running total cost to reach its children, and push them into the queue.
6. **Crucial rule:** If you find a new, cheaper path to a node that is already sitting in your queue, you simply update that node in the queue with the new, lower cost. You only stop the search when the goal node is the cheapest thing popped *out* of the queue.
* **The Late Goal Test (Crucial Detail):** Unlike BFS, UCS evaluates goals only when expanding a node, not when generating it.
* *Romania Example:* If exploring from Sibiu to Bucharest, it might first discover a path via Fagaras costing 310. However, it does not stop. It leaves that node on the frontier and explores Pitesti, eventually discovering a different path to Bucharest costing 278. Because it uses a late goal test, it successfully catches and swaps out the 310 path for the optimal 278 path.


* **Completeness & Optimality:**
* **Complete:** Yes, provided all action costs are greater than some small positive number (), preventing infinite loops of zero-cost actions.
* **Optimal:** Yes. The first solution it pops off the frontier is mathematically guaranteed to be at least as cheap as any other unexpanded node.


* **Complexity ( = optimal solution cost,  = lowest action cost):**
* **Time & Space:** .
* This can actually be much larger than BFS () because UCS will stubbornly explore massive trees of very low-cost actions before trying a high-cost (but potentially shorter) path. (Note: If all action costs are equal, the complexity simplifies to , making it similar to BFS).



### 3.4.3 Depth-First Search (DFS)

DFS plunges immediately to the deepest level of the search tree. If a node has no successors, it "backs up" to the next deepest node that still has unexplored options.

* **Implementation Mechanics:** It could be written as a `BEST-FIRST-SEARCH` using negative depth for evaluation, but it is typically implemented as a tree-like search without a `reached` table to save memory.
* **The Data Structure:** LIFO Stack (Last-In, First-Out)

**How it works on a Tree/Graph:**
1. You start at the root node.
2. You look at its child nodes. You pick the very first child and immediately move to it.
3. You take the other sibling nodes you didn't pick and push them onto the top of your stack so you don't forget them.
4. From your new position, you again pick the first available child and dive deeper down that branch, pushing any skipped siblings onto the top of the stack.
5. You keep diving deeper and deeper until you hit a "leaf node" (a dead end with no children).
6. When you hit a dead end, you stop, look at your stack, and pop off the node sitting at the very top. Because a stack is Last-In, First-Out, this node will always be the most recent intersection you bypassed. You jump to that node and start diving down its branch.

* **Completeness & Optimality:**
* **Complete:** Only in finite, acyclic state spaces. In cyclic spaces, it gets stuck in infinite loops (unless cycle-checking is coded in). In infinite spaces, it fails completely as it will wander down an endless path.
* **Optimal:** No. It returns the very first solution it hits, regardless of cost.


* **Complexity ( = maximum depth of the tree):**
* **Time:** Proportional to the number of states.
* **Space:** . This is its superpower. While BFS must store the entire expanding spherical "surface" of the search tree, DFS only stores a single "radius" (one path from root to leaf, plus unexpanded sibling nodes).
* *Why use it?* Memory efficiency. A problem requiring exabytes of memory in BFS requires only kilobytes in DFS. It is the workhorse algorithm for constraint satisfaction, propositional satisfiability, and logic programming.



### 3.4.4 Backtracking Search

Backtracking is an extreme, highly optimized variant of Depth-First Search designed to use even less memory.

* **How it differs from DFS:** 1.  Instead of generating *all* successors of a node at once and storing them, it generates exactly *one* successor at a time. The partially expanded node just remembers which child to generate next.
2.  Instead of allocating new memory for a brand-new state, it modifies the *current* state description directly. (This requires the ability to perfectly undo an action when backing up).
* **Space Complexity:** Requires only **one** single state description and a path of  actions. This is a massive savings over the  states needed for standard DFS.
* **Cycle Checking Trick:** Because it only holds one path, you can maintain an efficient set data structure for the current path, reducing the time it takes to check for a cyclic loop from  down to  time.
* *Why use it?* It is absolutely critical for solving problems that have massive state descriptions (like complex robotic assembly), where copying the state into memory multiple times would crash the system.

---
