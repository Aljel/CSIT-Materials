; Объявление внешних функций WinAPI
extern GetStdHandle: proc
extern WriteConsoleA: proc
extern ReadConsoleA: proc
extern lstrlenA: proc
extern ExitProcess: proc

.data
  ; Макрозамены для констант
  STD_OUTPUT_HANDLE equ -11
  STD_INPUT_HANDLE equ -10

  ; Дескрипторы потоков
  hStdInput qword ?
  hStdOutput qword ?
  
  ; Переменная для хранения результата вычислений
  result qword 5Dh ; 11369 в десятичной
  
  ; Строки пользовательского интерфейса
  strA db "a = ", 0
  strB db "b = ", 0
  strConst db "const = 5Dh", 0dh, 0ah, 0
  strF db "F = const + a - b = ", 0
  strMin db "min(a, b) = ", 0
  strInvalid db "Invalid character", 0dh, 0ah, 0
  strRange db "Number out of range", 0dh, 0ah, 0
  strExit db "Press any key to exit", 0dh, 0ah, 0
  strNext db " ", 0dh, 0ah, 0

  ; Макросы для работы со стеком

  ; Этот макрос выравнивает стек по 16-байтовой границе перед вызовом WinAPI функций
  ; arg - количество дополнительных qword параметров (сверх обязательных 4)
  STACKALLOC macro arg
    push R15              ; Сохраняем R15
    mov R15, RSP          ; Сохраняем текущий указатель стека
    sub RSP, 8*4          ; Выделяем место под 4 обязательных аргумента
    if arg                ; Если есть дополнительный аргумент
      sub RSP, 8*arg      ; Выделяем место под него
    endif
    and SPL, 0F0h         ; Выравниваем стек по 16-байтовой границе
  endm

  STACKFREE macro
    mov RSP, R15    ; Восстанавливаем стек из R15
    pop R15         ; Восстанавливаем R15
  endm

  ; Устанавливает 5-й аргумент функции WinAPI в нуль (требуется для ReadConsoleA и WriteConsoleA)
  ; 5-й и последующие аргументы передаются на стеке, начиная с [RSP+32]
  NULL_FIFTH_ARG macro
    mov qword ptr [RSP + 32], 0
  endm


.code

; Процедура вывода строки
; Параметр: string (qword)
PrintString proc uses RAX RCX RDX R8 R9, string: qword
  local bytesWritten: qword
  STACKALLOC 1           ; Выравниваем стек под 1 дополнительный параметр
  
  mov RCX, string        ; RCX = адрес строки
  call lstrlenA          ; получить длину строки (результат в RAX)
  
  mov RCX, hStdOutput    ; 1-й аргумент: дескриптор потока вывода
  mov RDX, string        ; 2-й аргумент: адрес строки
  mov R8, RAX            ; 3-й аргумент: длина строки
  lea R9, bytesWritten   ; 4-й аргумент: адрес переменной для кол-во написанных символов
  
  NULL_FIFTH_ARG         ; 5-й аргумент = 0
  
  call WriteConsoleA     ; вызовем функцию WinAPI
  
  STACKFREE
  ret 8
PrintString endp

; Процедура чтения знакового числа из строки
; Возвращает: RAX - число, R10 - статус (1 = ошибка, 0 = успех)
ReadNumber proc uses RBX RCX RDX R8 R9
  local readStr[64]: byte    ; Локальный буфер на стеке для хранения прочитанной строки (64 байта)
  local bytesRead: dword     ; Локальная переменная для хранения фактически прочитанных байт (4 байта)
  
  STACKALLOC 2
  
  ; Вызовем ReadConsoleA
  mov RCX, hStdInput         ; 1-й параметр: дескриптор потока ввода (консоль для чтения)
  lea RDX, readStr           ; 2-й параметр: адрес буфера для хранения прочитанной строки
                             ; LEA загружает адрес массива readStr на стеке
  mov R8, 64                 ; 3-й параметр: максимальный размер буфера (64 байта)
  lea R9, bytesRead          ; 4-й параметр: адрес переменной для сохранения кол-ва прочитанных символов
  NULL_FIFTH_ARG
  
  call ReadConsoleA
  
  xor RBX, RBX  ; Результат (число)
  xor RCX, RCX  ; Счетчик позиции
  
  ; Получим длину строки (исключая \r\n)
  mov ECX, bytesRead
  sub ECX, 2    ; Вычтем \r\n
  
  ; Инициализируем R8 для степеней 10
  mov R8, 1
  
  ; Начнем сканирование строки с конца
  cmp RCX, 0
  jle scanningComplete
  
  scanning_loop:
    dec RCX
    cmp RCX, -1
    je scanningComplete  ; Если прошли всю строку, выход
    
    ; Получим текущий символ
    xor RAX, RAX
    mov AL, readStr[RCX]
    
    ; Проверим на минус
    cmp AL, '-'
    je negate_and_complete

    ; Проверим диапазон '0''9'
    cmp AL, 30h
    jl error
    cmp AL, 39h
    jg error
      
    ; Преобразуем символ (ASCII) в цифру
    sub RAX, 30h
      
    ; Умножим RAX на текущую степень 10
    mul R8
      
    ; Добавим к результату
    add RBX, RAX
    
    ; Увеличим степень 10 (не просто add!)
    mov RAX, 10
    mul R8
    mov R8, RAX
      
    jmp scanning_loop

  negate_and_complete:
    neg RBX

    ; Проверяем, что минус на первом месте
    cmp RCX, 0            
    je scanningComplete   
    jmp error             
    
  scanningComplete:
    mov R10, 0
    mov RAX, RBX
    jmp read_number_exit
    
  error:
    mov R10, 1
    
  read_number_exit:
    STACKFREE
    ret
ReadNumber endp

; Процедура вывода знакового числа
; Параметр: число (qword)
PrintNumber proc uses RAX RCX RDX R8 R9 R10 R11, number: qword
  local numberStr[22]: byte
  
  STACKALLOC 1
  
  ; Инициализируем счетчик
  xor R8, R8
  
  ; Получим число
  mov RAX, number
  
  ; Проверим знак числа
  bt RAX, 63 ; Проверяет 63 бит (знак) и передает в CF
  jnc positive_number
  
  ; Число отрицательное
  mov numberStr[0], '-'
  inc R8
  neg RAX

  positive_number:
    mov RBX, 10
    xor RCX, RCX
    
  division_loop:
    xor RDX, RDX
    div RBX
    
    ; Преобразуем остаток в ASCII код
    add RDX, '0'
    
    ; Поместим в стек
    push RDX
    inc RCX
    
    ; Проверим, закончилось ли деление
    cmp RAX, 0
    jne division_loop
    
  stack_to_string:
    cmp RCX, 0
    je string_complete
    
    ; Возьмем цифру из стека
    pop RDX
    mov numberStr[R8], DL
    inc R8
    
    dec RCX
    jmp stack_to_string
  
  string_complete:
    ; Добавим нуль-терминатор
    mov numberStr[R8], 0
    
    ; Выведем строку
    lea RAX, numberStr
    push RAX
    call PrintString
  
  STACKFREE
  ret 8
PrintNumber endp

; Процедура ожидания ввода
WaitForInput proc uses RAX RCX RDX R8 R9 R10 R11
  local readStr[64]: byte
  local bytesRead: dword
  
  STACKALLOC 1
  
  ; Выведем сообщение
  mov RCX, hStdOutput
  lea RDX, strExit
  mov RAX, offset strExit
  call lstrlenA
  mov R8, RAX
  lea R9, bytesRead
  NULL_FIFTH_ARG
  
  call WriteConsoleA
  
  ; Прочтем один символ
  mov RCX, hStdInput
  lea RDX, readStr
  mov R8, 1
  lea R9, bytesRead
  NULL_FIFTH_ARG
  
  call ReadConsoleA
  
  STACKFREE
  ret
WaitForInput endp

nextLine proc
; Перейдем на новую строку
  lea RAX, strNext
  push RAX
  call PrintString
  ret
nextLine endp
; Проверяет находится ли число в диапазоне byte - Число должно находиться в RAX 
; Сделает R11 = 0, если число не выходит за пределы и R11 = 1 в противном случае
byteCheck proc 
    cmp RAX, 127   ; Сравниваем число сначала по верхней границе,
    jg overflow     
    cmp RAX, -128  ; а потом по нижней
    jl overflow 
    
    mov R11, 0       ; Если переполнения нет, то R11 = 0
    ret

    overflow: 
        mov R11, 1   ; Если переполнение есть, то R11 = 1
        ret
byteCheck endp

; Главная процедура
mainCRTStartup proc
  STACKALLOC
  
  ; Получим дескриптор для вывода
  mov RCX, STD_OUTPUT_HANDLE
  call GetStdHandle
  mov hStdOutput, RAX
  
  ; Получим дескриптор для ввода
  mov RCX, STD_INPUT_HANDLE
  call GetStdHandle
  mov hStdInput, RAX
  
  ; Выведем "const = 2C69h
  lea RAX, strConst
  push RAX
  call PrintString
  ; Выведем "a = "
  lea RAX, strA
  push RAX
  call PrintString
  
  ; Прочтем первое число
  call ReadNumber
  call byteCheck

  ; Проверим ошибку
  cmp R10, 1
  je input_error
  cmp R11, 1
  je range_error
  
  ; Сохраним первое число в RBX
  mov RBX, RAX
  
  ; Выведем "b = "
  lea RAX, strB
  push RAX
  call PrintString
  
  ; Прочтем второе число
  call ReadNumber
  call byteCheck

  ; Проверим ошибку
  cmp R10, 1
  je input_error
  cmp R11, 1
  je range_error
  
  ; Теперь у нас число "a" в RBX, "b" в RAX
  ; Делаем арифметические действия
  add result, RBX       
  sub result, RAX

  ; Выводим минимум
  cmp RBX, RAX          ; Сравниваем RBX с RAX
  jge is_smaller       ; Если RBX >= RAX, оставляем RAX (это минимум)
  mov RAX, RBX          ; Иначе RAX = RBX (это минимум)
  
  is_smaller:
    push RAX ; Сохраняем реальное значение 

    lea RAX, strMin ; Печатем нужную строку
    push RAX
    call PrintString

    pop RAX ; Возвращаем реальное значение
    push RAX
    call PrintNumber ; Печатаем реальное значение

  call nextLine

  ; Выведем "F = const + a - b = "
  lea RAX, strF
  push RAX
  call PrintString
  
  ; Выведем число
  mov RAX, result
  push RAX
  call PrintNumber

  call nextLine

  ; Выведем новую строку
  lea RAX, strExit
  push RAX
  call PrintString

  ; Ожидаем ввода
  call WaitForInput
  
  ; Завершим программу
  mov RCX, 0
  call ExitProcess
  
input_error:
  ; Выведем сообщение об ошибке
  lea RAX, strInvalid
  push RAX
  call PrintString
  
  ; Ожидаем ввода
  call WaitForInput
  
  ; Завершим программу
  mov RCX, 1
  call ExitProcess
  
range_error:
  ; Выведем сообщение об ошибке
  lea RAX, strRange
  push RAX
  call PrintString
  
  ; Ожидаем ввода
  call WaitForInput
  
  ; Завершим программу
  mov RCX, 1
  call ExitProcess
mainCRTStartup endp
end
