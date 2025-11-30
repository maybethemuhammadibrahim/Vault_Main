### 1. Greatest Common Divisor (GCD) & Primes

* **Primes and GCD:**
    * If $p$ is a prime number, then $\text{gcd}(p, n) = 1$ for any integer $n$, **unless** $n$ is a multiple of $p$.
    * *Example:* $\text{gcd}(7, 15) = 1$, but $\text{gcd}(7, 14) = 7$.
* **Pairwise vs. Setwise Relatively Prime:**
    * **Pairwise Relatively Prime:** Every distinct pair in the set must have a GCD of 1. (Stronger condition).
    * **Setwise Relatively Prime:** The GCD of *all* numbers in the set combined is 1, even if some pairs share factors. (Weaker condition).

### 2. Modular Multiplicative Inverse ($a^{-1}$)

* **Definition:** An integer $x$ is the inverse of $a \pmod m$ if:
    $$ax \equiv 1 \pmod m$$
* **Existence Condition:** An inverse only exists if $a$ and $m$ are **coprime** (i.e., $\text{gcd}(a, m) = 1$).
* **Concept:** It is the modular equivalent of division. To divide by $a$, we multiply by $a^{-1}$.

### 3. The Extended Euclidean Algorithm & Bézout's Identity

* **Bézout's Identity:** For any integers $a$ and $m$, there exist coefficients $x$ and $y$ such that:
    $$ax + my = \text{gcd}(a, m)$$
* **Deriving the Inverse:**
    1.  If $\text{gcd}(a, m) = 1$, then $ax + my = 1$.
    2.  Apply modulo $m$ to the equation. The term $my$ becomes 0.
    3.  Result: $ax \equiv 1 \pmod m$.
    4.  Therefore, the **Bézout coefficient $x$** is the modular inverse of $a$.
* **Calculation Steps:**
    1.  **Forward Pass:** Use Euclidean division until the remainder is 1.
    2.  **Backward Pass:** Substitute remainders upward to express 1 as a linear combination of $a$ and $m$.
    3.  **Adjust:** If the resulting coefficient is negative, add $m$ to make it positive.

### 4. Linear Congruences ($ax \equiv b \pmod m$)

* **Goal:** Solve for unknown $x$.
* **Algorithm:**
    1.  Find the inverse of $a$ (denoted $a^{-1}$) using the Extended Euclidean Algorithm.
    2.  Multiply $b$ by $a^{-1}$.
    3.  Solution: $x \equiv (a^{-1} \cdot b) \pmod m$.
* **Interpreting the Result:**
    * A result like $x \equiv 88 \pmod{89}$ is not just one number.
    * It represents an infinite set of solutions: $x = 88 + 89k$ (where $k$ is any integer).
    * $88$ is simply the smallest positive integer in that set.

### 5. Key Examples from Chat

* **Inverse of 7 mod 30:**
    * Found via Extended Euclidean: $13(7) - 3(30) = 1$.
    * Inverse: **13**.
* **Solving $55x \equiv 34 \pmod{89}$:**
    * Inverse of 55 mod 89 found to be 34.
    * $x \equiv 34 \times 34 \pmod{89}$.
    * $x \equiv 1156 \pmod{89} \to \mathbf{88}$.
* **Inverse of 2 mod 17:**
    * Coefficient was -8.
    * Adjusted: $-8 + 17 = \mathbf{9}$.
