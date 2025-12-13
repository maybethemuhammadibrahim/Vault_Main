### 1. Key Definitions

## 1. Performance Metrics
These measure the efficiency of an algorithm.

* **Time Complexity:** How the runtime grows as the input size ($n$) increases. You must know:
    * **Best Case:** The algorithm's behavior on ideal input (e.g., already sorted).
    * **Average Case:** The behavior on random input.
    * **Worst Case:** The behavior on the most difficult input (e.g., reverse sorted).
    * *Common notation:* $O(n^2)$ (Quadratic), $O(n \log n)$ (Linearithmic), $O(n)$ (Linear).
* **Space Complexity (Auxiliary Space):** The amount of *extra* memory required by the algorithm, excluding the input itself.

## 2. Algorithmic Properties
These describe how the algorithm behaves regarding the data's arrangement.

* **Stability:** A stable sort maintains the relative order of records with equal keys.
    * *Example:* If two items are "5" (5a and 5b), a stable sort ensures 5a comes before 5b in the result.
* **In-Place:** An algorithm that transforms the input using no auxiliary data structure (requires only small, constant $O(1)$ extra memory).
* **Adaptive:** An algorithm that runs faster if the input is already partially sorted (e.g., Insertion Sort).
* **Online:** The algorithm can sort a list as it receives it, piece-by-piece, rather than waiting for the entire list initially.

## 3. Sorting Mechanics
Terms related to *how* the sorting is actually performed.

* **Comparison-Based:** Sorts by comparing elements using operators ($<, >, =$).
    * *Limit:* Cannot be faster than $O(n \log n)$ on average.
    * *Examples:* Quick Sort, Merge Sort, Heap Sort.
* **Non-Comparison Based (Integer Sorting):** Sorts by using the binary representation or properties of the numbers.
    * *Benefit:* Can achieve $O(n)$ or $O(nk)$.
    * *Examples:* Radix Sort, Counting Sort.
* **Divide and Conquer:** A strategy where the problem is broken into smaller sub-problems, solved recursively, and then combined (e.g., Merge Sort).
* **Partitioning:** The process of rearranging elements around a "pivot" value (central to Quick Sort).
* **Swapping:** The fundamental operation of exchanging two elements in memory.
---

### 2. Algorithm Comparison Chart

**TC** = Time Complexity, **Comp** = Comparison Based, **D&C** = Divide & Conquer

| Algorithm | Best TC | Avg TC | Worst TC | Stable? | In-Place? | Adaptive? | Comp? | D&C? | Pivot? | Swaps? |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Bubble** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | **Yes** | **Yes** | **Yes** | **Yes** | No | No | **Yes** |
| **Selection** | $O(n^2)$ | $O(n^2)$ | $O(n^2)$ | No | **Yes** | No | **Yes** | No | No | **Yes** |
| **Insertion** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | **Yes** | **Yes** | **Yes** | **Yes** | No | No | No (Shifts) |
| **Shell** | $O(n \log n)$ | Gap Dep.* | $O(n^2)$ | No | **Yes** | **Yes** | **Yes** | No | No | **Yes** |
| **Comb** | $O(n \log n)$ | $O(n^2)$ | $O(n^2)$ | No | **Yes** | **Yes** | **Yes** | No | No | **Yes** |
| **Heap** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | No | **Yes** | No | **Yes** | No | No | **Yes** |
| **Merge** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | **Yes** | No | No | **Yes** | **Yes** | No | No |
| **Quick** | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$ | No | **Yes** | No | **Yes** | **Yes** | **Yes** | **Yes** |
| **Radix** | $O(nk)$ | $O(nk)$ | $O(nk)$ | **Yes** | No | No | No | No | No | No |

**Notes for Clarity:**
1.  **Selection Sort Stability:** It is **No** because swapping a long-distance element might jump over an equal element.
2.  **Insertion Sort Swapping:** It typically uses **Shifting** (copying values over) rather than direct swapping, which is more efficient.
3.  **Shell Sort Time Complexity:** Depends heavily on the "Gap Sequence" used.
4.  **Quick Sort In-Place:** Marked **Yes** as it sorts the array itself, but it technically uses $O(\log n)$ stack space for recursion.
5.  **Radix Sort TC:** $k$ is the number of digits/bits in the largest number.

