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


void mmu_init();

uint32_t mmu_nextFreeKernelPage();

uint32_t mmu_nextFreeTaskPage();

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs);

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3);

uint32_t mmu_initKernelDir();
/*
typedef struct str_page_dir_entry {
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
} __attribute__((__packed__)) page_dir_entry

typedef struct str_page_table_entry {
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
  uint8_t p:1;
} __attribute__((__packed__)) page_table_entry
*/


#endif	/* !__MMU_H__ */




