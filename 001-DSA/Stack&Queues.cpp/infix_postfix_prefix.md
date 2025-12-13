Here are the merged notes for converting Infix expressions, covering both Postfix and Prefix using the direct scanning methods.

### 1. Infix to Postfix
*The "Left-to-Right" Method*

**Precedence Order:** `^` (High) $\to$ `* /` (Medium) $\to$ `+ -` (Low)

1.  **Scan** the expression from **Left to Right**.
2.  **If Operand (A, B, 1, 2...):** Add directly to the **Answer String**.
3.  **If Left Parenthesis `(`:** Push onto **Stack**.
4.  **If Right Parenthesis `)`:**
    * Pop from Stack and add to Answer until you find a `(`.
    * Discard both parentheses.
5.  **If Operator (`^`, `*`, `/`, `+`, `-`):**
    * Check Stack Top.
    * **Loop:** While Stack is not empty AND Stack Top Precedence $\ge$ Current Precedence:
        * Pop Stack Top to Answer.
        * *(Exception: For Right-Associative operators like `^`, pop only if Top > Current)*.
    * Push the **Current Operator** onto Stack.
6.  **End:** Pop any remaining operators to the Answer.

---

### 2. Infix to Prefix
*The "Right-to-Left" Method*

**Precedence Order:** Same as above.

1.  **Scan** the expression from **Right to Left**.
2.  **If Operand:** Add directly to the **Answer String**.
3.  **If Right Parenthesis `)`:** Push onto **Stack** (Treats it like an "opener").
4.  **If Left Parenthesis `(`:**
    * Pop from Stack and add to Answer until you find a `)`.
    * Discard both parentheses.
5.  **If Operator (`^`, `*`, `/`, `+`, `-`):**
    * Check Stack Top.
    * **Loop:** While Stack is not empty AND Stack Top Precedence $>$ Current Precedence:
        * **Important:** Only pop if Top is **Strictly Greater**. If equal, do *not* pop.
        * Pop Stack Top to Answer.
    * Push the **Current Operator** onto Stack.
6.  **End:** Pop any remaining operators to the Answer.
7.  **Final Step:** **Reverse** the entire Answer String to get the result.

---

### 3. Summary Chart: Difference in Logic

| Feature | Infix $\to$ Postfix | Infix $\to$ Prefix (Direct) |
| :--- | :--- | :--- |
| **Scan Direction** | Left $\to$ Right | Right $\to$ Left |
| **Parenthesis to Push** | `(` | `)` |
| **Parenthesis to Pop** | `)` | `(` |
| **Operator Pop Condition** | Stack Top $\ge$ Current | Stack Top $>$ Current |
| **Associativity Handling**| Pop on Equal (Left-Assoc) | Push on Equal (Right-Assoc logic) |
| **Final Result** | Ready as is | Must **Reverse** string at end |
