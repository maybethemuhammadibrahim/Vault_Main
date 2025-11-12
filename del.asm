1

2
INCLUDE Irvine32.inc

;M Ibrahim
;24K-0649

.data
    var1 DWORD 5
    var2 DWORD 6
    yParam EQU [ebp + 12]
    xParam EQU [ebp + 8]

.code
main PROC
    push var2
    push var1
    call AddTwo
    call writeint
    exit
main ENDP

AddTwo PROC
    push ebp
    mov ebp, esp
    mov eax, yParam
    add eax, xParam
    pop ebp
    ret
AddTwo ENDP
END main

3
INCLUDE Irvine32.inc

.data
    count = 10
    arr WORD count DUP(0)

.code
main PROC
    push OFFSET arr
    push count
    call ArrayFill
    exit
main ENDP

ArrayFill PROC
    push ebp
    mov ebp, esp
    pushad
    mov esi, [ebp + 12]
    mov ecx, [ebp + 8]
    cmp ecx, 0
    je L2
L1:
    mov eax, 100h
    call RandomRange
    mov [esi], ax
    add esi, TYPE WORD
    loop L1
L2:
    popad
    pop ebp
    ret 8
ArrayFill ENDP
END main







4
INCLUDE Irvine32.inc
;Muhammad Ibrahim
;24K-0649

.data
	count = 10
	arr WORD count DUP(0)

.code
main PROC
	call makeArray
exit
main ENDP

makeArray PROC
	push ebp
	mov ebp, esp
	sub esp, 32            
	lea esi, [ebp - 30]    
	mov ecx, 30            

L1:
	mov BYTE PTR [esi], '*'  
	inc esi                  
	loop L1                  

	add esp, 32            
	pop ebp                
ret
makeArray ENDP
END main











5
INCLUDE Irvine32.inc
;Muhammad Ibrahim
;24K-0649

.data
	var1 DWORD 5
	var2 DWORD 6	
.code
main PROC
	call MySub
exit
main ENDP

MySub PROC
	push ebp
	mov ebp, esp
	sub esp, 88             
	mov DWORD PTR [ebp - 4], 10    
	mov DWORD PTR [ebp - 8], 20    
	mov esp, ebp            
	pop ebp                 
	ret
MySub ENDP
END main







6
INCLUDE Irvine32.inc
;Muhammad Ibrahim
;24K-0649

.data
	var1 DWORD 5
	var2 DWORD 6	
.code
main PROC
	push var2
	push var1
	call AddTwo
	call writeint
	exit
main ENDP

AddTwo PROC
	enter 0, 0
	mov eax, [ebp + 12]     
	add eax, [ebp + 8]      
	leave
	ret
AddTwo ENDP
END main

7
INCLUDE Irvine32.inc
;Muhammad Ibrahim
;24K-0649

.data
	
.code
main PROC
	call LocalProc
	call writeint
	exit
main ENDP

LocalProc PROC
	LOCAL temp : DWORD
	mov temp, 55
	mov eax, temp
	ret
LocalProc ENDP
END main






8
INCLUDE Irvine32.inc
;Muhammad Ibrahim
;24K-0649

.data
	
.code
main PROC
	mov ecx, 5
	mov eax, 0
	call CalcSum
L1:
	call WriteDec
	call crlf
	exit
main ENDP

CalcSum PROC
	cmp ecx, 0
	jz L2
	add eax, ecx
	dec ecx
	call CalcSum
	L2:
	ret
	CalcSum ENDP
END main
