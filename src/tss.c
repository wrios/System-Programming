/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;

void tss_init() {//La rutina debe solicitar dos paginas de area libre de tareas 
//donde copiar el código de la tarea y mapear dicha página
//a partir de la dirección virtual 0x08000000(128MB).
//Escribir una rutina (mmu initTaskDir) encargada de inicializar un directorio de páginas
//y tablas de páginas para una tarea, respetando la figura 3.
//Sugerencia: agregar a esta función todos los parámetros que considere necesarios.
  tss_entry* init_tss_1 = (tss_entry*) mmu_nextFreeTaskPage();
  tss_entry* init_tss_2 = (tss_entry*) mmu_nextFreeTaskPage();
  

}

