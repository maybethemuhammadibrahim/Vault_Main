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
