### Comprehensive Notes: Measures of Central Tendency

**Note on Terminology:**

* **Mean and Median** can generally *only* be calculated for **Quantitative** (numerical) data.
* **Mode** can be calculated for both **Quantitative** and **Qualitative** (categorical) data.
* Below, "Distinct/Ungrouped" refers to raw numerical data. "Grouped" refers to data organized into class intervals using **Limit Grouping** (e.g., 10–19).

---

### 1. The Mean (Arithmetic Average)

#### A. Ungrouped Data (Raw Data)

Sum all values and divide by the total number of observations ($n$).

* **Formula:** $\bar{x} = \frac{\sum x}{n}$
* **Example:** Data: $\{2, 5, 8\}$

$$\bar{x} = \frac{2 + 5 + 8}{3} = \frac{15}{3} = 5$$



#### B. Grouped Data (Limit Grouping)

Since we don't know exact values, we use the **midpoint ($x$)** of each class to represent the data.

* **Step 1:** Calculate Midpoint ($x$) for each class: $\frac{\text{Lower Limit} + \text{Upper Limit}}{2}$.
* **Step 2:** Multiply frequency ($f$) by midpoint ($x$).
* **Formula:** $\bar{x} = \frac{\sum fx}{\sum f}$
* **Example Calculation:**

| Class (Limits) | Frequency ($f$) | Midpoint ($x$) | $f \cdot x$ |
| --- | --- | --- | --- |
| 0 – 9 | 2 | 4.5 | 9.0 |
| 10 – 19 | 3 | 14.5 | 43.5 |
| **Total** | **$\sum f = 5$** |  | **$\sum fx = 52.5$** |

$$\bar{x} = \frac{52.5}{5} = 10.5$$

---

### 2. The Median (Positional Center)

#### A. Ungrouped Data

The middle value when data is ordered.

* **Step 1:** Order data from least to greatest.
* **Step 2:** Find position $\frac{n+1}{2}$.
* **Example:** Data: $\{8, 2, 5\} \rightarrow \text{Ordered: } \{2, 5, 8\}$. Center is **5**.

#### B. Grouped Data (Limit Grouping)

* **Crucial Step for Limit Grouping:** You must convert the Class Limits to **Class Boundaries** to find the true lower boundary ($L$). For 10–19, the Lower Boundary is **9.5**.
* **Formula:**

$$\tilde{x} = L + \frac{h}{f} \left( \frac{n}{2} - c \right)$$


* $L$ = Lower Class Boundary of the median class.
* $h$ = Class Interval Size (Upper Boundary - Lower Boundary).
* $f$ = Frequency of the median class.
* $n$ = Total frequency ($\sum f$).
* $c$ = Cumulative frequency of the class *preceding* the median class.


* **Example Calculation:**
* Data: Class 0–9 ($f=2$), Class 10–19 ($f=3$). Total $n=5$.
* Target Position: $n/2 = 2.5$. The 2.5th value falls in the 10–19 class.
* $L = 9.5$ (Boundary of 10)
* $h = 10$ (19.5 - 9.5)
* $c = 2$ (Frequency of 0–9)
* $f = 3$


$$\tilde{x} = 9.5 + \frac{10}{3} (2.5 - 2) = 9.5 + 3.33(0.5) = 11.165$$



---

### 3. The Mode (Most Frequent)

#### A. Ungrouped Data (Qualitative or Quantitative)

The value that appears most often.

* **Quantitative Example:** $\{2, 2, 5, 8\}$. Mode = **2**.
* **Qualitative Example:** $\{Red, Blue, Red\}$. Mode = **Red**.

#### B. Grouped Data (Limit Grouping)

Find the "Modal Class" (highest frequency), then use interpolation.

* **Formula:**

$$\hat{x} = L + \left( \frac{f_m - f_1}{2f_m - f_1 - f_2} \right) \times h$$


* $L$ = Lower Class Boundary of modal class.
* $f_m$ = Frequency of modal class.
* $f_1$ = Frequency of class *before* modal class.
* $f_2$ = Frequency of class *after* modal class.


* **Example Calculation:**
* Class 0–9 ($f=2$), Class 10–19 ($f=5$), Class 20–29 ($f=2$).
* Modal Class is 10–19 ($f=5$).
* $L = 9.5$
* $f_m = 5, f_1 = 2, f_2 = 2, h = 10$


$$\hat{x} = 9.5 + \left( \frac{5 - 2}{2(5) - 2 - 2} \right) \times 10$$


$$\hat{x} = 9.5 + \left( \frac{3}{6} \right) \times 10 = 9.5 + 5 = 14.5$$



---

### 4. Conceptual Comparison

| Measure | What it Measures | Best Used When... |
| --- | --- | --- |
| **Mean** | The mathematical center; balances the sum of differences. | Data is symmetric (Bell Curve) and has **no outliers**. It includes every value in the calculation. |
| **Median** | The positional center; splits data 50/50. | Data is **skewed** or has strong **outliers** (e.g., salaries, house prices). It is robust and resistant to extreme values. |
| **Mode** | The peak/most popular value. | Data is **Qualitative** (nominal) or you need to know the most common occurrence (e.g., most sold shoe size). |

**Would you like a specific practice problem to calculate all three from a single dataset?**
