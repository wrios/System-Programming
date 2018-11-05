/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    /* Offset = 0x00 */
        [GDT_ENTRY_CODES_KERNEL] = (gdt_entry) {
        (uint16_t)    0x99FF,         /* limit[0:15]  */        
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */ 
        /* 
            TSS de 32-bits disponible (?)
            readable prendido, el codigo del kernel puede leerse a si mismo
        */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [GDT_ENTRY_DATAS_KERNEL] = (gdt_entry) {
        (uint16_t)    0x99FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */ 
         /*
            Solamente activamos el bit de readable, ¿tiene sentido para un segmento de data?

        */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x0A,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [GDT_ENTRY_CODES_USER] = (gdt_entry) {
        (uint16_t)    0x99FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [GDT_ENTRY_DATAS_USER] = (gdt_entry) {
        (uint16_t)    0x99FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x06,           /* type         */ // Expand down puede no ir
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }, // Las bases del video kernel son turbias, preguntar
        [GDT_ENTRY_VIDEO_KERNEL] = (gdt_entry) {
        (uint16_t)    0x1F3F,         /* limit[0:15]  */
        (uint16_t)    0x8000,         /* base[0:15]   */
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [GDT_ENTRY_TASK_INIT] = (gdt_entry) {
        //TAREA INICIAL(Creo que no ejecuta, es solo para poder hacer el primer task swich)
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0x000,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x00,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x00,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x00,           // dpl         
        (uint8_t)     0x00,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x00,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_IDLE] = (gdt_entry) {
        //La tarea IDLE se encuentra en la dirección 0x00014000
        //La pila se alojará en la misma dirección que la pila del kernel y será mapeada
        //con identity mapping
        //Esta tarea ocupa 1 pagina de 4KB y debe ser mapeada con identity mapping
        //Además la misma debe compartir el mismo CR3 que el kernel(0x00027000).
        //type = 0x9
        //limit = 0x00FFF (maximo off_set una pagina)
        //base = 0x00014000
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x4000,         // base[0:15]
        (uint8_t)     0x01,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x00,           // offset de (preguntar 16 bits)          
        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A0] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A1] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A2] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A3] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A4] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A5] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A6] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A7] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A8] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_A9] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B0] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B1] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B2] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B3] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B4] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B5] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B6] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B7] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B8] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    },
        [GDT_ENTRY_TASK_B9] = (gdt_entry) {
        //limit de una pagina 0x00FFF
        //base 0x400000 (o + i*kb , donde i es la i-esima tarea)
        (uint16_t)    0xFFF,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x40,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type        
        //no es de sistema(s = 0)
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x03,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x01,           // db          

        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
