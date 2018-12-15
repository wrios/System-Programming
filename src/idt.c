/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/

#define IDT_ENTRY(numero, attr_numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) 0xb0;                                                          \
    idt[numero].attr = (uint16_t) attr_numero;                                                     \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);
/*
    segsel apunta a la pocisión de la gdt para acceder a la base del segmento
    attr atributos del vector de interrupciones
    offset_0_15 + offset_16_31 offset del segmento
*/

void idt_init() {
    IDT_ENTRY(0,(uint16_t)0x8E00);
    IDT_ENTRY(1,(uint16_t)0x8E00);
    IDT_ENTRY(2,(uint16_t)0x8E00);
    IDT_ENTRY(3,(uint16_t)0x8E00);
    IDT_ENTRY(4,(uint16_t)0x8E00);
    IDT_ENTRY(5,(uint16_t)0x8E00);
    IDT_ENTRY(6,(uint16_t)0x8E00);
    IDT_ENTRY(7,(uint16_t)0x8E00);
    IDT_ENTRY(8,(uint16_t)0x8E00);
    IDT_ENTRY(9,(uint16_t)0x8E00);
    IDT_ENTRY(10,(uint16_t)0x8E00);
    IDT_ENTRY(11,(uint16_t)0x8E00);
    IDT_ENTRY(12,(uint16_t)0x8E00);
    IDT_ENTRY(13,(uint16_t)0x8E00);
    IDT_ENTRY(14,(uint16_t)0x8E00);
    IDT_ENTRY(16,(uint16_t)0x8E00);
    IDT_ENTRY(17,(uint16_t)0x8E00);
    IDT_ENTRY(18,(uint16_t)0x8E00);
    IDT_ENTRY(19,(uint16_t)0x8E00);

    //son servicios de las tareas, dpl = 3
    IDT_ENTRY(71,(uint16_t)0xEE00);
    IDT_ENTRY(73,(uint16_t)0xEE00);
    IDT_ENTRY(76,(uint16_t)0xEE00);

    IDT_ENTRY(32,(uint16_t)0x8E00);
    IDT_ENTRY(33,(uint16_t)0x8E00);
    
        /*
        [15,20..31] reservadas por intel(no usar)
        [32..255] A definir por el usuario
        */
        /*
        0x8E00:
        |p=1 segmento presente
        |dpl=00 nivel de privilegio requerido kernel
        |0 = descriptor sistema
        |1= Tipo de puerta 32bits
        |110=interrupción
        |000
        |4 bits sin usar(0)
        */
}
