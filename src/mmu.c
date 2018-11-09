/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
extern void copyHomework(int task_kernel, int task_area_libre_tarea, int a);

uint32_t next_page_free_task_fisica;
uint32_t next_page_free_task_virtual;
uint32_t next_page_free_kernel;
uint8_t PERM_USER = 1;
uint8_t PERM_WRITE = 1;
uint32_t Table_size = 0x0400000;
uint32_t Directory_size = 1024;
uint32_t next_page_free;
uint32_t cr3 = 0x27000;


void mmu_init() {
    next_page_free_kernel = 0x100000;
    //pedir dos paginas de libres tareas para directorio y tabla de tareas
    next_page_free_task_fisica = 0x400000;
    next_page_free_task_virtual = 0x08000000;//mappear las dos paginas solicitadas
    //despues de lograr activar paginascion(ejercicio 5)
    //Manejo de memoria para manejar area libre de kernel(0x100000 a 0x3FFFFF)
    //Manejo de memoria para manejar area libre de tareas(0x400000 a 0x7FFFFF)
    //(sug: dos contadores)
    //creo que hay que hacer mappeo con identity mapping para are libre de kernel
}

uint32_t getCR3(){
    return cr3;
}

uint32_t mmu_nextFreeKernelPage() {
    uint32_t page_free_kernel = next_page_free_kernel;
    next_page_free_kernel += 0x1000;
    return page_free_kernel;
}

uint32_t mmu_nextFreeTaskPage_virtual() {//devuelve la dirección virtual
    uint32_t page_free_task = next_page_free_task_virtual;
    next_page_free_task_virtual += 0x1000;
    return page_free_task;
}

uint32_t mmu_nextFreeTaskPage_fisica() {//devuelve la dirección física
    uint32_t page_free_task = next_page_free_task_fisica;
    next_page_free_task_fisica += 0x1000;
    return page_free_task;
}

void setPageTable(page_table_entry* new_page_table_entry, uint32_t attrs){
    new_page_table_entry->us = ((attrs << 29)>>31);
    new_page_table_entry->rw = ((attrs << 30)>>31);
    new_page_table_entry->p = ((attrs << 31)>>31);
}

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {
   ///Indice del page directory & Indice del page table///
    uint32_t PDE_OFF = virtual >> 22;//off_set_directory
    uint32_t PTE_OFF = (virtual << 10) >> 22;//off_set_table
    ///dirección base del page directory///
    page_dir_entry* pd = (page_dir_entry*) ((cr3 >> 12) << 12);
    page_dir_entry pd_ent = pd[PDE_OFF];//selector de pagina
    page_table_entry* new_page_table_entry;
    ///Hay que chequar que la page table a la que referencia el pd_ent exista///
    if (pd_ent.p == 0){//crear y settear la page_table_entry
    //agregar una función de set(page_table_entry*, uint32_t attrs), sette los atributos
        new_page_table_entry = (page_table_entry*) mmu_nextFreeKernelPage();//new_kernel_page();        
        setPageTable(new_page_table_entry, attrs);
        ////la dirección es multiplo de 4k////
        pd[PDE_OFF].base = ((uint32_t)new_page_table_entry >> 12);
    }else{//si esta presente, alcanza con asignarle la dirección a la que apunta la 
    //page directory[PDE_OFF] para decirle que ese es el lugar donde empieza
        ////la dirección es multiplo de 4k////
        new_page_table_entry = (page_table_entry*) (pd[PDE_OFF].base << 12);
    }
    ///Completar la page_table_entry para que referencie a la dirección física///
    new_page_table_entry[PTE_OFF].us = 1;
    new_page_table_entry[PTE_OFF].rw = 1;
    new_page_table_entry[PTE_OFF].p = 1;
    ///la dirección física es multiplo de 4 k y en el descriptor va el numero sin los 3 ceros///
    new_page_table_entry[PTE_OFF].base = (phy >> 12);
    ///invalidar cache de traduciones///
    tlbflush();
}

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3) {
    ///Indice del page directory & Indice del page table///
    
    uint32_t PDE_OFF = virtual >> 22;//off_set_directory
    uint32_t PTE_OFF = (virtual << 10) >> 22;//off_set_table
    ///dirección base de la page directory///
    page_dir_entry* pd = (page_dir_entry*) ((cr3 >> 12) << 12);//selector de tabla(12 ultimos)
    if (pd[PDE_OFF].p == 1) {
        page_table_entry* pt_ent = (page_table_entry*) ((pd[PDE_OFF].base) << 12);
        pt_ent[PTE_OFF].p = 0;
    }
    
    return 0;
}

uint32_t mmu_initTaskDir(char jugador, uint32_t cr3){
    //las direcciones virtuales van en la page directory
    //las direcciones fisicas van la page table
    /*inicializar dirección virtual de directorio y tabla de la tarea*/

    // mapeamos los primeros 4mb del kernel para acceder a las interrupciones
    mmu_mappear4mbKernel();

    uint32_t dir_virtual_directory = mmu_nextFreeTaskPage_virtual();
    uint32_t dir_virtual_table = mmu_nextFreeTaskPage_virtual();
    mmu_nextFreeTaskPage_fisica();
    mmu_nextFreeTaskPage_fisica();
    page_dir_entry *pd = (page_dir_entry*) dir_virtual_directory;
    page_table_entry *pt = (page_table_entry*) dir_virtual_table;
    /*inicializar dirección fisica de la copia de la tarea*/
    for(int i = 0; i < Directory_size; i++)
    {
        pd[i] = (page_dir_entry){
            (uint8_t) 0x0,/*[0] = P(presente)*/
            (uint8_t) 0x0,/*[1] = (R/W)(0 Read Only | 1 puede ser escrita)*/
            (uint8_t) 0x0,/*[2] = (U/S)(0 es supervisor | 1 es usuario. En general 0->DLP=00 y 1->DLP=resto)*/
            (uint8_t) 0x0,/*[3] = PWT*/
            (uint8_t) 0x0,/*[4] = PCD*/
            (uint8_t) 0x0,/*[5] = (A)(Cada vez que la pagina es accedida este bit se setea)*/
            (uint8_t) 0x0,/*[6] = (D)(El SO la inicializa en 0, y se setea automaticamente cuando
  se escribe la página)*/
            (uint8_t) 0x0,/*[7] = (PS)(Si es 0 decribe una tabla de paginas de 4KB, y si es 1  4MB)*/
            (uint8_t) 0x0,/*[8] = (G)(Tiene efecto la activacion de lafuncionalidad Global, 
  que otorga ese caracter a la traduccion de esa ṕagina almacenada en la TLB. 
  La entrada no se flushea cuando se recarga el registro CR3)*/
            (uint8_t) 0x0,/*[11..9] = (DLP)privilege-Level*/
            (uint32_t) 0x0000/*[31..12] = Dirección base de la página*/
        };
        tlbflush();
    }
    pd[0].p = 0x1;/*[0] = P(presente)*/
    pd[0].us = 0x0;
    pd[0].rw = 0x1;/*[1] = (0 Read Only | 1 puede ser escrita)*/
    pd[0].base = (dir_virtual_table >> 12);/*[31..12] = Dirección base de la page_table*/


    //0x00000000 a 0x003FFFFF son exactamente 1024 paginas
    for(int i = 0; i<1024; i++)//1024 = 0x400(inicio del mappeo del area libre de tareas) 
    {
        //base de la pagina, base de la page_directory,base de la pagina,lvlRW1,lvlUS1
        ///Indice del page directory & Indice del page table///
        //uint32_t page = mmu_nextFreeKernelPage();
        uint32_t INDEX_PTE = i;//se omiten los 12 bits(0's), por que todas las paginas son de 4kb

        pt[INDEX_PTE].p = 0x0;
        pt[INDEX_PTE].rw = 0x1;
        pt[INDEX_PTE].us = 0x0;
        pt[INDEX_PTE].pcd = 0x0;
        pt[INDEX_PTE].a = 0x0; // no fue accedida
        pt[INDEX_PTE].d = 0x0; // no esta dirty
        pt[INDEX_PTE].pat = 0x0; //no quiero PAT
        pt[INDEX_PTE].g = 0x0; //no es global
        pt[INDEX_PTE].dpl = 0x3; //no es global
        pt[INDEX_PTE].base = i; //se omiten los 12 bits(0's), por que todas las paginas son de 4kb
        tlbflush();
    }



    uint32_t virt;
    uint32_t fisi = virt = mmu_nextFreeTaskPage_fisica();
    mmu_nextFreeTaskPage_fisica();
    uint32_t atr = 0x3; // U/S = 0, R/W = 1, P = 1

    mmu_mapPage(virt, cr3, fisi, atr);


    //mappear memoria para que el kernel copie el código en el mismo lugar
    // Hardcodeado para testear con el jugador siempre A
    uint32_t area_tarea_A = 0x10000;

    copyHomework(area_tarea_A, virt, 8192);


    return 0;
}

uint32_t mmu_mappear4mbKernel() {
 //cr3 = 0x28000 << (32-20)//
    /*inicializar el directorio de páginas en la dirección 0x27000*/
    page_dir_entry *pd = (page_dir_entry*) 0x27000;
    //page_table_entry *pt = (page_table_entry*) (pd[0].base<<12);
    page_table_entry *pt = (page_table_entry*) 0x28000;
    
    for(int i = 0; i < Directory_size; i++)
    {
        pd[i] = (page_dir_entry){
            (uint8_t) 0x0,/*[0] = P(presente)*/
            (uint8_t) 0x0,/*[1] = (R/W)(0 Read Only | 1 puede ser escrita)*/
            (uint8_t) 0x0,/*[2] = (U/S)(0 es supervisor | 1 es usuario. En general 0->DLP=00 y 1->DLP=resto)*/
            (uint8_t) 0x0,/*[3] = PWT*/
            (uint8_t) 0x0,/*[4] = PCD*/
            (uint8_t) 0x0,/*[5] = (A)(Cada vez que la pagina es accedida este bit se setea)*/
            (uint8_t) 0x0,/*[6] = (D)(El SO la inicializa en 0, y se setea automaticamente cuando
  se escribe la página)*/
            (uint8_t) 0x0,/*[7] = (PS)(Si es 0 decribe una tabla de paginas de 4KB, y si es 1  4MB)*/
            (uint8_t) 0x0,/*[8] = (G)(Tiene efecto la activacion de lafuncionalidad Global, 
  que otorga ese caracter a la traduccion de esa ṕagina almacenada en la TLB. 
  La entrada no se flushea cuando se recarga el registro CR3)*/
            (uint8_t) 0x0,/*[11..9] = (DLP)privilege-Level*/
            (uint32_t) 0x0000/*[31..12] = Dirección base de la página*/
        };
    }
    pd[0].p = 0x1;/*[0] = P(presente)*/
    pd[0].us = 0x0;
    pd[0].rw = 0x1;/*[1] = (0 Read Only | 1 puede ser escrita)*/
    pd[0].base = (0x28000 >> 12);/*[31..12] = Dirección base de la page_table*/
    
    //0x00000000 a 0x003FFFFF son exactamente 1024 paginas
    for(int i = 0;i<1024; i++)
    {
        //base de la pagina, base de la page_directory,base de la pagina,lvlRW1,lvlUS1
        ///Indice del page directory & Indice del page table///
        //uint32_t page = mmu_nextFreeKernelPage();
        uint32_t INDEX_PTE = i;//se omiten los 12 bits(0's), por que todas las paginas son de 4kb

        pt[INDEX_PTE].p = 0x1;
        pt[INDEX_PTE].rw = 0x1;
        pt[INDEX_PTE].us = 0x0;
        pt[INDEX_PTE].pcd = 0x0;
        pt[INDEX_PTE].a = 0x0; // no fue accedida
        pt[INDEX_PTE].d = 0x0; // no esta dirty
        pt[INDEX_PTE].pat = 0x0; //no quiero PAT
        pt[INDEX_PTE].g = 0x0; //no es global
        pt[INDEX_PTE].dpl = 0x3; //no es global
        pt[INDEX_PTE].base = i; //se omiten los 12 bits(0's), por que todas las paginas son de 4kb
        tlbflush();
    }
    //next_page_free = next_page_free + (0x1000*1024);
    return 0;
}

uint32_t mmu_initKernelDir() {
    mmu_mappear4mbKernel();
}








