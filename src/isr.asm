; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0x00
sched_task_selector:   dw 0x00

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask




error_mp_msg_0: db     'Error! Divide Error, aprende a dividir gato, numero: 0'
error_mp_len_0: equ    $ - error_mp_msg_0

error_mp_msg_1: db     'Error! Debug, numero: 1'
error_mp_len_1: equ    $ - error_mp_msg_1

error_mp_msg_2: db     'Error! NMI, numero: 2'
error_mp_len_2: equ    $ - error_mp_msg_2

error_mp_msg_3: db     'Error! Breakpoint, numero: 3'
error_mp_len_3: equ    $ - error_mp_msg_3

error_mp_msg_4: db     'Error! Overflow, numero: 4'
error_mp_len_4: equ    $ - error_mp_msg_4

error_mp_msg_5: db     'Error! BOUND Range Exceeded, numero: 5'
error_mp_len_5: equ    $ - error_mp_msg_5

error_mp_msg_6: db     'Error! OPCODE Invalid, numero: 6'
error_mp_len_6: equ    $ - error_mp_msg_6

error_mp_msg_7: db     'Error! Device Not Avaiable, numero: 7'
error_mp_len_7: equ    $ - error_mp_msg_7

error_mp_msg_8: db     'Error! Double Fault Exception, numero: 8'
error_mp_len_8: equ    $ - error_mp_msg_8

error_mp_msg_9: db     'Error! Coprocessor Segment Overrun, numero: 9'
error_mp_len_9: equ    $ - error_mp_msg_9

error_mp_msg_10: db     'Error! TSS Invalido, numero: 10'
error_mp_len_10: equ    $ - error_mp_msg_10

error_mp_msg_11: db     'Error! Segmento no Presente, numero: 11'
error_mp_len_11: equ    $ - error_mp_msg_11

error_mp_msg_12: db     'Error! Falta en el stack segment, numero: 12'
error_mp_len_12: equ    $ - error_mp_msg_12

error_mp_msg_13: db     'Error! General Protection, numero: 13'
error_mp_len_13: equ    $ - error_mp_msg_13

error_mp_msg_14: db     'Error! Page Fault, numero: 14'
error_mp_len_14: equ    $ - error_mp_msg_14

error_mp_msg_15: db     'Error! INTEL ACA NO LLEGA, numero: 15'
error_mp_len_15: equ    $ - error_mp_msg_15

error_mp_msg_16: db     'Error! X-87 fpu error punto flotante, numero: 16'
error_mp_len_16: equ    $ - error_mp_msg_16

error_mp_msg_17: db     'Error! Alignment Check, numero: 17'
error_mp_len_17: equ    $ - error_mp_msg_17

error_mp_msg_18: db     'Error! Machine Check, numero: 18'
error_mp_len_18: equ    $ - error_mp_msg_18

error_mp_msg_19: db     'Error! Excepcion SIMD Floating Point, numero: 19'
error_mp_len_19: equ    $ - error_mp_msg_19

error_mp_msg_71: db     'Error! Interrupcion read, numero: 71'
error_mp_len_71: equ    $ - error_mp_msg_71

error_mp_msg_73: db     'Error! Interrupcion move, numero: 73'
error_mp_len_73: equ    $ - error_mp_msg_73

error_mp_msg_76: db     'Error! Interrupcion divide, numero: 76'
error_mp_len_76: equ    $ - error_mp_msg_76
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
;%macro macro_name number_of_params
global _isr%1

_isr%1:
    mov eax, %1
    print_text_pm error_mp_msg_%1, error_mp_len_%1, 0x07, 0, 0
    xchg bx, bx

    jmp 0xB0:0xCACA
    ;jmp off_set_idle:segmento_idle, salta a la tarea idle (codigo en 0x14000)
    ;idle es una tarea que se ejecuta cuando necesitas pasar el tiempo
    ;offset_idle fruta
    ;por ejemplo: cuando una tarea muere y se necesita esperar al clock para 
    ;ejecutar la sisguiente tarea

    ;off_set_idle 0x1400(mirar dibujo del enunciado)
    ;segmento_idle 0x0(pertenece al segmento de cogido)
%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19



;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:

        iret


;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:

        iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr71
_isr71:

        iret
global _isr73
_isr73:

        iret
global _isr76
_isr76:

        iret                


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
