/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  50
#define SIZE_M                  50

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 200

// EMPIEZA EN 22 O 21?
#define GDT_IDX_NULL_DESC                0
#define GDT_ENTRY_CODES_KERNEL           22//0x16
#define GDT_ENTRY_DATAS_KERNEL           23//0x17
#define GDT_ENTRY_CODES_USER             24//0x18
#define GDT_ENTRY_DATAS_USER             25//0x19
#define GDT_ENTRY_VIDEO_KERNEL           26//0x1A
#define GDT_ENTRY_TASK_INIT              27//0x1B
#define GDT_ENTRY_TASK_IDLE              28//0x1B

#define GDT_ENTRY_TASK_A0                31
#define GDT_ENTRY_TASK_A1                32
#define GDT_ENTRY_TASK_A2                33
#define GDT_ENTRY_TASK_A3                34
#define GDT_ENTRY_TASK_A4                35
#define GDT_ENTRY_TASK_A5                36
#define GDT_ENTRY_TASK_A6                37
#define GDT_ENTRY_TASK_A7                38
#define GDT_ENTRY_TASK_A8                39
#define GDT_ENTRY_TASK_A9                40

#define GDT_ENTRY_TASK_B0                41
#define GDT_ENTRY_TASK_B1                42
#define GDT_ENTRY_TASK_B2                43
#define GDT_ENTRY_TASK_B3                44
#define GDT_ENTRY_TASK_B4                45
#define GDT_ENTRY_TASK_B5                46
#define GDT_ENTRY_TASK_B6                47
#define GDT_ENTRY_TASK_B7                48
#define GDT_ENTRY_TASK_B8                49
#define GDT_ENTRY_TASK_B9                50

#define TSS_MAX_AMOUNT_TASKS             22

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             0x08000000 /* direccion virtual del codigo */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_IDLE_CODE_SRC_ADDR  0x00014000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR          0x00027000
#define KERNEL_PAGE_TABLE_0      0x00028000

#endif  /* !__DEFINES_H__ */
