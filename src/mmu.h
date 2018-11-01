/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

uint32_t getCR3();

void mmu_init();/*llama a mmu_initKernelDir() ?*/

uint32_t mmu_nextFreeKernelPage();

uint32_t mmu_nextFreeTaskPage();

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs);

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3);

uint32_t mmu_initKernelDir();/*inicializar cr3 de kernel ?*/

typedef struct str_page_dir_entry {
  uint8_t p;/*[0] = P(presente)*/
  uint8_t rw;/*[1] = (R/W)(0 Read Only | 1 puede ser escrita)*/
  uint8_t us;/*[2] = (U/S)(0 es supervisor | 1 es usuario. En general 0->DLP=00 y 1->DLP=resto)*/
  uint8_t pwt;/*[3] = PWT*/
  uint8_t pcd;/*[4] = PCD*/
  uint8_t a;/*[5] = (A)(Cada vez que la pagina es accedida este bit se setea)*/
  uint8_t ignored;/*[6] = D(El SO la inicializa en 0, y se setea automaticamente cuando
  se escribe la página)*/
  uint8_t ps;/*[7] = (PS)(Si es 0 decribe una tabla de paginas de 4KB, y si es 1  4MB)*/
  uint8_t g;/*[8] = (G)(Tiene efecto la activacion de lafuncionalidad Global, 
  que otorga ese caracter a la traduccion de esa ṕagina almacenada en la TLB. 
  La entrada no se flushea cuando se recarga el registro CR3)*/
  uint8_t dpl;/*[11..9] = (DLP)privilege-Level*/
  uint32_t base;/*[31..12] = Dirección base de la página*/
} __attribute__((__packed__)) page_dir_entry;

typedef struct str_page_table_entry {
  
  uint8_t p;/*[0]*/
  uint8_t rw;/*[1] = R/W*/
  uint8_t us;/*[2] = U/S*/
  uint8_t pwt;/*[3] = PWT*/
  uint8_t pcd;/*[4] = PCD*/
  uint8_t a;/*[5] = A*/
  uint8_t d;/*[6] = D*/
  uint8_t pat;/*[7] = PAT*/
  uint8_t g;/*[8] = G*/
  uint8_t dpl;/*[11..9] = (DLP)privilege-Level*/
  /*[31..12] = Dirección física de la tabla de página de 4M*/
  uint32_t base;
} __attribute__((__packed__)) page_table_entry;

void setPageTable(page_table_entry* new_page_table_entry, uint32_t attrs);

#endif	/* !__MMU_H__ */

