### Constraint Satisfaction Problems (CSPs)

**Formal Definition:** A state-space search problem where the state is defined by a set of variables $V = \{X_1, X_2, ..., X_n\}$, each taking a value from a corresponding domain of values $D = \{D_1, D_2, ..., D_n\}$. A set of constraints $C$ specifies the allowable combinations of values.

**Intuitive Explanation:** Standard search problems (like navigating a maze) ask "How do I get from start to finish?" CSPs ask "What is the valid configuration?" You are given empty slots (variables), a list of items to fill them with (domains), and rules about what cannot go together (constraints). Solving it means finding a combination that breaks no rules.

---

### Why CSP is Preferred Over Standard Search

Standard search algorithms (like A* or BFS) treat states as black boxes. They only know if a state is the goal or not. CSPs expose the internal structure of the state.

**Benefits:**
1.  **Early Failure Detection:** A standard search might explore a long path before realizing it fails. A CSP evaluates constraints locally; if $X_1$ and $X_2$ violate a rule, the algorithm immediately discards all future states containing that combination, drastically reducing the search space.
2.  **Universal Heuristics:** Standard search requires domain-specific heuristics (e.g., straight-line distance for routing). CSPs use general-purpose heuristics derived from the structure itself, such as "assign the variable with the fewest remaining options first."
3.  **Problem Independence:** A single, standardized CSP solver can resolve scheduling, routing, and configuration problems without needing custom algorithms for each.

---

### Modeling Real-World Problems: Identifying V, D, and C

To convert a real-world problem into a CSP, categorize the data into Variables, Domains, and Constraints. 
* **Variables (V):** The entities requiring a decision. 
* **Domains (D):** The available options for those entities.
* **Constraints (C):** The physical, logical, or business rules restricting combinations.

| Problem Type | Variables ($V$) | Domains ($D$) | Constraints ($C$) |
| :--- | :--- | :--- | :--- |
| **Assignment** | The classes to be taught. | The pool of available professors. | Professor X cannot teach two classes at the same time. |
| **Timetabling** | The specific class sessions. | Time slots and room numbers. | A room cannot host two classes simultaneously; Room size must exceed class size. |
| **Hardware Config** | Motherboard, CPU, RAM slots. | Available hardware models. | CPU socket type must match Motherboard socket type. |
| **Factory Scheduling** | Production tasks. | Machines and time blocks. | Task B cannot start until Task A finishes. |

---

### Classification of Variables

**Formal Definition:** Variables represent the state components. Their classification dictates the type of algorithms required to solve the CSP.

* **Discrete Variables (Finite Domain):** Variables with a countable, limited set of options. 
    * *Example:* Colors for a map region (Red, Green, Blue). Easily solved by standard logical algorithms.
* **Discrete Variables (Infinite Domain):** Variables with a countable but unlimited set of options.
    * *Example:* Integer starting days for a construction project. Requires algebraic constraints (e.g., $Start_1 + Duration < Start_2$) rather than listing all combinations.
* **Continuous Variables (Real-Valued):** Variables that can take on any fractional value within a range. 
    * *Example:* The exact temperature of a chemical reactor, or precise GPS coordinates. These cannot be solved by simple search; they require operations research techniques like Linear Programming.

---

### Classification of Constraints

**Formal Definition:** Constraints are relations specifying valid combinations of variable assignments. 

* **Unary Constraints:** Restricts the value of a single variable. 
    * *Example:* South Australia cannot be Green.
* **Binary Constraints:** Relates two variables. 
    * *Example:* South Australia and Western Australia cannot be the same color.
* **Higher-Order Constraints:** Relates three or more variables. 
    * *Example:* $X + Y + Z = 10$. 
* **Soft Constraints (Preferences):** Rules that should ideally be met but can be broken if necessary. These turn a CSP into an optimization problem. 
    * *Example:* Professor Smith *prefers* to teach in the morning. 

---

### Assignments and Solutions

**Formal Definition:** An assignment is a mapping of values to variables. 

* **Consistent (Legal) Assignment:** Breaks zero constraints.
* **Complete Assignment:** Every variable in the problem has been assigned a value.
* **Partial Assignment:** Some variables remain blank.
* **Solution:** An assignment that is both complete and consistent.

---

### Solving CSPs: Algorithm Dry Run

Standard CSPs are solved using **Backtracking Search**, a depth-first search that chooses values for one variable at a time and backtracks when no legal values are left.



**Intuitive Example:** Map coloring for 3 adjacent regions: Western Australia (WA), Northern Territory (NT), and South Australia (SA). 
* **Domain:** {Red, Blue}
* **Constraint:** Adjacent regions cannot share a color.

**Step-by-Step Dry Run:**
1.  **Start:** All regions empty.
2.  **Pick Variable:** Choose WA. 
3.  **Assign Value:** Assign WA = Red. (Consistent).
4.  **Pick Variable:** Choose NT. NT touches WA. 
5.  **Assign Value:** NT cannot be Red. Assign NT = Blue. (Consistent).
6.  **Pick Variable:** Choose SA. SA touches both WA and NT.
7.  **Evaluate:** SA cannot be Red (touches WA). SA cannot be Blue (touches NT). 
8.  **Dead End (Conflict):** No values left for SA.
9.  **Backtrack:** Go back to the last decision point (NT). 
10. **Re-assign:** Try a different color for NT. No colors left (Domain only had Red/Blue). 
11. **Backtrack:** Go back to WA. 
12. **Re-assign:** Assign WA = Blue. 
13. **Repeat:** The process continues, ultimately proving mathematically that 3 adjacent regions require at least 3 colors. The algorithm terminates and returns "Failure/No Solution."

---

### Constraint Propagation: Inference in CSPs

**Formal Definition:** The process of using constraints to reduce the legal values for a variable, which in turn reduces the legal values for other connected variables.

**Intuitive Explanation:** Instead of blindly guessing and backtracking, the system uses logic to cross off impossible options in advance. If you know X must be greater than Y, and Y is assigned 5, you immediately cross off 1, 2, 3, 4, and 5 from X's options. 

#### Types of Local Consistency

1.  **Node Consistency (1-Consistency)**
    * **Definition:** All unary constraints are satisfied.
    * **Intuition:** Trimming a variable's domain based only on rules applying directly to it. If $X < 5$, cross off 5, 6, 7 from $X$'s domain immediately.

2.  **Arc Consistency (2-Consistency)**
    * **Definition:** Every value in a variable's domain satisfies the binary constraints with another variable.
    * **Intuition:** Looking at variables in pairs. If $Y = X^2$, and $X = \{0, 1, 2, 3\}$, then $Y$ must be $\{0, 1, 4, 9\}$. Any other numbers in $Y$'s domain are crossed off because no $X$ value could generate them. 

3.  **Path Consistency (3-Consistency)**
    * **Definition:** A two-variable set $\{X_i, X_j\}$ is consistent with a third variable $X_m$.
    * **Intuition:** Looking at variables in triples. As seen in the map example above, arc consistency alone doesn't catch the problem that WA, NT, and SA mutually block each other. Path consistency looks ahead at the "path" between three variables to eliminate configurations that will inevitably fail.

4.  **K-Consistency**
    * **Definition:** For any consistent assignment to $k-1$ variables, a valid value can be found for any $k$-th variable.
    * **Strongly K-Consistent:** A graph that is $k$-consistent, $k-1$-consistent, down to 1-consistent.
    * **The Tradeoff:** If a problem with $n$ variables is strongly $n$-consistent, you can solve it instantly without backtracking (Time complexity: $O(n^2 d)$). However, making a graph $n$-consistent requires exponential time and memory. Therefore, algorithms typically only enforce 2-consistency (Arc) or 3-consistency (Path) to balance the cost of preprocessing vs. the cost of searching.