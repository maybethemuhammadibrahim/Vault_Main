Here are the consolidated notes and charts for Data Transfer, Shift/Rotate, and Arithmetic instructions in MASM x86 Assembly.

---

## 1. Multiplication and Division Reference Charts

These instructions involve implicit registers (`AL`, `AX`, `EAX`, `DX`, `EDX`) depending on the operand size.

### **MUL (Unsigned Multiply)**

Logic: `Multiplicand × Multiplier = Product`

| Operand Size | Multiplicand (Implicit) | Multiplier (Src) | Product (Implicit Destination) |
| --- | --- | --- | --- |
| **8-bit** | `AL` | Reg/Mem8 | `AX` |
| **16-bit** | `AX` | Reg/Mem16 | `DX:AX` (High part in DX, Low in AX) |
| **32-bit** | `EAX` | Reg/Mem32 | `EDX:EAX` (High part in EDX, Low in EAX) |

### **DIV (Unsigned Divide)**

Logic: `Dividend ÷ Divisor = Quotient (and Remainder)`

| Operand Size | Dividend (Implicit) | Divisor (Src) | Quotient (Result) | Remainder (Result) |
| --- | --- | --- | --- | --- |
| **8-bit** | `AX` | Reg/Mem8 | `AL` | `AH` |
| **16-bit** | `DX:AX` | Reg/Mem16 | `AX` | `DX` |
| **32-bit** | `EDX:EAX` | Reg/Mem32 | `EAX` | `EDX` |

*(Note: For `IDIV` (Signed Divide), the register usage is identical to `DIV`, but you must sign-extend the dividend using `CBW`, `CWD`, or `CDQ` before dividing.)*

---

## 2. Operand Reference Lists

### **Data Transfer Instructions**

#### **MOV (Move Data)**

Copies data from source to destination.

* **Dest:** Reg, Mem
* **Src:** Reg, Mem, Imm
* **Restriction:** Sizes must match. No `Mem, Mem`. No `SegReg` to `Imm`.

| Format | Example |
| --- | --- |
| `MOV Reg, Reg` | `MOV AX, BX` |
| `MOV Reg, Mem` | `MOV EAX, [var]` |
| `MOV Mem, Reg` | `MOV [var], CL` |
| `MOV Reg, Imm` | `MOV ECX, 10` |
| `MOV Mem, Imm` | `MOV BYTE PTR [var], 5` |

#### **XCHG (Exchange Data)**

Swaps contents of two operands.

* **Restriction:** Sizes must match. No `Mem, Mem`. No Immediate operands.

| Format | Example |
| --- | --- |
| `XCHG Reg, Reg` | `XCHG AX, BX` |
| `XCHG Reg, Mem` | `XCHG EAX, [var]` |
| `XCHG Mem, Reg` | `XCHG [var], CL` |

#### **MOVSX (Move with Sign Extension)**

Copies a smaller signed source to a larger destination, filling the upper bits with the sign bit.

* **Dest:** Reg (16/32 bit)
* **Src:** Reg, Mem (8/16 bit)
* **Restriction:** Dest must be larger than Src. No immediate.

| Format | Example |
| --- | --- |
| `MOVSX Reg16, Reg8/Mem8` | `MOVSX AX, BL` |
| `MOVSX Reg32, Reg8/Mem8` | `MOVSX EAX, BYTE PTR [var]` |
| `MOVSX Reg32, Reg16/Mem16` | `MOVSX ECX, DX` |

#### **MOVZX (Move with Zero Extension)**

Copies a smaller unsigned source to a larger destination, filling the upper bits with zeros.

* **Same operand rules as MOVSX.**

---

### **Stack Instructions**

#### **PUSH**

Decrements `ESP` and copies operand to the stack.

* **Size:** In 32-bit mode, operands are generally 32-bit (or 16-bit). 8-bit pushes are illegal.

| Format | Example |
| --- | --- |
| `PUSH Reg16/32` | `PUSH EAX` |
| `PUSH Mem16/32` | `PUSH [var]` |
| `PUSH Imm8/16/32` | `PUSH 10` (Pushes as 32-bit value) |

#### **POP**

Copies data from stack to operand and increments `ESP`.

* **Size:** 16 or 32 bits only. No CS or IP registers.

| Format | Example |
| --- | --- |
| `POP Reg16/32` | `POP EBX` |
| `POP Mem16/32` | `POP [var]` |

---

### **Shift and Rotate Instructions**

**Instructions:** `SHL` (Shift Left), `SHR` (Shift Right), `SAL` (Arith Left), `SAR` (Arith Right), `ROL` (Rotate Left), `ROR` (Rotate Right), `RCL` (Rotate Carry Left), `RCR` (Rotate Carry Right).

**Unified Operand Rules:**

* **Dest:** Reg, Mem (8/16/32)
* **Count:** Imm8 (constant) or `CL` register.

| Format | Example |
| --- | --- |
| `Op Reg, Imm8` | `SHL EAX, 4` |
| `Op Mem, Imm8` | `SAR BYTE PTR [var], 1` |
| `Op Reg, CL` | `ROR BX, CL` |
| `Op Mem, CL` | `RCL DWORD PTR [var], CL` |

**Double Precision Shifts (SHLD, SHRD)**
Shifts Destination while filling empty bits from a Source operand (Src is not modified).

* **Format:** `SHLD Dest, Src, Count`

| Format | Example |
| --- | --- |
| `SHLD Reg16/32, Reg16/32, Imm8` | `SHLD AX, BX, 4` |
| `SHLD Mem16/32, Reg16/32, Imm8` | `SHLD [var], EAX, 2` |
| `SHLD Reg16/32, Reg16/32, CL` | `SHLD AX, BX, CL` |
| `SHLD Mem16/32, Reg16/32, CL` | `SHLD [var], EBX, CL` |

---

### **Arithmetic Instructions**

#### **MUL (Unsigned) & DIV (Unsigned)**

Takes **one operand** (the source). Implicit registers determine the other side.

* **Src:** Reg, Mem (8/16/32). **No Immediate operands allowed.**

| Format | Example |
| --- | --- |
| `MUL Reg8/16/32` | `MUL BL` / `MUL BX` / `MUL EBX` |
| `MUL Mem8/16/32` | `MUL BYTE PTR [var]` |
| `DIV Reg8/16/32` | `DIV CX` |
| `DIV Mem8/16/32` | `DIV DWORD PTR [var]` |

#### **IMUL (Signed Multiply)**

Has three distinct variations.

**1. One-Operand Format (Standard)**
Behaves exactly like `MUL` (uses EDX:EAX for 32-bit), but signed.

* `IMUL Reg/Mem`

**2. Two-Operand Format**
`IMUL Dest, Src`
Multiplies Dest by Src. Result is truncated to fit in Dest.

* **Dest:** Reg16/32
* **Src:** Reg16/32, Mem16/32, Imm8/16/32
* *Note:* No 8-bit operands allowed here.

| Format | Example |
| --- | --- |
| `IMUL Reg, Reg` | `IMUL EAX, EBX` |
| `IMUL Reg, Mem` | `IMUL AX, [var]` |
| `IMUL Reg, Imm` | `IMUL BX, 10` |

**3. Three-Operand Format**
`IMUL Dest, Src, Imm`
Multiplies Src by Imm, stores result in Dest. Result truncated.

* **Dest:** Reg16/32
* **Src:** Reg16/32, Mem16/32
* **Imm:** Immediate value

| Format | Example |
| --- | --- |
| `IMUL Reg, Reg, Imm` | `IMUL EAX, EBX, 5` |
| `IMUL Reg, Mem, Imm` | `IMUL CX, [var], 100` |