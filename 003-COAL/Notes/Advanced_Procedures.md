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
# Advanced MASM: Stack Frames, Procedures & Local Variables

## I. Foundation: The Raw Stack Frame

Before using MASM's high-level directives, you must understand what the CPU actually executes. Every abstraction ultimately compiles down to these fundamental operations.

### The Standard Stack Frame Structure

A procedure needs to:
1. Accept parameters from the caller
2. Preserve registers that must survive the call
3. Allocate space for local variables
4. Clean up before returning

```asm
; RAW IMPLEMENTATION (No PROC directive)
RawProc:
    ; === PROLOGUE ===
    push ebp                ; Save caller's base pointer
    mov  ebp, esp           ; Set our base pointer to current stack top
    sub  esp, 8             ; Allocate 8 bytes for locals
                            ; Creates: [ebp-4] and [ebp-8]

    ; === REGISTER PRESERVATION ===
    push ebx                ; Save callee-saved registers
    push esi                ; (EBX, ESI, EDI must be preserved)
    push edi

    ; === FUNCTION LOGIC ===
    ; At this point, the stack looks like:
    ; [ebp+12] = Param2 (second parameter)
    ; [ebp+8]  = Param1 (first parameter)
    ; [ebp+4]  = Return address (pushed by CALL)
    ; [ebp]    = Old EBP (saved base pointer)
    ; [ebp-4]  = LocalVar1
    ; [ebp-8]  = LocalVar2
    ; [esp]    = Top of stack (after pushing EDI)

    mov eax, [ebp+8]        ; Read first parameter
    add eax, [ebp+12]       ; Add second parameter
    mov [ebp-4], eax        ; Store result in local variable

    ; === EPILOGUE ===
    pop edi                 ; Restore registers in REVERSE order
    pop esi
    pop ebx
    
    mov esp, ebp            ; Deallocate locals (reset stack pointer)
    pop ebp                 ; Restore caller's base pointer
    ret 8                   ; Return and pop 8 bytes of parameters
                            ; (STDCALL convention: callee cleans stack)
```

### Why EBP? The Base Pointer's Purpose

- **ESP changes constantly** (every push/pop moves it)
- **EBP stays fixed** during the function, providing stable reference points
- Parameters are at **positive offsets** from EBP: `[ebp+8]`, `[ebp+12]`
- Locals are at **negative offsets** from EBP: `[ebp-4]`, `[ebp-8]`

---

## II. High-Level Directives Explained

### 1. PROTO - The Function Signature

`PROTO` declares a function's interface **before** it's implemented. Think of it as a forward declaration in C.

```asm
; Syntax: FunctionName PROTO [calling_convention] param1:type, param2:type, ...
AddTwo PROTO :DWORD, :DWORD
GetString PROTO :DWORD
```

**Critical Points:**
- **No code is generated** - this is purely for the assembler's benefit
- Required for `INVOKE` to validate arguments at assembly time
- Can be placed in include files for cross-module calls
- The colon before the type is mandatory: `:DWORD` not `DWORD`

**Common Error:**
```asm
; WRONG - Missing colon
AddTwo PROTO DWORD, DWORD

; CORRECT
AddTwo PROTO :DWORD, :DWORD
```

---

### 2. PROC - The Implementation

`PROC` automates the prologue and epilogue, translating parameter names into stack offsets.

```asm
AddTwo PROC val1:DWORD, val2:DWORD
    ; MASM automatically inserts:
    ;   push ebp
    ;   mov ebp, esp
    
    mov eax, val1           ; Assembles to: mov eax, [ebp+8]
    add eax, val2           ; Assembles to: add eax, [ebp+12]
    
    ret                     ; Assembles to: mov esp, ebp
                            ;                pop ebp
                            ;                ret 8
AddTwo ENDP
```

**Parameter Mechanics:**
- Parameters are **stack offset aliases**, not memory variables
- First parameter starts at `[ebp+8]` (after return address at `[ebp+4]` and saved EBP at `[ebp]`)
- Parameters pushed **right-to-left** by convention (last parameter pushed first)
- The order in memory is **left-to-right** (first param at lower address)

**Calling Example:**
```asm
; This code:
push 20                     ; Second parameter
push 10                     ; First parameter
call AddTwo

; Results in stack:
; [ebp+12] = 20 (val2)
; [ebp+8]  = 10 (val1)
```

**Naming Restrictions:**
- Don't use register names: `PROC eax:DWORD` will cause errors
- Don't reuse segment register names: `cs`, `ds`, `es`, `fs`, `gs`, `ss`
- Avoid MASM keywords: `BYTE`, `WORD`, `DWORD`, `OFFSET`, etc.

---

### 3. LOCAL - Stack Variable Allocation

`LOCAL` replaces manual `sub esp, N` instructions.

```asm
MyProc PROC
    LOCAL sum:DWORD         ; [ebp-4]
    LOCAL count:WORD        ; [ebp-8] due to alignment
    LOCAL flag:BYTE         ; [ebp-12] due to alignment
    
    ; MASM generates: sub esp, 12
    
    mov sum, 0              ; INITIALIZE - contains garbage otherwise!
    mov count, 0
    mov flag, 0
    ret
MyProc ENDP
```

**Critical Nuances:**

**1. Garbage Values:**
Local variables contain **random data** from previous stack usage. Always initialize before reading.

```asm
MyProc PROC
    LOCAL temp:DWORD
    
    ; WRONG - temp contains garbage
    mov eax, temp           ; EAX now has unpredictable value
    
    ; CORRECT
    mov temp, 0
    mov eax, temp           ; EAX = 0
    ret
MyProc ENDP
```

**2. Automatic Alignment:**
MASM aligns locals to DWORD boundaries (4 bytes) for performance, even if you request smaller sizes.

```asm
MyProc PROC
    LOCAL a:BYTE            ; Occupies 4 bytes, not 1
    LOCAL b:BYTE            ; Occupies 4 bytes, not 1
    LOCAL c:WORD            ; Occupies 4 bytes, not 2
    
    ; MASM generates: sub esp, 12  (not sub esp, 4)
    ret
MyProc ENDP
```

**3. Declaration Order:**
Locals are allocated in the order declared, with the first LOCAL at the highest address.

```asm
LOCAL first:DWORD          ; [ebp-4]
LOCAL second:DWORD         ; [ebp-8]
LOCAL third:DWORD          ; [ebp-12]
```

---

### 4. INVOKE - The High-Level Call

`INVOKE` is a compile-time macro that generates the appropriate `PUSH` and `CALL` sequence.

```asm
; High-level
INVOKE AddTwo, 10, 20

; Expands to (approximately):
push 20                    ; Right-to-left
push 10
call AddTwo
```

**Advantages:**
- Type checking against PROTO declaration
- Automatic parameter counting
- Cleaner, more readable code
- Works with expressions: `INVOKE AddTwo, eax, [myVar]`

**With ADDR Operator:**
```asm
MyProc PROC
    LOCAL buffer[100]:BYTE
    
    INVOKE FillBuffer, ADDR buffer, 100
    ret
MyProc ENDP
```

---

### 5. ADDR vs OFFSET - The Critical Distinction

This is one of the most misunderstood aspects of MASM.

| Aspect | OFFSET | ADDR |
|--------|--------|------|
| **Timing** | Link-time constant | Run-time calculation (for locals) |
| **Usage** | Global/static variables only | Global OR local variables |
| **Implementation** | Direct memory address | Generates `LEA` for locals |
| **Effect on EAX** | None | **Clobbers EAX** for locals |

**Examples:**

```asm
.data
globalVar DWORD 100

.code
MyProc PROC
    LOCAL localVar:DWORD
    
    ; Using OFFSET (only for globals)
    push OFFSET globalVar   ; Direct address, no registers affected
    
    ; Using ADDR on global (behaves like OFFSET)
    push ADDR globalVar     ; Same as OFFSET for globals
    
    ; Using ADDR on local (generates LEA)
    push ADDR localVar      ; MASM generates:
                            ;   lea eax, [ebp-4]
                            ;   push eax
                            ; WARNING: EAX is now destroyed!
    
    call SomeFunction
    ret
MyProc ENDP
```

**Common Pitfall:**
```asm
MyProc PROC
    LOCAL temp:DWORD
    
    mov eax, 42             ; EAX = 42
    INVOKE DoSomething, ADDR temp, eax
    ; BUG: ADDR temp generates LEA into EAX first,
    ; destroying the value 42 before pushing it!
    
    ; SOLUTION: Reorder arguments or save EAX
    INVOKE DoSomething, eax, ADDR temp  ; Push eax before LEA
    ret
MyProc ENDP
```

---

## III. Calling Conventions In-Depth

The calling convention determines **who cleans the stack** and **parameter order**.

### STDCALL (Windows API Standard)

**Rules:**
- Parameters pushed **right-to-left**
- **Callee** cleans the stack
- Return value in EAX (or EAX:EDX for 64-bit values)

```asm
.model flat, stdcall       ; Set default convention

AddTwo PROC STDCALL val1:DWORD, val2:DWORD
    mov eax, val1
    add eax, val2
    ret 8                  ; Callee cleans 8 bytes
AddTwo ENDP

; Caller's code:
push 20
push 10
call AddTwo
; No cleanup needed - callee did it with RET 8
```

**Advantages:**
- Smaller code size (cleanup appears once in function, not at every call site)
- Consistent cleanup even if function called many times

**Disadvantages:**
- Cannot support variable arguments (like printf)

---

### CDECL (C Language Default)

**Rules:**
- Parameters pushed **right-to-left**
- **Caller** cleans the stack
- Return value in EAX

```asm
AddTwo PROC C val1:DWORD, val2:DWORD
    mov eax, val1
    add eax, val2
    ret                    ; Just RET, no parameter count
AddTwo ENDP

; Caller's code:
push 20
push 10
call AddTwo
add esp, 8                 ; Caller cleans stack
```

**Advantages:**
- Supports variable arguments (caller knows how many args it pushed)
- Compatible with C standard library

**Disadvantages:**
- Larger code size (cleanup code at every call site)

---

### Convention Comparison Table

| Aspect | STDCALL | CDECL | FASTCALL |
|--------|---------|-------|----------|
| **Cleanup** | Callee | Caller | Callee |
| **Parameters** | Right-to-left | Right-to-left | First 2 in ECX/EDX, rest on stack |
| **Varargs Support** | No | Yes | No |
| **RET Instruction** | `ret N` | `ret` | `ret N` (for stack params) |
| **Use Case** | Win32 API | C libraries | Performance-critical code |

**Mixing Conventions:**
You can override the default on a per-function basis:
```asm
.model flat, stdcall       ; Default is STDCALL

MyFunc PROC C arg1:DWORD   ; This function uses CDECL
    ret
MyFunc ENDP
```

---

## IV. Register Preservation Rules

Understanding which registers must be preserved is critical to prevent subtle bugs.

### Callee-Saved Registers (Must Preserve)

These registers must have the **same value** after your function returns:
- **EBX** - Base register
- **ESI** - Source index
- **EDI** - Destination index
- **EBP** - Base pointer (frame pointer)

```asm
MyProc PROC
    push ebx               ; Save
    push esi
    push edi
    
    ; Use EBX, ESI, EDI freely
    mov ebx, 100
    mov esi, OFFSET buffer
    mov edi, OFFSET dest
    
    pop edi                ; Restore in REVERSE order
    pop esi
    pop ebx
    ret
MyProc ENDP
```

### Caller-Saved Registers (Can Destroy)

These registers can be modified without preservation:
- **EAX** - Accumulator (also holds return value)
- **ECX** - Counter
- **EDX** - Data (also holds high 32 bits of return value for 64-bit results)

```asm
MyProc PROC
    ; No need to save EAX, ECX, EDX
    mov eax, 42
    mov ecx, 100
    mov edx, 200
    ; Return value goes in EAX
    ret
MyProc ENDP
```

**Implication for Callers:**
If you need EAX/ECX/EDX preserved across a function call, **you** must save them:

```asm
mov eax, 123               ; Important value in EAX
push eax                   ; Save it
call SomeFunction          ; Might destroy EAX
pop eax                    ; Restore it
```

---

## V. Advanced Topics & Pitfalls

### 1. The ENTER/LEAVE Instructions

**ENTER** was added to x86 to simplify Pascal-style stack frame creation.

```asm
; ENTER equivalent
enter 8, 0                 ; Allocate 8 bytes for locals

; Equivalent to:
push ebp
mov ebp, esp
sub esp, 8
```

**Why You Should Never Use ENTER:**
- **10-40x slower** than the three-instruction sequence on modern processors
- Implemented in complex microcode, not optimized hardware
- No advantage in code size for simple frames
- `LEAVE` instruction (its counterpart) is fine to use, as it's fast

**Use This Instead:**
```asm
; Fast prologue
push ebp
mov ebp, esp
sub esp, 8

; Fast epilogue
mov esp, ebp               ; Or use: leave (this one is fast)
pop ebp
```

---

### 2. The Double-Prologue Bug

**Never mix PROC directives with manual prologue code.**

```asm
; WRONG - PROC already generates prologue
MyProc PROC val1:DWORD
    push ebp               ; PROC already did this!
    mov ebp, esp           ; PROC already did this!
    
    mov eax, val1          ; DISASTER: val1 translates to [ebp+8]
                           ; But EBP was moved again, so this reads
                           ; the WRONG memory location!
    ret
MyProc ENDP
```

**The Fix:** Never manually create prologue/epilogue when using PROC. Let MASM handle it, or write a raw label-based function.

---

### 4. The Mysterious Return Address

Many beginners don't realize that `CALL` pushes the return address onto the stack **before** the function executes.

```asm
; Caller executes:
push 20                    ; Parameter at [esp]
push 10                    ; Parameter at [esp]
call MyProc                ; Pushes return address at [esp]

; Inside MyProc, before prologue:
; [esp]   = Return address
; [esp+4] = 10 (first parameter)
; [esp+8] = 20 (second parameter)

; After prologue (push ebp, mov ebp, esp):
; [ebp+4] = Return address
; [ebp+8] = 10 (first parameter)  <- This is why params start at [ebp+8]
; [ebp+12] = 20 (second parameter)
```

This is why the first parameter is at `[ebp+8]` and not `[ebp+4]`.

---

### 5. Nested Function Calls

When a function calls another function, its local variables must remain accessible.

```asm
OuterProc PROC
    LOCAL outerVar:DWORD
    
    mov outerVar, 100
    
    INVOKE InnerProc       ; InnerProc will:
                           ;   1. Push its own EBP
                           ;   2. Set EBP = ESP
                           ;   3. Use its own locals
                           ;   4. Restore old EBP on return
    
    mov eax, outerVar      ; Still accessible because EBP was restored
    ret
OuterProc ENDP
```

The **chain of saved EBP values** creates a linked list that debuggers use to display call stacks.

---

## VI. Practical Debugging Tips

### Viewing the Stack in a Debugger

When debugging, examine these key memory locations:
- `[ESP]` - Top of stack
- `[EBP]` - Saved base pointer (points to caller's frame)
- `[EBP+4]` - Return address
- `[EBP+8]` onwards - Parameters
- `[EBP-4]` onwards - Local variables

### Common Stack Corruption Symptoms

**1. Return to Wrong Address:**
- Symptom: Program crashes immediately after `ret`
- Cause: Stack pointer (ESP) misaligned before `ret`
- Debug: Ensure pushes and pops are balanced

**2. Corrupted Local Variables:**
- Symptom: Local variables change unexpectedly
- Cause: Buffer overflow, writing past array bounds
- Debug: Check all array accesses

**3. Preserved Registers Changed:**
- Symptom: Caller's EBX/ESI/EDI have wrong values after call
- Cause: Forgot to save/restore callee-saved registers
- Debug: Verify push/pop pairs for EBX, ESI, EDI

---

## VII. Quick Reference Table

| Directive | Purpose | Low-Level Equivalent | Key Warnings |
|-----------|---------|---------------------|--------------|
| **PROTO** | Function declaration | None (assembler directive) | Required before INVOKE; must match PROC signature |
| **PROC** | Function implementation | Label + prologue/epilogue code | Don't manually add prologue; parameter names are stack aliases |
| **LOCAL** | Allocate local variables | `sub esp, N` | Contains garbage; must initialize; aligned to 4 bytes |
| **INVOKE** | Call function | `push ... / call` | Right-to-left parameter order; validates against PROTO |
| **ADDR** | Get address | `lea eax, [ebp-X]` (locals) | **Clobbers EAX** for locals; use carefully in INVOKE |
| **OFFSET** | Get address | Direct address constant | Globals only; resolved at link-time |
| **RET N** | Return + clean stack | `ret N` | Used with STDCALL; N = bytes to pop |
| **RET** | Return | `ret` | Used with CDECL; caller cleans stack |

---

## VIII. Complete Working Example

```asm
.386
.model flat, stdcall
option casemap:none

; Prototypes
AddThreeNumbers PROTO :DWORD, :DWORD, :DWORD
DisplayResult PROTO :DWORD

.data
result DWORD ?

.code
start:
    ; Call with INVOKE
    INVOKE AddThreeNumbers, 10, 20, 30
    mov result, eax
    INVOKE DisplayResult, result
    
    ; Exit
    ret

; Function implementation
AddThreeNumbers PROC num1:DWORD, num2:DWORD, num3:DWORD
    LOCAL total:DWORD
    
    ; Initialize local (contains garbage otherwise)
    mov total, 0
    
    ; Perform calculation
    mov eax, num1
    add eax, num2
    add eax, num3
    mov total, eax
    
    ; Return value in EAX
    mov eax, total
    ret
AddThreeNumbers ENDP

DisplayResult PROC value:DWORD
    ; Preserve callee-saved register
    push ebx
    
    mov ebx, value
    ; ... display code ...
    
    ; Restore register
    pop ebx
    ret
DisplayResult ENDP

end start
```

