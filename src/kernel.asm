; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC
extern IDT_DESC
extern print
extern pintar_pantalla
extern idt_init

extern pic_reset
extern pic_enable
extern pic_disable

extern _isr32
extern _isr33

extern mmu_init
extern mmu_initKernelDir
extern mmu_initTaskDir

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

   ; xchg bx, bx


;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    ;print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0    
    
    ; Habilitar A20
    call A20_enable
    ; Cargar la GDT
    lgdt [GDT_DESC]
    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ; Saltar a modo protegido

    jmp 0xb0:MP 
    ; Pasamos a modo protegido, 8*22 = 0xb0
    ; posicion 22 de la gdt, el descriptor del segmento codigo kernel

BITS 32
MP:
    ; Establecer selectores de segmentos

    xor eax, eax
    mov ax, 0xB8 ; Indice de la GDT 23*8 (bytes) - segmento de data de kernel
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax    
    mov ax, 0xD0 ; Indice de nuestro segmento de video para el kernel 26*8 (bytes)
    mov fs, ax
    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, 0x27000
    ; Imprimir mensaje de bienvenida
    call pintar_pantalla    
    
    ; Inicializar pantalla
    
   ; mov ax, 0xD0 ; Indice de nuestro segmento de video para el kernel 26*8 (bytes)
    ; Inicializar el manejador de memoria
    ;call mmu_init
    ; Inicializar el directorio de paginas
    xchg bx, bx    
    call mmu_initKernelDir    
    
    ; Cargar directorio de paginas
    call mmu_init

    mov eax, 0x00027000; page_directory
    ;shl eax, 12

    mov cr3, eax
    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000 ;habilito Unidad de paginación
    mov cr0, eax
    ; Inicializar tss
    
    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler


    ; Inicializar la IDT
    call idt_init
    ; Cargar IDT
    lidt [IDT_DESC]
    ; Configurar controlador de interrupciones
    call pic_reset 
    call pic_enable
    ; Cargar tarea inicial
    call tss_init_gdt
    ;mov edi, 
    ;mov esi, [GDT_DESC]
    ;add esi, (8<< 4); accediendo a la tarea IDLE dentro de la gdt

    ;pic_disable
    
    ;despues de remapear el PIC y habilitarlo, tenemos que la interrupción
    ;de reloj está mapeada a la interrupción 32 y el  teclado a la 33
    ;resta habilitar las interrupciones con la intrucción sti

    ;Leemos del teclado a través del puerto 0x60(in al, 0x60)

    ;IRQ0 para timer
    ;IRQ1 para teclado

    ; Saltar a la primera tarea: Idle
    
    ;Para pasar a paginación, hacer un Aling 4096
    ;Activar paginación
    ;Armar un directorio de paginas y tablas de paginas
    ;Poner en CR3 la dirección base del directorio de páginas


    ; Habilitar interrupciones
    sti ;se activa el flag IF del registro EFLAGS


    ;Limpiar bits PCD y PWT de CR3
        ;lo hicimos al shiftear
    
    ;setear el bit PG de CR0

    ;int 0x47
    
    

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
