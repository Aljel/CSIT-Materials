.model small
.stack 100h
.386

.data 
Names db 'Alexander Zhelezko 251', 0Dh, 0Ah, '$'

.code

start:
mov AX, @data 
mov DS, AX
mov DX, offset Names
mov AH, 09h
int 21h

mov EAX, 65536 
mov EBX, 10 
mov CX, 0 ;обнуление счётчика

breakBigNum: ;заносим в стек
  inc CX ;увеличение счётчика
  mov EDX, 0 ;очищение значения EDX
  div EBX ;деление EAX на EBX
  push EDX ;занесение остатка от деление в стек
  cmp EAX, 0 ;сравнение частного с нулем
  jne breakBigNum ;если EAX != 0, то возвращаемся к breakBigNum

printNum: 
  pop EDX 
  call printElem 
  loop printNum

mov EAX, 4C00h
int 21h

printElem proc
  push EAX
  mov AH, 02h
  add EDX, 30h 
  int 21h 
  pop EAX
  ret 
printElem endp

end start