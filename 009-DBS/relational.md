### **1. Unary Operations (Operations on a Single Table)**

Unary operations filter data either horizontally (rows) or vertically (columns).

#### **SELECT ($\sigma$)**

* **Purpose:** Filters rows (tuples) based on a specific boolean condition. Discards tuples that evaluate to false.
* **Notation:** $\sigma_{\text{condition}}(R)$
* **Properties:**
* Produces a relation with the exact same schema (attributes) as the input.
* **Commutative:** The order doesn't matter. $\sigma_{c1}(\sigma_{c2}(R)) = \sigma_{c2}(\sigma_{c1}(R))$
* **Cascading:** A sequence of selects can be combined using `AND`. $\sigma_{c1}(\sigma_{c2}(R)) = \sigma_{c1 \text{ AND } c2}(R)$


* **SQL Equivalent:** The `WHERE` clause.
* *Math:* $\sigma_{\text{Salary} > 30000}(\text{EMPLOYEE})$
* *SQL:* `SELECT * FROM EMPLOYEE WHERE Salary > 30000;`



#### **PROJECT ($\pi$)**

* **Purpose:** Filters columns (attributes). It keeps only the specified columns and creates a vertical partition.
* **Notation:** $\pi_{\text{attribute\_list}}(R)$
* **Properties:**
* **Removes Duplicates:** Because relational algebra is based on mathematical sets (which cannot contain duplicate elements), PROJECT automatically eliminates duplicate rows.
* **Not Commutative:** You cannot reverse the order of projections unless the outer list is a subset of the inner list.


* **SQL Equivalent:** The `SELECT` (specifically `SELECT DISTINCT`) clause.
* *Math:* $\pi_{\text{Lname, Fname}}(\text{EMPLOYEE})$
* *SQL:* `SELECT DISTINCT Lname, Fname FROM EMPLOYEE;`



#### **RENAME ($\rho$)**

* **Purpose:** Renames a relation (table) and/or its attributes (columns). Very useful for saving intermediate results or handling self-joins.
* **Notation:** * $\rho_{S(B_1, B_2, ..., B_n)}(R)$ — Renames both the table to $S$ and the columns to $B_1, B_2...$
* $\rho_{S}(R)$ — Renames only the table.


* **SQL Equivalent:** The `AS` keyword (Aliasing).
* *SQL:* `SELECT Fname AS First_Name FROM EMPLOYEE AS E;`



---

### **2. Set Theory Operations (Operations on Two Tables)**

**Crucial Prerequisite:** To use `UNION`, `INTERSECTION`, or `DIFFERENCE`, the two relations must be **Type Compatible** (also known as Union Compatible). This means:

1. They have the exact same number of columns.
2. The corresponding columns have identical or compatible data types.

| Operation | Symbol | Purpose | SQL Equivalent |
| --- | --- | --- | --- |
| **UNION** | $\cup$ | Returns all unique rows present in $R$, $S$, or both. | `UNION` |
| **INTERSECTION** | $\cap$ | Returns only the rows that exist in *both* $R$ and $S$. | `INTERSECT` |
| **DIFFERENCE** | $-$ | Returns rows present in $R$ but *not* in $S$. | `EXCEPT` or `MINUS` |

---

### **3. Binary Operations (Combining Tables)**

These operations combine data from two different relations. They do *not* require type compatibility.

#### **CARTESIAN PRODUCT (Cross Product) ($\times$)**

* **Purpose:** Combines every single row in relation $R$ with every single row in relation $S$. If $R$ has 5 rows and $S$ has 4 rows, the result has 20 rows.
* **Notation:** $R \times S$
* **Note:** Usually meaningless on its own unless followed by a SELECT operation to filter related rows.
* **SQL Equivalent:** `CROSS JOIN` (or separating tables with a comma in the `FROM` clause without a `WHERE` condition).

#### **JOIN ($\bowtie$)**

* **Purpose:** Combines a Cartesian Product and a SELECT operation into one efficient step. Matches rows from two tables based on a condition.
* **Notation:** $R \bowtie_{\text{condition}} S$
* **Types of Joins:**
* **Theta Join ($\bowtie_{\theta}$):** A join with any general condition (e.g., $<$, $>$, $\ge$).
* **Equijoin:** A join where the condition *only* uses equality ($=$).
* **Natural Join ($*$):** A special equijoin that automatically matches columns with the same exact names in both tables and *removes the duplicate column* from the result.


* **SQL Equivalent:** `INNER JOIN ... ON`
* *Math:* $\text{DEPARTMENT} \bowtie_{\text{Mgr\_SSN = SSN}} \text{EMPLOYEE}$
* *SQL:* `SELECT * FROM DEPARTMENT JOIN EMPLOYEE ON Mgr_SSN = SSN;`
* *Natural Join SQL:* `NATURAL JOIN`



---

### **4. Advanced Operations**

#### **AGGREGATE FUNCTIONS ($\mathcal{F}$)**

* **Purpose:** Computes a summary value for a set of tuples (e.g., MAX, MIN, SUM, COUNT, AVERAGE). Can be grouped by specific attributes.
* **Notation:** $_{Grouping\_Attributes}\mathcal{F}_{Aggregate\_Functions}(R)$
* **Properties:** Unlike standard relational algebra, the COUNT function *does not* remove duplicates.
* **SQL Equivalent:** `GROUP BY` combined with aggregate functions.
* *Math:* $_{\text{DNO}}\mathcal{F}_{\text{COUNT SSN, AVERAGE Salary}}(\text{EMPLOYEE})$
* *SQL:* `SELECT DNO, COUNT(SSN), AVG(Salary) FROM EMPLOYEE GROUP BY DNO;`



#### **DIVISION ($\div$)**

* **Purpose:** Used for queries that include the word "every" or "all". For example, "Find the employees who have worked on *all* projects."
* **Notation:** $R \div S$
* **Mental Model:** If $R$ contains (Employee, Project) and $S$ contains all (Projects), $R \div S$ returns only the Employees associated with every Project in $S$.
* **SQL Equivalent:** There is no direct `DIVIDE` operator in SQL. It is famously solved using double negation: `NOT EXISTS` (find an employee for whom there does *not exist* a project that they have *not* worked on).

---

### **5. Bonus Fact: The "Complete Set"**

You only actually need 5 foundational operations: **$\sigma, \pi, \cup, -, \times$**.
Every other operation (like Joins or Intersections) is just syntactic sugar and can be mathematically derived from these five! For example, an Intersection ($R \cap S$) is just $R - (R - S)$.

---

Do these SQL mappings help clarify how the mathematical formulas translate into actual database queries?