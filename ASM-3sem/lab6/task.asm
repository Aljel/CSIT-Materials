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

  xor RCX, RCX

  call ExitProcess
mainCRTStartup endp
public mainCRTStartup
end