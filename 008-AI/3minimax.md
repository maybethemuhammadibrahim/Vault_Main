### 1. Multi-Agent Environments

**Formal Definition:** Systems containing multiple interacting agents where the optimal action depends on the behavior and intentions of other agents.

**Explanation:** When deciding how to handle environments with multiple agents, AI can take three approaches:
1. **The Economy Stance:** Treat agents in the aggregate. Instead of predicting individual actions, predict macro-level trends (e.g., supply and demand). 
2. **The Environmental Stance:** Treat adversaries as a random element of the environment (like weather). This is ineffective for games because it ignores the adversary's explicit intent to defeat you.
3. **The Game Theory Stance:** Explicitly model adversaries using game-tree search. This approach assumes opponents are actively trying to win and models their optimal responses to your actions.

---

### 2. Two-Player, Zero-Sum Games

**Formal Definition:** A deterministic, fully observable game played by two alternating players where an advantage for one player equates to an exact, corresponding disadvantage for the opponent.

**Explanation:** In a zero-sum game, there is no "win-win" scenario. If you gain 1 point, your opponent essentially loses 1 point. We refer to the player trying to maximize the score as **MAX** and the opponent trying to minimize the score as **MIN**. 

**Key Terminology:**
| Term | Intuitive Meaning |
| :--- | :--- |
| **State / Position** | The current configuration of the board or environment. |
| **Action / Move** | A legal play available to a player. |
| **Perfect Information** | Fully observable. Both players see the entire board (e.g., Chess). |
| **Imperfect Information** | Partially observable. Some data is hidden from players (e.g., Poker). |

**Formal Game Elements:**
Every game requires these mathematical components to function in an AI model:
* $S_0$: The initial setup of the game.
* $\text{TO-MOVE}(s)$: A function identifying whose turn it is.
* $\text{ACTIONS}(s)$: A function listing all legal moves available.
* $\text{RESULT}(s, a)$: The transition model. It outputs the exact new state after an action is taken.
* $\text{IS-TERMINAL}(s)$: A true/false check to see if the game has ended.
* $\text{UTILITY}(s, p)$: The final score awarded to a player at the end of the game (e.g., 1 for win, 0 for loss).

---

### 3. Minimax Algorithm

**Formal Definition:** A recursive search algorithm that calculates the optimal move in a zero-sum game by assuming both players play perfectly.

**Explanation:** Minimax explores all possible future moves by building a decision tree down to the end of the game (the terminal states). It evaluates the final scores and works backward. MAX will always choose the path with the highest score, and MIN will always choose the path with the lowest score. By working backward, the AI determines the best immediate move to make right now.

* **Algorithm Type:** Depth-first exploration.
* **Time Complexity:** $O(b^m)$ where $b$ is the number of legal moves per turn and $m$ is the maximum depth of the game.
* **Space Complexity:** $O(m)$ or $O(bm)$ depending on how actions are generated.
* **The Problem:** The exponential time complexity makes Minimax impractical for real-world games. Chess has roughly $10^{123}$ possible states, which a computer cannot calculate in time.

**Solutions for Complexity:**
Because we lack the time to search to the end of the game, we must stop the search early. When we stop, we use one of two methods to guess who is winning:
1.  **Heuristic Evaluation Function:** A mathematical formula that looks at board features to estimate a score.
2.  **Fast Simulations:** Playing out a simplified, randomized version of the game to the end multiple times and averaging the results.

---

### 4. Alpha-Beta Pruning

**Formal Definition:** An optimization technique for the minimax algorithm that eliminates branches in the search tree that cannot possibly influence the final decision.

**Explanation:** Alpha-beta pruning saves time by ignoring bad options. If you are evaluating a sequence of moves and discover that it gives your opponent a guaranteed opportunity to crush you, you stop analyzing that branch immediately. You "prune" it because you already know you will not choose it.

It relies on two tracking variables:
* $\alpha$: The highest (best) value MAX can guarantee so far.
* $\beta$: The lowest (best) value MIN can guarantee so far.

As the algorithm runs, if a node's value falls outside the acceptable range established by $\alpha$ and $\beta$, the search terminates for that specific branch, radically improving efficiency without changing the final output.