# MASM Irvine32 Procedure Reference Notes

Assumes: `INCLUDE Irvine32.inc` (and linking with Irvine32 library).  
Only `.data` and `.code` segments are shown in examples (no prologue/epilogue macros or INCLUDE lines for brevity).

Format per entry:
- Name
- Plain English description
- How to use (steps / notes)
- Register roles (inputs / outputs)
- Example (minimal usage)

---

## 1. Clrscr
Clears the console screen and moves cursor to (0,0).
**How to use:** Just call `Clrscr` before output to reset the screen.
**Registers:** None required; none returned.
```asm
.data

.code
main PROC
    call Clrscr
    ; Continue with output...
    ret
main ENDP
```
**Expected output:** Console screen clears completely and cursor moves to top-left corner.

---

## 2. Crlf
**Plain English description:** Prints newline (carriage return + line feed).
**How to use:** Call after output to advance to next line.
**Registers:** None.
```asm
.data

.code
main PROC
    call Crlf
    ret
main ENDP
```
**Expected output:** Cursor moves to beginning of next line.

---

## 3. WriteBin
**Plain English description:** Displays unsigned 32-bit value in EAX as binary.
**How to use:** Load EAX with value, call.
**Registers:** Input EAX = value to print.
```asm
.data

.code
main PROC
    mov eax, 10101010b
    call WriteBin
    ret
main ENDP
```
**Expected output:** "0000 0000 0000 0000 0000 0000 1010 1010" (spaced binary representation)

---

## 4. WriteChar
**Plain English description:** Writes character in AL.
**How to use:** Put ASCII code in AL, call.
**Registers:** Input AL = character.
```asm
.data

.code
main PROC
    mov al, 'A'
    call WriteChar
    ret
main ENDP
```
**Expected output:** "A" (the character 'A' displayed)

---

## 5. WriteDec
**Plain English description:** Writes unsigned decimal in EAX.
**How to use:** Load EAX with value, call.
**Registers:** Input EAX = unsigned number.
```asm
.data

.code
main PROC
    mov eax, 12345
    call WriteDec
    ret
main ENDP
```
**Expected output:** "12345"

---

## 6. WriteHex
**Plain English description:** Writes 32-bit hex value in EAX (uppercase).
**How to use:** Load EAX, call.
**Registers:** Input EAX = value.
```asm
.data

.code
main PROC
    mov eax, 0ABCDEF12h
    call WriteHex
    ret
main ENDP
```
**Expected output:** "ABCDEF12"

---

## 7. WriteInt
**Plain English description:** Writes signed decimal in EAX.
**How to use:** Put signed value in EAX.
**Registers:** Input EAX = signed integer.
```asm
.data

.code
main PROC
    mov eax, -1234
    call WriteInt
    ret
main ENDP
```
**Expected output:** "-1234"

---

## 8. WriteString
**Plain English description:** Writes null-terminated string at EDX.
**How to use:** Set EDX = OFFSET string, call.
**Registers:** Input EDX = address of string.
```asm
.data
msg BYTE "Hello, Irvine32!",0

.code
main PROC
    mov edx, OFFSET msg
    call WriteString
    ret
main ENDP
```
**Expected output:** "Hello, Irvine32!"

---

## 9. ReadChar
**Plain English description:** Reads one key, returns ASCII in AL (no echo suppression).
**How to use:** Call; AL gets character.
**Registers:** Output AL = character.
```asm
.data

.code
main PROC
    call ReadChar
    ; AL now contains key
    ret
main ENDP
```
**Expected output:** Program waits for single keypress, character stored in AL (not displayed)

---

## 10. ReadDec
**Plain English description:** Reads unsigned decimal number from keyboard.
**How to use:** Call; user types digits + Enter.
**Registers:** Output EAX = unsigned value.
```asm
.data

.code
main PROC
    call ReadDec
    ; EAX now has number
    ret
main ENDP
```
**Expected output:** Program waits for numeric input, stores unsigned integer in EAX

---

## 11. ReadHex
**Plain English description:** Reads hexadecimal number (0–9, A–F).
**How to use:** Call; type hex + Enter.
**Registers:** Output EAX = value.
```asm
.data

.code
main PROC
    call ReadHex
    ; EAX has parsed hex number
    ret
main ENDP
```
**Expected output:** Program waits for hexadecimal input, stores converted value in EAX

---

## 12. ReadInt
**Plain English description:** Reads signed decimal number.
**How to use:** Call; may include leading '-' sign.
**Registers:** Output EAX = signed value.
```asm
.data

.code
main PROC
    call ReadInt
    ; EAX has signed integer
    ret
main ENDP
```
**Expected output:** Program waits for signed integer input, stores value in EAX

---

## 13. ReadString
**Plain English description:** Reads line of text (up to ECX chars). Stores null terminator. Returns length.
**How to use:** EDX=buffer, ECX=max (not counting terminator). Call.
**Registers:** Input EDX = buffer address, ECX = max count. Output EAX = chars read (not including null).
```asm
.data
buffer BYTE 51 DUP(0)   ; Room for 50 chars + null

.code
main PROC
    mov edx, OFFSET buffer
    mov ecx, 50
    call ReadString
    ; EAX = length, buffer = text
    ret
main ENDP
```
**Expected output:** Program waits for text input, stores string in buffer and length in EAX

---

## 14. Delay
**Plain English description:** Pauses execution for EAX milliseconds.
**How to use:** Set EAX to delay interval; call.
**Registers:** Input EAX = ms delay.
```asm
.data

.code
main PROC
    mov eax, 1000    ; 1 second
    call Delay
    ret
main ENDP
```
**Expected output:** Program pauses for 1 second with no visible output

---

## 15. Randomize
**Plain English description:** Seeds pseudo-random generator with system time.
**How to use:** Call once near program start.
**Registers:** None required.
```asm
.data

.code
main PROC
    call Randomize
    ; Now use RandomRange / Random32 etc.
    ret
main ENDP
```
**Expected output:** No visible output, random number generator is seeded

---

## 16. DumpRegs
**Plain English description:** Displays register state (EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP, EIP, EFLAGS).
**How to use:** Call at point of interest.
**Registers:** Inputs none; outputs to console.
```asm
.data

.code
main PROC
    mov eax, 12345678h
    mov ecx, 0DEADh
    call DumpRegs
    ret
main ENDP
```
**Expected output:** Table showing all register values including EAX=12345678h, ECX=0000DEADh

---

## 17. DumpMem
**Plain English description:** Displays memory block in hex (and possibly ASCII).
**How to use:** ESI=start, ECX=length, EBX=type (1=byte, 2=word, 4=dword).
**Registers:** Input ESI, ECX, EBX.
```asm
.data
arr BYTE 11h,22h,33h,44h,55h,66h

.code
main PROC
    mov esi, OFFSET arr
    mov ecx, LENGTHOF arr
    mov ebx, 1          ; bytes
    call DumpMem
    ret
main ENDP
```
**Expected output:** Hexadecimal dump showing memory contents: 11 22 33 44 55 66

---

## 18. GetDateTime
**Plain English description:** Gets current system date/time (via Windows API through Irvine library). Store into SYSTEMTIME structure (if using direct API with INVOKE).
**How to use:** Define SYSTEMTIME struct; call `GetLocalTime` (or provided wrapper if available). Then read fields.
**Registers:** (Wrapper itself: no specific register returns; data stored in struct).
```asm
.data
systime SYSTEMTIME <>

.code
main PROC
    INVOKE GetLocalTime, ADDR systime
    ; Fields: systime.wYear, wMonth, wDay, wHour, ...
    ret
main ENDP
```
**Expected output:** No console output, SYSTEMTIME structure filled with current date/time values

---

## 19. GetMaxXY
**Plain English description:** Returns console buffer size.
**How to use:** Call; then read DX (columns), AX (rows).
**Registers:** Output DX=columns, AX=rows.
```asm
.data
cols WORD ?
rows WORD ?

.code
main PROC
    call GetMaxXY
    mov cols, dx
    mov rows, ax
    ret
main ENDP
```
**Expected output:** No visible output, DX and AX contain console dimensions

---

## 20. GetTextColor
**Plain English description:** Returns current text color attributes.
**How to use:** Call; AL foreground (0–15), AH background (0–7).
**Registers:** Output AL=FG, AH=BG.
```asm
.data
fg BYTE ?
bg BYTE ?

.code
main PROC
    call GetTextColor
    mov fg, al
    mov bg, ah
    ret
main ENDP
```
**Expected output:** No visible output, AL and AH contain current color attributes

---

## 21. Gotoxy
**Plain English description:** Moves cursor to (row, col).
**How to use:** DH=row (Y), DL=col (X), call.
**Registers:** Input DH=row, DL=col.
```asm
.data
msg BYTE "At (10,5)",0

.code
main PROC
    mov dh, 5
    mov dl, 10
    call Gotoxy
    mov edx, OFFSET msg
    call WriteString
    ret
main ENDP
```
**Expected output:** "At (10,5)" appears at column 10, row 5 of console

---

## 22. MsgBox
**Plain English description:** Displays message box with title (OK button).
**How to use:** EDX=message string, EBX=title string, call.
**Registers:** Input EDX=message, EBX=title.
```asm
.data
mtext  BYTE "Hello from MASM!",0
mtitle BYTE "Info",0

.code
main PROC
    mov edx, OFFSET mtext
    mov ebx, OFFSET mtitle
    call MsgBox
    ret
main ENDP
```
**Expected output:** Windows message box with title "Info" and message "Hello from MASM!"

---

## 23. MsgBoxAsk
**Plain English description:** Yes/No message box, returns selection.
**How to use:** Set EDX, EBX; call; check EAX (6=Yes, 7=No).
**Registers:** Input EDX=question, EBX=title. Output EAX=6/7.
```asm
.data
qtext  BYTE "Continue operation?",0
qtitle BYTE "Confirm",0

.code
main PROC
    mov edx, OFFSET qtext
    mov ebx, OFFSET qtitle
    call MsgBoxAsk
    cmp eax, 6
    jne noPath
    ; Yes chosen
    jmp done
noPath:
    ; No chosen
done:
    ret
main ENDP
```
**Expected output:** Yes/No message box with user's choice returned in EAX

---

## 24. SetTextColor
**Plain English description:** Sets console text colors.
**How to use:** EAX = FG + (BG * 16). Call before output.
**Registers:** Input EAX = attribute byte.
```asm
.data
msg BYTE "Yellow on Blue",0

.code
main PROC
    mov eax, (1 * 16) + 0Eh    ; BG=1 (blue), FG=0Eh (yellow)
    call SetTextColor
    mov edx, OFFSET msg
    call WriteString
    ret
main ENDP
```
**Expected output:** "Yellow on Blue" displayed in yellow text on blue background

---

## 25. WaitMsg
**Plain English description:** Displays "Press Enter to continue..." (or similar) and waits for Enter.
**How to use:** Call at pause points.
**Registers:** None.
```asm
.data

.code
main PROC
    call WaitMsg
    ret
main ENDP
```
**Expected output:** "Press [Enter] to continue..." message displayed, program waits for Enter key

---

### Color Attribute Quick Reference (Foreground / Background)
Values: 0=Black 1=Blue 2=Green 3=Cyan 4=Red 5=Magenta 6=Brown 7=LightGray 8=DarkGray 9=LightBlue 0Ah=LightGreen 0Bh=LightCyan 0Ch=LightRed 0Dh=LightMagenta 0Eh=Yellow 0Fh=White

---

### Notes
- Always initialize required registers before calling each procedure.
- Some procedures (e.g., `DumpRegs`, `DumpMem`) are primarily for debugging.
- For RNG after `Randomize`, use `RandomRange` or `Random32`.
- `ReadString` truncates input if user exceeds ECX limit (remaining chars discarded).
- `GetDateTime` example uses Windows API; if your Irvine32 version includes a wrapper named `GetDateTime`, adapt accordingly.

End of reference.
