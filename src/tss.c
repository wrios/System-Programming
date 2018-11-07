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



tss_init_gdt(uint32_t i, uint32_t cr3){

  mmu_initTaskDir();


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

void tss_idle_initial(uint32_t cr3) {//La rutina debe solicitar dos paginas de area libre de tareas 
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
      (uint16_t)  0x0, //ss0 es la pila del kernel de esta tarea 
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, // esp1
      (uint16_t)  0x0,  //  ss1,
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, //esp2
      (uint16_t)  0x0, //ss2,
      (uint16_t)  0x0, // unused
      (uint32_t)  0x00027000, // cr3
      (uint32_t)  0x14000,
      (uint32_t)  0x0, // eflags
      (uint32_t)  0x0, // eax
      (uint32_t)  0x0, // ecx
      (uint32_t)  0x0, // edx
      (uint32_t)  0x0, // ebx
      (uint32_t)  0x27000, // Y ESTO TAMBIÉN QUE ONDA
      (uint32_t)  0x27000, // QUE ONDA ESTO?
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
  uint32_t vir = 0x14000;
  uint32_t phy = 0x14000;
  uint32_t attr = 0x1; //  U/S = 0, R/W = 0, P = 1

  mmu_mapPage(vir, cr3, phy, attr);

}

tss_inicializar(uint32_t jugador, uint32_t cr3){

  // A = 1
  if (jugador){
    for(uint32_t i = 1; i < 11 ; i++){
      //buscamos página libre
      if(!gdt[i].p){
        tss_init_gdt(i);
        break;
      }
      // si no encontramos página libre
      if(i==10){
          //
      }
    }

  // B = 0
  }else{
    for(uint32_t i = 11; i < 21 ; i++){
      //buscamos página libre
      if(!gdt[i].p){
        tss_init_gdt(i);        
        break;
      }
      // si no encontramos página libre
      if(i==10){
         //        
      }
    }

  }

}


