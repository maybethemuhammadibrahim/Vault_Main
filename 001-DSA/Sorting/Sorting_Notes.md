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
Here are one-liner explanations for the stability of each algorithm:

* **Bubble Sort (Stable):** It only swaps adjacent elements if they are strictly greater, so equal elements are never reordered.
* **Selection Sort (Unstable):** Long-distance swaps can force an element to "jump" over an identical value later in the list (e.g., swapping a `5` at the start with a `2` found after another `5`).
* **Insertion Sort (Stable):** New elements are inserted *behind* any pre-existing equal values during the shift process, preserving the original order.
* **Shell Sort (Unstable):** Elements are sorted in independent "gap" groups, allowing identical values to cross each other depending on which gap group they fall into.
* **Comb Sort (Unstable):** Like Shell Sort, the large "gap" comparisons allow elements to jump over identical values, disrupting the relative order.
* **Heap Sort (Unstable):** The process of building the heap structure rearranges elements based on tree-position rather than index, ignoring original order.
* **Merge Sort (Stable):** During the merge phase, if two values are equal, the code explicitly picks the element from the "left" (earlier) sub-array first.
* **Quick Sort (Unstable):** The partitioning step swaps elements across the pivot from long distances, frequently jumping one equal value over another.
* **Radix Sort (Stable):** It processes numbers digit-by-digit (LSD to MSD) using a stable sub-routine (like Counting Sort), which preserves the sorting work done on previous digits.

