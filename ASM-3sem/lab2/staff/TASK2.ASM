.model small
.stack 100h

.data 
Names db 'Zhelezko Alexander 251', 0Dh, 0Ah, '$'
enter db 0Dh, 0Ah, '$'

.code

start:
mov AX, @data
mov DS, AX

;адрес начала строки
mov DX, offset Names
call print

mov AL, 1 ; занесение первой цифры в регистр AL
mov BL, 2 ; занесение второй цифры в регистр BL

push AX ; добавление AX в стек

mov DL, AL  ; переносим AL в DL, чтобы вывести символ в AL
add DL, 30h ; перевод цифры в символьную форму с помощью кода ASCII и команды ADD
call printElem

mov DL, ' ' ; переносим AL в DL, чтобы вывести символ в AL
call printElem

mov DL, BL  ; переносим BL в DL, чтобы вывести символ в BL
add DL, 30h ;перевод цифры в символьную форму с помощью кода ASCII и команды ADD
call printElem

pop AX ; удаление AX из стека

XCHG AL, BL ; меняем местами AL и BL

mov DX, offset enter ;адрес начала строки
call print

mov DL, AL  ; переносим AL в DL, чтобы вывести символ в AL
add DL, 30h ; перевод цифры в символьную форму с помощью кода ASCII и команды ADD
call printElem

mov DL, ' ' ; переносим AL в DL, чтобы вывести символ в AL
call printElem

mov DL, BL  ; переносим BL в DL, чтобы вывести символ в BL
add DL, 30h ; перевод цифры в символьную форму с помощью кода ASCII и команды ADD
call printElem

mov AX,4C00h ; функция завершения программы 4Ch с кодом возврата 0
int 21h      ; вызов функции DOS

; вывод строки
print proc
    mov AH, 09h ;Функция DOS 09h вывода на экран
    int 21h ;вызов функции DOS 09h
    ret ; возврат в точку вызова
print endp

; вывод символа
printElem proc
    mov AH, 02h ; номер функции вывода одного символа
    int 21h ;вызов функции 02h
    ret ; возврат в точку вызова
printElem endp

end start