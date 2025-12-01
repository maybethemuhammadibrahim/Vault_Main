### I. The Anchor: Raw Assembly vs. High-Level Syntax

To understand the keywords, we must first see what they replace. This "Raw" procedure demonstrates the manual management of the stack, parameters, and local variables.

```masm
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

#### Summary Comparison Table

| Feature | Raw Assembly | High-Level MASM |
| :--- | :--- | :--- |
| **Start** | `push ebp` / `mov ebp, esp` | `PROC` |
| **Locals** | `sub esp, 4` / `mov [ebp-4], val` | `LOCAL var:DWORD` |
| **Calling** | `push val` / `push offset val` | `INVOKE Proc, val, ADDR val` |
| **Cleanup** | `mov esp, ebp` / `pop ebp` | `LEAVE` (or auto-generated by `ENDP`) |
| **Return** | `ret 8` (Manual calculation) | `ret` (Auto-calculated by MASM) |
