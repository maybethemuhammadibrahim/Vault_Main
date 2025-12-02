INCLUDE Irvine32.inc

.data
    arr DWORD 3,5,7,11
    
.code
main PROC
    mov ecx, LENGTHOF arr
    mov esi, OFFSET arr
    
L1:
    push [esi]
    call checkPrime
    add esi, TYPE arr
    
    cmp eax, 0
    je notPrimeLabel
    
    loop L1
    
    ; If loop finishes, all are prime
    mov eax, 1
    call WriteInt
    jmp finish
    
notPrimeLabel:
    mov eax, 0
    call WriteInt
    
finish:
    exit
main ENDP

checkPrime PROC
    push ebp
    mov ebp, esp
    ; Save registers used in main or modified here
    push ecx
    push ebx
    push edx    ; Div changes EDX, good practice to save
    
    numLocal EQU [ebp + 8]
    
    mov eax, numLocal
    cmp eax, 1
    jbe notPrimeProc
    cmp eax, 2
    je isPrime
    
    ; Check if even
    test eax, 1
    jz notPrimeProc
    
    ; --- LOGIC FIX START ---
    ; Use a compare-based loop.
    ; We need to stop when divisor > num/2.
    
    mov ebx, 3          ; Start divisor
    
checkLoop:
    mov eax, numLocal
    mov edx, 0
    mov ecx, 2
    div ecx             ; EAX = num/2 (The Limit)
    
    cmp ebx, eax        ; Is Divisor > Limit (num/2)?
    ja isPrime          ; If yes, we are done. It's prime.
    
    mov eax, numLocal
    mov edx, 0
    div ebx             ; Divide num by current divisor
    cmp edx, 0
    je notPrimeProc     ; Remainder 0? Not prime.
    
    add ebx, 2          ; Next odd divisor
    jmp checkLoop       ; Manual jump instead of LOOP
    
    ; --- LOGIC FIX END ---

isPrime:
    mov eax, 1
    jmp done
    
notPrimeProc:
    mov eax, 0
    
done:
    ; Restore registers in REVERSE order of pushing
    pop edx
    pop ebx
    pop ecx
    
    mov esp, ebp
    pop ebp
    ret 4
checkPrime ENDP

END main
