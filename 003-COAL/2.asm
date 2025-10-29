INCLUDE Irvine32.inc

.data
    msgInput BYTE "Enter any 8-bit value: ",0
    msgResult BYTE "Encrypted value: ",0
    value BYTE ?

.code
main PROC
    mov edx, OFFSET msgInput
    call WriteString
    call ReadInt          
    mov value, al         

    mov al, value
    rol al, 2             ;rotate left twice
    shr al, 1             ;shift right once
    mov value, al

    mov edx, OFFSET msgResult
    call WriteString
    movzx eax, value
    call WriteDec
    call CrLf

    exit
main ENDP
END main
