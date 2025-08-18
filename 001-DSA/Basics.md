### **1. The Stack (Programming)**
- A special memory region storing **activation records** (stack frames) for functions.  
- **Primary use**: Manages function calls, local variables, return addresses, and control flow.  
- **LIFO Principle**: Last-In, First-Out (most recent call finishes first).  
- **Stack Frame Contents**:  
  - Local variables (automatic storage).  
  - Function parameters.  
  - Return address (execution resume point).  
- **Managed automatically** by compiler/OS (transparent push/pop on call/return).  
- **Static allocation only** (fixed-size, no dynamic memory).  
- **One-liner**:  
  *A LIFO memory structure storing function call frames (locals, params, return addresses), managed automatically at runtime.*  

---

### **2. The Heap (Programming)**
- Memory region for **dynamic allocation** (blocks allocated/freed at runtime).  
- **No LIFO order** (unlike stack).  
- **Management**:  
  - Manual: `new/delete` (C++), `malloc/free` (C).  
  - Automatic: Garbage collection (Java/Python).  
- **Persistent memory**: Lives until explicitly freed/garbage-collected.  
- **Use cases**: Variable-sized/long-lived data (objects, trees, linked lists).  
- **Slower than stack** (fragmentation risk).  
- **One-liner**:  
  *A runtime-managed memory region for dynamic, variable-sized allocations (data surviving function calls).*  

---

### **3. Physical Data Structures**  
- **Focus**: How data is *physically stored* in memory (implementation-level).  
- **Examples**:  
  - **Arrays**: Contiguous memory blocks.  
  - **Linked Lists**: Pointer-connected nodes.  
- **Role**: Foundation for logical structures.  

---

### **4. Logical Data Structures**  
- **Focus**: Abstract data organization (independent of memory layout).  
- **Examples**:  
  - **Linear**: Stack, Queue, Deque (built atop arrays/linked lists).  
  - **Hierarchical**: Trees, Graphs.  
  - **Tabular**: Hash Tables.  
- **Emphasizes** operations (e.g., `push`, `enqueue`) and data relationships.  

---

### **5. Abstract Data Types (ADTs)**  
- **Definition**: Mathematical model defining *operations* while hiding *implementation*.  
- **Purpose**: Encapsulates behavior (interface vs. implementation).  
- **Examples**:  
  - **Stack ADT**: `push()`, `pop()`, `peek()`.  
  - **Queue ADT**: `enqueue()`, `dequeue()`.  
- **One-liner**:  
  *A conceptual data model with defined operations, hiding implementation details.*  

---

### **6. Time Complexity (Big-O)**  
- Measures algorithmic runtime growth relative to input size (`n`).  
- **Big-O Notation**: Describes worst-case growth (ignores constants).  
- **Common Notations**:  
  - **O(1)**: Constant time (array index access).  
  - **O(log n)**: Logarithmic (binary search).  
  - **O(n)**: Linear (array traversal).  
  - **O(n²)**: Quadratic (nested loops).  
  - **O(2ⁿ)**: Exponential (recursive Fibonacci).  
- **Beginner Tip**:  
  *Count loops: single → O(n), nested → O(n²), input halving → O(log n).*  

---

### **Explicit Changes Made**:  
1. **Headings standardized** (e.g., "Stack" → "The Stack (Programming)").  
2. **Minor formatting tweaks**:  
   - Consistent bullet-point spacing.  
   - Bolded key terms (e.g., **LIFO**).  
3. **Added sub-bullets** for clarity (e.g., under "Stack Frame Contents").  
4. **Kept all original content**—no substantive additions/removals.  

Let me know if you'd like further refinements!
