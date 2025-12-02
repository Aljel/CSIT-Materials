#import "conf.typ" : conf
#show: conf.with(
  title: [Лабораторная работа №6],
  type: "referat",
  info: (
      author: (
        name: [Железко Александра Дмитриевича],
        faculty: [КНиИТ],
        group: "251",
        sex: "male"
      ),
      inspector: (
        degree: "Старший преподаватель",
        name: "Е. М. Черноусова"
      )
  ),
  settings: (
    title_page: (
      enabled: true
    ),
    contents_page: (
      enabled: true
    )
  )
)


#align(left)[= Текст программы на языке ассемблера с комментариями]  

```nasm
; Используя директиву extrn, сообщим ассемблеру, что в коде будут использоваться функции, определенные вне данного модуля (функции WinAPI):
extrn ExitProcess :proc,
      MessageBoxA :proc
.data
; С помощью директивы DB определим строки для хранения заголовка окна и выводимого сообщения:
caption db 'Инфо', 0
message db 'Железко Александр',  0

.code
mainCRTStartup proc
; выделить 40 байт (32 на аргументы + 8 для установки правильного смещения)
  sub RSP, 8*5

;В MASMx86-64, в соответствии с соглашением __fastcall, нам потребуется передать значения аргументов в регистры RCX, RDX, R8 и R9 соответственно:
  xor RCX, RCX
  lea RDX, message
  lea R8, caption
  xor R9, R9

  call MessageBoxA

; Теперь, когда все необходимые действия выполнены, нам остается лишь корректно завершить выполнение программы. Для этого вызовем функцию ExitProcess с аргументом 0, говорящем о том, что выполнение программы прошло без ошибок:ч
  xor RCX, RCX
  call ExitProcess
mainCRTStartup endp
public mainCRTStartup
end
```

#align(left)[= Скриншот запуска программы]  
#v(0.5cm)
#align(center)[#image("y.jpg", width: 95%)]
