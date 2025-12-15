# 7.2 Orthogonal Diagonalization

## 1. The Big Picture
**Text:** "In this section we will be concerned with the problem of diagonalizing a symmetric matrix \(A\)."

**Context & Intuition:**
Normally, when we diagonalize a matrix, we are trying to find a "better viewpoint" (coordinate system) where the matrix just acts as a simple scaling machine (stretching or shrinking axes).

* **The Problem:** Sometimes, this new coordinate system is skewed—the axes aren't perpendicular to each other.
* **The Solution:** We are looking for the "Holy Grail" of matrices: **Symmetric Matrices**. These matrices allow us to find a coordinate system that is both **scaled perfectly** AND retains a perfect **90-degree grid** (orthogonal).

---

## 2. Orthogonal Similarity
**Text:** "Definition 1: If \(A\) and \(B\) are square matrices, then we say that \(B\) is orthogonally similar to \(A\) if there is an orthogonal matrix \(P\) such that \(B = P^T A P\)."

**Context & Intuition:**
* **Standard Diagonalization:** Uses the formula \(D = P^{-1} A P\). Calculating an inverse (\(P^{-1}\)) is hard and messy.
* **Orthogonal Diagonalization:** Uses \(D = P^T A P\).
* **Why is this better?** For an orthogonal matrix, the inverse **is** the transpose (\(P^{-1} = P^T\)). You don't have to do any difficult inverse calculations; you just flip the rows and columns of \(P\).

---

## 3. The Spectral Theorem (The Rules of the Game)
**Text:** "If \(A\) is an \(n \times n\) matrix with real entries, then the following are equivalent: (a) \(A\) is orthogonally diagonalizable. (b) \(A\) has an orthonormal set of \(n\) eigenvectors. (c) \(A\) is symmetric."

**Context & Intuition:**
This is the most important theorem in this section. It tells us exactly when we can use this method.

1. **Symmetric (\(A = A^T\)):** If you flip the matrix over its main diagonal and it looks identical, you hit the jackpot.
2. **Real Eigenvalues:** A symmetric matrix will never give you complex numbers (like \(2+3i\)). This is physically significant (e.g., in quantum mechanics, observables must be real).
3. **Orthonormal Set:** You can find eigenvectors that are all perpendicular (orthogonal) and length 1 (normal).

---

## 4. Properties of Symmetric Matrices
**Text:** "If \(A\) is a symmetric matrix with real entries, then: (a) The eigenvalues of \(A\) are all real numbers. (b) Eigenvectors from different eigenspaces are orthogonal."

**Context & Intuition:**
* **Point (b) is the magic trick.** Usually, you have to work hard to prove vectors are perpendicular. With a symmetric matrix, if two eigenvectors come from *different* eigenvalues (e.g., \(\lambda=2\) and \(\lambda=5\)), they are **automatically** perpendicular (90°) to each other. You don't even have to check.

---

## 5. The Process: How to do it
**Text:**
"Step 1. Find a basis for each eigenspace of \(A\)."
"Step 2. Apply the Gram–Schmidt process to each of these bases..."
"Step 3. Form the matrix \(P\)..."

**Context & Intuition:**
* **Step 1:** Find the eigenvalues and eigenvectors just like normal.
* **Step 2 (The Cleanup):**
    * If all eigenvalues are distinct (different numbers), the vectors are already orthogonal. You just need to normalize them (divide by their length).
    * If you have a **repeated** eigenvalue (e.g., \(\lambda = 2\) appears twice), the two eigenvectors you find might not be perpendicular *to each other*. You must use **Gram-Schmidt** to force them to be perpendicular.
* **Step 3:** Stack these cleaned-up vectors into matrix \(P\).

***

# Example Question (Solved in Depth)

**Problem:**
Find an orthogonal matrix \(P\) that diagonalizes the symmetric matrix:
\[A = \begin{bmatrix} 4 & 2 & 2 \\ 2 & 4 & 2 \\ 2 & 2 & 4 \end{bmatrix}\]

### Step 1: Find Eigenvalues
We solve \(\det(A - \lambda I) = 0\).
\[\det \begin{bmatrix} 4-\lambda & 2 & 2 \\ 2 & 4-\lambda & 2 \\ 2 & 2 & 4-\lambda \end{bmatrix} = 0\]

*Skipping the algebra for brevity:* The characteristic polynomial is \(-(\lambda - 2)^2(\lambda - 8) = 0\).
**Eigenvalues:** \(\lambda = 2\) (multiplicity 2) and \(\lambda = 8\) (multiplicity 1).

### Step 2: Find Eigenvectors

**A. For \(\lambda = 8\):**
We solve \((A - 8I)x = 0\):
\[\begin{bmatrix} -4 & 2 & 2 \\ 2 & -4 & 2 \\ 2 & 2 & -4 \end{bmatrix} \to \text{Row Reduce} \to \begin{bmatrix} 1 & -1 & 0 \\ 0 & 1 & -1 \\ 0 & 0 & 0 \end{bmatrix}\]
This tells us \(x_1 = x_2\) and \(x_2 = x_3\).
The eigenvector is: **\(v_1 = (1, 1, 1)\)**.

**B. For \(\lambda = 2\):**
We solve \((A - 2I)x = 0\):
\[\begin{bmatrix} 2 & 2 & 2 \\ 2 & 2 & 2 \\ 2 & 2 & 2 \end{bmatrix} \to \begin{bmatrix} 1 & 1 & 1 \\ 0 & 0 & 0 \\ 0 & 0 & 0 \end{bmatrix}\]
Equation: \(x_1 + x_2 + x_3 = 0\).
Since we have two free variables, we need two vectors.
Let \(x_2 = 1, x_3 = 0 \implies x_1 = -1 \rightarrow \mathbf{v_2} = (-1, 1, 0)\)
Let \(x_2 = 0, x_3 = 1 \implies x_1 = -1 \rightarrow \mathbf{v_3} = (-1, 0, 1)\)

### Step 3: Orthogonalize and Normalize (The Crucial Step)

**For \(\lambda = 8\):**
Vector \(v_1 = (1, 1, 1)\) is already orthogonal to the others (because \(\lambda=8\) is unique).
We just **normalize** it (divide by length \(\sqrt{1^2+1^2+1^2} = \sqrt{3}\)).
\[\mathbf{u_1} = \begin{bmatrix} 1/\sqrt{3} \\ 1/\sqrt{3} \\ 1/\sqrt{3} \end{bmatrix}\]

**For \(\lambda = 2\):**
We have \(v_2 = (-1, 1, 0)\) and \(v_3 = (-1, 0, 1)\).
Are they orthogonal? Dot product: \((-1)(-1) + (1)(0) + (0)(1) = 1\). **NO.** They are not zero.
We must use **Gram-Schmidt** to fix \(v_3\).

1. Keep \(v_2\) as is: \(\mathbf{w_2} = (-1, 1, 0)\).
2. Fix \(v_3\) by subtracting the part of it that points in the \(v_2\) direction:
   \[w_3 = v_3 - \frac{v_3 \cdot w_2}{w_2 \cdot w_2} w_2\]
   \[w_3 = (-1, 0, 1) - \frac{1}{2} (-1, 1, 0)\]
   \[w_3 = (-1, 0, 1) - (-0.5, 0.5, 0) = (-0.5, -0.5, 1)\]
   To make math easy, multiply by 2 (direction stays same): \(\mathbf{w_3} = (-1, -1, 2)\).

Now **normalize** these two:
* Length of \(w_2\) is \(\sqrt{2}\). \(\mathbf{u_2} = \begin{bmatrix} -1/\sqrt{2} \\ 1/\sqrt{2} \\ 0 \end{bmatrix}\)
* Length of \(w_3\) is \(\sqrt{6}\). \(\mathbf{u_3} = \begin{bmatrix} -1/\sqrt{6} \\ -1/\sqrt{6} \\ 2/\sqrt{6} \end{bmatrix}\)

### Final Answer: The Matrix P
We combine our orthonormal vectors \(u_1, u_2, u_3\) into the matrix \(P\).

\[
P = \begin{bmatrix}
1/\sqrt{3} & -1/\sqrt{2} & -1/\sqrt{6} \\
1/\sqrt{3} & 1/\sqrt{2} & -1/\sqrt{6} \\
1/\sqrt{3} & 0 & 2/\sqrt{6}
\end{bmatrix}
\]

This matrix \(P\) is orthogonal. If you calculate \(P^T A P\), you will get exactly:
\[D = \begin{bmatrix} 8 & 0 & 0 \\ 0 & 2 & 0 \\ 0 & 0 & 2 \end{bmatrix}\]
