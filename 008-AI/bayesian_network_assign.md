### The Pattern for Bayesian Network Formulas

To identify the correct formula, follow these three rules based on the structure of the network:

1.  **The Joint Probability Rule:** The probability of any complete state is the product of each node given its parents.
    $$P(S, R, Wo, We, M, B) = P(S)P(R|S)P(Wo|R)P(We|R,Wo)P(M|We)P(B|S,Wo)$$
2.  **Marginalization (Summing Out):** If a variable is missing from your query, you must sum the probabilities of all its possible states.
    $$P(X) = \sum_{y} P(X, y) = P(X, y) + P(X, \neg y)$$
3.  **Conditional Probability (Bayes' Rule):** To find the probability of a node given its descendant (evidence), use the definition of conditional probability.
    $$P(X|E) = \frac{P(X, E)}{P(E)}$$

---

### Solutions

**Network Structure Deduced from CPTs:**
* $S \rightarrow R \rightarrow Wo$
* $R, Wo \rightarrow We \rightarrow M$
* $S, Wo \rightarrow B$

#### (a) What is the probability that it rains? $P(R)$
**Pattern:** R has parent S. Sum out S.
**Formula:** $$P(R) = \sum_{s} P(R|s)P(s) = P(R|S)P(S) + P(R|\neg S)P(\neg S)$$
**Work:**
$$P(R) = (0.7 \times 0.25) + (0.3 \times 0.75) = 0.175 + 0.225$$
**Answer:** **0.4000**

#### (b) What is the probability that the grass is wet? $P(We)$
**Pattern:** We has parents R and Wo. Sum out R and Wo. First, calculate the joint probabilities of R and Wo.
**Formula:** $$P(We) = \sum_{r, wo} P(We|r, wo)P(r, wo)$$
**Work:**
* $P(R) = 0.4000$, $P(\neg R) = 0.6000$
* $P(R, Wo) = P(Wo|R)P(R) = 0.7 \times 0.4 = 0.28$
* $P(R, \neg Wo) = P(\neg Wo|R)P(R) = 0.3 \times 0.4 = 0.12$
* $P(\neg R, Wo) = P(Wo|\neg R)P(\neg R) = 0.2 \times 0.6 = 0.12$
* $P(\neg R, \neg Wo) = P(\neg Wo|\neg R)P(\neg R) = 0.8 \times 0.6 = 0.48$
* $P(We) = (0.12 \times 0.28) + (0.25 \times 0.12) + (0.10 \times 0.12) + (0.08 \times 0.48)$
* $P(We) = 0.0336 + 0.0300 + 0.0120 + 0.0384$
**Answer:** **0.1140**

#### (c) Given that it is spring, how likely is someone mowing? $P(M|S)$
**Pattern:** Find $P(We|S)$ first, as We connects S to M.
**Formula:** $$P(M|S) = P(M|We)P(We|S) + P(M|\neg We)P(\neg We|S)$$
**Work:**
1.  Calculate probabilities of R and Wo given S:
    * $P(R, Wo|S) = P(Wo|R)P(R|S) = 0.7 \times 0.7 = 0.49$
    * $P(R, \neg Wo|S) = P(\neg Wo|R)P(R|S) = 0.3 \times 0.7 = 0.21$
    * $P(\neg R, Wo|S) = P(Wo|\neg R)P(\neg R|S) = 0.2 \times 0.3 = 0.06$
    * $P(\neg R, \neg Wo|S) = P(\neg Wo|\neg R)P(\neg R|S) = 0.8 \times 0.3 = 0.24$
2.  Calculate $P(We|S)$:
    * $P(We|S) = (0.12 \times 0.49) + (0.25 \times 0.21) + (0.10 \times 0.06) + (0.08 \times 0.24)$
    * $P(We|S) = 0.0588 + 0.0525 + 0.0060 + 0.0192 = 0.1365$
    * $P(\neg We|S) = 1 - 0.1365 = 0.8635$
3.  Calculate $P(M|S)$:
    * $P(M|S) = (0.02 \times 0.1365) + (0.42 \times 0.8635) = 0.00273 + 0.36267$
**Answer:** **0.3654**

#### (d) Given that it is spring, how likely is a bird on the lawn? $P(B|S)$
**Pattern:** B depends on S and Wo. We know S, so we must sum out Wo given S.
**Formula:** $$P(B|S) = \sum_{wo} P(B|S, wo)P(wo|S)$$
**Work:**
1.  Find $P(Wo|S)$:
    * $P(Wo|S) = P(Wo|R)P(R|S) + P(Wo|\neg R)P(\neg R|S) = (0.7 \times 0.7) + (0.2 \times 0.3) = 0.49 + 0.06 = 0.55$
    * $P(\neg Wo|S) = 1 - 0.55 = 0.45$
2.  Calculate $P(B|S)$:
    * $P(B|S) = (0.8 \times 0.55) + (0.4 \times 0.45) = 0.44 + 0.18$
**Answer:** **0.6200**

#### (e) Probability of spring given birds and no worms? $P(S|B, \neg Wo)$
**Pattern:** Reverse inference. Use conditional probability $P(X|E) = P(X, E) / P(E)$.
**Formula:** $$P(S | B, \neg Wo) = \frac{P(S, B, \neg Wo)}{P(B, \neg Wo)}$$
**Work:**
1.  Calculate Numerator $P(S, B, \neg Wo)$:
    * $P(S, B, \neg Wo) = P(S)P(\neg Wo|S)P(B|S, \neg Wo)$
    * From (d), $P(\neg Wo|S) = 0.45$
    * Numerator $= 0.25 \times 0.45 \times 0.4 = 0.0450$
2.  Calculate $P(\neg S, B, \neg Wo)$ for the denominator:
    * $P(\neg S, B, \neg Wo) = P(\neg S)P(\neg Wo|\neg S)P(B|\neg S, \neg Wo)$
    * $P(\neg Wo|\neg S) = 1 - [P(Wo|R)P(R|\neg S) + P(Wo|\neg R)P(\neg R|\neg S)]$
    * $P(\neg Wo|\neg S) = 1 - [(0.7 \times 0.3) + (0.2 \times 0.7)] = 1 - [0.21 + 0.14] = 0.65$
    * $P(\neg S, B, \neg Wo) = 0.75 \times 0.65 \times 0.4 = 0.1950$
3.  Calculate final probability:
    * Denominator $= 0.0450 + 0.1950 = 0.2400$
    * $P(S | B, \neg Wo) = 0.0450 / 0.2400$
**Answer:** **0.1875**

Here is the step-by-step logical breakdown of the calculation for $P(S|B, \neg Wo)$.

This problem is a classic example of **Scenario 4 (Backward Direction / Bayes' Rule)** combined with **Marginalization**. You are observing effects ($B$ and $\neg Wo$) and trying to determine the probability of a root cause ($S$).

### The Formula Structure
To find $P(S | B, \neg Wo)$, you apply Bayes' Rule. 

$$P(S | B, \neg Wo) = \frac{P(S, B, \neg Wo)}{P(B, \neg Wo)}$$

* **Numerator:** The joint probability that all three events happen together (It is Spring, there are Birds, and there are no Worms).
* **Denominator:** The total probability of the evidence happening (There are Birds and no Worms), regardless of what season it is.

---

### Step 1: The Numerator $P(S, B, \neg Wo)$
To calculate the joint probability of a subset of nodes, you use the chain rule, ordering the variables from cause to effect based on the network structure ($S \rightarrow Wo \rightarrow B$).

$$P(S, B, \neg Wo) = P(S) \times P(\neg Wo|S) \times P(B|S, \neg Wo)$$

**Why this works:**
1.  **$P(S)$**: Start with the probability of the root cause.
2.  **$P(\neg Wo|S)$**: Multiply by the probability of the first effect given the cause. (This was calculated in part 'd' by summing out the intermediate node, Rain).
3.  **$P(B|S, \neg Wo)$**: Multiply by the probability of the final effect given all its direct parents. Because $B$'s parents are $S$ and $Wo$, this is a direct lookup from your given CPTs.

**Calculation:**
* $0.25 \times 0.45 \times 0.4 = 0.0450$

---

### Step 2: The Denominator $P(B, \neg Wo)$
You need the total probability of seeing Birds and no Worms. Because $S$ is missing from this term, you must use **Marginalization** to sum out both possible states of $S$ (Spring and Not Spring).

$$P(B, \neg Wo) = P(S, B, \neg Wo) + P(\neg S, B, \neg Wo)$$

You already have the first half (the numerator, $0.0450$). You must now calculate the second half, $P(\neg S, B, \neg Wo)$. You apply the exact same chain rule pattern used in Step 1, but substitute $\neg S$ for $S$:

$$P(\neg S, B, \neg Wo) = P(\neg S) \times P(\neg Wo|\neg S) \times P(B|\neg S, \neg Wo)$$

---

### Step 3: The Sub-Calculation for $P(\neg Wo|\neg S)$
You have $P(\neg S) = 0.75$ and $P(B|\neg S, \neg Wo) = 0.4$ (from the CPT). You are missing $P(\neg Wo|\neg S)$.

Because $S$ does not directly cause $Wo$ (it goes $S \rightarrow R \rightarrow Wo$), you must sum out the intermediate node ($R$) to find the relationship between $\neg S$ and $\neg Wo$.

**The logic:** To find out if there are no worms when it is not spring, you must account for the fact that it might rain or not rain when it is not spring.

1.  Find the probability that there *are* worms given it is not spring:
    $$P(Wo|\neg S) = P(Wo|R)P(R|\neg S) + P(Wo|\neg R)P(\neg R|\neg S)$$
    $$P(Wo|\neg S) = (0.7 \times 0.3) + (0.2 \times 0.7) = 0.21 + 0.14 = 0.35$$
2.  Subtract from 1 to find the probability of *no* worms:
    $$P(\neg Wo|\neg S) = 1 - 0.35 = 0.65$$

---

### Step 4: Final Assembly
Now plug everything back into the denominator and solve.

1.  **Calculate $P(\neg S, B, \neg Wo)$:**
    $$0.75 \times 0.65 \times 0.4 = 0.1950$$
2.  **Calculate total Denominator $P(B, \neg Wo)$:**
    $$0.0450 (\text{from Step 1}) + 0.1950 = 0.2400$$
3.  **Divide Numerator by Denominator:**
    $$P(S|B, \neg Wo) = \frac{0.0450}{0.2400} = 0.1875$$