INCLUDE Irvine32.inc

.data
    stri BYTE "####FAST", 0
    chari BYTE "#", 0

.code
strTrim PROC, 
    str1:DWORD, len1:DWORD, charPtr:DWORD

    mov esi, str1      ; Point to string
    mov ecx, len1      ; Loop counter
    
    ; Get the character to compare (dereference pointer)
    mov ebx, charPtr   ; ebx = address of chari
    mov dl, [ebx]      ; dl = actual character '#'
    cld
loop1:
    lodsb              ; Loads byte at [ESI] into AL, inc ESI
    cmp al, dl         ; Compare AL with the delimiter (DL)
    jne endLoop
    loop loop1

endLoop:
   
    mov eax, len1
    sub eax, ecx            

    mov esi, str1
    add esi, eax
    mov edi, str1
    mov ecx, len1
    cld
    rep movsb

    mov edx, str1

    
    ret
strTrim ENDP

main PROC
    ; Use INVOKE for procedures with named parameters
    INVOKE strTrim, OFFSET stri, LENGTHOF stri, OFFSET chari
    exit
main ENDP
END main
