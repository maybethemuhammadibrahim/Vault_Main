```python?code_reference&code_event_index=2
# PMF: {x: P(x)}
pmf = {0: 0.6, 1: 0.2, 2: 0.2}

prob_greater = 0
for x_f in pmf:
    for x_t in pmf:
        if x_f > x_t:
            prob_greater += pmf[x_f] * pmf[x_t]

print(f"{prob_greater=}")

```
```text?code_stdout&code_event_index=2
prob_greater=0.28

```
To find the probability that there are more accidents on Friday ($X_F$) than on Thursday ($X_T$), we calculate $P(X_F > X_T)$. Given that the daily accidents are independent and identically distributed, the joint probability is the product of individual probabilities.

### 1. Identify Possible Outcomes
The condition $X_F > X_T$ is satisfied by the following pairs $(X_F, X_T)$:
* $(1, 0)$
* $(2, 0)$
* $(2, 1)$

### 2. Calculate Individual Probabilities
Using the PMF: $P(0) = 0.6, P(1) = 0.2, P(2) = 0.2$.

* $P(X_F=1, X_T=0) = P(1) \cdot P(0) = 0.2 \cdot 0.6 = 0.12$
* $P(X_F=2, X_T=0) = P(2) \cdot P(0) = 0.2 \cdot 0.6 = 0.12$
* $P(X_F=2, X_T=1) = P(2) \cdot P(1) = 0.2 \cdot 0.2 = 0.04$

### 3. Sum the Probabilities
$$P(X_F > X_T) = 0.12 + 0.12 + 0.04 = 0.28$$

**Result:**
The probability that there are more accidents on Friday than on Thursday is **0.28**.



```python?code_reference&code_event_index=2
import pandas as pd

# Joint distribution table
# x is columns, y is rows
data = {
    'X=0': [0.5, 0.2],
    'X=1': [0.2, 0.1]
}
df = pd.DataFrame(data, index=['Y=0', 'Y=1'])

# Marginal X
p_x0 = df['X=0'].sum()
p_x1 = df['X=1'].sum()

# Marginal Y
p_y0 = df.loc['Y=0'].sum()
p_y1 = df.loc['Y=1'].sum()

print(f"Marginal X: P(0)={p_x0}, P(1)={p_x1}")
print(f"Marginal Y: P(0)={p_y0}, P(1)={p_y1}")

# Independence check X, Y
indep_xy = (df.iloc[0,0] == p_x0 * p_y0)
print(f"X, Y Independent: {indep_xy} ({df.iloc[0,0]} vs {p_x0 * p_y0})")

# U = X + Y, V = X - Y
# Outcomes:
# (0,0) -> U=0, V=0, P=0.5
# (1,0) -> U=1, V=1, P=0.2
# (0,1) -> U=1, V=-1, P=0.2
# (1,1) -> U=2, V=0, P=0.1

uv_pmf = {
    (0, 0): 0.5,
    (1, 1): 0.2,
    (1, -1): 0.2,
    (2, 0): 0.1
}

u_vals = [0, 1, 2]
v_vals = [-1, 0, 1]

p_u = {u: sum(p for (uu, vv), p in uv_pmf.items() if uu == u) for u in u_vals}
p_v = {v: sum(p for (uu, vv), p in uv_pmf.items() if vv == v) for v in v_vals}

print(f"Marginal U: {p_u}")
print(f"Marginal V: {p_v}")

# Check U=0, V=0
indep_uv = (uv_pmf.get((0,0), 0) == p_u[0] * p_v[0])
print(f"U, V Independent: {indep_uv} ({uv_pmf.get((0,0), 0)} vs {p_u[0] * p_v[0]})")


```
```text?code_stdout&code_event_index=2
Marginal X: P(0)=0.7, P(1)=0.30000000000000004
Marginal Y: P(0)=0.7, P(1)=0.30000000000000004
X, Y Independent: False (0.5 vs 0.48999999999999994)
Marginal U: {0: 0.5, 1: 0.4, 2: 0.1}
Marginal V: {-1: 0.2, 0: 0.6, 1: 0.2}
U, V Independent: False (0.5 vs 0.3)

```
### (a) Are $X$ and $Y$ independent?

To check for independence, we must determine if $P(X=x, Y=y) = P(X=x)P(Y=y)$ for all possible values of $x$ and $y$.

1.  **Calculate Marginal Distributions:**
    * $P(X=0) = P(0,0) + P(0,1) = 0.5 + 0.2 = 0.7$
    * $P(X=1) = P(1,0) + P(1,1) = 0.2 + 0.1 = 0.3$
    * $P(Y=0) = P(0,0) + P(1,0) = 0.5 + 0.2 = 0.7$
    * $P(Y=1) = P(0,1) + P(1,1) = 0.2 + 0.1 = 0.3$

2.  **Compare Joint to Product of Marginals:**
    Check the case $(x=0, y=0)$:
    * $P(X=0, Y=0) = 0.5$
    * $P(X=0) \cdot P(Y=0) = 0.7 \cdot 0.7 = 0.49$

Since $0.5 \neq 0.49$, $X$ and $Y$ are **not independent**.

---

### (b) Are $(X + Y)$ and $(X - Y)$ independent?

Let $U = X + Y$ and $V = X - Y$. We check if $P(U=u, V=v) = P(U=u)P(V=v)$.

1.  **Determine Joint Distribution of $(U, V)$:**
    * For $(X,Y) = (0,0)$: $U=0, V=0$ with $P=0.5$
    * For $(X,Y) = (1,0)$: $U=1, V=1$ with $P=0.2$
    * For $(X,Y) = (0,1)$: $U=1, V=-1$ with $P=0.2$
    * For $(X,Y) = (1,1)$: $U=2, V=0$ with $P=0.1$

2.  **Calculate Marginal Distributions:**
    * $P(U=0) = 0.5$
    * $P(U=1) = 0.2 + 0.2 = 0.4$
    * $P(U=2) = 0.1$
    * $P(V=0) = P(U=0, V=0) + P(U=2, V=0) = 0.5 + 0.1 = 0.6$

3.  **Compare Joint to Product of Marginals:**
    Check the case $(u=0, v=0)$:
    * $P(U=0, V=0) = 0.5$
    * $P(U=0) \cdot P(V=0) = 0.5 \cdot 0.6 = 0.3$

Since $0.5 \neq 0.3$, $(X + Y)$ and $(X - Y)$ are **not independent**.


### Joint Distribution Table for $U = X + Y$ and $V = X - Y$

To determine independence, we compare the joint probability $P(u, v)$ with the product of the marginal probabilities $P(u) \cdot P(v)$.

**Table 1: Joint Distribution $P(u, v)$**

| $U \setminus V$ | -1 | 0 | 1 | Marginal $P(u)$ |
| :--- | :--- | :--- | :--- | :--- |
| **0** | 0 | 0.5 | 0 | **0.5** |
| **1** | 0.2 | 0 | 0.2 | **0.4** |
| **2** | 0 | 0.1 | 0 | **0.1** |
| **Marginal $P(v)$** | **0.2** | **0.6** | **0.2** | **1.0** |

**Table 2: Product of Marginals $P(u) \cdot P(v)$**

| $U \setminus V$ | -1 ($P=0.2$) | 0 ($P=0.6$) | 1 ($P=0.2$) |
| :--- | :--- | :--- | :--- |
| **0** ($P=0.5$) | 0.1 | 0.3 | 0.1 |
| **1** ($P=0.4$) | 0.08 | 0.24 | 0.08 |
| **2** ($P=0.1$) | 0.02 | 0.06 | 0.02 |

### Comparison and Conclusion
For independence, every cell in **Table 1** must equal the corresponding cell in **Table 2**.

* **Observation:** $P(U=0, V=0) = 0.5$, but $P(U=0) \cdot P(V=0) = 0.3$.
* **Result:** Since $0.5 \neq 0.3$, $(X + Y)$ and $(X - Y)$ are **not independent**.


### Rewritten Question
Given a joint probability distribution for two random variables $X$ and $Y$:
* $P(0, 0) = 0.2$
* $P(0, 2) = 0.3$
* $P(1, 1) = 0.1$
* $P(2, 0) = 0.3$
* $P(2, 2) = 0.1$

Find the probability mass functions (PMF) for the following transformations:
1.  Sum: $Z = X + Y$
2.  Difference: $U = X - Y$
3.  Product: $V = XY$

---

### Intuition: What are we doing?
You are essentially **regrouping** probabilities. 

Imagine you have five buckets of money, and each bucket has a label $(x, y)$ and a specific amount of cash (the probability).
* In Part (a), you decide to relabel the buckets based on the **sum** of the two numbers on the label. 
* If multiple original buckets now have the same new label (e.g., $X+Y=2$), you pour the cash from those buckets into one single "New Label 2" bucket.
* The PMF is simply the final count of how much cash (probability) is in each new bucket.



---

### Step 1: Mapping Table
We calculate the new values for each pair $(x, y)$ provided in the problem.

| $(x, y)$ | $P(x, y)$ | $Z = X + Y$ | $U = X - Y$ | $V = XY$ |
| :--- | :--- | :--- | :--- | :--- |
| (0, 0) | 0.2 | 0 | 0 | 0 |
| (0, 2) | 0.3 | 2 | -2 | 0 |
| (1, 1) | 0.1 | 2 | 0 | 1 |
| (2, 0) | 0.3 | 2 | 2 | 0 |
| (2, 2) | 0.1 | 4 | 0 | 4 |

---

### (a) PMF of $Z = X + Y$
Sum the probabilities for each unique value of $Z$.
* $P(Z=0) = P(0,0) = \mathbf{0.2}$
* $P(Z=2) = P(0,2) + P(1,1) + P(2,0) = 0.3 + 0.1 + 0.3 = \mathbf{0.7}$
* $P(Z=4) = P(2,2) = \mathbf{0.1}$

| $z$ | 0 | 2 | 4 |
| :--- | :--- | :--- | :--- |
| $P(Z=z)$ | 0.2 | 0.7 | 0.1 |

---

### (b) PMF of $U = X - Y$
Sum the probabilities for each unique value of $U$.
* $P(U=-2) = P(0,2) = \mathbf{0.3}$
* $P(U=0) = P(0,0) + P(1,1) + P(2,2) = 0.2 + 0.1 + 0.1 = \mathbf{0.4}$
* $P(U=2) = P(2,0) = \mathbf{0.3}$

| $u$ | -2 | 0 | 2 |
| :--- | :--- | :--- | :--- |
| $P(U=u)$ | 0.3 | 0.4 | 0.3 |

---

### (c) PMF of $V = XY$
Sum the probabilities for each unique value of $V$.
* $P(V=0) = P(0,0) + P(0,2) + P(2,0) = 0.2 + 0.3 + 0.3 = \mathbf{0.8}$
* $P(V=1) = P(1,1) = \mathbf{0.1}$
* $P(V=4) = P(2,2) = \mathbf{0.1}$

| $v$ | 0 | 1 | 4 |
| :--- | :--- | :--- | :--- |
| $P(V=v)$ | 0.8 | 0.1 | 0.1 |

```python?code_reference&code_event_index=2
import pandas as pd

# 3.11
# X = min(D1, D2), Y = max(D1, D2)
joint_311 = {}
for d1 in range(1, 7):
    for d2 in range(1, 7):
        x, y = min(d1, d2), max(d1, d2)
        joint_311[(x, y)] = joint_311.get((x, y), 0) + 1/36

# Marginal X
p_x_311 = {}
for (x, y), p in joint_311.items():
    p_x_311[x] = p_x_311.get(x, 0) + p

# (d) P(Y=5 | X=2)
prob_y5_given_x2 = joint_311[(2, 5)] / p_x_311[2]

# 3.14
# rows are y, columns are x
table_314 = [
    [0, 0.06, 0.06, 0.10], # y=1
    [0.10, 0.10, 0.04, 0.04], # y=2
    [0.40, 0.10, 0, 0]  # y=3
]
x_vals = [1, 2, 3, 4]
y_vals = [1, 2, 3]
dist_z = {}
for i, y in enumerate(y_vals):
    for j, x in enumerate(x_vals):
        z = x * y
        p = table_314[i][j]
        if p > 0:
            dist_z[z] = dist_z.get(z, 0) + p

# 3.15
table_315 = [
    [0.52, 0.20, 0.04], # y=0
    [0.14, 0.02, 0.01], # y=1
    [0.06, 0.01, 0]     # y=2
]
# (a) At least one failure = 1 - P(0,0)
prob_at_least_one = 1 - table_315[0][0]

# (b) Independence check
# Marginal X (columns)
p_x0 = sum(row[0] for row in table_315)
p_x1 = sum(row[1] for row in table_315)
p_x2 = sum(row[2] for row in table_315)

# Marginal Y (rows)
p_y0 = sum(table_315[0])
p_y1 = sum(table_315[1])
p_y2 = sum(table_315[2])

check_indep = (table_315[0][0] == p_x0 * p_y0)

print("3.11 Results:")
print(f"P(X=2): {p_x_311[2]}")
print(f"P(Y=5 | X=2): {prob_y5_given_x2}")
print("\n3.14 Distribution of Z:")
print(dict(sorted(dist_z.items())))
print("\n3.15 Results:")
print(f"P(at least one): {prob_at_least_one}")
print(f"Marginal X: {p_x0}, {p_x1}, {p_x2}")
print(f"Marginal Y: {p_y0}, {p_y1}, {p_y2}")
print(f"Independence check (0,0): {table_315[0][0]} vs {p_x0 * p_y0}")

```
```text?code_stdout&code_event_index=2
3.11 Results:
P(X=2): 0.25
P(Y=5 | X=2): 0.2222222222222222

3.14 Distribution of Z:
{2: 0.16, 3: 0.46, 4: 0.2, 6: 0.14, 8: 0.04}

3.15 Results:
P(at least one): 0.48
Marginal X: 0.72, 0.23, 0.05
Marginal Y: 0.76, 0.17, 0.06999999999999999
Independence check (0,0): 0.52 vs 0.5472

```
### 3.11. Two Dice Tossing
Let $X = \min(D_1, D_2)$ and $Y = \max(D_1, D_2)$. There are 36 equally likely outcomes for $(D_1, D_2)$.

**(a) Joint PMF $P(X=x, Y=y)$:**
* If $x = y$: There is only one pair $(x, x)$, so $P(x, x) = 1/36$.
* If $x < y$: There are two pairs $(x, y)$ and $(y, x)$, so $P(x, y) = 2/36$.
* If $x > y$: $P(x, y) = 0$ (since $X$ is the minimum).

**(b) Are $X$ and $Y$ independent?**
No. If $X=6$, then $Y$ must be 6 ($P=1$). If $X=1$, $Y$ can be anything from 1 to 6. The distribution of $Y$ depends on $X$.
Check: $P(X=1, Y=1) = 1/36$. 
Marginal $P(X=1) = P(1,1) + P(1,2) + ... + P(1,6) = 1/36 + 5(2/36) = 11/36$.
Marginal $P(Y=1) = P(1,1) = 1/36$.
$11/36 \cdot 1/36 \neq 1/36$.

**(c) PMF of $X$:**
The probability $P(X=x)$ is the sum of probabilities in the row for $x$.
$P(X=x) = \frac{1 + 2(6-x)}{36}$ for $x \in \{1, 2, 3, 4, 5, 6\}$.

| $x$ | 1 | 2 | 3 | 4 | 5 | 6 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| $P(X=x)$ | $11/36$ | $9/36$ | $7/36$ | $5/36$ | $3/36$ | $1/36$ |

**(d) If $X=2$, find $P(Y=5 | X=2)$:**
$P(Y=5 | X=2) = \frac{P(X=2, Y=5)}{P(X=2)} = \frac{2/36}{9/36} = \frac{2}{9} \approx 0.222$.

---

### 3.14. ISP Charging $Z = X \cdot Y$
We multiply each $x$ by its corresponding $y$ where $P(x, y) > 0$ and sum the probabilities for identical $z$ values.

| $z$ | 2 | 3 | 4 | 6 | 8 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| $P(Z=z)$ | 0.16 | 0.46 | 0.20 | 0.14 | 0.04 |

*Calculation for $z=3$: $P(X=3, Y=1) + P(X=1, Y=3) = 0.06 + 0.40 = 0.46$.*

---

### 3.15. Hardware Failures
**(a) Probability of at least one hardware failure:**
This is the complement of zero failures in both labs.
$P(\text{at least one}) = 1 - P(X=0, Y=0)$
$P(\text{at least one}) = 1 - 0.52 = \mathbf{0.48}$.

**(b) Are $X$ and $Y$ independent?**
Calculate marginals:
* $P(X=0) = 0.52 + 0.14 + 0.06 = 0.72$
* $P(Y=0) = 0.52 + 0.20 + 0.04 = 0.76$

Check independence condition: $P(0, 0) = P(X=0)P(Y=0)$?
$0.52 \neq 0.72 \cdot 0.76$
$0.52 \neq 0.5472$

**Conclusion:** $X$ and $Y$ are **not independent**.


### Rewritten Question
A small computer lab tracks daily failures. Let $X$ be the number of hardware failures and $Y$ be the number of software failures. Their joint probability distribution $P(x, y)$ is:
* $P(0, 0) = 0.6$ (No failures)
* $P(0, 1) = 0.1$ (Software only)
* $P(1, 0) = 0.1$ (Hardware only)
* $P(1, 1) = 0.2$ (Both fail)

**Tasks:**
1.  **Independence:** Determine if $X$ and $Y$ are independent.
2.  **Expected Value:** Calculate the expected total number of failures per day, $E(X + Y)$.

---

### Intuition: What are we doing?
* **Part (a) Independence:** We are checking if hardware and software failures are linked. If they were independent, knowing that a hardware failure occurred would tell you nothing about the likelihood of a software failure. In this lab, however, the data suggests that when one fails, the other might be more likely to fail (perhaps due to a power surge affecting both).
* **Part (b) Expectation:** We are finding the "long-term average." If you recorded the total failures ($X+Y$) every day for a year and averaged them, what number would you get?



---

### (a) Are $X$ and $Y$ independent?
To be independent, $P(x, y)$ must equal $P(x) \cdot P(y)$ for all pairs.

1.  **Find Marginals:**
    * $P(X=0) = P(0,0) + P(0,1) = 0.6 + 0.1 = \mathbf{0.7}$
    * $P(Y=0) = P(0,0) + P(1,0) = 0.6 + 0.1 = \mathbf{0.7}$

2.  **Check Condition for $(0,0)$:**
    * $P(X=0, Y=0) = 0.6$
    * $P(X=0) \cdot P(Y=0) = 0.7 \cdot 0.7 = 0.49$

**Conclusion:** Since $0.6 \neq 0.49$, $X$ and $Y$ are **not independent**. In fact, since $P(1,1)$ is higher than expected ($0.2$ vs $0.09$), failures tend to happen together.

---

### (b) Compute $E(X + Y)$
You can solve this using two methods. 

**Method 1: Linearity of Expectation (Fastest)**
The expected value of a sum is always the sum of the expected values, even if variables are dependent.
1.  $E(X) = (0 \cdot 0.7) + (1 \cdot 0.3) = 0.3$
2.  $E(Y) = (0 \cdot 0.7) + (1 \cdot 0.3) = 0.3$
3.  $E(X + Y) = E(X) + E(Y) = 0.3 + 0.3 = \mathbf{0.6}$

**Method 2: Distribution of Total Failures ($Z = X + Y$)**
1.  $P(Z=0) = P(0,0) = 0.6$
2.  $P(Z=1) = P(0,1) + P(1,0) = 0.2$
3.  $P(Z=2) = P(1,1) = 0.2$
4.  $E(Z) = (0 \cdot 0.6) + (1 \cdot 0.2) + (2 \cdot 0.2) = 0 + 0.2 + 0.4 = \mathbf{0.6}$

**Result:** The expected total number of failures per day is **0.6**.