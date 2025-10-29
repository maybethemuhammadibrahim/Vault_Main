INCLUDE Irvine32.inc

.data
    Marks DWORD 10 DUP(?)
    inputMSG BYTE "Enter Student Marks: ",0
    msg1 BYTE "Passed Students: ",0
    msg2 BYTE "Failed Students: ",0
    pass DWORD 0
    fail DWORD 0

.code
Count PROC
    mov ecx, LENGTHOF Marks
    mov esi, OFFSET Marks
    mov eax, 0
nextMark:
    mov ebx, [esi]
    cmp ebx, 50
    jl failLabel
    inc pass
    jmp cont
failLabel:
    inc fail
cont:
    add esi, TYPE Marks
    loop nextMark
    ret
Count ENDP

main PROC
    mov ecx, LENGTHOF Marks
    mov esi, OFFSET Marks
inputLoop:
    mov edx, OFFSET inputMSG
    call WriteString
    call ReadInt
    mov [esi], eax
    add esi, TYPE Marks
    loop inputLoop

    call Count

    mov edx, OFFSET msg1
    call WriteString
    mov eax, pass
    call WriteDec
    call CrLf

    mov edx, OFFSET msg2
    call WriteString
    mov eax, fail
    call WriteDec
    call CrLf

    exit
main ENDP
END main
