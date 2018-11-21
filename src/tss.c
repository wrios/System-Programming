/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "gdt.h"

tss tss_initial;
tss tss_idle;
tss tss_array[TSS_MAX_AMOUNT_TASKS];


// inicializar tarea para un jugador, se llama en la función tss_inicializar
void tss_init_gdt(uint32_t i, uint32_t cr3){
  //mmu_initTaskDir(char jugador, uint32_t cr3);
  /*
  if (i >= 21 ) {
    mmu_initTaskDir('B', cr3);
  }else{
    mmu_initTaskDir('A', cr3);
  }
  */
  

  uint32_t pag_1_vir = mmu_nextFreeTaskPage_virtual();
  uint32_t pag_2_vir = mmu_nextFreeTaskPage_virtual();
  uint32_t pag_1_fis = mmu_nextFreeTaskPage_fisica();
  uint32_t pag_2_fis = mmu_nextFreeTaskPage_fisica();


  uint32_t atr = 0x3; // U/S = 0, R/W = 1, P = 1

  mmu_mapPage(pag_1_vir, cr3, pag_1_fis, atr);
  mmu_mapPage(pag_2_vir, cr3, pag_2_fis, atr);

  gdt[i].p = 1;

  // seteo la dirección correspondiente a la entrada de la tss de esta tarea
  gdt[i].base_0_15 = (uint32_t) &tss_array[i] >> 16;
  gdt[i].base_23_16 = ((uint32_t) &tss_array[i] << 8) >> 24;
  gdt[i].base_31_24 = (uint32_t) &tss_array[i] >> 24;

  gdt[i].limit_0_15 = 0x67;
  gdt[i].limit_16_19 = 0x0;
}

void tss_idle_initial() {
//La rutina debe solicitar dos paginas de area libre de tareas 
//donde copiar el código de la tarea y mapear dicha página
//a partir de la dirección virtual 0x08000000(128MB).
//Escribir una rutina (mmu initTaskDir) encargada de inicializar un directorio de páginas
//y tablas de páginas para una tarea, respetando la figura 3.
//Sugerencia: agregar a esta función todos los parámetros que considere necesarios.
  //tss_entry* init_tss_1 = (tss_entry*) mmu_nextFreeTaskPage();
  //tss_entry* init_tss_2 = (tss_entry*) mmu_nextFreeTaskPage();
  tss_array[20] = (tss) {
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint32_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint32_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint32_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint32_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x0,
      (uint16_t)  0x1, //dtrap
      (uint16_t)  0x0 //iomap
  };
    tss_array[21] = (tss) {
      (uint16_t)  0x0, // ptl
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, // esp0 PREGUNTAR QUE ONDA ESTO
      (uint16_t)  0x0, //ss0 PREGUNTAR QUE ONDA ESTO
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, // esp1
      (uint16_t)  0x0,  //  ss1,
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, //esp2
      (uint16_t)  0x0, //ss2,
      (uint16_t)  0x0, // unused
      (uint32_t)  0x00027000, // cr3
      (uint32_t)  0x14000,
      (uint32_t)  0x202, // eflags (Interrupciones habilitadas)
      (uint32_t)  0x0, // eax
      (uint32_t)  0x0, // ecx
      (uint32_t)  0x0, // edx
      (uint32_t)  0x0, // ebx
      (uint32_t)  0x27000, // Y ESTO TAMBIÉN QUE ONDA(esp3)
      (uint32_t)  0x27000, // QUE ONDA ESTO?(ebp3)
      (uint32_t)  0x0, // esi
      (uint32_t)  0x0, // edi
      (uint16_t)  0x17, // es
      (uint16_t)  0x0, // unused
      (uint16_t)  0x16, // cs
      (uint16_t)  0x0, // unused
      (uint16_t)  0x17, // segmento pila del kernel RE PREGUNTAR
      (uint16_t)  0x0, // unused
      (uint16_t)  0x17, // ds kernel
      (uint16_t)  0x0, //unused
      (uint16_t)  0x1A, // fs kernel - PREGUNTAR
      (uint16_t)  0x0, // unused
      (uint16_t)  0x17, // gs data kernel
      (uint16_t)  0x0, //unused
      (uint16_t)  0x0, //ldt - PREGUNTAR
      (uint16_t)  0x0, //unused
      (uint16_t)  0x1, //dtrap
      (uint16_t)  0x0 //iomap
  };

  // identity mapping por enunciado
  uint32_t attr = 0x3; //  U/S = 0, R/W = 1, P = 1
  tss_array[21].eip = 0x00014000;
  tss_array[21].cr3 = getCR3();
  tss_array[21].esp0 = 0x27000;
  gdt[28].base_0_15 = ((uint32_t)(&tss_array[21]) << 24) >> 24;
  gdt[28].base_23_16 = ((uint32_t)(&tss_array[21]) << 8) >> 24;
  gdt[28].base_31_24 = (uint32_t)(&tss_array[21]) >> 24;
  gdt[28].limit_0_15 = 0x67;
  gdt[28].limit_16_19 = 0x0;
  uint32_t phy mmu_nextFreeTaskPage();
  mmu_mapPage(0x08000000, tss_array[21].cr3, phy, attr);
}

void tss_inicializar(tss* tss_task, uint32_t jugador){//inicializa una tarea
  //Inicio la GDT 
  gdt[jugador].base_0_15 = ((uint32_t)tss_task) && 0xFFFF; 
  gdt[jugador].base_23_16 = (((uint32_t)tss_task) >> 16) && 0xFFFF;
  gdt[jugador].base_31_24 = (((uint32_t)tss_task) >> 24) && 0xFFFF;
  gdt[jugador].limit_0_15 = sizeof(*tss_task) && 0xFFFF;
  gdt[jugador].limit_16_19 = (sizeof(*tss_task) >> 16) && 0xFFFF;
  //Inicio directorio y tabla para la tarea
  mmu_initTaskDir(tss_task);
  
  for(uint32_t i = 0; i < 1024; i++)
  {
    mmu_mapPage(0x08000000, tss_task->cr3, i, 0x3);
  }
  
  uint32_t page_pila0 = mmu_nextFreeKernelPage();
  mmu_mapPage(0x08000000, tss_task->cr3, page_pila0, 0x3);
  tss_set_attr(tss_task, page_pila0);
  if (jugador <= 10) {//jugador tipos A
    copyHomework(0x10000, tss_task->eip);
  }
  else{//jugador tipo B
    copyHomework(0x12000, tss_task->eip);
  }
}

void tss_set_attr(tss* tss_task, uint32_t pila0){
  tss_task->ebp = pila0;
  tss_task->esp = pila0;
  tss_task->eflags = 0x202;
}
