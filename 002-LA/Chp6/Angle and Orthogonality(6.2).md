## **Angles, Orthogonality & Inequalities**

### **1. The Cauchy-Schwarz Inequality (SEE MISS's FORMULA)** 
This is the "Golden Rule" of inner product spaces. It guarantees that geometry works (i.e., that we can calculate angles).

* **The Formula:**
    $$|\langle \mathbf{u}, \mathbf{v} \rangle| \le \|\mathbf{u}\| \|\mathbf{v}\|$$
    *Alternatively written as:* $\langle \mathbf{u}, \mathbf{v} \rangle^2 \le \langle \mathbf{u}, \mathbf{u} \rangle \langle \mathbf{v}, \mathbf{v} \rangle$

* **Concept:** The inner product of two vectors can never be larger than the product of their individual lengths.

### **2. Triangle Inequalities**
These define the concept of "shortest path."

* **Vector Form:**
    $$\|\mathbf{u} + \mathbf{v}\| \le \|\mathbf{u}\| + \|\mathbf{v}\|$$
    *(The length of the combined vector is never more than adding the two lengths separately).*
* **Distance Form:**
    $$d(\mathbf{u}, \mathbf{v}) \le d(\mathbf{u}, \mathbf{w}) + d(\mathbf{w}, \mathbf{v})$$
    *(Going directly from $\mathbf{u}$ to $\mathbf{v}$ is shorter than taking a detour through $\mathbf{w}$).*

---

### **3. Orthogonality (Perpendicularity)**
In general vector spaces, we don't use a protractor. We use the inner product to check for perpendicularity.

* **Definition:** Two vectors $\mathbf{u}$ and $\mathbf{v}$ are orthogonal if:
    $$\langle \mathbf{u}, \mathbf{v} \rangle = 0$$

* **Generalized Pythagorean Theorem:**
    If (and only if) $\mathbf{u}$ and $\mathbf{v}$ are orthogonal, then:
    $$\|\mathbf{u} + \mathbf{v}\|^2 = \|\mathbf{u}\|^2 + \|\mathbf{v}\|^2$$

---

### **4. Conceptual: Why does Orthogonality depend on the Inner Product?**

You asked: *"Why are vectors orthogonal in Euclidean space but NOT in Weighted space?"*

**The Explanation:**
Imagine "Orthogonality" not just as a 90-degree visual angle, but as **"Zero Interaction."**

* **Euclidean Space:** The grid is perfectly square. $\mathbf{u}=(1,1)$ and $\mathbf{v}=(1,-1)$ cancel each other out perfectly. One goes up-right, the other goes down-right. The "up" cancels the "down." Result: 0.
* **Weighted Space:** Imagine stretching the grid. If you weight the x-axis by 3 (make it 3x more important), the horizontal movement now dominates. The "up/down" cancellation isn't enough to overcome the massive horizontal overlap. The vectors now "interact" or "overlap" in this stretched space.

> **Visual Analogy:** If you draw a perfect "+" sign on a balloon (orthogonal lines) and then stretch the balloon horizontally (weighted inner product), the lines are no longer 90 degrees. They are skewed. The "angle" changes because the "space" changed.

---

### **5. How to Find Angles (Euclidean vs. Weighted)**

The formula for the angle $\theta$ is **always the same**:
$$\cos \theta = \frac{\langle \mathbf{u}, \mathbf{v} \rangle}{\|\mathbf{u}\| \|\mathbf{v}\|}$$

However, the **values** you plug in change depending on the inner product rules.

#### **Case A: Using Euclidean Inner Product**
*Vectors:* $\mathbf{u} = (1, 1)$, $\mathbf{v} = (1, -1)$
*Inner Product:* $\mathbf{u} \cdot \mathbf{v} = u_1v_1 + u_2v_2$

1.  **Calculate Inner Product:**
    $\langle \mathbf{u}, \mathbf{v} \rangle = (1)(1) + (1)(-1) = 0$
2.  **Calculate Norms:**
    $\|\mathbf{u}\| = \sqrt{1^2 + 1^2} = \sqrt{2}$
    $\|\mathbf{v}\| = \sqrt{1^2 + (-1)^2} = \sqrt{2}$
3.  **Find Angle:**
    $\cos \theta = \frac{0}{\sqrt{2}\sqrt{2}} = 0 \implies \mathbf{\theta = 90^\circ}$ (Orthogonal)

#### **Case B: Using Weighted Inner Product**
*Weights:* $w_1 = 3, w_2 = 2$
*Formula:* $\langle \mathbf{u}, \mathbf{v} \rangle = 3u_1v_1 + 2u_2v_2$

1.  **Calculate Inner Product:**
    $\langle \mathbf{u}, \mathbf{v} \rangle = 3(1)(1) + 2(1)(-1) = 3 - 2 = \mathbf{1}$
2.  **Calculate Norms (MUST use weights too!):**
    $\|\mathbf{u}\| = \sqrt{\langle \mathbf{u}, \mathbf{u} \rangle} = \sqrt{3(1)^2 + 2(1)^2} = \sqrt{5}$
    $\|\mathbf{v}\| = \sqrt{\langle \mathbf{v}, \mathbf{v} \rangle} = \sqrt{3(1)^2 + 2(-1)^2} = \sqrt{5}$
3.  **Find Angle:**
    $\cos \theta = \frac{1}{\sqrt{5}\sqrt{5}} = \frac{1}{5} = 0.2$
    $\theta = \arccos(0.2) \approx \mathbf{78.5^\circ}$

**Exam Takeaway:**
* In Euclidean, they are $90^\circ$.
* In Weighted, they are $78.5^\circ$.
* **Always** recalculate the norms $\|\mathbf{u}\|$ using the weighted formula. Do not use the Euclidean length for a weighted angle problem!
