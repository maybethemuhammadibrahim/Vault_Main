Here is the comparison table for **Binary Search Trees (Standard)** vs. **AVL Trees**.

### Time Complexity & Rotation Cost Chart

| Operation | BST Rotation Cost | AVL Rotation Cost | BST Total Time (Avg) | BST Total Time (Worst) | AVL Total Time (Avg & Worst) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Search** | N/A | N/A | $O(\log n)$ | $O(n)$ | $O(\log n)$ |
| **Insertion**| N/A | $O(1)$ | $O(\log n)$ | $O(n)$ | $O(\log n)$ |
| **Deletion** | N/A | $O(\log n)$ | $O(\log n)$ | $O(n)$ | $O(\log n)$ |

### Key Takeaways
1.  **BST Rotations:** Standard BSTs do **not** perform rotations. This is why they can become skewed (worst case $O(n)$).
2.  **AVL Insertion Rotations:** Requires at most **1 re-balancing** (single or double rotation), so the rotation cost is constant $O(1)$.
3.  **AVL Deletion Rotations:** Re-balancing a node might cause its parent to become unbalanced. This can propagate all the way up the tree, requiring up to $O(\log n)$ rotations.
4.  **Total Time:** In AVL trees, the bottleneck is always the traversal (finding the node), which takes $O(\log n)$. Even if deletion does $O(\log n)$ rotations, the total complexity remains $O(\log n) + O(\log n) = O(\log n)$.
