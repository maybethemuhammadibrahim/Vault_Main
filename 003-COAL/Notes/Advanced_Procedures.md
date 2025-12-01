### 1\. The Stack Frame (Activation Record)

A stack frame is a dedicated area of the stack used by a single procedure call. It stores passed arguments, the return address, saved registers, and local variables.

**The Creation Sequence:**

1.  **Caller:** Pushes arguments onto the stack.
2.  **Caller:** Calls the subroutine (pushes the **Return Address**).
3.  **Callee (Subroutine):** Pushes **EBP** (saves the caller's frame pointer).
4.  **Callee:** Sets **EBP = ESP** (establishes the new base pointer).
5.  **Callee:** Decrements **ESP** (reserves space for local variables).
6.  **Callee:** Pushes registers that need preserving (e.g., EBX, EDI).

-----

### 2\. The Prologue: Why use EBP?

The instructions `push ebp` and `mov ebp, esp` constitute the standard function **prologue**.

**Why not just use ESP?**

  * **Stability (The "Anchor"):** ESP is volatile; it changes with every `PUSH` and `POP`. If you used ESP to access parameters (e.g., `[esp+4]`), the offsets would change constantly. EBP remains static throughout the function.
  * **Separation:** EBP creates a "zero point" reference:
      * **Positive Offsets (`[ebp+8]`):** Function Arguments.
      * **Negative Offsets (`[ebp-4]`):** Local Variables.
  * **Debugging:** It creates a linked list of stack frames, allowing debuggers to "unwind" the stack and show a trace of which function called which.

-----

### 3\. Parameter Passing

There are two primary ways to pass data to a procedure.

#### A. Passing by Value

A copy of the value is pushed onto the stack. Used for integers and small data types.

```assembly
.data
val1 DWORD 5
val2 DWORD 6

.code
; Pushing values directly
push val2
push val1
call AddTwo
```

#### B. Passing by Reference

The address (offset) of the object is pushed. **Mandatory for arrays** to avoid copying every single element (which consumes stack space and time).

```assembly
.data
array DWORD 50 DUP(?)

.code
; Pushing the address (OFFSET)
push OFFSET array
call ArrayFill
```

-----

### 4\. Local Variables

Local variables are created by subtracting from ESP to "grow" the stack downwards.

**Visualizing the Layout:**
| Syntax | Location | Description |
| :--- | :--- | :--- |
| `[ebp + 12]` | High Address | Argument 2 |
| `[ebp + 8]` | ... | Argument 1 |
| `[ebp + 4]` | ... | Return Address |
| `[ebp]` | ... | Saved Old EBP |
| `[ebp - 4]` | Low Address | **Local Variable 1** |

**Implementation & Cleanup:**
It is critical to reset ESP to EBP before returning, effectively "deleting" the local variables.

```assembly
MySub PROC
    push ebp
    mov  ebp, esp
    sub  esp, 8          ; Reserve 8 bytes for locals (e.g., two DWORDs)

    ; Using Symbolic Names for readability (Recommended)
    X_local EQU DWORD PTR [ebp-4]
    Y_local EQU DWORD PTR [ebp-8]

    mov X_local, 10      ; Assign value to local var
    mov Y_local, 20

    ; --- EPILOGUE ---
    mov esp, ebp         ; CRITICAL: Remove locals (snap ESP back)
    pop ebp              ; Restore caller's base pointer
    ret
MySub ENDP
```

-----

### 5\. Calling Conventions (Cleaning the Stack)

After a function finishes, the arguments pushed onto the stack must be removed. There are two main conventions for this.

#### A. C Calling Convention (cdecl)

The **Caller** is responsible for cleaning the stack. This allows for variable argument lists (like `printf`).

```assembly
; Caller Code
push 6          ; Arg 2
push 5          ; Arg 1
call AddTwo
add  esp, 8     ; Caller cleans up (2 args * 4 bytes = 8)
```

#### B. STDCALL Convention

The **Callee** (the procedure itself) cleans the stack using `RET n`. The integer `n` is the number of bytes to remove.

```assembly
AddTwo PROC
    push ebp
    mov  ebp, esp
    
    mov  eax, [ebp + 12] ; Get 2nd param
    add  eax, [ebp + 8]  ; Add 1st param
    
    pop  ebp
    ret  8               ; Return and pop 8 bytes (Cleanup)
AddTwo ENDP
```

-----

### 6\. Best Practices: Saving Registers

If a procedure modifies registers (other than EAX, which holds the return value), it should save and restore them.

**Correct Placement:**
Push registers *after* setting up EBP and *before* the body. Pop them in reverse order before the epilogue.

```assembly
MySub PROC
    push ebp
    mov  ebp, esp
    
    ; Save registers we plan to use
    push ecx
    push edx    

    ; --- Procedure Body ---
    mov  eax, [ebp+8]   ; Access param
    ; ... perform logic using ECX and EDX ...

    ; Restore registers (LIFO order - Last In, First Out)
    pop  edx
    pop  ecx
    
    pop  ebp
    ret
MySub ENDP
```

### ArrayFill Procedure Example

**Purpose:**
The `ArrayFill` procedure fills an array with a pseudorandom sequence of 16-bit integers.

**Arguments:**

1.  **Pointer to Array:** Passed by reference (offset).
2.  **Array Length:** Passed by value.

#### 1\. Sample Call

In this example, we define an array of 100 words and pass the arguments to the procedure.

```assembly
.data
count = 100
array WORD count DUP(?)

.code
; Argument 1: Array Offset (Passed by Reference)
push OFFSET array

; Argument 2: Array Length (Passed by Value)
push count

call ArrayFill
```

#### 2\. Procedure Prologue

Inside `ArrayFill`, the standard prologue initializes the stack frame pointer (`EBP`).

```assembly
ArrayFill PROC
    push ebp        ; Save caller's base pointer
    mov  ebp, esp   ; Initialize new base pointer
    
    ; ... Body of procedure ...
```

#### 3\. Resulting Stack Frame Layout

After the prologue executes, the stack frame is structured as follows (accessed relative to `EBP`):

| Offset | Content | Description |
| :--- | :--- | :--- |
| `[ebp + 12]` | **Array Offset** | First pushed argument |
| `[ebp + 8]` | **Count** | Second pushed argument |
| `[ebp + 4]` | **Return Address** | Pushed by `CALL` |
| `[ebp]` | **Saved EBP** | Pushed by `PUSH EBP` |

### I. The Anchor: Raw Assembly vs. High-Level Syntax

To understand the keywords, we must first see what they replace. This "Raw" procedure demonstrates the manual management of the stack, parameters, and local variables.

```assembly
; RAW IMPLEMENTATION (How the CPU actually executes it)
; Goal: A procedure that takes 2 params, saves EBX, and has 1 local variable.

RawProc LABEL NEAR
    ; 1. Prologue (Building the Stack Frame)
    push ebp                ; Save caller's base pointer
    mov  ebp, esp           ; Set our frame reference
    sub  esp, 4             ; Reserve 4 bytes for a local variable!
                            ; (CRITICAL: Guard against interrupts/pushes)

    ; 2. Preservation
    push ebx                ; Save EBX (Callee-saved register)

    ; 3. The Logic
    ; Stack view:
    ; [ebp+12] = Param2
    ; [ebp+8]  = Param1
    ; [ebp+4]  = Return Address
    ; [ebp]    = Old EBP
    ; [ebp-4]  = LocalVar1
    ; [ebp-8]  = Saved EBX

    mov eax, [ebp+8]        ; Get Param1
    add eax, [ebp+12]       ; Add Param2
    mov [ebp-4], eax        ; Store sum in LocalVar1

    ; 4. Epilogue (Cleanup)
    pop ebx                 ; Restore EBX first (LIFO order)
    mov esp, ebp            ; Snap ESP back to EBP (Discards locals)
    pop ebp                 ; Restore caller's EBP
    ret 8                   ; Return and pop 8 bytes of params (StdCall)
```

-----

### II. The Keywords Explained

#### 1\. PROC & PARAMETERS

  * **Purpose:** Automates the "Prologue" and parameter offsets.
  * **Syntax:** `Name PROC [USES regList], param1:Type, param2:Type`
  * **Mechanism:**
      * Generates `push ebp` / `mov ebp, esp` automatically.
      * Creates symbol names for `[ebp+8]`, etc., so you can use `param1` instead of offsets.
      * **USES:** Automatically generates `push` at the start and `pop` at the end for listed registers.
      * **RET:** It silently converts `ret` to `ret N` based on the total size of declared parameters.

#### 2\. LOCAL

  * **Purpose:** Replaces manual `sub esp, N`. Declares local variables within `PROC`.
  * **Syntax:** `LOCAL varName:Type`
  * **Mechanism:**
      * MASM calculates the total size of all `LOCAL` variables.
      * It inserts `sub esp, TotalSize` immediately after the prologue.
      * It creates a negative offset symbol (e.g., `varName` = `[ebp-4]`).
  * **Why use it?** It prevents the bug where you forget to reserve `ESP` space (the "Hardware Interrupt" risk).

#### 3\. INVOKE & ADDR

  * **Purpose:** A high-level wrapper for `PUSH` and `CALL`.
  * **Syntax:** `INVOKE ProcName, arg1, arg2`
  * **Mechanism:**
      * Checks parameter types against the `PROTO` or `PROC` definition.
      * Pushes arguments in Reverse Order (stdcall requirement).
      * Calls the procedure.
  * **The ADDR Operator:**
      * **Usage:** `INVOKE MyProc, ADDR myVar`
      * **Function:** Passes the *address* (pointer) of a variable.
      * **Intelligence:**
          * If `myVar` is **Global**: It generates `push OFFSET myVar`.
          * If `myVar` is **Local**: It generates `lea eax, [ebp-X]` followed by `push eax`. (This is why `ADDR` is unique to MASM/INVOKE; it handles the `LEA` logic for you).

#### 4\. ENTER

  * **Purpose:** A CPU instruction meant to replace the prologue (`push ebp`, `mov ebp, esp`, `sub esp, N`).
  * **Syntax:** `ENTER numBytes, nestingLevel`
  * **Usage:** **Rarely used.**
  * **Why?** It is historically slower (in clock cycles) on modern CPUs than simply writing the `push/mov/sub` instructions manually.

#### 5\. LEAVE

  * **Purpose:** The standard "Epilogue" instruction.
  * **Syntax:** `LEAVE`
  * **Mechanism:** Performs exactly two actions:
    1.  `MOV ESP, EBP` (Instantaneously frees all local variables/stack space).
    2.  `POP EBP` (Restores the previous stack frame).
  * **Note:** It does **not** return or clean up arguments; it only cleans up the stack frame.

-----

### III. Critical Concepts & Safety

#### The "Stack Corruption" Rules

1.  **Manual ESP Subtraction:**
    If you do not use `LOCAL`, you **must** use `sub esp, N` before using `[ebp-N]`. If you don't, hardware interrupts will overwrite your data because the CPU assumes anything below `ESP` is garbage.

2.  **Type Mismatch (The Crash Generator):**

      * **Scenario:** Proc expects `DWORD` (4 bytes), but you push `WORD` (2 bytes).
      * **Result:**
          * **Read Error:** The Proc reads 4 bytes, consuming your `WORD` + 2 bytes of the *next* item on the stack (garbage or next param).
          * **Return Error:** The Proc executes `RET 4`. The stack only had 2 bytes pushed. `ESP` returns 2 bytes higher than it started. The stack is now broken, and the Caller will crash.

11. **Why ENTER Should Never Be Mixed With PROC**

Both build stack frames.
If you write:

MyProc PROC arg1:DWORD
    ENTER 4,0       ; disaster


You get:
MASM-generated push ebp
ENTER-generated push ebp
Parameter offsets shifted by 4 bytes
→ arguments now read as garbage

Using ENTER inside a PROC = double prologue = corrupted stack.

Use PROC + LOCAL.
Ignore ENTER.


#### Summary Comparison Table

| Feature | Raw Assembly | High-Level MASM |
| :--- | :--- | :--- |
| **Start** | `push ebp` / `mov ebp, esp` | `PROC` |
| **Locals** | `sub esp, 4` / `mov [ebp-4], val` | `LOCAL var:DWORD` |
| **Calling** | `push val` / `push offset val` | `INVOKE Proc, val, ADDR val` |
| **Cleanup** | `mov esp, ebp` / `pop ebp` | `LEAVE` (or auto-generated by `ENDP`) |
| **Return** | `ret 8` (Manual calculation) | `ret` (Auto-calculated by MASM) |
