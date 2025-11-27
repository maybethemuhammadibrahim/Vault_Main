### **1. Internal nodes**

All internal nodes (nodes with at least one child) are at indexes:

```
0 to n/2 - 1
```

Why?
Because any index `i` where `left(i)` or `right(i)` extends beyond the array is automatically a leaf.

---

### **2. All leaf nodes cluster at the end**

All leaf nodes sit in:

```
n/2 to n - 1
```

This is what makes building a heap bottom-up so efficient—leaves don’t need heapifying.

---

### **3. Parent–child mapping is fixed and O(1)**

No pointers, no extra memory. Just arithmetic.

```
parent(i) = (i - 1) / 2
left(i)   = 2*i + 1
right(i)  = 2*i + 2
```
---

### **4. Height of a heap is always O(log n)**

Binary heap → Complete tree → Minimal height.

This is what guarantees logarithmic performance even in worst case.

---

### **5. Insertion touches at most one path**

When inserting, you add the new element at index `size` and bubble it up.

Worst-case number of swaps:

```
height = log2(n)
```
---

### **6. Deletion (extract-max/extract-min) also touches one path**

You replace the root with the last element and bubble it down.

Again, only one path → O(log n) operations.

---

### **7. Heaps are “complete binary trees”**

This means:

• All levels are fully filled
• Except possibly the last
• And the last is filled from left to right

This gives heaps a compact array representation with no gaps.

---
