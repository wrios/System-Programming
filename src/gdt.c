/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "i386.h"

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
        (uint8_t)     0x02,           /* limit[16:19] */
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
        (uint8_t)     0x02,           /* type         */ // Expand down puede no ir
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
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }
};

void create_tss_descriptores(){
    
    for(uint32_t i = 31; i < 51; i++)
    {
        gdt[i] = (gdt_entry) {
            (uint16_t)    0x67,         // limit[0:15]
            (uint16_t)    0x0000,         // base[0:15]
            (uint8_t)     0x00,           // base[23:16]
            //type 10B1(B es el bit de Busy, inicialmente en 0)
            (uint8_t)     0x09,           // type
            (uint8_t)     0x00,           // s
            (uint8_t)     0x03,           // dpl
            (uint8_t)     0x01,           // p
            (uint8_t)     0x00,           // limit[16:19]
            (uint8_t)     0x00,           // avl
            (uint8_t)     0x00,           // l
            (uint8_t)     0x01,           // db
            (uint8_t)     0x00,           // g
            (uint8_t)     0x00,           // base[31:24]
        };
    };
    

    gdt[GDT_ENTRY_TASK_INIT] = (gdt_entry) {
        (uint16_t)    0x67,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x00,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x00,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x00,           // db          
        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    };
    gdt[GDT_ENTRY_TASK_IDLE] = (gdt_entry) {
        (uint16_t)    0x67,         // limit[0:15] 
        (uint16_t)    0x0000,         // base[0:15]  
        (uint8_t)     0x00,           // base[23:16] 
        //type 10B1(B es el bit de Busy, inicialmente en 0)
        (uint8_t)     0x09,           // type
        (uint8_t)     0x00,           // s           
        (uint8_t)     0x00,           // dpl         
        (uint8_t)     0x01,           // p           
        (uint8_t)     0x00,           // limit[16:19]
        (uint8_t)     0x00,           // avl         
        (uint8_t)     0x00,           // l           
        (uint8_t)     0x00,           // db          
        (uint8_t)     0x00,           // g           
        (uint8_t)     0x00,           // base[31:24]  
    };

}

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
