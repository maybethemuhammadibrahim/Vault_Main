Here are your simplified notes for String Matching Algorithms.

---

# 1. The Naive Approach (Brute Force)
*The "Check Every Single Spot" Method*

### How it Works
This is the most intuitive way to search. You place the Pattern at the very beginning of the Text. You compare the first character, then the second, and so on.
1.  If you find a mismatch, you stop immediately.
2.  You slide the Pattern **one step** to the right.
3.  You start comparing again from the first character of the Pattern.

### Example Case
* **Text:** `A B A B C`
* **Pattern:** `A B C`
* **Step 1:** Compare `A B A` vs `A B C`. Match `A`, Match `B`, Mismatch `A` vs `C`. Slide 1.
* **Step 2:** Compare `B A B` vs `A B C`. Mismatch `B` vs `A` immediately. Slide 1.
* **Step 3:** Compare `A B C` vs `A B C`. **Match Found.**

### Edge Cases
* **Repeating Characters:** Text `AAAA...` and Pattern `AAAAB` triggers the worst-case scenario because you match almost until the end every single time before failing.

### Time Complexity
* **Time:** $O(n \times m)$ (where $n$ is Text length, $m$ is Pattern length).
* **Why?** In the worst case, for every character in the Text, you scan the entire Pattern.

---

# 2. Rabin-Karp Algorithm
*The "Rolling Hash" Method*

### How it Works
Instead of comparing characters one by one, this algorithm compares **numbers**. It converts the Pattern into a unique numerical "Hash Value" (like a fingerprint).
1.  Calculate the hash of the Pattern.
2.  Calculate the hash of the current window of text (same length as Pattern).
3.  **Compare Hashes:** If the numbers match, *then* you check the characters manually to be sure. If numbers don't match, you know the strings aren't the same.
4.  **Rolling Hash:** When sliding to the next spot, you don't recalculate the whole hash. You subtract the value of the character leaving the window and add the value of the new character entering (like a conveyor belt).

### Edge Cases
* **Spurious Hits (Collisions):** Sometimes two different strings generate the same hash value (e.g., 5+2 = 7 and 4+3 = 7). This forces a manual check that turns out to be false.

### Time Complexity
* **Average:** $O(n + m)$. You usually skip character comparisons.
* **Worst:** $O(n \times m)$. If you use a bad math formula for hashing, every substring might collide, forcing you to check characters manually every time.

---

# 3. KMP (Knuth-Morris-Pratt)
*The "Don't Look Back" Method*

### How it Works
The Naive approach is dumb because it forgets what it just matched. KMP is smart; it uses past matches to skip ahead.
If you match `A B A B` and fail on the next letter, KMP knows that the sub-segment `A B` is at the start of your pattern. Instead of sliding 1 step, it slides far enough to align that `A B` again. You never move backward in the main Text.

### The Secret Weapon: LPS Table (Longest Prefix Suffix)
This table tells us: **"If I mismatch at index $i$, how much of the pattern have I already matched that effectively restarts the pattern?"**

**Generating the LPS Table:**
We look for the longest sequence of characters at the *end* of a sub-pattern that matches the *start* of the pattern.
* **Pattern:** `A B A B A C`
1.  `A`: No prefix/suffix overlap. (Value: 0)
2.  `A B`: No overlap. (Value: 0)
3.  `A B A`: Ends with `A`, starts with `A`. Length 1. (Value: 1)
4.  `A B A B`: Ends with `A B`, starts with `A B`. Length 2. (Value: 2)
5.  `A B A B A`: Ends with `A B A`, starts with `A B A`. Length 3. (Value: 3)
6.  `A B A B A C`: Ends with `C`. No overlap with start `A`. (Value: 0)
* **Result:** `[0, 0, 1, 2, 3, 0]`

### Example Case
* **Text:** `A B A B A D ...`
* **Pattern:** `A B A B A C`
* **Mismatch:** We match everything up to `C` (index 5) against `D` in text.
* **Jump:** We look at the LPS value of the previous character (`A` at index 4). The value is **3**.
* **Action:** We keep the Text pointer where it is, but we shift the Pattern so that index 3 (`B`) aligns with the current spot. We saved comparisons!

### Time Complexity
* **Time:** $O(n + m)$.
* **Why?** We traverse the Text once ($n$) and build the table once ($m$). The text pointer never moves backward.

---

# 4. Boyer-Moore Algorithm
*The "Backwards Skipping" Method*

### How it Works
This is often the standard for practical use (Ctrl+F often uses variants of this). It compares the Pattern against the Text from **Right to Left**.
If the last character of the pattern doesn't match the text, we can potentially skip the *entire* length of the pattern.

### The Secret Weapon: Bad Match Table
This table tells us: **"I saw a character in the Text that didn't match. How far left is that character inside my own Pattern?"**

**Generating the Table (Formulaic Approach):**
For every character in the alphabet, we find its **last occurrence index** in the Pattern and plug it into this formula:
$$Value = \max(1, \text{Length of Pattern} - \text{Index} - 1)$$

*(Note: If a character is not in the pattern at all, we treat its Index as -1).*

**Example Calculation:**
* **Pattern:** `T O O T H`
* **Length ($m$):** 5

| Character | Last Index Found | Calculation ($5 - \text{Index} - 1$) | Final Value (Max of 1, Calc) |
| :--- | :--- | :--- | :--- |
| **T** | 3 | $5 - 3 - 1 = 1$ | **1** |
| **O** | 2 | $5 - 2 - 1 = 2$ | **2** |
| **H** | 4 | $5 - 4 - 1 = 0$ | **1** |
| **Others (*)** | -1 (Not found) | $5 - (-1) - 1 = 5$ | **5** |

### How to use this value?
When a mismatch happens:
1.  Look at the character in the **Text** that caused the mismatch.
2.  Check that character's value in your table.
3.  Slide the pattern to the right by that amount.

### Example Case
* **Text:** `T R U S T W O R T H Y`
* **Pattern:** `T O O T H`
1.  Align Pattern at start. Compare last char `H` with Text `T` (index 4).
2.  **Mismatch!** The text has a `T`.
3.  **Consult Table:** Where is `T` in my pattern? It is at index 3.
4.  **Jump:** Shift the pattern so the `T` in the pattern aligns with the `T` in the text.

*(Scenario 2: If the text had a `Z`, which isn't in the pattern at all, we would jump the WHOLE pattern length past that `Z`.)*

### Edge Cases
* **Small Alphabets:** DNA (A, C, G, T) works less effectively than English text because "Bad Characters" reappear too frequently, preventing large jumps.

### Time Complexity
* **Best Case:** $O(n / m)$. (Sub-linear!). If the text characters are not in the pattern, we skip $m$ steps at a time.
* **Worst Case:** $O(n \times m)$. (Rare, usually involves repetitive patterns).

---

# Summary of Time Complexities

| Algorithm | Pre-processing | Matching Time (Avg) | Matching Time (Worst) | Best Use Case |
| :--- | :--- | :--- | :--- | :--- |
| **Naive** | None | $O(n \times m)$ | $O(n \times m)$ | Short strings, simple implementation. |
| **Rabin-Karp** | $O(m)$ | $O(n + m)$ | $O(n \times m)$ | Finding **multiple** patterns at once (plagiarism detection). |
| **KMP** | $O(m)$ | $O(n)$ | $O(n)$ | Streaming text, finding matches in highly repetitive data (e.g., DNA). |
| **Boyer-Moore**| $O(m + \sigma)^*$ | $O(n)$ | $O(n \times m)$ | **General purpose text search** (Ctrl+F). Fast on large alphabets. |

*\* $\sigma$ = Size of alphabet (e.g., 256 for ASCII).*
