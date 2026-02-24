### Part 1: The Clean Slate Implementation

When designing a database schema, it is best practice to define your rules (constraints) precisely when you create the tables.

Here is the complete SQL implementation:

```sql
-- 1. Departments Table (Parent Table)
CREATE TABLE Departments (
    DepartmentID INT,
    DepartmentName VARCHAR(100) NOT NULL,
    Location VARCHAR(100) DEFAULT 'Main Campus',
    
    -- Table-level Primary Key Constraint
    CONSTRAINT PK_Departments PRIMARY KEY (DepartmentID)
);

-- 2. Employees Table (Child of Departments, Parent of Projects)
CREATE TABLE Employees (
    EmployeeID INT,
    FirstName VARCHAR(50) NOT NULL,
    LastName VARCHAR(50) NOT NULL,
    Email VARCHAR(100) CONSTRAINT UQ_EmpEmail UNIQUE,
    HireDate DATE DEFAULT CURRENT_DATE,
    Salary DECIMAL(10, 2) CONSTRAINT CHK_EmpSalary CHECK (Salary >= 30000),
    DepartmentID INT,
    
    -- Table-level Primary and Foreign Key Constraints
    CONSTRAINT PK_Employees PRIMARY KEY (EmployeeID),
    CONSTRAINT FK_Employees_Departments FOREIGN KEY (DepartmentID) 
        REFERENCES Departments(DepartmentID)
        ON DELETE SET NULL 
        ON UPDATE CASCADE
);

-- 3. Projects Table (Child of Employees)
CREATE TABLE Projects (
    ProjectID INT,
    ProjectName VARCHAR(100) NOT NULL,
    Budget DECIMAL(12, 2),
    LeadEmployeeID INT,
    
    -- Table-level Constraints
    CONSTRAINT CHK_ProjBudget CHECK (Budget > 0 AND Budget <= 10000000),
    CONSTRAINT PK_Projects PRIMARY KEY (ProjectID),
    CONSTRAINT FK_Projects_Employees FOREIGN KEY (LeadEmployeeID)
        REFERENCES Employees(EmployeeID)
        ON DELETE RESTRICT
        ON UPDATE CASCADE
);

```

*(Note: While `NOT NULL` and `DEFAULT` can theoretically be given explicit constraint names using the `CONSTRAINT` keyword in some database systems, it is standard practice to simply define them inline as shown above.)*

---

### Part 2: Detailed Breakdown of Keywords & Constraints

#### 1. The `CONSTRAINT` Keyword (Naming Constraints)

* **What it is:** A keyword used to explicitly assign a name to a rule you are applying to a column or table.
* **Syntax:** `CONSTRAINT constraint_name constraint_type`
* **Why it's essential:** If you do not name a constraint, the Database Management System (DBMS) will generate a random, unreadable name for it (e.g., `SYS_C001043`). If you ever need to modify or delete that rule later, you won't know what to call it.
* **Naming Convention:** A standard practice is `ConstraintType_TableName_ColumnName` (e.g., `PK_Employees`, `FK_Projects_Employees`, `CHK_EmpSalary`).

#### 2. Basic Data Types & `NOT NULL`

* **Data Types:** Define the nature of the data (e.g., `INT` for whole numbers, `VARCHAR(n)` for variable-length strings, `DATE`, `DECIMAL(precision, scale)`).
* **`NOT NULL`:** Ensures that a column cannot be left empty. Any `INSERT` or `UPDATE` operation attempting to place a `NULL` value here will fail. It is typically defined at the column level.

#### 3. Default Values (`DEFAULT <value>`)

* **What it is:** Specifies a value to be inserted automatically if the user omits that column during an `INSERT` statement.
* **Detail:** `DEFAULT` is technically not classified as a "constraint" in the strict relational algebra sense in all DBMSs, but it acts as a rule. You do not use the `CONSTRAINT` keyword to name a `DEFAULT` clause in standard SQL.
* **Examples:** `DEFAULT 0`, `DEFAULT 'Pending'`, `DEFAULT CURRENT_DATE`.

#### 4. Primary Key (`PRIMARY KEY`)

* **What it is:** Enforces **Entity Integrity**. It uniquely identifies each record in the table.
* **Detail:** A Primary Key automatically implies `NOT NULL` and `UNIQUE`. A table can only have one primary key (though it can be a composite key made of multiple columns).

#### 5. Foreign Key and Referential Actions

* **What it is:** Enforces **Referential Integrity**. It links a column (or columns) in one table to the Primary Key in another table, ensuring that relationships between records remain valid.
* **`ON DELETE` and `ON UPDATE` Clauses:** These dictate what happens to the *child* rows when the referenced *parent* row is deleted or updated.
* `CASCADE`: If the parent row is deleted/updated, automatically delete/update the matching child rows.
* `SET NULL`: If the parent row is deleted/updated, set the foreign key value in the child rows to `NULL` (requires the child column to allow `NULL`s).
* `RESTRICT` (or `NO ACTION`): Prevents the deletion/update of the parent row if any child rows reference it. This is usually the default behavior if no clause is specified.
* `SET DEFAULT`: Sets the child row's foreign key to its defined default value.



#### 6. The `CHECK` Clause

* **What it is:** Enforces **Domain Constraints** by ensuring that all values in a column satisfy a specific Boolean expression (a condition that evaluates to True, False, or Unknown).
* **Detail:** It can be used to restrict ranges (e.g., `Age >= 18`), specific lists of values (e.g., `Status IN ('Active', 'Inactive')`), or formatting.

---

### Part 3: Altering Stuff Later (The `ALTER TABLE` Command)

If you need to change rules after the table has already been created, you use the `ALTER TABLE` command. This is where explicitly naming your constraints (using the `CONSTRAINT` keyword) pays off.

#### 1. Adding a Constraint After Creation

If you forgot to add a `CHECK` or `FOREIGN KEY` constraint initially, you can add it later. *Note: If existing data violates the new constraint, the `ALTER TABLE` command will fail.*

```sql
-- Adding a CHECK constraint
ALTER TABLE Employees
ADD CONSTRAINT CHK_EmpEmailFormat CHECK (Email LIKE '%@%');

-- Adding a FOREIGN KEY constraint
ALTER TABLE Projects
ADD CONSTRAINT FK_Projects_Depts FOREIGN KEY (DepartmentID)
    REFERENCES Departments(DepartmentID);

```

#### 2. Dropping a Constraint

To remove a rule, you must know its name. This is why we named them during table creation.

```sql
-- Removing the salary check constraint
ALTER TABLE Employees
DROP CONSTRAINT CHK_EmpSalary;

-- Removing a foreign key constraint
ALTER TABLE Employees
DROP CONSTRAINT FK_Employees_Departments;

```

#### 3. Modifying `NOT NULL` and `DEFAULT`

Adding or dropping `NOT NULL` and `DEFAULT` is usually done by modifying the column itself, rather than using `ADD/DROP CONSTRAINT` (though syntax can vary slightly between Oracle, SQL Server, and PostgreSQL).

```sql
-- To add a DEFAULT value to an existing column
ALTER TABLE Employees
ALTER COLUMN Salary SET DEFAULT 50000;

-- To drop a DEFAULT value
ALTER TABLE Employees
ALTER COLUMN Salary DROP DEFAULT;

-- To make an existing column NOT NULL (Oracle/MySQL syntax usually requires MODIFY)
ALTER TABLE Projects
MODIFY Budget DECIMAL(12, 2) NOT NULL;

```


### 1. Dropping an Unnamed Primary Key

Because a table can only ever have one Primary Key, most SQL dialects allow you to drop it without knowing the name:

```sql
ALTER TABLE TableName DROP PRIMARY KEY;

```

*Note: For other unnamed constraints (like `CHECK` or `UNIQUE`), you cannot drop them blindly. You must first query your database's data dictionary (e.g., `information_schema.table_constraints`) to find the system-generated name, and then use `ALTER TABLE ... DROP CONSTRAINT system_name`.*

### 2. Adding Multiple Constraints to One Attribute

You simply list them one after the other, separated by spaces. The order of these specific keywords generally does not matter.

```sql
Email VARCHAR(100) UNIQUE DEFAULT 'no-reply@company.com'

```

### 3. Best Practices for ON DELETE and ON UPDATE

* **`ON UPDATE CASCADE`:** **Best Practice.** If a parent's identifier changes, the child records should automatically update to stay linked. This prevents broken references.
* **`ON DELETE RESTRICT` (or `NO ACTION`):** **Best Practice.** This is the safest approach. It prevents you from accidentally deleting a parent record that child records still rely on, protecting you from unintended data loss.
* **When to use `ON DELETE CASCADE`:** Reserve this strictly for strong "part-of" relationships where the child absolutely cannot exist without the parent (e.g., deleting an `Order` should logically delete all of its `OrderItems`).

Would you like me to show you the specific SQL query used to find those auto-generated constraint names in the `information_schema`?
