
# ASM Notes

## Program Segments

* **Definition**: In MASM (Microsoft Macro Assembler), a program is divided into logical sections called **segments**.
* **Purpose**: Segments help organize code and data, and they inform the assembler and linker how to arrange the program in memory.

### Common Segments

1. **`.data` Segment**

   * **Definition**: Used to define and store initialized data, such as constants and variables.
   * **Use**: Declares storage that will be placed in memory and can be accessed during program execution.
   * **Example**:

     ```asm
     .data
     myVar DWORD 25   ; define a 32-bit variable initialized with 25
     ```
   * **Machine code note**: The assembler reserves memory for `myVar` and places the constant value (`19h` in hex) into the program’s data section.

2. **`.code` Segment**

   * **Definition**: Contains the program’s instructions (the actual logic).
   * **Use**: This is where assembly instructions are written.
   * **Example**:

     ```asm
     .code
     main PROC
        mov eax, myVar   ; move variable into eax
        add eax, 5       ; add 5
        ret
     main ENDP
     ```
   * **Machine code note**: Each instruction (`mov`, `add`, etc.) is converted into a sequence of bytes (opcodes + operands). For example, `add eax, 5` generates a different opcode than `add eax, ebx`.

3. **Stack Segment (optional)**

   * **Definition**: A section of memory used for function calls, local variables, and temporary storage.
   * **Use**: Typically allocated with the `.stack` directive.
   * **Example**:

     ```asm
     .stack 100h  ; allocate 256 bytes for the stack
     ```

---

## Instruction Pointer (IP/EIP/RIP)

* **Definition**: A special CPU register that always holds the address of the next instruction to execute.
* **Context**: When the program runs, the CPU fetches the instruction at the address stored in the instruction pointer, decodes it, and executes it.
* **Machine code note**: Every assembly instruction occupies some number of bytes in memory. The instruction pointer increments by that number after each instruction, pointing to the next one.

---

## Integer Literals

* **Definition**: A direct representation of integer values in assembly code.
* **General Form**:

  ```
  [{+ | − }] digits [radix]
  ```
* **Radix (base)**: Defines the numbering system.

  * `h` → hexadecimal (base 16)
  * `b` → binary (base 2)
  * `d` → decimal (base 10, optional since default)
* **Examples**:

  ```asm
  mov eax, 25d   ; decimal
  mov ebx, 100h  ; hexadecimal (256 decimal)
  mov ecx, 1010b ; binary (decimal 10)
  ```
* **Machine code note**: The assembler encodes these literals directly as binary numbers into the instruction’s operand field.

---

## Constant Integer Expressions

* **Definition**: Mathematical expressions composed of integer literals and arithmetic operators.
* **Requirement**: Must evaluate to an integer within 32 bits (`0` through `FFFFFFFFh`).
* **Examples**:

  ```asm
  .data
  A DWORD 5 * 2      ; result = 10
  B DWORD 12 - 1 MOD 5 ; result = 1
  ```
* **Machine code note**: The assembler evaluates these at **assembly time**. Only the final numeric result is stored in the program’s data section.

---

## Operator Precedence in Assembly Expressions

* **Order of evaluation** (similar to mathematics):

  1. Parentheses
  2. Unary operators (e.g., `−`)
  3. Multiplication, division, modulus
  4. Addition, subtraction
* **Examples**:

  * `+5 * 2` → result = 10
  * `12 − 1 MOD 5` → `12 − (1 MOD 5)` → 11
  * `−5 + 2` → result = −3
  * `(4 + 2) * 6` → 36

---

## Real Number Literals

* **Definition**: Constants representing floating-point numbers.
* **General Form**:

  ```
  [sign]integer.[integer][exponent]
  ```

  where:

  * `sign` = `+` or `−`
  * `exponent` = `E[{+,−}]integer`
* **Examples**:

  ```asm
  +3.0
  -44.2E+05
  ```
* **Machine code note**: The assembler encodes these into floating-point representations used by the FPU (Floating Point Unit).

---

## Character Literals

* **Definition**: A single character enclosed in single or double quotes.
* **Use**: Stored as the character’s **ASCII code** in memory.
* **Examples**:

  ```asm
  'A'  ; stored as 41h
  "a"  ; stored as 61h
  ```

---

## String Literals

* **Definition**: A sequence of characters (including spaces), enclosed in single or double quotes.
* **Use**: Stored as an array of ASCII codes.
* **Examples**:

  ```asm
  'ABC'     ; 41h, 42h, 43h
  "Hello"   ; 48h, 65h, 6Ch, 6Ch, 6Fh
  ```

---

## Reserved Words

* **Definition**: Words with special meaning in MASM syntax (e.g., instruction mnemonics, directives, registers).
* **Note**: Not case-sensitive (`MOV`, `mov`, `Mov` are equivalent).

---

## Identifiers

* **Definition**: Programmer-chosen names that represent variables, constants, labels, or procedures. Can also be called as variable names
* **Rules**: Must begin with a letter, underscore, or `@`. Cannot be a reserved word.

---

## Directives

* **Definition**: Commands recognized and executed by the assembler, not by the CPU.
* **Use**: Directives tell the assembler how to organize the program, allocate memory, or define constants.
* **Example**:

  * `.data` defines a data segment
  * `.code` defines a code segment
* `variable declaration` actually arranges the data in memory not executes it

### Difference between Directive and Instruction

* **Directive**: Assembler-only, not translated into machine code (e.g., `.data`).
* **Instruction**: CPU-executable, translated into machine code (e.g., `mov eax, 5`).

---

## Program Segments (Revisited)

* **Definition**: Logical divisions of a program (data, code, stack) created using assembler directives.
* **Purpose**: Allows separation of storage, instructions, and runtime stack.

---

## Instructions

* **Definition**: Executable statements that are converted into machine code by the assembler.
* **Format**:

  ```
  [label:] mnemonic [operands] [;comment]
  ```
* **Example**:

  ```asm
  start: mov eax, ebx ; copy ebx into eax
  ```
* **Machine code note**: The assembler assigns an opcode (binary value) for `mov`, followed by encoded information about the operands.

---

## Labels

* **Definition**: Identifiers marking positions in code or data for reference.

### Data Labels

* Used to name storage locations in the `.data` segment.
* **Example**:

  ```asm
  value DWORD 10
  mov eax, value   ; refers to memory location of 'value'
  ```

### Code Labels

* Used to mark positions in the `.code` segment, often as jump targets.
* **Example**:

  ```asm
  start:
     mov eax, 1
     jmp done
  done:
     ret
  ```

---

## Instruction Mnemonics

* **Definition**: Human-readable abbreviations for machine instructions.
* **Examples**:

  * `mov` → move data
  * `add` → addition
  * `sub` → subtraction
* **Context**: Each mnemonic corresponds to a unique opcode (or family of opcodes) in machine language.


