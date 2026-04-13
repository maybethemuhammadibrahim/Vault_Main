```python?code_reference&code_event_index=2
import scipy.stats as st

results = {}

# 3.20 (a)
results['3.20a'] = st.binom.pmf(3, 20, 0.05)

# 3.20 (b) P(Binom(4, 0.05) <= 1)
results['3.20b'] = st.binom.cdf(1, 4, 0.05)

# 3.21 P(Binom(20, 0.4) >= 10) = 1 - P(X <= 9)
results['3.21'] = 1 - st.binom.cdf(9, 20, 0.4)

# 3.22 P(Binom(16, 0.05) > 3) = 1 - P(X <= 3)
results['3.22'] = 1 - st.binom.cdf(3, 16, 0.05)

# 3.23 (a) P(Binom(15, 0.05) >= 4) = 1 - P(X <= 3)
results['3.23a'] = 1 - st.binom.cdf(3, 15, 0.05)

# 3.23 (b) NegBinom: 3rd success on 10th trial -> 2 successes in first 9, then success
results['3.23b'] = st.binom.pmf(2, 9, 0.05) * 0.05

# 3.24 (a) P(Binom(10, 0.2) >= 5) = 1 - P(X <= 4)
results['3.24a'] = 1 - st.binom.cdf(4, 10, 0.2)

# 3.24 (b) Geom: at least 5 to find first success -> first 4 are failures
results['3.24b'] = (0.8)**4

# 3.25 (a) Expected failures before first success
results['3.25a'] = 0.1 / 0.9

# 3.25 (b) P(Binom(10, 0.9) == 8)
results['3.25b'] = st.binom.pmf(8, 10, 0.9)

for k, v in results.items():
    print(f"{k}: {v:.6f}")

```
```text?code_stdout&code_event_index=2
3.20a: 0.059582
3.20b: 0.985981
3.21: 0.244663
3.22: 0.007004
3.23a: 0.005467
3.23b: 0.003143
3.24a: 0.032793
3.24b: 0.409600
3.25a: 0.111111
3.25b: 0.193710

```
Per your preference for concise, structured, and direct responses, here is the analysis and solution.

### Pattern Identification
The underlying pattern across all these problems is **Bernoulli trials**. Every scenario involves a sequence of independent events with exactly two possible outcomes (success/failure) and a constant probability. 

Depending on what is being measured, the problems use one of three related probability distributions:
1. **Binomial Distribution**: Measures the number of successes in a fixed number of trials.
2. **Negative Binomial Distribution**: Measures the number of trials required to achieve a fixed number of successes.
3. **Geometric Distribution**: A special case of the Negative Binomial, measuring the number of trials required to achieve the *first* success.

---

### Solutions

**3.20. Quality Control ($p = 0.05$ for defect)**
* **(a)** Binomial ($n=20, k=3$). 
    $P(X = 3) = \binom{20}{3}(0.05)^3(0.95)^{17} \approx 0.0596$
* **(b)** Negative Binomial. Requiring $\ge 5$ tests for 2 defects means finding $\le 1$ defect in the first 4 tests. 
    $P(X \le 1)$ for $n=4$: $\binom{4}{0}(0.95)^4 + \binom{4}{1}(0.05)(0.95)^3 \approx 0.9860$

**3.21. Lab Virus ($p = 0.4$ for entry)**
* Binomial ($n=20, k \ge 10$). 
    $P(X \ge 10) = \sum_{k=10}^{20} \binom{20}{k}(0.4)^k(0.6)^{20-k} \approx 0.2447$

**3.22. Computer Parts ($p = 0.05$ for defect)**
* Binomial ($n=16, k > 3 \implies k \ge 4$). 
    $P(X \ge 4) = 1 - \sum_{k=0}^{3} \binom{16}{k}(0.05)^k(0.95)^{16-k} \approx 0.0070$

**3.23. Lecture Cancelations ($p = 0.05$ for cancelation)**
* **(a)** Binomial ($n=15, k \ge 4$). 
    $P(X \ge 4) = 1 - \sum_{k=0}^{3} \binom{15}{k}(0.05)^k(0.95)^{15-k} \approx 0.0055$
* **(b)** Negative Binomial. Exactly 2 cancelations in the first 9 classes, followed by a cancelation on the 10th. 
    $\left[ \binom{9}{2}(0.05)^2(0.95)^7 \right] \times 0.05 \approx 0.0031$

**3.24. Search Engine ($p = 0.20$ for keyword presence)**
* **(a)** Binomial ($n=10, k \ge 5$). 
    $P(X \ge 5) = \sum_{k=5}^{10} \binom{10}{k}(0.20)^k(0.80)^{10-k} \approx 0.0328$
* **(b)** Geometric. Requiring $\ge 5$ visits for the first success implies exactly 0 successes in the first 4 visits. 
    $P(X=0 \text{ in } 4 \text{ trials}) = (0.80)^4 = 0.4096$

**3.25. Windows Closing ($p_{proper} = 0.90, p_{improper} = 0.10$)**
* **(a)** Geometric Expected Value. Number of failures before the first success. 
    $E[X] = \frac{1-p}{p} = \frac{0.10}{0.90} \approx 0.111 \text{ users}$
* **(b)** Binomial ($n=10, k=8$). 
    $P(X = 8) = \binom{10}{8}(0.90)^8(0.10)^2 \approx 0.1937$
