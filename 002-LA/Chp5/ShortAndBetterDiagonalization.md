### 1. Eigenvalues and Eigenvectors

**Definition:**
If  is an  matrix, a **non-zero** vector  is an **eigenvector** if  is just a scalar multiple of .


*  (lambda) is the **eigenvalue**.
*  is the **eigenvector** corresponding to .

**The Characteristic Equation:**
To find eigenvalues, we solve for where the determinant is zero:


* An  matrix has at most  distinct eigenvalues.

**Shortcuts & Properties:**

1. **Triangular Matrices:** If  is triangular (upper, lower, or diagonal), the eigenvalues are simply the **entries on the main diagonal**.
2. **Invertibility:** A square matrix  is invertible if and only if ** is NOT an eigenvalue**.

---

### 2. The Invertible Matrix Theorem (Summary)

If  is an  matrix, the following statements are **equivalent**. If one is true, they are all true; if one is false, they are all false.

**Basic Properties**

* (a)  is invertible.
* (g) .
* (n)  has rank .
* (r) ** is not an eigenvalue of .**

**Systems of Equations**

* (b)  has only the trivial solution ().
* (f)  has exactly one solution for every .

**Vector Spaces (Rows & Columns)**

* (h/i) Column/Row vectors are linearly independent.
* (j/k) Column/Row vectors span .
* (l/m) Column/Row vectors form a basis for .

**Subspaces**

* (o) The nullity of  is 0.
* (p) The orthogonal complement of the null space is .

---

### 3. Similar Matrices

Matrices  and  are **similar** if:



*(Where  is an invertible change-of-basis matrix)*

**Concept:**
They represent the **exact same linear transformation**, but viewed from **different coordinate systems**.

* Think of it like measuring the same object in inches vs. centimeters.

**Shared Properties:**
Similar matrices always have the same:

* Eigenvalues
* Determinant
* Rank
* Trace (sum of diagonal elements)

---

### 4. Diagonalization

A matrix  is **diagonalizable** if it is similar to a diagonal matrix . This means we can factor  as:


**The Components:**

* ** (Diagonal Matrix):** Contains the **eigenvalues** of  on the diagonal.
* ** (Invertible Matrix):** Contains the **eigenvectors** of  as its columns.

**Why do this?**
It reveals the simplest form of the transformation. In the "P-basis," the matrix simply stretches or shrinks vectors; it does not rotate or shear them.

---

### 5. The Diagonalization Test (AM vs. GM)

To know if  is diagonalizable, we compare two counts for every eigenvalue .

**1. Algebraic Multiplicity (AM):**

* How many times  appears as a root in the characteristic equation.
* *Example:* If , then AM = 3.

**2. Geometric Multiplicity (GM):**

* The number of linearly independent eigenvectors for .
* *Calculation:* Dimension of the Null Space of .

**The Theorem:**
A matrix is diagonalizable **if and only if**:

1. The sum of all AMs equals  (the matrix size).
2. **For every eigenvalue: GM = AM.**

| Condition | Result |
| --- | --- |
| **GM = AM** (for all ) | **Diagonalizable** |
| **GM < AM** (for any ) | **Not Diagonalizable** (Defective) |

---

### 6. Step-by-Step: How to Diagonalize

If asked to find  and :

1. **Find Eigenvalues:** Solve .
2. **Find Eigenvectors:** For each , solve .
3. **Check Condition:** Ensure we have enough eigenvectors (GM = AM).
4. **Construct :** Stack the eigenvectors as columns.
5. **Construct :** Place corresponding eigenvalues on the diagonal.
* *Critical:* The order of columns in  must match the order of entries in .



---

### 7. Application: Computing High Powers ()

Calculating  manually involves tedious matrix multiplication. Diagonalization makes this instant.

**The Formula:**


**Why it works:**
When you square , the middle terms cancel out ():


**How to use it:**

1. Diagonalize  to get  and .
2. Raise the diagonal entries of  to the power  (this is easy math).
* If , then .


3. Compute the final product .
