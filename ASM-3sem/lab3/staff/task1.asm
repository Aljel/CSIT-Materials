.model small
.stack 100h

.data 
Names db 'Alexander Zhelezko 251', 0Dh, 0Ah, '$'

.code

start:
mov AX, @data 
mov DS, AX
mov DX, offset Names
mov AH, 09h
int 21h
mov AX, 1500 ; занесение числа
mov BX, 10 ; занесение делителя
mov CX, 0 ; обнуляем счётчик

breakNum: ; заносим в стек цифры
  inc CX ; увеличение счётчика
  mov DX, 0 ; очищение значения DX
  div BX ; деление AX на BX
  push DX ; занесение остатка от деление в стек
  cmp AX, 0 ; сравнение частного с нулем
  jne breakNum ; если AX != 0, то возвращаемся к breakNum

printNum:
  pop DX
  call printElem
  loop printNum

mov AX, 4C00h
int 21h

printElem proc
  push AX
  mov AH, 02h
  add DL, 30h 
  int 21h 
  pop AX
  ret 
printElem endp

end start