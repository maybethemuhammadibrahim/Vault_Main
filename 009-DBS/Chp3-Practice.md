Here are the notes with SQL queries added after each question:

---

**How SQL Processes Implicit Joins (The Nested Loop)**

Conceptually, when you comma-separate tables in the FROM clause and link them in the WHERE clause, SQL executes a "Nested Loop Join":
- Outer Loop: SQL grabs the very first row from Table A.
- Inner Loop: It scans through every single row in Table B.
- Evaluate: For each Table B row, it checks if your WHERE A.id = B.id condition is TRUE.
- Combine: If TRUE, it stitches those two rows together and sends them to the result set.
- Repeat: It moves to the second row of Table A and does it all again.

---

**Practice Questions by Join Type**

Schema Reminder:
- Employee(Fname, Lname, Ssn, Salary, Super_ssn, Dno)
- Department(Dname, Dnumber, Mgr_ssn)
- Project(Pname, Pnumber, Plocation, Dnum)
- Works_On(Essn, Pno, Hours)

---

**1. The Two-Table Join (Direct Links)**

**Q1:** List the names of employees who work exactly 20 hours on project number 10.

```sql
SELECT E.Fname, E.Lname
FROM Employee E, Works_On W
WHERE E.Ssn = W.Essn
  AND W.Pno = 10
  AND W.Hours = 20;
```

*How it works:* SQL loops through Employee, finds a row, then scans Works_On. If the SSNs match AND the project is 10 AND hours are 20, it outputs the name.

---

**Q2:** Find the names of projects and their controlling department names for projects located in 'Stafford'.

```sql
SELECT P.Pname, D.Dname
FROM Project P, Department D
WHERE P.Dnum = D.Dnumber
  AND P.Plocation = 'Stafford';
```

*How it works:* SQL takes a 'Stafford' project, scans the Department table to find the matching Dnumber, and pairs them up.

---

**Q3:** Retrieve the first names of employees and the name of the department they work for, only for employees earning more than $60,000.

```sql
SELECT E.Fname, D.Dname
FROM Employee E, Department D
WHERE E.Dno = D.Dnumber
  AND E.Salary > 60000;
```

*How it works:* SQL filters Employee for high earners first, then nested-loops into Department to find the matching department name.

---

**2. The Chain of Joins (3+ Tables)**

**Q1:** List the project names that have an employee named 'John Smith' working on them.

```sql
SELECT P.Pname
FROM Employee E, Works_On W, Project P
WHERE E.Ssn = W.Essn
  AND W.Pno = P.Pnumber
  AND E.Fname = 'John'
  AND E.Lname = 'Smith';
```

*How it works:* SQL finds 'John Smith', uses his SSN to find his project numbers in Works_On, and then uses those numbers to look up the actual project names in Project.

---

**Q2:** Find the names of employees who work on a project controlled by the 'Administration' department.

```sql
SELECT DISTINCT E.Fname, E.Lname
FROM Employee E, Works_On W, Project P, Department D
WHERE E.Ssn = W.Essn
  AND W.Pno = P.Pnumber
  AND P.Dnum = D.Dnumber
  AND D.Dname = 'Administration';
```

*How it works:* SQL creates a massive 4-table chain. It evaluates if an employee's work log connects to a project that connects to a department named 'Administration'.

---

**Q3:** Retrieve the total hours logged by all employees on projects controlled by the department managed by employee SSN '888665555'.

```sql
SELECT SUM(W.Hours)
FROM Works_On W, Project P, Department D
WHERE W.Pno = P.Pnumber
  AND P.Dnum = D.Dnumber
  AND D.Mgr_ssn = '888665555';
```

*How it works:* SQL finds the department managed by '888665555', finds all projects under that department, and then pulls every hour log associated with those projects.

---

**3. The Advanced Self-Join (Table to Itself)**

**Q1:** Find the names of employees who work in the exact same department as their supervisor.

```sql
SELECT E.Fname, E.Lname
FROM Employee E, Employee S
WHERE E.Super_ssn = S.Ssn
  AND E.Dno = S.Dno;
```

*How it works:* SQL treats E and S as two tables. It matches an employee to their supervisor, then checks if their department numbers (Dno) are identical.

---

**Q2:** Find all pairs of employees (first names only) who work in the same department, but ensure a person is not paired with themselves.

```sql
SELECT E1.Fname, E2.Fname
FROM Employee E1, Employee E2
WHERE E1.Dno = E2.Dno
  AND E1.Ssn < E2.Ssn;
```

*How it works:* This creates a combination of all employees. E1.Dno = E2.Dno keeps only those in the same department. E1.Ssn != E2.Ssn prevents "John paired with John". (Note: E1.Ssn < E2.Ssn is often used instead to prevent duplicate reversed pairs like John-Jane and Jane-John).

---

**Q3:** List the names of supervisors who manage an employee earning more than they do.

```sql
SELECT DISTINCT S.Fname, S.Lname
FROM Employee E, Employee S
WHERE E.Super_ssn = S.Ssn
  AND E.Salary > S.Salary;
```

*How it works:* SQL links the worker (E) to the boss (S), then outputs the boss's name if the worker's salary is strictly greater.

---

**4. Multi-Path / Set Operations (UNION, INTERSECT, EXCEPT)**

**Q1:** List the SSNs of employees who either work on project 10 OR manage department 5.

```sql
SELECT Essn FROM Works_On WHERE Pno = 10
UNION
SELECT Mgr_ssn FROM Department WHERE Dnumber = 5;
```

*How it works:* SQL runs the top query, gets a list of SSNs. Runs the bottom query, gets a list of SSNs. UNION stacks them together and deletes any duplicates.

---

**Q2:** Find the SSNs of employees who work on BOTH project 10 AND project 20.

```sql
SELECT Essn FROM Works_On WHERE Pno = 10
INTERSECT
SELECT Essn FROM Works_On WHERE Pno = 20;
```

*How it works:* SQL generates two separate lists of SSNs. INTERSECT looks at both lists and only outputs the SSNs that exist in both of them.

---

**Q3:** List the SSNs of employees who manage a department but do NOT work on any projects.

```sql
SELECT Mgr_ssn FROM Department
EXCEPT
SELECT Essn FROM Works_On;
```

*How it works:* SQL takes the list of all manager SSNs. Then, it subtracts any SSN that appears in the Works_On table. The remainder is managers who have no project hours logged.

---

Would you like to move on to aggregate functions with GROUP BY and HAVING?
