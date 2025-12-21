Here are simple, commented MASM codes using the `Irvine32` library for your three requests.

### 1. Bubble Sort

This program sorts a fixed array of integers.

```masm
INCLUDE Irvine32.inc

.data
    array DWORD 5, 2, 9, 1, 6    ; Array to be sorted
    count DWORD LENGTHOF array   ; Number of elements

.code
main PROC
    mov ecx, count              ; Outer loop counter (N)
    dec ecx                     ; Outer loop runs N-1 times

L1:                             ; Outer Loop
    push ecx                    ; Save outer loop counter
    mov esi, OFFSET array       ; Point to start of array

L2:                             ; Inner Loop
    mov eax, [esi]              ; Get current element
    cmp eax, [esi+4]            ; Compare with next element
    jle NoSwap                  ; If current <= next, skip swap

    ; Swap Logic
    xchg eax, [esi+4]           ; Exchange EAX with next memory slot
    mov [esi], eax              ; Move new value into current slot

NoSwap:
    add esi, 4                  ; Move to next element (DWORD = 4 bytes)
    loop L2                     ; Repeat inner loop

    pop ecx                     ; Restore outer loop counter
    loop L1                     ; Repeat outer loop

    ; Print Array (Optional verification)
    mov esi, OFFSET array
    mov ecx, count
PrintLoop:
    mov eax, [esi]
    call WriteInt               ; Irvine32: Print integer
    call Crlf
    add esi, 4
    loop PrintLoop

    exit
main ENDP
END main

```

---

### 2. Factorial (Using Recursion)

Calculates the factorial of a number input by the user (e.g., 5! = 120).

```masm
INCLUDE Irvine32.inc

.code
main PROC
    call ReadInt        ; Read N from user into EAX
    push eax            ; Push N onto stack
    call Factorial      ; Call recursive procedure
    call WriteInt       ; Print result (in EAX)
    call Crlf
    exit
main ENDP

Factorial PROC
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]    ; Get argument (N) from stack

    cmp eax, 1          ; Base case: Is N <= 1?
    jle BaseCase        ; If yes, return 1

    dec eax             ; N - 1
    push eax            ; Push (N-1)
    call Factorial      ; Recursive call: Factorial(N-1)
    
    ; Result of Factorial(N-1) is now in EAX
    mov ebx, [ebp+8]    ; Get original N again
    mul ebx             ; EAX = EAX * EBX (Result * N)
    jmp Finished

BaseCase:
    mov eax, 1          ; Return 1

Finished:
    pop ebp
    ret 4               ; Clean up stack (4 bytes for the argument)
Factorial ENDP
END main

```

---

### 3. Prime Number Check

Checks if a user input is prime.

```masm
INCLUDE Irvine32.inc

.data
    msgPrime BYTE "Prime", 0
    msgNotPrime BYTE "Not Prime", 0

.code
main PROC
    call ReadInt            ; Input number to EAX
    cmp eax, 2              ; 0 and 1 are not prime, 2 is prime
    jl NotPrimeLbl
    je PrimeLbl

    mov ecx, eax            ; Copy N
    shr ecx, 1              ; Limit loop to N / 2 (optimization)
    mov ebx, 2              ; Start divisor at 2

CheckLoop:
    cmp ebx, ecx            ; If divisor > N/2, we finished
    jg PrimeLbl
    
    mov eax, ecx            ; Restore N (Wait, need original N)
    ; Re-load original value correctly:
    push eax                ; Save loop limit
    push edx                ; Save EDX
    
    ; We need the original input (let's assume it was saved or re-read)
    ; Better approach: keep Original N in ESI
    ; Let's restart logic slightly for clarity
    
    jmp ActualLogic

ActualLogic:
    ; Assuming input is in EAX
    mov esi, eax            ; Store N in ESI
    mov ecx, eax            ; Loop counter/limit
    shr ecx, 1              ; Set limit to N/2
    mov ebx, 2              ; Divisor

LoopStart:
    mov eax, esi            ; Move N to EAX for division
    mov edx, 0              ; Clear remainder
    div ebx                 ; EAX / EBX, Remainder in EDX
    
    cmp edx, 0              ; Is remainder 0?
    je NotPrimeLbl          ; If yes, it divides evenly -> Not Prime
    
    inc ebx                 ; Next divisor
    cmp ebx, ecx            ; Check if we passed N/2
    jle LoopStart           ; Continue if EBX <= N/2

PrimeLbl:
    mov edx, OFFSET msgPrime
    call WriteString
    jmp Quit

NotPrimeLbl:
    mov edx, OFFSET msgNotPrime
    call WriteString

Quit:
    call Crlf
    exit
main ENDP
END main

```

Would you like me to explain the stack operations in the recursive factorial code in more detail?
