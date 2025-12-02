# x86 String Primitives - Complete Guide

## Overview

String primitives are specialized x86 instructions designed for **efficient bulk memory operations**. They operate on contiguous memory blocks (arrays, strings, buffers) and are significantly faster than manual loops for large data transfers.

**Key Advantage:** Hardware-optimized for memory-to-memory operations with automatic pointer updates.

---

## I. The Foundation: Registers and Flags

Every string primitive relies on this specific register/flag set:

### Critical Registers

| Register | Full Name | Purpose | Used By |
|----------|-----------|---------|---------|
| **ESI** | Extended Source Index | Points to **source** memory (data being read) | MOVS, CMPS, LODS |
| **EDI** | Extended Destination Index | Points to **destination** memory (data being written) | MOVS, CMPS, SCAS, STOS |
| **ECX** | Extended Counter | Loop counter for REP prefixes | All (with REP) |
| **AL/AX/EAX** | Accumulator | Data register for single-operand operations | SCAS, STOS, LODS |

### The Direction Flag (DF)

Controls whether pointers increment (forward) or decrement (backward).

| DF State | Value | Effect | Set By | Behavior |
|----------|-------|--------|--------|----------|
| **Clear** | 0 | **Forward** processing | `CLD` | ESI/EDI **increment** after each operation |
| **Set** | 1 | **Backward** processing | `STD` | ESI/EDI **decrement** after each operation |

**Critical Rule:** Always explicitly set DF with `CLD` or `STD` before string operations. DF state is **not guaranteed** after function calls or interrupts.

```asm
; WRONG - Assumes DF is clear
mov esi, OFFSET source
mov edi, OFFSET dest
rep movsb                   ; May process backward if DF=1!

; CORRECT - Explicitly set direction
cld                         ; Clear DF (forward processing)
mov esi, OFFSET source
mov edi, OFFSET dest
rep movsb                   ; Guaranteed forward processing
```

---

## II. Size Suffixes and Auto-Increment

The suffix determines operand size and how much ESI/EDI change after each operation.

| Suffix | Full Name | Size | Accumulator | ESI/EDI Change (DF=0) | ESI/EDI Change (DF=1) |
|--------|-----------|------|-------------|----------------------|----------------------|
| **B** | Byte | 1 byte | **AL** | +1 | -1 |
| **W** | Word | 2 bytes | **AX** | +2 | -2 |
| **D** | Doubleword | 4 bytes | **EAX** | +4 | -4 |

### Examples

```asm
; MOVSB - Moves 1 byte, increments ESI/EDI by 1
cld
mov esi, OFFSET source
mov edi, OFFSET dest
movsb                       ; [EDI] = [ESI], ESI++, EDI++

; MOVSW - Moves 2 bytes, increments ESI/EDI by 2
movsw                       ; [EDI:EDI+1] = [ESI:ESI+1], ESI+=2, EDI+=2

; MOVSD - Moves 4 bytes, increments ESI/EDI by 4
movsd                       ; [EDI:EDI+3] = [ESI:ESI+3], ESI+=4, EDI+=4
```

---

## III. The Instructions

### 1. MOVSB/MOVSW/MOVSD - Move String

**Purpose:** Memory-to-memory copy operation.

**Register Usage:**
- **ESI** (Required) - Source address (data being read)
- **EDI** (Required) - Destination address (data being written)
- **ECX** (With REP) - Number of elements to copy
- **DF** (Required) - Direction control

**Typical Usage Pattern:**
```asm
cld                         ; Direction: forward
mov esi, OFFSET source      ; ESI = address of source
mov edi, OFFSET dest        ; EDI = address of destination
mov ecx, count              ; ECX = number of elements
rep movsb                   ; Repeat MOVSB ECX times
```

#### Example 1: Copy String Forward

```asm
.data
source BYTE "Hello World", 0
dest   BYTE 12 DUP(?)

.code
main PROC
    cld                     ; Forward direction
    mov esi, OFFSET source  ; ESI → "Hello World"
    mov edi, OFFSET dest    ; EDI → destination buffer
    mov ecx, 12             ; Copy 12 bytes (including null)
    rep movsb               ; Repeat: [EDI++] = [ESI++]
    
    ; After execution:
    ; dest = "Hello World", 0
    ; ESI points 12 bytes past source
    ; EDI points 12 bytes past dest
    ; ECX = 0
    
    mov edx, OFFSET dest
    call WriteString        ; Output: "Hello World"
    ret
main ENDP
```

#### Example 2: Copy Array Backward (Overlapping Memory)

```asm
.data
buffer BYTE "ABCDEFGH", 0

.code
; Shift string 2 positions right: "ABCDEFGH" → "XXABCDEF"
main PROC
    std                     ; Backward direction (critical for overlap)
    mov esi, OFFSET buffer + 5  ; Start from 'F'
    mov edi, OFFSET buffer + 7  ; Write to position of 'H'
    mov ecx, 6              ; Move 6 characters
    rep movsb               ; Copy backward
    
    ; Result: "ABABCDEF", 0
    ; (First two chars duplicated)
    
    cld                     ; Restore forward direction
    ret
main ENDP
```

**Why Backward?** When source and destination overlap, copying backward prevents overwriting source data before it's copied.

#### Example 3: Copy DWORD Array

```asm
.data
sourceArray DWORD 10, 20, 30, 40, 50
destArray   DWORD 5 DUP(?)

.code
main PROC
    cld
    mov esi, OFFSET sourceArray
    mov edi, OFFSET destArray
    mov ecx, 5              ; 5 DWORDs
    rep movsd               ; Each iteration copies 4 bytes
    
    ; After: destArray = {10, 20, 30, 40, 50}
    ; Total bytes copied: 5 * 4 = 20 bytes
    ret
main ENDP
```

---

### 2. CMPSB/CMPSW/CMPSD - Compare Strings

**Purpose:** Compare two memory blocks byte-by-byte/word-by-word/dword-by-dword.

**Register Usage:**
- **ESI** (Required) - First operand address
- **EDI** (Required) - Second operand address
- **ECX** (With REP) - Maximum comparisons
- **DF** (Required) - Direction control
- **FLAGS** (Output) - Comparison result

**Flags Affected:**
- **ZF** (Zero Flag) - Set if bytes are equal
- **CF** (Carry Flag) - Set if [ESI] < [EDI] (unsigned)
- **SF** (Sign Flag) - Set if [ESI] < [EDI] (signed)

**Typical Usage Pattern:**
```asm
cld
mov esi, OFFSET string1
mov edi, OFFSET string2
mov ecx, length
repe cmpsb                  ; Repeat while equal (ZF=1)
; After: ZF=1 if all equal, ZF=0 if mismatch found
```

#### Example 1: String Equality Check

```asm
.data
password BYTE "SECRET", 0
input    BYTE "SECRET", 0
msgEqual BYTE "Access Granted", 0
msgNotEqual BYTE "Access Denied", 0

.code
main PROC
    cld
    mov esi, OFFSET password
    mov edi, OFFSET input
    mov ecx, 6              ; Compare 6 characters
    repe cmpsb              ; Repeat while equal
    
    ; After REPE CMPSB:
    ; If strings match: ZF=1, ECX=0
    ; If mismatch: ZF=0, ECX = remaining count
    
    jne not_equal
    mov edx, OFFSET msgEqual
    jmp done
    
not_equal:
    mov edx, OFFSET msgNotEqual
    
done:
    call WriteString
    ret
main ENDP
```

#### Example 2: Lexicographic Comparison (Alphabetical Order)

```asm
.data
source BYTE "MARTIN"
dest   BYTE "MARTINEZ"
str1   BYTE "Source is smaller", 0dh, 0ah, 0
str2   BYTE "Source is not smaller", 0dh, 0ah, 0

.code
main PROC
    cld
    mov esi, OFFSET source
    mov edi, OFFSET dest
    mov ecx, LENGTHOF source    ; Compare 6 bytes
    repe cmpsb                  ; Compare until mismatch or end
    
    ; After REPE CMPSB:
    ; Stopped at position 6 where source ends
    ; Last comparison: 0 (source null) vs 'E' (dest[6])
    ; 0 < 'E', so CF=1
    
    jb source_smaller           ; Jump if CF=1 (unsigned below)
    mov edx, OFFSET str2        ; "source is not smaller"
    jmp done
    
source_smaller:
    mov edx, OFFSET str1        ; "source is smaller"
    
done:
    call WriteString
    ret
main ENDP
```

**Key Point:** `REPE` continues while `ZF=1` (bytes equal) and `ECX > 0`. When a mismatch occurs, flags reflect the **last comparison**.

#### Example 3: Find First Difference

```asm
.data
str1 BYTE "HELLO WORLD"
str2 BYTE "HELLO EARTH"
msg  BYTE "Difference at position: ", 0

.code
main PROC
    cld
    mov esi, OFFSET str1
    mov edi, OFFSET str2
    mov ecx, 11
    repe cmpsb
    
    ; Find position of difference
    mov eax, 11
    sub eax, ecx            ; Position = total - remaining
    dec eax                 ; Adjust for 0-based index
    
    ; EAX = 6 (position where 'W' != 'E')
    
    call WriteDec
    ret
main ENDP
```

---

### 3. SCASB/SCASW/SCASD - Scan String

**Purpose:** Search memory for a specific value.

**Register Usage:**
- **AL/AX/EAX** (Required) - Search value
- **EDI** (Required) - Memory to scan
- **ECX** (With REP) - Maximum scan count
- **DF** (Required) - Direction control
- **FLAGS** (Output) - Comparison result

**Critical Note:** ESI is **NOT used**. SCAS only uses EDI.

**Typical Usage Pattern:**
```asm
cld
mov edi, OFFSET string
mov al, search_char
mov ecx, max_length
repne scasb                 ; Repeat while not equal (ZF=0)
; After: ZF=1 if found, EDI points PAST the found byte
```

#### Example 1: Find Character in String

```asm
.data
alpha BYTE "ABCDEFGH", 0
msg1  BYTE "FOUND F", 0
msg2  BYTE "F NOT PRESENT", 0

.code
main PROC
    cld
    mov edi, OFFSET alpha   ; EDI → string to scan
    mov al, 'F'             ; AL = search value
    mov ecx, LENGTHOF alpha ; Scan 9 bytes (including null)
    repne scasb             ; Repeat while NOT equal (stop when equal)
    
    ; After REPNE SCASB:
    ; If found: ZF=1, EDI points ONE BYTE PAST 'F'
    ; If not found: ZF=0, ECX=0
    
    jnz not_found           ; Jump if ZF=0 (not found)
    
    ; Found - EDI now points past 'F', back up
    dec edi                 ; EDI now points to 'F'
    mov edx, OFFSET msg1
    call WriteString
    jmp quit

not_found:
    mov edx, OFFSET msg2
    call WriteString

quit:
    exit
main ENDP
```

**Important:** After SCASB finds a match, EDI points to the **next byte**, not the matched byte. Always `dec edi` if you need the address of the found character.

#### Example 2: Calculate String Length (strlen)

```asm
.data
myString BYTE "Hello, World!", 0

.code
main PROC
    cld
    mov edi, OFFSET myString    ; EDI → start of string
    mov al, 0                   ; AL = null terminator
    mov ecx, 0FFFFFFFFh         ; Max count (essentially unlimited)
    repne scasb                 ; Scan for null terminator
    
    ; After REPNE SCASB:
    ; EDI points ONE BYTE PAST the null terminator
    ; ECX = 0FFFFFFFFh - (length + 1)
    
    ; Calculate length
    mov eax, 0FFFFFFFFh
    sub eax, ecx                ; EAX = bytes scanned
    dec eax                     ; Subtract 1 (don't count null)
    
    ; EAX = 13 (length of "Hello, World!")
    
    call WriteDec
    ret
main ENDP
```

#### Example 3: Find Last Occurrence (Backward Scan)

```asm
.data
myString BYTE "Programming", 0  ; Find last 'g'

.code
main PROC
    std                         ; Backward direction
    mov edi, OFFSET myString + 10   ; Start from last char
    mov al, 'g'
    mov ecx, 11                 ; Scan 11 characters
    repne scasb
    
    ; If found, EDI points ONE BYTE BEFORE the 'g'
    inc edi                     ; Adjust to point at 'g'
    
    cld                         ; Restore forward direction
    ret
main ENDP
```

---

### 4. STOSB/STOSW/STOSD - Store String

**Purpose:** Fill memory with a value (like memset in C).

**Register Usage:**
- **AL/AX/EAX** (Required) - Value to store
- **EDI** (Required) - Destination address
- **ECX** (With REP) - Number of elements to store
- **DF** (Required) - Direction control

**Critical Note:** ESI is **NOT used**. STOS only uses EDI.

**Typical Usage Pattern:**
```asm
cld
mov edi, OFFSET buffer
mov al, fill_value
mov ecx, count
rep stosb                   ; Repeat: [EDI++] = AL
```

#### Example 1: Clear Buffer (Fill with Zeros)

```asm
.data
Count = 100
buffer BYTE Count DUP(?)

.code
main PROC
    cld
    mov al, 0               ; AL = fill value (zero)
    mov edi, OFFSET buffer  ; EDI → destination
    mov ecx, Count          ; Fill 100 bytes
    rep stosb               ; Repeat: [EDI++] = 0
    
    ; After: buffer = {0, 0, 0, ..., 0} (100 zeros)
    ret
main ENDP
```

#### Example 2: Fill Array with Pattern

```asm
.data
Count = 100
string1 BYTE Count DUP(?)

.code
main PROC
    cld
    mov al, 0FFh            ; AL = 0xFF (255)
    mov edi, OFFSET string1 ; EDI → destination
    mov ecx, Count          ; Fill 100 bytes
    rep stosb               ; Fill entire buffer with 0xFF
    
    ; After: string1 = {0xFF, 0xFF, ..., 0xFF}
    ret
main ENDP
```

#### Example 3: Initialize DWORD Array

```asm
.data
array DWORD 50 DUP(?)

.code
main PROC
    cld
    mov eax, 12345678h      ; EAX = initial value
    mov edi, OFFSET array   ; EDI → array
    mov ecx, 50             ; 50 DWORDs
    rep stosd               ; Fill with 0x12345678
    
    ; After: array = {0x12345678, 0x12345678, ...}
    ; Total bytes written: 50 * 4 = 200 bytes
    ret
main ENDP
```

#### Example 4: Create Patterned Buffer

```asm
.data
buffer BYTE 10 DUP(?)

.code
main PROC
    cld
    mov edi, OFFSET buffer
    
    ; Manually fill with pattern (not using REP)
    mov al, 'A'
    stosb                   ; buffer[0] = 'A'
    mov al, 'B'
    stosb                   ; buffer[1] = 'B'
    mov al, 'C'
    stosb                   ; buffer[2] = 'C'
    ; ... and so on
    
    ; Result: buffer = "ABC..."
    ret
main ENDP
```

---

### 5. LODSB/LODSW/LODSD - Load String

**Purpose:** Load data from memory into accumulator (less commonly used than others).

**Register Usage:**
- **ESI** (Required) - Source address
- **AL/AX/EAX** (Output) - Loaded value
- **ECX** (With REP) - Count (rarely used with LODS)
- **DF** (Required) - Direction control

**Critical Note:** EDI is **NOT used**. LODS only uses ESI.

**Why Rarely Used with REP:** Loading repeatedly into AL/AX/EAX overwrites the previous value, making REP pointless. LODS is typically used in manual loops for **processing** each element.

**Typical Usage Pattern:**
```asm
cld
mov esi, OFFSET source
mov ecx, count
@@loop:
    lodsb               ; AL = [ESI++]
    ; Process AL here
    loop @@loop
```

#### Example 1: Array Processing (Multiply Each Element)

```asm
INCLUDE Irvine32.inc

.data
array      DWORD 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
multiplier DWORD 10

.code
main PROC
    cld
    mov esi, OFFSET array       ; ESI → source
    mov edi, OFFSET array       ; EDI → destination (same location)
    mov ecx, LENGTHOF array     ; Loop 10 times
    
L1:
    lodsd                       ; EAX = [ESI], ESI += 4
    mul multiplier              ; EAX *= 10
    stosd                       ; [EDI] = EAX, EDI += 4
    loop L1
    
    ; After: array = {10, 20, 30, ..., 100}
    exit
main ENDP
END main
```

**Register Flow:**
1. `LODSD`: Loads `array[i]` into EAX, advances ESI
2. `MUL`: Processes value in EAX
3. `STOSD`: Stores result back, advances EDI
4. Both pointers move in lockstep

#### Example 2: Character Processing (Uppercase Conversion)

```asm
.data
source BYTE "hello world", 0
dest   BYTE 12 DUP(?)

.code
main PROC
    cld
    mov esi, OFFSET source
    mov edi, OFFSET dest
    mov ecx, 11
    
@@loop:
    lodsb                   ; AL = [ESI++]
    cmp al, 'a'
    jb @@store              ; Not lowercase, skip
    cmp al, 'z'
    ja @@store              ; Not lowercase, skip
    sub al, 32              ; Convert to uppercase
@@store:
    stosb                   ; [EDI++] = AL
    loop @@loop
    
    ; After: dest = "HELLO WORLD"
    ret
main ENDP
```

#### Example 3: Why LODS Without REP

```asm
; WRONG - REP makes no sense with LODS
rep lodsb                   ; Loads [ESI] into AL repeatedly
                            ; Only final load survives!
                            ; ESI advances, but all but last value lost

; CORRECT - Use in manual loop
@@loop:
    lodsb                   ; AL = [ESI++]
    call ProcessByte        ; Do something with AL
    loop @@loop
```

---

## IV. REP Prefix Variants

The REP prefix family controls loop behavior based on flags.

| Prefix | Full Name | Condition to Continue | Use With |
|--------|-----------|----------------------|----------|
| **REP** | Repeat | `ECX > 0` | MOVS, STOS, LODS |
| **REPE/REPZ** | Repeat While Equal/Zero | `ECX > 0 AND ZF = 1` | CMPS, SCAS |
| **REPNE/REPNZ** | Repeat While Not Equal/Not Zero | `ECX > 0 AND ZF = 0` | CMPS, SCAS |

### Operation Flow

```
; REP prefix pseudo-code
WHILE ECX > 0 DO
    Execute instruction
    Decrement ECX
    IF prefix is REPE AND ZF = 0 THEN EXIT
    IF prefix is REPNE AND ZF = 1 THEN EXIT
END WHILE
```

### When to Use Each Prefix

**REP (Unconditional Repeat):**
```asm
rep movsb               ; Copy until ECX = 0
rep stosb               ; Fill until ECX = 0
```

**REPE (Repeat While Equal):**
```asm
repe cmpsb              ; Compare until mismatch or ECX = 0
                        ; Use for: "Are these strings equal?"
```

**REPNE (Repeat While Not Equal):**
```asm
repne scasb             ; Scan until match or ECX = 0
                        ; Use for: "Find this character"
```

### Example: Exit Conditions

```asm
; REPNE SCASB exits when:
; 1. Match found (ZF=1), OR
; 2. ECX becomes 0

.data
str1 BYTE "ABCDEF", 0

.code
; Case 1: Found
mov edi, OFFSET str1
mov al, 'D'
mov ecx, 10
repne scasb             ; Stops at 'D', ZF=1, ECX=4

; Case 2: Not found
mov edi, OFFSET str1
mov al, 'Z'
mov ecx, 7
repne scasb             ; Scans all, ZF=0, ECX=0
```

---

## V. Complete Comparison Table

| Instruction | ESI Used? | EDI Used? | Accumulator Used? | Memory Operation | Typical REP Prefix |
|-------------|-----------|-----------|-------------------|------------------|--------------------|
| **MOVSB/W/D** | ✓ (Source) | ✓ (Dest) | ✗ | `[EDI] = [ESI]` | REP |
| **CMPSB/W/D** | ✓ (Op1) | ✓ (Op2) | ✗ | `[ESI] - [EDI]` | REPE, REPNE |
| **SCASB/W/D** | ✗ | ✓ (Target) | ✓ (Search value) | `AL/AX/EAX - [EDI]` | REPNE (usually) |
| **STOSB/W/D** | ✗ | ✓ (Dest) | ✓ (Store value) | `[EDI] = AL/AX/EAX` | REP |
| **LODSB/W/D** | ✓ (Source) | ✗ | ✓ (Load dest) | `AL/AX/EAX = [ESI]` | Rarely (use in loop) |

---

## VI. Common Use Cases and Patterns

### Pattern 1: Memory Copy (memcpy)

```asm
; Copy N bytes from source to dest
MemCopy PROC
    ; Assume: ESI=source, EDI=dest, ECX=count
    cld
    rep movsb
    ret
MemCopy ENDP
```

### Pattern 2: Memory Fill (memset)

```asm
; Fill N bytes with value
MemFill PROC
    ; Assume: EDI=dest, AL=value, ECX=count
    cld
    rep stosb
    ret
MemFill ENDP
```

### Pattern 3: String Length (strlen)

```asm
StrLen PROC pString:PTR BYTE
    push edi
    cld
    mov edi, pString
    mov al, 0
    mov ecx, 0FFFFFFFFh
    repne scasb             ; Find null terminator
    mov eax, 0FFFFFFFFh
    sub eax, ecx
    dec eax                 ; Length = scanned - 1
    pop edi
    ret
StrLen ENDP
```

### Pattern 4: String Compare (strcmp)

```asm
StrCmp PROC pStr1:PTR BYTE, pStr2:PTR BYTE
    push esi
    push edi
    
    mov esi, pStr1
    mov edi, pStr2
    
    ; Find length of first string
    push edi
    mov edi, esi
    mov al, 0
    mov ecx, 0FFFFFFFFh
    repne scasb
    mov ebx, 0FFFFFFFFh
    sub ebx, ecx            ; EBX = length + 1
    pop edi
    
    ; Compare
    mov ecx, ebx
    cld
    repe cmpsb
    
    ; Return value based on flags
    mov eax, 0              ; Equal
    je @@done
    mov eax, -1             ; Less than
    jb @@done
    mov eax, 1              ; Greater than
    
@@done:
    pop edi
    pop esi
    ret
StrCmp ENDP
```

### Pattern 5: In-Place Processing

```asm
; Double each element in array
DoubleArray PROC pArray:PTR DWORD, count:DWORD
    push esi
    push edi
    
    mov esi, pArray
    mov edi, pArray         ; Same location
    mov ecx, count
    cld
    
@@loop:
    lodsd                   ; Load element
    shl eax, 1              ; Multiply by 2
    stosd                   ; Store back
    loop @@loop
    
    pop edi
    pop esi
    ret
DoubleArray ENDP
```

---

## VII. Performance Considerations

### When String Primitives Excel

1. **Large Block Operations:** Copying/filling >100 bytes
2. **Sequential Access:** Processing array elements in order
3. **Simple Operations:** Direct copy, fill, or scan

### When to Use Manual Loops Instead

1. **Complex Processing:** Per-element calculations
2. **Non-Sequential Access:** Random access patterns
3. **Small Sizes:** Overhead may exceed benefit for <16 bytes

### Optimization Tips

```asm
; TIP 1: Use DWORD operations when possible
; Instead of:
mov ecx, 100
rep movsb               ; 100 iterations

; Use:
mov ecx, 25
rep movsd               ; 25 iterations (4x faster)

; TIP 2: Align addresses to 4-byte boundaries
; Misaligned MOVSD can be slower than aligned MOVSB

; TIP 3: Combine LODS/STOS for in-place transforms
@@loop:
    lodsd               ; Load
    ; Transform EAX
    stosd               ; Store
    loop @@loop
```

---

## VIII. Critical Warnings and Gotchas

### Warning 1: Direction Flag State

```asm
; WRONG - DF state unknown
call SomeFunction       ; Might set DF=1
rep movsb               ; May copy backward!

; CORRECT
call SomeFunction
cld                     ; Always set direction explicitly
rep movsb
```

### Warning 2: EDI Advancement in SCAS

```asm
repne scasb             ; Find character
; BUG: EDI now points PAST the found byte
mov byte ptr [edi], 'X' ; Modifies WRONG location!

; FIX:
repne scasb
dec edi                 ; Back up to found byte
mov byte ptr [edi], 'X' ; Correct location
```

### Warning 3: Overlapping MOVS

```asm
; Copying within same buffer
; WRONG: Forward copy with overlap
mov esi, OFFSET buffer
mov edi, OFFSET buffer + 2
mov ecx, 10
cld
rep movsb               ; Overwrites source data!

; CORRECT: Use backward copy
mov esi, OFFSET buffer + 9
mov edi, OFFSET buffer + 11
mov ecx, 10
std                     ; Backward to avoid overwrite
rep movsb
cld                     ; Restore direction
```

### Warning 4: Register Preservation

```asm
MyFunc PROC
    ; String primitives modify these registers:
    ; ESI, EDI, ECX (with REP), FLAGS
    
    ; If you need them preserved:
    push esi
    push edi
    push ecx
    
    ; String operations here
    rep movsb
    
    pop ecx
    pop edi
    pop esi
    ret
MyFunc
