Here is your quick-reference guide for all algorithms, simplified for exam use.

### **1. Union ()**

* **The Algorithm:** Make pairs . Start at . Follow transitions for both simultaneously.
* **Rule for Final:** The pair is Final if **EITHER**  OR  is Final.
* **Edge Case:** If one machine hits a Dead State, **keep going**. The other machine might still accept the string.
* **Multiple Finals:** If  has finals , any pair containing  *or*  counts towards the "OR" condition.

### **2. Intersection ()**

* **The Algorithm:** Same table as Union. Pairs .
* **Rule for Final:** The pair is Final **ONLY IF**  AND  are **BOTH** Final.
* **Edge Case:** If **any** machine hits a Dead State, that path is dead forever. You can stop tracing it.

---

### **3. Concatenation ()**

* **The Algorithm:** Start with . Fill the table normally.
* **The "Magic" Rule:** Whenever a **Final State of FA1** appears in your set, immediately **ADD** the **Start State of FA2** to that set.
* *Visual:* .


* **Rule for Final:** The set is Final only if it contains a **Final State of FA2**. (FA1 finals don't count anymore).
* **Edge Case (Loops):** If FA1 loops on a final state, you keep calculating FA1's next move *AND* add FA2's start state again. You never "leave" FA1 until it naturally dies.
* **Multiple Finals:** If FA1 has finals  and , *either one* appearing triggers the "Add Start2" rule.

### **4. Kleene Star ()**

* **The Algorithm:** Create a **New Start State** (call it ).  behaves exactly like the Old Start, but is also Final.
* **The "Magic" Rule:** Whenever an **Old Final State** appears in your set, immediately **ADD** the **Old Start State** to that set.
* **Rule for Final:** Any set containing an **Old Final State** (or the New Start ) is Final.
* **Edge Case:**  must be final to accept the empty string ().

---

### **5. Reversing an Automata ()**

* **The Algorithm:**
1. **Flip Arrows:** Reverse the direction of every single arrow.
2. **Swap Roles:** Make the **Old Start** the **New Final**. Make the **Old Finals** the **New Starts**.


* **Multiple Finals Edge Case (Crucial):**
* You cannot have multiple Start states.
* **The Fix:** Create a **New Dummy Start State**. Add -transitions (empty jumps) from this Dummy Start to **all** the Old Final states.


* **Result:** This almost always creates an **NFA** (Non-Deterministic). You usually have to convert it back to a DFA using a table.
