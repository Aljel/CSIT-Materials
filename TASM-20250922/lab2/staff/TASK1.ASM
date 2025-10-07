.model tiny  
.code                     
org 100h                  
                         
start:
  ; печать фамилии, имени и номера группы
  mov AH,09h
  mov DX, offset Names
  int 21h
  
  ; переход на следующую строку
  call printEnter

  ; вывод цифр
  mov AX, 1    ; занесение первой цифры в регистр AX
  ADD AX, 30h  ; перевод цифры в символьную форму с помощью кода ASCII и команды ADD
  mov BX, 2    ; занесение второй цифры в регистр BX
  ADD BX, 30h  ; перевод цифры в символьную форму с помощью кода ASCII и команды ADD
  
  ; вывод первой цифры
  mov DX, AX
  mov AH, 02h
  int 21h

  ; вывод пробела 
  mov DL, 00h   ;код пробела из ASCII
  int 21h 

  ; вывод второй цифры 
  mov DX, BX
  mov AH, 02h
  int 21h 

  ; завершение программы
  mov AX, 4C00h
  int 21h
  
  ; переход на следующую строку
  printEnter proc
    mov DX, offset nline
    mov AH, 09h
    int 21h
    ret
  printEnter endp


Names db 'Zhelezko Alexander 251$'
nline db 0Dh,0Ah,'$'
end start