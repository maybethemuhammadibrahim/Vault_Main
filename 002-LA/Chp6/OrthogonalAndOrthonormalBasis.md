Here are refined, exam-style notes based on your text. I have verified all the calculations and added conceptual clarifications.

-----

## **Exam Notes: Orthogonal & Orthonormal Sets**

### **1. Definitions: The Difference**

In an inner product space, we look at sets of vectors $S = \{ \mathbf{v}_1, \mathbf{v}_2, \dots, \mathbf{v}_n \}$.

| Term | Definition | Math Condition |
| :--- | :--- | :--- |
| **Orthogonal Set** | Every pair of distinct vectors is orthogonal (perpendicular). | $\langle \mathbf{v}_i, \mathbf{v}_j \rangle = 0$ for all $i \neq j$. |
| **Orthonormal Set** | (1) The set is Orthogonal **AND** <br> (2) Every vector has a length (norm) of 1. | $\langle \mathbf{v}_i, \mathbf{v}_j \rangle = 0$ <br> $\|\mathbf{v}_i\| = 1$ |

> **Key Theorem:** If an orthogonal set consists of non-zero vectors, it is automatically **Linearly Independent**.

-----

### **2. How to "Normalize" an Orthogonal Set**

If you have an Orthogonal set (perpendicular but wrong lengths), you can turn it into an Orthonormal set by dividing each vector by its own length.

**Formula:**
$$\mathbf{q}_i = \frac{\mathbf{v}_i}{\|\mathbf{v}_i\|}$$

-----

### **3. Orthogonal & Orthonormal Bases**

A basis is just a set of vectors that "spans" the space and is independent.

  * **Orthogonal Basis:** A basis where all vectors are mutually orthogonal.
  * **Orthonormal Basis:** A basis where all vectors are mutually orthogonal and have length 1.

#### **Examples of Orthonormal Bases:**

1.  **Standard Euclidean Vectors ($\mathbb{R}^n$):**
    $S = \{ (1,0,0), (0,1,0), (0,0,1) \}$ is orthonormal.

      * *Check:* Dot product is 0, lengths are 1.

2.  **Polynomials ($P_n$):**
    $S = \{ 1, x, x^2, \dots, x^n \}$

    > **CRITICAL WARNING:** This set is Orthonormal **ONLY** if the inner product is defined as the "dot product of coefficients" (treating $1+2x$ like vector $(1,2)$).
    > If the inner product is defined by an **Integral** (e.g., $\int_{-1}^1 p(x)q(x)dx$), this basis is **NOT** orthogonal. (This is a common exam trap).

3.  **Matrices ($M_{22}$):**
    The standard basis $E_1 = \begin{bmatrix}1 & 0\\0 & 0\end{bmatrix}, E_2 = \begin{bmatrix}0 & 1\\0 & 0\end{bmatrix}, \dots$ is orthonormal under the Frobenius inner product (sum of entry products).

-----

### **4. Finding Coordinates (The "Easy" Way)**

Normally, to write a vector $\mathbf{u}$ as a combination of basis vectors ($c_1\mathbf{v}_1 + c_2\mathbf{v}_2 \dots$), you have to solve a system of linear equations (Gaussian elimination).

**However**, if the basis is Orthogonal or Orthonormal, there are simple formulas.

#### **Case A: The Basis is ORTHOGONAL**

Use this if the vectors are perpendicular but have random lengths.
$$\mathbf{u} = \frac{\langle \mathbf{u}, \mathbf{v}_1 \rangle}{\|\mathbf{v}_1\|^2}\mathbf{v}_1 + \frac{\langle \mathbf{u}, \mathbf{v}_2 \rangle}{\|\mathbf{v}_2\|^2}\mathbf{v}_2 + \dots + \frac{\langle \mathbf{u}, \mathbf{v}_n \rangle}{\|\mathbf{v}_n\|^2}\mathbf{v}_n$$

#### **Case B: The Basis is ORTHONORMAL (Best Case)**

Use this if the vectors are perpendicular AND length 1. The denominator disappears (because $1^2 = 1$).
$$\mathbf{u} = \langle \mathbf{u}, \mathbf{v}_1 \rangle \mathbf{v}_1 + \langle \mathbf{u}, \mathbf{v}_2 \rangle \mathbf{v}_2 + \dots + \langle \mathbf{u}, \mathbf{v}_n \rangle \mathbf{v}_n$$

-----

### **5. Verified Practice Problems (From your text)**

#### **Problem 1: Coordinates relative to an Orthonormal Basis**

**Given:**
Basis $S = \{ \mathbf{v}_1, \mathbf{v}_2, \mathbf{v}_3 \}$ in $\mathbb{R}^3$.
$\mathbf{v}_1 = (0, 1, 0)$
$\mathbf{v}_2 = (-\frac{4}{5}, 0, \frac{3}{5})$
$\mathbf{v}_3 = (\frac{3}{5}, 0, \frac{4}{5})$
Target Vector $\mathbf{u} = (1, 1, 1)$.

**Task:** Find coordinate vector $(\mathbf{u})_S$.

**Verification:**

1.  **Check Orthonormality:**

      * $\|\mathbf{v}_2\| = \sqrt{(-\frac{4}{5})^2 + (\frac{3}{5})^2} = \sqrt{\frac{16}{25} + \frac{9}{25}} = \sqrt{1} = 1$. (Checked: Normalized).
      * $\mathbf{v}_2 \cdot \mathbf{v}_3 = (-\frac{4}{5})(\frac{3}{5}) + (0) + (\frac{3}{5})(\frac{4}{5}) = -\frac{12}{25} + \frac{12}{25} = 0$. (Checked: Orthogonal).
      * *Conclusion: The set is indeed Orthonormal.*

2.  **Calculate Coefficients (Using Orthonormal Formula):**

      * $c_1 = \langle \mathbf{u}, \mathbf{v}_1 \rangle = (1)(0) + (1)(1) + (1)(0) = \mathbf{1}$
      * $c_2 = \langle \mathbf{u}, \mathbf{v}_2 \rangle = (1)(-\frac{4}{5}) + (1)(0) + (1)(\frac{3}{5}) = -\frac{1}{5}$
      * $c_3 = \langle \mathbf{u}, \mathbf{v}_3 \rangle = (1)(\frac{3}{5}) + (1)(0) + (1)(\frac{4}{5}) = \frac{7}{5}$

**Final Answer:**
$(\mathbf{u})_S = (1, -\frac{1}{5}, \frac{7}{5})$ ✅ *(Verified)*

-----

#### **Problem 2: Orthonormal Basis from Orthogonal Basis**

**Given:**
$\mathbf{w}_1 = (0, 2, 0)$
$\mathbf{w}_2 = (3, 0, 3)$
$\mathbf{w}_3 = (-4, 0, 4)$
Target $\mathbf{u} = (1, 2, 4)$.

**Part A: Normalize to find Basis $v$**

1.  **Vector $\mathbf{w}_1$:**
      * Norm: $\|\mathbf{w}_1\| = \sqrt{0^2+2^2+0^2} = 2$.
      * Result: $\mathbf{v}_1 = \frac{(0,2,0)}{2} = \mathbf{(0, 1, 0)}$.
2.  **Vector $\mathbf{w}_2$:**
      * Norm: $\|\mathbf{w}_2\| = \sqrt{3^2+0^2+3^2} = \sqrt{18} = 3\sqrt{2}$.
      * Result: $\mathbf{v}_2 = \frac{(3,0,3)}{3\sqrt{2}} = \mathbf{(\frac{1}{\sqrt{2}}, 0, \frac{1}{\sqrt{2}})}$.
3.  **Vector $\mathbf{w}_3$:**
      * Norm: $\|\mathbf{w}_3\| = \sqrt{(-4)^2+0^2+4^2} = \sqrt{32} = 4\sqrt{2}$.
      * Result: $\mathbf{v}_3 = \frac{(-4,0,4)}{4\sqrt{2}} = \mathbf{(-\frac{1}{\sqrt{2}}, 0, \frac{1}{\sqrt{2}})}$.

**Part B: Find Coefficients for $\mathbf{u}$ using new basis**

1.  $\langle \mathbf{u}, \mathbf{v}_1 \rangle = (1,2,4) \cdot (0,1,0) = \mathbf{2}$.
2.  $\langle \mathbf{u}, \mathbf{v}_2 \rangle = (1)(\frac{1}{\sqrt{2}}) + 2(0) + 4(\frac{1}{\sqrt{2}}) = \frac{5}{\sqrt{2}}$.
3.  $\langle \mathbf{u}, \mathbf{v}_3 \rangle = (1)(-\frac{1}{\sqrt{2}}) + 2(0) + 4(\frac{1}{\sqrt{2}}) = \frac{3}{\sqrt{2}}$.

**Final Equation:**
$\mathbf{u} = 2\mathbf{v}_1 + \frac{5}{\sqrt{2}}\mathbf{v}_2 + \frac{3}{\sqrt{2}}\mathbf{v}_3$ ✅ *(Verified)*
