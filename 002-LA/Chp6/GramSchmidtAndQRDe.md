## **1. The Gram-Schmidt Process**

**Goal:** Turn a "messy" basis $\{ \mathbf{u}_1, \mathbf{u}_2, \dots \}$ into a "clean" **Orthogonal** basis $\{ \mathbf{v}_1, \mathbf{v}_2, \dots \}$ or **Orthonormal** basis $\{ \mathbf{q}_1, \mathbf{q}_2, \dots \}$.

### **The Concept**
We build the new vectors one by one. To get the next vector, we take the old vector and **subtract** the parts of it that point in the directions of the vectors we have already built.

### **The Algorithm (Step-by-Step)**

* **Step 1:** Keep the first vector as is.
    $$\mathbf{v}_1 = \mathbf{u}_1$$
* **Step 2:** Take $\mathbf{u}_2$ and subtract its projection onto $\mathbf{v}_1$.
    $$\mathbf{v}_2 = \mathbf{u}_2 - \frac{\langle \mathbf{u}_2, \mathbf{v}_1 \rangle}{\|\mathbf{v}_1\|^2} \mathbf{v}_1$$
* **Step 3:** Take $\mathbf{u}_3$ and subtract projections onto $\mathbf{v}_1$ **and** $\mathbf{v}_2$.
    $$\mathbf{v}_3 = \mathbf{u}_3 - \frac{\langle \mathbf{u}_3, \mathbf{v}_1 \rangle}{\|\mathbf{v}_1\|^2} \mathbf{v}_1 - \frac{\langle \mathbf{u}_3, \mathbf{v}_2 \rangle}{\|\mathbf{v}_2\|^2} \mathbf{v}_2$$
* **Normalization (Optional but usually required):**
    To get the orthonormal basis $\{ \mathbf{q}_i \}$, divide each $\mathbf{v}_i$ by its length:
    $$\mathbf{q}_i = \frac{\mathbf{v}_i}{\|\mathbf{v}_i\|}$$

---

### **Worked Example (Verified)**
**Given:** $\mathbf{u}_1 = (1, 1, 1)$, $\mathbf{u}_2 = (0, 1, 1)$, $\mathbf{u}_3 = (0, 0, 1)$ in $\mathbb{R}^3$.

#### **Phase 1: Find Orthogonal Vectors ($\mathbf{v}$)**

* **Step 1:**
    $\mathbf{v}_1 = \mathbf{u}_1 = \mathbf{(1, 1, 1)}$

* **Step 2:**
    Calculate $\mathbf{v}_2 = \mathbf{u}_2 - \text{proj}_{\mathbf{v}_1}\mathbf{u}_2$
    * $\langle \mathbf{u}_2, \mathbf{v}_1 \rangle = (0)(1) + (1)(1) + (1)(1) = 2$
    * $\|\mathbf{v}_1\|^2 = 1^2 + 1^2 + 1^2 = 3$
    * $\mathbf{v}_2 = (0, 1, 1) - \frac{2}{3}(1, 1, 1) = (-\frac{2}{3}, \frac{1}{3}, \frac{1}{3})$
    * *Exam Tip: You can scale this vector by 3 to make math easier for the next step, but let's stick to the strict formula.* $\mathbf{v}_2 = \mathbf{(-\frac{2}{3}, \frac{1}{3}, \frac{1}{3})}$

* **Step 3:**
    Calculate $\mathbf{v}_3 = \mathbf{u}_3 - \text{proj}_{\mathbf{v}_1}\mathbf{u}_3 - \text{proj}_{\mathbf{v}_2}\mathbf{u}_3$
    * **Part A (on $\mathbf{v}_1$):**
        $\langle \mathbf{u}_3, \mathbf{v}_1 \rangle = 1$, $\|\mathbf{v}_1\|^2 = 3 \implies \text{proj} = \frac{1}{3}(1,1,1)$
    * **Part B (on $\mathbf{v}_2$):**
        $\langle \mathbf{u}_3, \mathbf{v}_2 \rangle = (0)(-\frac{2}{3}) + (0)(\frac{1}{3}) + (1)(\frac{1}{3}) = \frac{1}{3}$
        $\|\mathbf{v}_2\|^2 = (-\frac{2}{3})^2 + (\frac{1}{3})^2 + (\frac{1}{3})^2 = \frac{4}{9} + \frac{1}{9} + \frac{1}{9} = \frac{6}{9} = \frac{2}{3}$
        Factor = $\frac{1/3}{2/3} = \frac{1}{2}$
        $\text{proj} = \frac{1}{2}(-\frac{2}{3}, \frac{1}{3}, \frac{1}{3})$
    * **Combine:**
        $\mathbf{v}_3 = (0,0,1) - (\frac{1}{3}, \frac{1}{3}, \frac{1}{3}) - (-\frac{1}{3}, \frac{1}{6}, \frac{1}{6})$
        $\mathbf{v}_3 = \mathbf{(0, -\frac{1}{2}, \frac{1}{2})}$

#### **Phase 2: Normalize to get Orthonormal Vectors ($\mathbf{q}$)**

1.  $\|\mathbf{v}_1\| = \sqrt{3} \implies \mathbf{q}_1 = (\frac{1}{\sqrt{3}}, \frac{1}{\sqrt{3}}, \frac{1}{\sqrt{3}})$
2.  $\|\mathbf{v}_2\| = \sqrt{\frac{2}{3}} \implies \mathbf{q}_2 = (-\frac{2}{\sqrt{6}}, \frac{1}{\sqrt{6}}, \frac{1}{\sqrt{6}})$
3.  $\|\mathbf{v}_3\| = \sqrt{\frac{1}{2}} \implies \mathbf{q}_3 = (0, -\frac{1}{\sqrt{2}}, \frac{1}{\sqrt{2}})$

---

## **2. QR-Decomposition**

If $A$ is an $m \times n$ matrix with linearly independent columns, it can be factored into:
$$A = QR$$
* **Q:** An orthonormal matrix (columns are $\mathbf{q}_1, \mathbf{q}_2 \dots$ found via Gram-Schmidt).
* **R:** An invertible **Upper Triangular** matrix.

### **How to Construct R**
You do not need to do Gaussian elimination to find $R$. You use the inner products of the *original* vectors ($\mathbf{u}$) against the *new orthonormal* vectors ($\mathbf{q}$).

$$R = \begin{bmatrix} \langle \mathbf{u}_1, \mathbf{q}_1 \rangle & \langle \mathbf{u}_2, \mathbf{q}_1 \rangle & \langle \mathbf{u}_3, \mathbf{q}_1 \rangle \\ 0 & \langle \mathbf{u}_2, \mathbf{q}_2 \rangle & \langle \mathbf{u}_3, \mathbf{q}_2 \rangle \\ 0 & 0 & \langle \mathbf{u}_3, \mathbf{q}_3 \rangle \end{bmatrix}$$

### **Worked Example (Verified)**
For the matrix $A$ with columns $\mathbf{u}_1, \mathbf{u}_2, \mathbf{u}_3$ from the previous example:

**1. The Q Matrix**
Simply place the $\mathbf{q}$ vectors as columns.
$$Q = \begin{bmatrix} \frac{1}{\sqrt{3}} & -\frac{2}{\sqrt{6}} & 0 \\ \frac{1}{\sqrt{3}} & \frac{1}{\sqrt{6}} & -\frac{1}{\sqrt{2}} \\ \frac{1}{\sqrt{3}} & \frac{1}{\sqrt{6}} & \frac{1}{\sqrt{2}} \end{bmatrix}$$

**2. The R Matrix**
Calculate the dot products:

* **Row 1:**
    * $r_{11} = \mathbf{u}_1 \cdot \mathbf{q}_1 = \frac{3}{\sqrt{3}} = \sqrt{3}$
    * $r_{12} = \mathbf{u}_2 \cdot \mathbf{q}_1 = \frac{2}{\sqrt{3}}$
    * $r_{13} = \mathbf{u}_3 \cdot \mathbf{q}_1 = \frac{1}{\sqrt{3}}$
* **Row 2:**
    * $r_{22} = \mathbf{u}_2 \cdot \mathbf{q}_2 = \frac{2}{\sqrt{6}}$
    * $r_{23} = \mathbf{u}_3 \cdot \mathbf{q}_2 = \frac{1}{\sqrt{6}}$
* **Row 3:**
    * $r_{33} = \mathbf{u}_3 \cdot \mathbf{q}_3 = \frac{1}{\sqrt{2}}$

**Final Result:**
$$R = \begin{bmatrix} \frac{3}{\sqrt{3}} & \frac{2}{\sqrt{3}} & \frac{1}{\sqrt{3}} \\ 0 & \frac{2}{\sqrt{6}} & \frac{1}{\sqrt{6}} \\ 0 & 0 & \frac{1}{\sqrt{2}} \end{bmatrix}$$
