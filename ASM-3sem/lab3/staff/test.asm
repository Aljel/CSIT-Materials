.model small
.stack 100h
.data 
  result db ?
.code

start:
mov AX, 4       
mov BX, 30        

cmp AX, BX       ; Сравниваем с вторым числом
jl AX_is_less   ; Если AX <= BX, переход к метке
                 ; Если AX больше BX
mov DX, BX       ; Сохраняем BX как результат
jmp notRealEnd   ; Переход к завершению

AX_is_less:
    mov DX, AX   ; Сохраняем AX как результат

notRealEnd:
  mov AX, 4C00h
  int 21h

end start