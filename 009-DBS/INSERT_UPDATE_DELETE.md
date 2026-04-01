Here are your SQL notes categorized by operation, formatted for clarity, and expanded with technical context and best practices.

### 1. Data Insertion (`INSERT`)

The `INSERT` statement adds new rows to a table. 

**Standard Insert Operations**
* **Implicit Column Order:** Requires passing a value for every column in the exact order defined in the table schema.
    ```sql
    INSERT INTO table_name VALUES (value1, value2, value3);
    ```
* **Explicit Column Order (Best Practice):** Highly recommended. It prevents errors if the table structure changes (e.g., a new column is added) and improves readability.
    ```sql
    INSERT INTO table_name (column1, column3) VALUES (value1, value3);
    ```

**Advanced Insert Operations**
* **Insert Multiple Rows (Oracle `INSERT ALL`):** Standard SQL uses `VALUES (v1), (v2)`. Oracle requires `INSERT ALL` terminating with a selection from the dummy table `dual`.
    ```sql
    INSERT ALL
      INTO tasks (title, priority) VALUES ('Task 1', 1)
      INTO tasks (title, priority) VALUES ('Task 2', 2)
      INTO tasks (title, priority) VALUES ('Task 3', 3)
    SELECT * FROM dual;
    ```
* **Insert from Another Table (`INSERT INTO ... SELECT`):** Copies data resulting from a query directly into a target table.
    ```sql
    INSERT INTO tasks (title)
    SELECT department FROM employee;
    
    INSERT INTO tasks (title)
    SELECT email FROM employee WHERE empNo = 'E101';
    ```
* **Insert Using Subqueries as Values:** Evaluates queries dynamically to generate values for a new row.
    ```sql
    INSERT INTO tasks (task_id, title, priority)
    VALUES (
      (SELECT COUNT(*) FROM employee),
      (SELECT email FROM employee WHERE empNo = 'E102'),
      (SELECT COUNT(*) FROM employee)
    );
    ```

---

### 2. Data Modification (`UPDATE`)

The `UPDATE` statement modifies existing records. A `WHERE` clause is critical; omitting it will update every row in the table.

**Update Operations**
* **Single Column:**
    ```sql
    UPDATE employee
    SET empName = 'BlackSmith'
    WHERE empNo = 'E100';
    ```
* **Multiple Columns:**
    ```sql
    UPDATE employee
    SET empName = 'BlackSmith',
        department = 'Iron'
    WHERE empNo = 'E100';
    ```
* **Update Using String Functions (`REPLACE`):** Useful for bulk-cleaning or migrating data format.
    ```sql
    UPDATE employee
    SET email = REPLACE(email, '@colorado.edu', '@nu.edu.pk')
    WHERE empNo = 'E100';
    ```

---

### 3. Data Removal (`DELETE` vs. `TRUNCATE`)

Both commands remove data, but they operate differently at the database engine level.

| Feature | `DELETE` | `TRUNCATE` |
| :--- | :--- | :--- |
| **Command Type** | DML (Data Manipulation Language) | DDL (Data Definition Language) |
| **Speed** | Slower. Scans and logs each row deletion. | Faster. Deallocates data pages; minimal logging. |
| **Selectivity** | Can remove specific rows using a `WHERE` clause. | Removes **all** rows. Cannot use a `WHERE` clause. |
| **Rollback** | Can be rolled back (if uncommitted). | Cannot be rolled back in most databases (including Oracle). |
| **Triggers** | Activates `ON DELETE` triggers. | Does not activate triggers. |
| **Storage** | Leaves high-water mark intact (does not free space). | Resets high-water mark (reclaims storage space). |

**Syntax Examples:**
```sql
-- DELETE specific row
DELETE FROM employee WHERE empNo = 'E101';

-- DELETE all rows (slower)
DELETE FROM employee;

-- TRUNCATE all rows (faster, resets table state)
TRUNCATE TABLE employee;
```

---

### 4. Additional Essential Concept: Transaction Control

When executing `INSERT`, `UPDATE`, or `DELETE` commands (DML), the changes are strictly written to your current session. To make them permanent or to undo them, you must use Transaction Control Language (TCL):

* **`COMMIT;`** – Saves all pending DML transactions to the database permanently.
* **`ROLLBACK;`** – Undoes all pending DML transactions since the last commit.
