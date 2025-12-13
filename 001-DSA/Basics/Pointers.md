
## 1. What is a Pointer?

* A **pointer** is a variable that **stores the memory address** of another variable.
* Instead of holding a value directly, it holds the location of the value in memory.
* Syntax:

  ```cpp
  int *p;   // pointer to an integer
  char *c;  // pointer to a character
  ```

### Example:

```cpp
int x = 10;
int *ptr = &x;   // ptr stores the address of x
```

---

## 2. Meaning of `*` and `&` in C/C++

* `&` → **Address-of operator**

  * Used to get the memory address of a variable.

  ```cpp
  int x = 5;
  cout << &x;  // prints the address of x
  ```

* `*` → **Dereference (indirection) operator**

  * Used to access the value stored at the address the pointer holds.

  ```cpp
  int x = 5;
  int *p = &x;     
  cout << *p;  // prints 5 (value at address stored in p)
  ```

📌 **Mnemonic:**

* `&` → "Where is it?" (address)
* `*` → "What is inside?" (value at that address)

---

## 3. Why Different Pointer Types Exist (`int*`, `char*`, etc.)

* Each pointer type tells the compiler **how many bytes** to read from memory when dereferencing.

  * `int*` → reads `sizeof(int)` bytes
  * `char*` → reads `sizeof(char)` bytes (usually 1)
  * `double*` → reads `sizeof(double)` bytes

### Example:

```cpp
int a = 100;
int *pi = &a;
char *pc = (char*)&a;  // cast int* to char*

cout << *pi;  // prints 100 (interprets memory as int)
cout << (int)*pc; // prints lower byte of 100 (implementation-dependent)
```

💡 **Key Point:**

* `char*` can technically hold the address of any type (because memory is byte-addressable).
* But dereferencing without a proper cast leads to **wrong interpretation** of data.

---

## 4. Void Pointers (`void*`)

* A **generic pointer type** that can point to any data type.
* Cannot be directly dereferenced (since the type is unknown).
* Must be **cast** to the correct type before dereferencing.

### Example:

```cpp
int x = 42;
void *vp = &x;          // void pointer
cout << *(int*)vp;      // cast back to int* before dereferencing
```

💡 Common in:

* Memory allocation functions (`malloc` in C).
* Generic functions that work with multiple data types.

---

## 5. Pointer to Pointer (`**`)

* A pointer can store the address of another pointer → known as a **pointer to pointer**.
* Used in cases like **2D arrays**, **command-line arguments**, or **dynamic memory allocation of arrays of pointers**.

### Example:

```cpp
int x = 10;
int *p = &x;      // pointer to int
int **pp = &p;    // pointer to pointer

cout << **pp;  // prints 10
```

### Memory Block Diagram

```
   x = 10
   +-------+
   |   10  |   <-- variable x
   +-------+
       ^
       |
   +-------+
   |  &x   |   <-- pointer p stores address of x
   +-------+
       ^
       |
   +-------+
   |  &p   |   <-- pointer pp stores address of p
   +-------+
```

---

## 6. Arrays as Pointers

* When passing an array to a function, **only the address of the 0th element** is passed (not the entire array).
* Hence, arrays "decay" into pointers.

### Example:

```cpp
void printArr(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";   // same as *(arr + i)
    }
}

int main() {
    int nums[5] = {1, 2, 3, 4, 5};
    printArr(nums, 5);   // nums → &nums[0]
}
```

📌 **Note:**

* Inside `printArr`, `arr` is just a pointer, so `sizeof(arr)` will give **size of pointer**, not the actual array.

---

## 7. Pointer Arithmetic

* Only makes sense with arrays/contiguous memory.
* Adding/subtracting moves the pointer by multiples of the data type size.

### Example:

```cpp
int arr[3] = {10, 20, 30};
int *p = arr;     // points to arr[0]

cout << *p;       // 10
cout << *(p+1);   // 20
cout << *(p+2);   // 30
```

---

## 7. 2D Arrays as Pointers

A **2D array** in C/C++ is essentially a **contiguous block of memory** arranged in rows and columns.

For example:

```cpp
int B[2][3] = { {1, 2, 3}, {4, 5, 6} };
```

### How memory is laid out

```
Row 0 → [ 1   2   3 ]
Row 1 → [ 4   5   6 ]
```

Stored **linearly in memory**:

```
[ 1 ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ]
```

### Decay into pointers

* `B` → points to the first row (`&B[0]`), type: `int (*)[3]` (pointer to an array of 3 ints).
* `*B` → points to the first element of row 0 (`&B[0][0]`).
* `**B` → dereferences to the actual value at `B[0][0]` (→ `1` in this example).

📌 **Answering “What does a single dereference of a double pointer do?”**
If you have a `T **pp` (pointer to a pointer):

* `pp` → address of a pointer
* `*pp` → the pointer it points to (first level)
* `**pp` → the actual value stored (second level dereference)

In the case of a 2D array:

* `B` → pointer to row
* `*B` → pointer to column (row’s 0th element)
* `**B` → actual integer stored

---

### 2D Pointer Arithmetic Examples

Let `B[2][3]` again be:

```
B = [ [1, 2, 3], [4, 5, 6] ]
```

* `B + 1` → skips **1 full row** (3 ints) → `&B[1][0]`
* `*B + 1` → skips **1 element in row 0** → `&B[0][1]`
* `*(B + 1) + 1` → skips **1 row, then 1 element** → `&B[1][1]`
* `*(*B + 1)` → value at row 0, col 1 → `2`
* `*(*(B + 1) + 2)` → value at row 1, col 2 → `6`
* `*(*(B + i) + j)` → general formula for accessing `B[i][j]`

💡 **Why?**
Because `B[i][j]` is internally translated to `*(*(B + i) + j)`.

---

### Key Insights

* A **2D array is not exactly a `**` pointer**, but pointer arithmetic makes it behave similarly.
* Rows are laid out **contiguously**, so pointer math works reliably.
* `B[i]` is itself a pointer to the start of row `i`.
* This explains why both notations are equivalent:

  * `B[i][j]`
  * `*(*(B + i) + j)`

---
## 8. Common Pitfalls & Best Practices

❌ **Dereferencing NULL/Uninitialized Pointers**

```cpp
int *p;   // uninitialized, random address
*p = 5;   // undefined behavior
```

✅ Initialize pointers:

```cpp
int *p = nullptr;   // C++11+
```

❌ **Mismatched pointer types**

```cpp
char *c;
int *i = (int*)c;   // risky without proper alignment
```

❌ **Dangling Pointers** (pointing to freed memory)

```cpp
int *p = new int(10);
delete p;
*p = 20;   // ERROR: dangling pointer
```

✅ Best practice: set pointer to `nullptr` after `delete`.

---

## 🔑 Key Takeaways

* Pointers store **addresses**, not values.
* `&` gets an address, `*` accesses the value at an address.
* Pointer types matter → they control **how many bytes** are read.
* `void*` is a universal pointer but requires casting.
* Arrays decay into pointers when passed to functions.
* Always **initialize pointers** and avoid dereferencing invalid memory.
