### Part 1: Categorization of SQL Commands

SQL commands are grouped by their operational purpose:

* **DDL (Data Definition Language):** Defines or modifies the database structure.
    * `CREATE`: Builds databases and objects (tables, indexes, views, stored procedures, functions, triggers).
    * `ALTER`: Modifies the structure of existing objects (adding/removing columns or constraints).
    * `COMMENT`: Adds descriptive notes to objects in the data dictionary for documentation, without affecting structure.
    * `RENAME`: Changes the name of an existing object.
    * *(Other DDL commands include `DROP` and `TRUNCATE`)*
* **DQL (Data Query Language):** Retrieves data (e.g., `SELECT`).
* **DML (Data Manipulation Language):** Modifies data within tables (e.g., `INSERT`, `UPDATE`, `DELETE`).
* **DCL (Data Control Language):** Manages access and permissions (e.g., `GRANT`, `REVOKE`).
* **TCL (Transaction Control Language):** Manages state and transactions (e.g., `COMMIT`, `ROLLBACK`).

---

### Part 2: Data Types in SQL

When defining columns, you must specify the data type.

| Data Type | Description | Example |
| :--- | :--- | :--- |
| **INT** | Stores whole numbers. | Employee age: `25` |
| **BIGINT** | Stores very large whole numbers. | Population: `9876543210` |
| **DECIMAL / NUMERIC** | Stores fixed-point numbers with exact precision. | Product Price: `345.25` |
| **REAL / FLOAT** | Stores approximate floating-point numbers. | Value of Pi(π): `3.14159` |
| **MONEY / SMALLMONEY**| Stores currency values. | Salary: `7500.75` |
| **DATE** | Stores only date values. | Birthdate: `'20-SEP-1999'` |
| **TIME / DATETIME** | Stores time or both date and time. | Meeting: `'2026-01-25 14:30'` |
| **CHAR(n)** | Stores fixed-length text. | Gender: `'M'` |
| **NCHAR(n)** | Stores fixed-length Unicode text. | Grade symbol: `N'Ü'` |
| **VARCHAR(n)** | Stores variable-length text. | Name: `'Hammad'` |
| **NVARCHAR(n)** | Stores variable-length Unicode text. | Name in Urdu: `N'علی'` |
| **BINARY / VARBINARY(n)**| Stores fixed or variable-length binary data. | File checksum: `0xFA01` |
| **BIT** | Stores boolean values (0 or 1). | IsActive: `1` |

---

### Part 3: The Clean Slate Implementation (DDL - CREATE)

It is best practice to define rules (constraints) precisely during table creation. 

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

*(Note: While `NOT NULL` and `DEFAULT` can be given explicit constraint names in some database systems, standard practice defines them inline.)*

---

### Part 4: Detailed Breakdown of Keywords & Constraints

#### 1. The `CONSTRAINT` Keyword
* **Definition:** Explicitly assigns a name to a column or table rule.
* **Syntax:** `CONSTRAINT constraint_name constraint_type`
* **Purpose:** Prevents the DBMS from generating unreadable system names (e.g., `SYS_C001043`), allowing for easy identification during future modifications (`ALTER` or `DROP`).
* **Naming Convention:** `ConstraintType_TableName_ColumnName` (e.g., `PK_Employees`, `FK_Projects_Employees`).

#### 2. `NOT NULL`
* Ensures a column cannot be left empty. Any `INSERT` or `UPDATE` passing a `NULL` value fails.

#### 3. `DEFAULT <value>`
* Specifies a value inserted automatically if omitted during an `INSERT`.
* *Note:* Not technically a strict relational algebra "constraint," but acts as a rule. Do not use the `CONSTRAINT` keyword for `DEFAULT` in standard SQL.

#### 4. Primary Key (`PRIMARY KEY`)
* Enforces **Entity Integrity** by uniquely identifying each record.
* Implies `NOT NULL` and `UNIQUE`. A table allows only one primary key.

#### 5. Foreign Key and Referential Actions
* Enforces **Referential Integrity** by linking columns between tables.
* **`ON DELETE` / `ON UPDATE` Clauses:** Dictate behavior of child rows when parent rows change.
    * `CASCADE`: Automatically delete/update matching child rows.
    * `SET NULL`: Sets child foreign key to `NULL`.
    * `RESTRICT` (or `NO ACTION`): Prevents parent modification if child rows exist.
    * `SET DEFAULT`: Sets the child row's foreign key to its default value.

#### 6. The `CHECK` Clause
* Enforces **Domain Constraints** by validating values against a Boolean expression.
* Use cases: Value ranges (`Age >= 18`), specific lists (`Status IN ('Active', 'Inactive')`), or string formatting.

---

### Part 5: Modifying Structure Later (DDL - ALTER)

The `ALTER TABLE` command modifies existing table structures without deleting the table.

#### 1. Adding Constraints After Creation
If existing data violates the new constraint, the `ALTER TABLE` execution will fail.

```sql
-- Add a PRIMARY KEY
ALTER TABLE table1 ADD PRIMARY KEY (Column1);

-- Add a UNIQUE constraint
ALTER TABLE table1 ADD CONSTRAINT const_N UNIQUE (Column1, Column2);

-- Add a FOREIGN KEY
ALTER TABLE table1 ADD FOREIGN KEY (Column_Name) REFERENCES table2 (Column_Name);

-- Add a CHECK constraint (Two acceptable syntaxes)
ALTER TABLE table1 ADD CHECK (Column_Condition);
ALTER TABLE table1 ADD CONSTRAINT Constraint_Name CHECK (Condition);
```

#### 2. Modifying Columns and Data Types
Used to change a column's data type, size, or `NULL`/`NOT NULL` properties.

```sql
-- Modify data type and allow NULLs (Syntax varies; Oracle/MySQL shown)
ALTER TABLE Table1 MODIFY Column_Name datatype NULL;

-- Enforce NOT NULL on an existing column
ALTER TABLE Projects MODIFY Budget DECIMAL(12, 2) NOT NULL;

-- Set a DEFAULT value
ALTER TABLE Employees ALTER COLUMN Salary SET DEFAULT 50000;

-- Drop a DEFAULT value
ALTER TABLE Employees ALTER COLUMN Salary DROP DEFAULT;
```

#### 3. Dropping Constraints
Requires the exact constraint name (unless it is a primary key).

```sql
-- Drop a named CHECK constraint
ALTER TABLE Employees DROP CONSTRAINT CHK_EmpSalary;

-- Drop an unnamed PRIMARY KEY
ALTER TABLE TableName DROP PRIMARY KEY;
```
*(Note: To drop unnamed `CHECK` or `UNIQUE` constraints, query the data dictionary (e.g., `information_schema.table_constraints`) to locate the system-generated name first.)*

---

### Part 6: Relational Database Best Practices

#### 1. Applying Multiple Inline Constraints
List them sequentially separated by spaces. Order does not impact execution.
```sql
Email VARCHAR(100) UNIQUE DEFAULT 'no-reply@company.com'
```

#### 2. Managing Referential Actions
* **`ON UPDATE CASCADE`:** **Recommended.** Keeps child records accurately linked if a parent's identifier changes.
* **`ON DELETE RESTRICT` (or `NO ACTION`):** **Recommended.** Prevents accidental data loss by blocking the deletion of parent records tied to child records.
* **`ON DELETE CASCADE`:** Use strictly for "part-of" relationships where child records have no logical existence without the parent (e.g., `Orders` and `OrderItems`).
