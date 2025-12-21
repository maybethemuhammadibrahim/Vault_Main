### 1. The Logic Group (AND, OR, XOR, NOT)

These instructions are used to manipulate individual bits.

#### **AND**

* **Operation:** Performs Boolean AND on each pair of bits. Result is 1 only if both bits are 1.
* **Dest Register:** **Modified** (Stores the result).
* **Flags Affected:**
* **CF, OF:** Always **Cleared** (0).
* **SF, ZF, PF:** Updated based on result.
* **AF:** Undefined.


* **Primary Uses:**
1. **Masking (Clearing specific bits):** Forces bits to 0 while keeping others.
```assembly
AND AL, 0Fh   ; Clears the upper 4 bits, keeps lower 4 bits

```


2. **Checking if a number is Odd/Even (Destructive):**
```assembly
AND AL, 1     ; If Result=1 (ZF=0), it's Odd. If Result=0 (ZF=1), it's Even.
; Warning: This destroys the value in AL! Use TEST for a safe check.

```


3. **Convert Lowercase to Uppercase:** `AND AL, 11011111b` (clears bit 5).



#### **OR**

* **Operation:** Performs Boolean OR. Result is 1 if either bit is 1.
* **Dest Register:** **Modified**.
* **Flags Affected:**
* **CF, OF:** Always **Cleared** (0).
* **SF, ZF, PF:** Updated based on result.
* **AF:** Undefined.


* **Primary Uses:**
1. **Setting specific bits:** Forces bits to 1.
```assembly
OR AL, 80h    ; Sets the MSB (bit 7) to 1, leaves others alone.

```


2. **Checking for Zero (Non-Destructive check of value):**
```assembly
OR EAX, EAX   ; Value doesn't change, but flags are updated.
JZ IsZero     ; Jump if EAX was 0.

```





#### **XOR** (Exclusive OR)

* **Operation:** Result is 1 if bits are different. Result is 0 if bits are same.
* **Dest Register:** **Modified**.
* **Flags Affected:**
* **CF, OF:** Always **Cleared** (0).
* **SF, ZF, PF:** Updated based on result.
* **AF:** Undefined.


* **Primary Uses:**
1. **Toggling Bits:** Flips 1 to 0 and 0 to 1.
```assembly
XOR AL, 1     ; Flips bit 0 (Changes Even to Odd, Odd to Even).

```


2. **Clearing a Register (The Efficient Way):**
```assembly
XOR EAX, EAX  ; Sets EAX to 0. (Faster/smaller than MOV EAX, 0).

```


3. **Simple Encryption:** `XOR` value with a key twice returns the original value.



#### **NOT**

* **Operation:** Inverts all bits (1's Complement). `0->1`, `1->0`.
* **Dest Register:** **Modified**.
* **Flags Affected:** **NONE**. (Crucial Exam Point: `NOT` never changes any flags).
* **Primary Uses:**
1. **Inverting a number.**
2. **Part of 2's Complement:** `NOT EAX` followed by `INC EAX` = Negate number.



---

### 2. The Comparison Group (TEST, CMP)

These instructions are **non-destructive**. They perform logic/math to update flags but **throw away the result**.

#### **TEST** (The "Silent" AND)

* **Operation:** Performs an implicit `AND`, updates flags, but discards result.
* **Dest Register:** **Unchanged**.
* **Flags Affected:** Same as `AND` (CF=0, OF=0, updates SF, ZF, PF).
* **Primary Uses:**
1. **Checking Even/Odd (Safe):**
```assembly
TEST AL, 1
JZ  IsEven    ; Jump if Zero Flag = 1 (Result was 0, meaning bit 0 was 0)
JNZ IsOdd     ; Jump if Zero Flag = 0 (Result was 1, meaning bit 0 was 1)

```


2. **Checking specific bits status:**
```assembly
TEST AL, 10000000b ; Check if Bit 7 is set (Negative?)
JNZ  Bit7Set       ; Jump if the result is not zero (Bit 7 was 1)

```





#### **CMP** (The "Silent" SUB)

* **Operation:** Performs an implicit `SUB` (Dest - Source), updates flags, discards result.
* **Dest Register:** **Unchanged**.
* **Flags Affected:** Updates **All** Status Flags (CF, OF, SF, ZF, AF, PF) just like `SUB`.
* **Primary Uses:**
1. **Equality Check:**
```assembly
CMP EAX, EBX
JE  Equal      ; Jump if EAX == EBX (ZF=1)

```


2. **Greater/Less Check:**
```assembly
CMP EAX, 10
JG  Greater    ; Jump if EAX > 10 (Signed)
JA  Above      ; Jump if EAX > 10 (Unsigned)

```





---

### Summary Table for Exam

| Instruction | Logic | Dest Modified? | CF/OF | SF/ZF/PF | Uses |
| --- | --- | --- | --- | --- | --- |
| **AND** | `A & B` | **Yes** | 0 | Updated | Masking, Clear bits |
| **OR** | `A | B` | **Yes** | 0 | Updated | Set bits, Merge |
| **XOR** | `A ^ B` | **Yes** | 0 | Updated | Toggle bits, Clear Reg |
| **NOT** | `~A` | **Yes** | **--** | **--** | Invert bits |
| **TEST** | `A & B` | **No** | 0 | Updated | Check Bits, **Even/Odd** |
| **CMP** | `A - B` | **No** | Updated | Updated | Compare (`<`, `>`, `=`) |