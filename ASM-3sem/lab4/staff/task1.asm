.model small
.stack 100h
.data
    array db 10 dup (?) ; Массив 
    nl db 0Dh, 0Ah, '$' ; Перевод строки
    Names db 'Alexander Zhelezko 251', 0Dh, 0Ah, '$' ; Имя и группа
.code
start:
    mov AX, @data
    mov DS, AX

    ; Вывод имени и группы
    mov DX, offset Names
    mov AH, 09h
    int 21h


    xor SI, SI ; SI = индекс массива. Устанавливаем в ноль
    mov CX, 10
    mov AL, 1
    fill_array:
        mov array[SI], AL
        inc AL
        inc SI
        loop fill_array

    xor SI, SI ; SI = индекс массива (0)

    print_even_loop:
        cmp SI, 10 ; Обработали все 10 элементов?
        jge even_done
        mov AL, array[SI] ; AL = текущий элемент
        shr AL, 1 ; сдвиг вправо на бит 
        jc skip_even ; Если нечетное, пропускаем
        mov AL, array[SI] ; Восстанавливаем исходное значение
        call print_number ; Выводим четное число
        skip_even:
            inc SI ; Переходим к следующему элементу
            jmp print_even_loop
        even_done:
            ; Перевод строки
            lea DX, nl
            mov AH, 09h
            int 21h

    xor SI, SI ; SI = индекс массива (0)

    print_odd_loop:
        cmp SI, 10 ; Обработали все 10 элементов?
        jge odd_done
        mov AL, array[SI] ; AL = текущий элемент
        shr AL, 1 ; сдвиг вправо на бит 
        jnc skip_odd ; Если четное, пропускаем
        mov AL, array[SI] ; Восстанавливаем исходное значение
        call print_number ; Выводим нечетное число
        skip_odd:
            inc SI ; Переходим к следующему элементу
            jmp print_odd_loop
        odd_done:
            ; Перевод строки
            lea DX, nl
            mov AH, 09h
            int 21h
    ; Завершение программы
    mov AX, 4C00h
    int 21h

    print_number proc
        push AX
        push BX
        push CX
        push DX
        mov BX, 10 ; BX = делитель (10)
        xor CX, CX ; CX = счетчик цифр
        mov AH, 0  ; Чтобы AL стало AX
        divide_loop:
            xor DX, DX
            div BX ; AX / 10 -> AX (частное), DX (остаток-цифра)
            push DX ; Сохраняем цифру в стек
            inc CX ; Увеличиваем счетчик цифр
            cmp AX, 0 ; Есть еще разряды?
            jnz divide_loop ; Если да, продолжаем разбор

        push CX
        cmp CX, 1 ; Одна цифра?
        je one_digit    
        ; Две цифры (10)
        mov CX, 3 ; Пробелов 2
        jmp print_spaces_loop  
        one_digit:
        mov CX, 4 ; Пробелов 3
        print_spaces_loop:
            mov AH, 02h ; Вывести символ
            mov dl, ' ' ; Пробел
            int 21h
            loop print_spaces_loop
        pop CX

        print_digits:
            pop AX ; Извлекаем цифру из стека
            add AL, '0' ; Преобразуем в ASCII-код
            mov DL, AL
            mov AH, 02h ; AH = 02h (вывод символа)
            int 21h ; Прерывание DOS
            loop print_digits ; Повторяем для всех цифр
        pop DX
        pop CX
        pop BX
        pop AX
        ret
    print_number endp

end start