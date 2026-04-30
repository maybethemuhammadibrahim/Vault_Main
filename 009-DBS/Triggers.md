## Database Triggers: Comprehensive Notes

### 1. Introduction to Database Triggers
**Formal Definition:** Triggers are named PL/SQL blocks stored directly within the database that execute (fire) implicitly in response to specific triggering events. 

**Intuitive Explanation:** Think of a trigger as an automated security system or a mousetrap. You do not manually execute a trigger the way you run a standard script or stored procedure. Instead, you "arm" it by attaching it to a specific database event. When that event happens (someone opens a door, or someone inserts data), the trigger fires automatically. They are used to enforce business rules, audit changes, or synchronize data without requiring the user to explicitly call them.

### 2. Triggering Events
Triggers can be attached to four primary categories of events:

1.  **DML Events (Data Manipulation Language):** Fired by `INSERT`, `UPDATE`, or `DELETE` statements on a specific table. They can trigger `BEFORE` or `AFTER` the action occurs.
2.  **DDL Events (Data Definition Language):** Fired by `CREATE`, `ALTER`, or `DROP` statements. These are typically used by administrators to audit structural changes to the database (e.g., tracking who dropped a table).
3.  **System Events:** Fired by the database system itself, such as database startup or shutdown.
4.  **User Events:** Fired by user connection activities, such as `LOGON` or `LOGOFF`.

### 3. General Trigger Syntax
```sql
CREATE [OR REPLACE] TRIGGER trigger_name 
{BEFORE | AFTER} triggering_event ON table_name
[FOR EACH ROW] 
[FOLLOWS another_trigger_name]
[ENABLE | DISABLE] 
[WHEN condition]
DECLARE
    -- Variable declarations
BEGIN
    -- Executable PL/SQL statements
EXCEPTION
    -- Exception handling
END;
/
```

---

### 4. DML Triggers (Data Manipulation Language)
**Formal Definition:** Specialized stored programs that execute implicitly when an `INSERT`, `UPDATE`, or `DELETE` statement is issued against an associated table.

**Intuitive Explanation:** DML triggers are the traffic cops for your table's data. They intercept data trying to enter, modify, or leave a table. Because they can act `BEFORE` a change happens, they can inspect the incoming data and alter it, reject it, or log it. 

#### Pseudo Records: `:NEW` and `:OLD`
When a DML trigger operates on a row-by-row basis (`FOR EACH ROW`), it gains access to the data being modified via pseudo records.
*   **:OLD** represents the existing row data *before* the modification.
*   **:NEW** represents the incoming row data *after* the modification.

| Action | `:OLD` Content | `:NEW` Content |
| :--- | :--- | :--- |
| **INSERT** | Empty (NULL) | Contains the incoming values being inserted. |
| **UPDATE** | Contains the existing values before the update. | Contains the incoming new values. |
| **DELETE** | Contains the existing values being deleted. | Empty (NULL) |

*(To fetch a specific column value, use dot notation: `:NEW.column_name` or `:OLD.column_name`)*

#### DML Trigger Examples (Intermediate Level)
*Assume an existing schema with a table named `employees (emp_id NUMBER, emp_name VARCHAR2(30), salary NUMBER)`.*

**Example 1: Before Insert Trigger**
Automatically ensures that all employee names are stored in uppercase before they hit the database table.
```sql
CREATE OR REPLACE TRIGGER trg_bi_employees
BEFORE INSERT ON employees
FOR EACH ROW
BEGIN
    :NEW.emp_name := UPPER(:NEW.emp_name);
END;
/
```

**Example 2: Before Update Trigger**
Prevents users from decreasing an employee's salary.
```sql
CREATE OR REPLACE TRIGGER trg_bu_employees
BEFORE UPDATE ON employees
FOR EACH ROW
BEGIN
    IF :NEW.salary < :OLD.salary THEN
        RAISE_APPLICATION_ERROR(-20001, 'Salary cannot be decreased.');
    END IF;
END;
/
```

**Example 3: Before Delete Trigger**
Prevents deletion of a specific critical record (e.g., the CEO).
```sql
CREATE OR REPLACE TRIGGER trg_bd_employees
BEFORE DELETE ON employees
FOR EACH ROW
BEGIN
    IF :OLD.emp_name = 'CEO' THEN
        RAISE_APPLICATION_ERROR(-20002, 'The CEO record cannot be deleted.');
    END IF;
END;
/
```

**Example 4: Combined DML Trigger (All-in-One)**
Using conditional logic to handle all three DML events in a single trigger block.
```sql
CREATE OR REPLACE TRIGGER trg_dml_employees
BEFORE INSERT OR UPDATE OR DELETE ON employees
FOR EACH ROW
BEGIN
    IF INSERTING THEN
        DBMS_OUTPUT.PUT_LINE('New employee record created.');
    ELSIF UPDATING THEN
        DBMS_OUTPUT.PUT_LINE('Existing employee record modified.');
    ELSIF DELETING THEN
        DBMS_OUTPUT.PUT_LINE('Employee record removed.');
    END IF;
END;
/
```

---

### 5. Practical DML Application: Table Auditing
**Formal Definition:** The systematic recording of DML activities performed on a specific database table to track data provenance and user interactions.

**Intuitive Explanation:** This is the equivalent of a flight data recorder for a specific table. If a value changes unexpectedly, the audit table tells you exactly who changed it, what the old value was, what the new value is, and exactly when it happened.

**Setup the Audit Log Table:**
```sql
CREATE TABLE emp_audit (
    new_name VARCHAR2(30),
    old_name VARCHAR2(30),
    user_name VARCHAR2(30),
    entry_date DATE,
    operation VARCHAR2(30)
);
```

**The Auditing Trigger:**
```sql
CREATE OR REPLACE TRIGGER emp_audit_trg
BEFORE INSERT OR DELETE OR UPDATE ON employees
FOR EACH ROW
DECLARE
    v_user VARCHAR2(30);
    v_date DATE;
BEGIN
    -- Fetch session user and current time
    SELECT user, sysdate INTO v_user, v_date FROM dual;

    IF INSERTING THEN
        INSERT INTO emp_audit (new_name, old_name, user_name, entry_date, operation)
        VALUES (:NEW.emp_name, NULL, v_user, v_date, 'Insert');
        
    ELSIF DELETING THEN
        INSERT INTO emp_audit (new_name, old_name, user_name, entry_date, operation)
        VALUES (NULL, :OLD.emp_name, v_user, v_date, 'Delete');
        
    ELSIF UPDATING THEN
        INSERT INTO emp_audit (new_name, old_name, user_name, entry_date, operation)
        VALUES (:NEW.emp_name, :OLD.emp_name, v_user, v_date, 'Update');
    END IF;
END;
/
```

---

### 6. Practical DML Application: Synchronized Backup
**Formal Definition:** Utilizing triggers to synchronously replicate DML transactions from a primary operational table to a secondary identical table in real-time.

**Intuitive Explanation:** A live mirror. As soon as you add, change, or remove a record in the main table, the trigger immediately performs the exact same action on a backup table.

**Setup Backup Table:**
```sql
-- Creates an empty clone of the employees table structure
CREATE TABLE employees_backup AS SELECT * FROM employees WHERE 1=2;
```

**The Synchronization Trigger:**
```sql
CREATE OR REPLACE TRIGGER emp_sync_trg 
BEFORE INSERT OR DELETE OR UPDATE ON employees
FOR EACH ROW
BEGIN
    IF INSERTING THEN
        INSERT INTO employees_backup (emp_id, emp_name, salary) 
        VALUES (:NEW.emp_id, :NEW.emp_name, :NEW.salary);
        
    ELSIF DELETING THEN
        DELETE FROM employees_backup WHERE emp_id = :OLD.emp_id;
        
    ELSIF UPDATING THEN
        UPDATE employees_backup 
        SET emp_name = :NEW.emp_name, salary = :NEW.salary
        WHERE emp_id = :OLD.emp_id;
    END IF;
END;
/
```

---

### 7. DDL Triggers (Data Definition Language)
**Formal Definition:** Triggers that execute implicitly when structural changes (like `CREATE`, `DROP`, `ALTER`) are made to database objects.

**Intuitive Explanation:** While DML triggers watch the *data* inside the house, DDL triggers watch the *walls* of the house. They monitor whether users are building new tables, dropping existing ones, or altering column definitions. 

#### Schema vs. Database Auditing
You can monitor at two different scopes:
*   **Schema Level:** Monitors only the objects owned by the current user.
*   **Database Level:** Monitors all objects across the entire database (requires administrative privileges).

**Setup DDL Audit Table:**
```sql
CREATE TABLE schema_audit (
    ddl_date DATE,
    ddl_user VARCHAR2(30),
    object_created VARCHAR2(30),
    object_name VARCHAR2(30),
    ddl_operation VARCHAR2(30)
);
```

**Schema Auditing Trigger:**
```sql
CREATE OR REPLACE TRIGGER schema_audit_trg
AFTER DDL ON SCHEMA
BEGIN
    INSERT INTO schema_audit VALUES (
        SYSDATE,
        SYS_CONTEXT('USERENV', 'CURRENT_USER'),
        ORA_DICT_OBJ_TYPE, 
        ORA_DICT_OBJ_NAME,
        ORA_SYSEVENT
    );
END;
/
```

**Database Auditing Trigger:**
*(Must be logged in as an administrator like SYS or SYSTEM)*
```sql
CREATE OR REPLACE TRIGGER db_audit_trg
AFTER DDL ON DATABASE
BEGIN
    INSERT INTO schema_audit VALUES (
        SYSDATE,
        SYS_CONTEXT('USERENV', 'CURRENT_USER'),
        ORA_DICT_OBJ_TYPE, 
        ORA_DICT_OBJ_NAME, 
        ORA_SYSEVENT
    );
END;
/
```

---

### Schema vs. Database Auditing
*   **Schema Auditing:** Monitors objects owned by the current user.
*   **Database Auditing:** Monitors all objects across the database (requires `ADMINISTER DATABASE TRIGGER` privilege).

*(For Database Auditing, replace `ON SCHEMA` with `ON DATABASE` and execute as a privileged user).*

---

## 4. System / Database Event Triggers
**Definition:** PL/SQL blocks that execute implicitly when specific database or schema-level system events occur.

### LOGON and LOGOFF Triggers
*Tracks user connection activity. Can be Schema-level or Database-level.*

**Schema Level LOGON:**
```sql
CREATE OR REPLACE TRIGGER hr_lgon_audit
AFTER LOGON ON SCHEMA
BEGIN
    INSERT INTO hr_evnt_audit VALUES(
        ora_sysevent, SYSDATE, TO_CHAR(sysdate, 'hh24:mi:ss'), NULL, NULL
    );
    COMMIT;
END;
/
```

**Database Level LOGOFF:**
```sql
CREATE OR REPLACE TRIGGER db_lgof_audit
BEFORE LOGOFF ON DATABASE
BEGIN
    INSERT INTO db_evnt_audit VALUES(
        USER, ora_sysevent, NULL, NULL, SYSDATE, TO_CHAR(sysdate, 'hh24:mi:ss')
    );
END;
/
```

### STARTUP and SHUTDOWN Triggers
*Tracks the physical state of the database server. Always Database-level. Requires DBA privileges.*

**STARTUP Trigger:**
```sql
CREATE OR REPLACE TRIGGER startup_audit
AFTER STARTUP ON DATABASE
BEGIN
    INSERT INTO startup_audit VALUES(
        ora_sysevent, SYSDATE, TO_CHAR(sysdate, 'hh24:mm:ss')
    );
END;
/
```

---

## 5. INSTEAD-OF Triggers
**Definition:** Specialized triggers created exclusively on views that intercept standard DML statements and execute custom PL/SQL logic *instead of* the original statement.

**Purpose:** Required to perform `INSERT`, `UPDATE`, or `DELETE` operations on non-modifiable views (views containing joins, aggregate functions, `GROUP BY`, etc.).

**Setup:**
```sql
CREATE TABLE trainer (full_name VARCHAR2(20));
CREATE TABLE subject (subject_name VARCHAR2(15));
CREATE VIEW db_lab_09 AS SELECT full_name, subject_name FROM trainer, subject;
```

**INSTEAD-OF INSERT Trigger:**
```sql
CREATE OR REPLACE TRIGGER tr_Io_Insert
INSTEAD OF INSERT ON db_lab_09 FOR EACH ROW
BEGIN
    INSERT INTO trainer (full_name) VALUES (:new.full_name);
    INSERT INTO subject (subject_name) VALUES (:new.subject_name);
END;
/
```

**INSTEAD-OF UPDATE Trigger:**
```sql
CREATE OR REPLACE TRIGGER io_update
INSTEAD OF UPDATE ON db_lab_09 FOR EACH ROW
BEGIN
    UPDATE trainer SET full_name = :new.full_name WHERE full_name = :old.full_name;
    UPDATE subject SET subject_name = :new.subject_name WHERE subject_name = :old.subject_name;
END;
/
```

---

## 6. Compound Triggers
**Definition:** A multi-tasking trigger that encapsulates multiple timing point blocks (Before Statement, Before Row, After Row, After Statement) within one unified execution context. This allows sharing state and variables across different execution phases without requiring multiple separate triggers.

---

## 7. Frequently Asked Questions (Q&A)

**Q: In the DML auditing trigger, what does `SELECT user, sysdate INTO...` do, and what is `dual`?**
*   **`user`**: A built-in Oracle pseudo-column returning the database username of the current session.
*   **`INTO`**: PL/SQL syntax used to take the output of a `SELECT` query and store it into variables (`v_user`, `v_date`) for later use in the code. It does not insert data into a table.
*   **`dual`**: A built-in dummy table with one row and one column. It is required because Oracle mandates a `FROM` clause in all `SELECT` queries, even when just fetching system functions.

**Q: When setting up the backup table, what does `WHERE 1=2` mean?**
*   It is a deliberate false condition. Because 1 never equals 2, the query evaluates to false for all rows. The result is that Oracle creates an exact structural clone of the target table (columns and data types) but copies zero rows of data.

**Q: In the DDL trigger, what is `SYS_CONTEXT` and the other `ORA_` variables?**
*   **`SYS_CONTEXT('USERENV', 'CURRENT_USER')`**: A function fetching current session data. `'USERENV'` specifies the built-in environment namespace, and `'CURRENT_USER'` requests the active username.
*   **`ORA_DICT_OBJ_TYPE`**: Returns the type of object altered (e.g., TABLE, VIEW).
*   **`ORA_DICT_OBJ_NAME`**: Returns the name of the altered object.
*   **`ORA_SYSEVENT`**: Returns the DDL action that fired the trigger (e.g., CREATE, DROP).

**Q: What is the difference between DDL ON SCHEMA and DDL ON DATABASE?**
*   **Schema:** Monitors structural changes only within a specific user's environment. Safer for developers.
*   **Database:** Monitors structural changes globally across the entire server. Requires DBA privileges.

**Q: What is the difference between LOGON/LOGOFF and STARTUP/SHUTDOWN events?**
*   **LOGON/LOGOFF:** Tracks user connection activity. Can be scoped to a specific user (`ON SCHEMA`) or all users globally (`ON DATABASE`).
*   **STARTUP/SHUTDOWN:** Tracks the physical power state of the Oracle database service itself. This can only be applied globally (`ON DATABASE`) and requires DBA privileges.
