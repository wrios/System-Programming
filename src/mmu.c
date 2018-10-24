/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


void mmu_init() {
}

uint32_t mmu_nextFreeKernelPage() {
    return 0;
}

uint32_t mmu_nextFreeTaskPage() {
    return 0;
}

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {
    /*uint32_t PDE_OFF = virtual >> 22;
    uint32_t PTE_OFF = (virtual << 10) >> 22;
    pdt_entry pd[] = (*pd) (cr3 >> 12) << 12;
    pdt_entry pdt_ent = pd[PDE_OFF];s
    if (pdt_ent.p == 0){
        pt_entry ptabledir[] = (pt_entry*) new_kernel_page();
        pdt_entry newpd = getNewPD(); // esto lo pone todo en cero 
        newpd.base = &(ptabledir >> 12); // is this real or is just fanta sea?
        newpd.p = 0x1;
        newpd.us = PERM_USER;
        newpd.rw = PERM_WRITE;
        pd[pde_off] = newpd;
    }
    pt_entry pt = (*pt_entry) *(pd[PDE_OFF].base[PTE_OFF]) << 12;
    pt_entry pte =pt[PTE_OFF];
    pt.base = (fisica >> 12);
    pt.rw = getRw(attr);
    pt.p = 0x1;
    pt[PTE_OFF] = PTE;
    TLBFLUSH();
*/
}

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3) {
    return 0;
}

uint32_t mmu_initKernelDir() {
 /*   page_dir_entry *pd = 0x27000;

    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].p = 1;
    pd[0].tabla = (0x28000 >> 12);

    // marco con un for las demás entradas como no presentes for(){pd[i] = 0} (igual que arriba)

    page_table_entry *pt = (pd[0].tabla<<12);

    
    for(int i = 0; i < count; i++)
    {
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].p = 1;
        pt[i].base = i; // va con algún shifteo?
    }
    
*/

    return 0;
}








