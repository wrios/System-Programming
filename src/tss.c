/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "gdt.h"


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
      (uint16_t)  0x0,// ptl
      (uint16_t)  0x0,//unused
      (uint32_t)  0x0,//esp0
      (uint16_t)  0x0,//ss0
      (uint16_t)  0x0,//unused
      (uint32_t)  0x0,//esp1
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3),//ss0
      (uint16_t)  0x0,//unused
      (uint32_t)  0x0,//esp2
      (uint16_t)  0x0,//ss2
      (uint16_t)  0x0,//unused
      (uint32_t)  0x0,//cr3
      (uint32_t)  0x0,//eip
      (uint32_t)  0x202,//eflags
      (uint32_t)  0x0,//eax
      (uint32_t)  0x0,//ecx
      (uint32_t)  0x0,//edx
      (uint32_t)  0x0,//ebx
      (uint32_t)  0x0,//esp
      (uint32_t)  0x0,//ebp
      (uint32_t)  0x0,//esi
      (uint32_t)  0x0,//edi
      (uint16_t)  (GDT_ENTRY_DATAS_USER<<3) + 0x03,//es
      (uint16_t)  0x0,//unused
      (uint16_t)  (GDT_ENTRY_CODES_USER<<3) + 0x03,//cs
      (uint16_t)  0x0,//unused
      (uint16_t)  (GDT_ENTRY_DATAS_USER<<3) + 0x03,//ss
      (uint16_t)  0x0,//unused
      (uint16_t)  (GDT_ENTRY_DATAS_USER<<3) + 0x03,//ds
      (uint16_t)  0x0,//unused
      (uint16_t)  (GDT_ENTRY_DATAS_USER<<3) + 0x03,//fs
      (uint16_t)  0x0,//unused
      (uint16_t)  (GDT_ENTRY_DATAS_USER<<3) + 0x03,//gs
      (uint16_t)  0x0,//unused
      (uint16_t)  0x0,//ldt
      (uint16_t)  0x0,//unused
      (uint16_t)  0x0, //dtrap
      (uint16_t)  0x0 //iomap
  };
    tss_array[21] = (tss) {
      (uint16_t)  0x0, // ptl
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x00027000, //comparte con el kernel (esp0)
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3), //ss0 PREGUNTAR QUE ONDA ESTO
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, // esp1
      (uint16_t)  0x0,  //  ss1,
      (uint16_t)  0x0, // reserved
      (uint32_t)  0x0, //esp2
      (uint16_t)  0x0, //ss2,
      (uint16_t)  0x0, // unused
      (uint32_t)  0x00027000, // cr3
      (uint32_t)  0x00000,
      (uint32_t)  0x202, // eflags (Interrupciones habilitadas)
      (uint32_t)  0x0, // eax
      (uint32_t)  0x0, // ecx
      (uint32_t)  0x0, // edx
      (uint32_t)  0x0, // ebx
      (uint32_t)  0x00000,//esp pedir paginas para C y D(esp3)
      (uint32_t)  0x00000,// ebp pedir paginas para C y D(ebp3)
      (uint32_t)  0x0, // esi
      (uint32_t)  0x0, // edi
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3) + 0x03, // es
      (uint16_t)  0x0, // unused
      (uint16_t)  (GDT_ENTRY_CODES_KERNEL<<3) + 0x03, // cs
      (uint16_t)  0x0, // unused
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3) + 0x03, // ss segmento pila del kernel RE PREGUNTAR
      (uint16_t)  0x0, // unused
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3) + 0x03, // ds kernel
      (uint16_t)  0x0, //unused
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3) + 0x03, // fs kernel - PREGUNTAR
      (uint16_t)  0x0, // unused
      (uint16_t)  (GDT_ENTRY_DATAS_KERNEL<<3) + 0x03, // gs data kernel
      (uint16_t)  0x0, //unused
      (uint16_t)  0x0, //ldt - PREGUNTAR
      (uint16_t)  0x0, //unused
      (uint16_t)  0x0, //dtrap
      (uint16_t)  0x0 //iomap
  };
  
  // identity mapping por enunciado
  uint32_t attr = 0x7; //  U/S = 1, R/W = 1, P = 1
  tss_array[21].cr3 = 0x27000;//comparte cr3 con el kernel
  tss_array[21].esp0 = 0x27000;//comparte pila0 con el kernel

  gdt[27].base_0_15 = ((uint32_t)(&tss_array[20]) << 16) >> 16;
  gdt[27].base_23_16 = ((uint32_t)(&tss_array[20]) << 8) >> 24;
  gdt[27].base_31_24 = (uint32_t)(&tss_array[20]) >> 24;

  gdt[28].base_0_15 = ((uint32_t)(&tss_array[21]) << 16) >> 16;
  gdt[28].base_23_16 = ((uint32_t)(&tss_array[21]) << 8) >> 24;
  gdt[28].base_31_24 = (uint32_t)(&tss_array[21]) >> 24;
  

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
            (uint8_t)     0x00,           // db
            (uint8_t)     0x00,           // g
            (uint8_t)     0x00,           // base[31:24]
        };
    };


  /*Iniciar pd, pt para la tarea y copiar codigo y dato*/
  uint32_t phy = mmu_nextFreeTaskPage_fisica();
  mmu_mapPage(phy, tss_array[21].cr3, phy, attr);
  //mmu_mapPage(0x8000000, tss_array[21].cr3, phy, attr);
  uint32_t phy2 = mmu_nextFreeTaskPage_fisica();
  //mmu_mapPage(0x8001000, tss_array[21].cr3, phy2, attr);
  mmu_mapPage(phy2, tss_array[21].cr3, phy2, attr);

  copyHomework((char*) 0x14000,(char*)  phy);
  copyHomework((char*) 0x15000,(char*)  phy2);

  mmu_unmapPage(phy, tss_array[21].cr3);
  mmu_unmapPage(phy2, tss_array[21].cr3);

  mmu_mapPage(TASK_CODE, tss_array[21].cr3, phy, 0x7);
  mmu_mapPage(TASK_CODE+0x1000, tss_array[21].cr3, phy2, 0x7);
  tss_array[21].eip = TASK_CODE;


  tss_array[21].esp = 0x27000;
  tss_array[21].ebp = 0x27000;
}

void tss_inicializar(tss* tss_task, uint32_t jugador){//inicializa una tarea
  //Inicio la GDT 
  gdt[jugador + 31].base_0_15 = (((uint32_t)tss_task) << 16) >> 16; 
  gdt[jugador + 31].base_23_16 = (((uint32_t)tss_task) << 8) >> 24;
  gdt[jugador + 31].base_31_24 = (((uint32_t)tss_task) >> 24);
  //gdt[jugador + 31].limit_0_15 = sizeof(*tss_task) && 0xFFFF;
  //gdt[jugador + 31].limit_16_19 = (sizeof(*tss_task) >> 16) && 0xFFFF;
  //Inicio directorio y tabla para la tarea
  uint32_t thisCR3 =  mmu_initTaskDir(tss_task); 
  for(uint32_t i = 0; i < 1024; i++)//mapeo el kernel y area libre de kernel a cada tarea
  {
    mmu_mapPage(i, tss_task->cr3, i, 0x3);//pd y pt como supervisor
  }
  
  uint32_t page1 = mmu_nextFreeTaskPage_fisica();
  uint32_t page2 = mmu_nextFreeTaskPage_fisica();
  //la tarea tiene mapeada 2 paginas virtuales, para C y D
  mmu_mapPage(page1, thisCR3, page1, 0x5);
  mmu_mapPage(page2, thisCR3, page2, 0x5);
  if (jugador <= 10) {//jugador tipos A
    copyHomework((char* )0x10000, (char* )tss_task->eip);
    copyHomework((char* )0x11000, (char* )tss_task->eip+0x1000);
  }
  else{//jugador tipo B
    copyHomework((char* )0x12000, (char* )tss_task->eip);
    copyHomework((char* )0x13000, (char* )tss_task->eip+0x1000);
  }
  mmu_unmapPage(page1, thisCR3);
  mmu_unmapPage(page2, thisCR3);
  mmu_mapPage(TASK_CODE, tss_task->cr3, page1, 0X7);
  mmu_mapPage(TASK_CODE+0x1000, tss_task->cr3, page2, 0x7);
  tss_task->eip = TASK_CODE;
  //la tercer pagina virtual es para la pila lvl0
  uint32_t page_pila0 = mmu_nextFreeKernelPage();
  mmu_mapPage(0x08002000, tss_task->cr3, page_pila0, 0x3);
  tss_set_attr(tss_task);

}

void tss_set_attr(tss* tss_task){
  tss_task->dtrap = 0x0;
  tss_task->ldt = 0x0;
  tss_task->ebp = 0x08002000;
  tss_task->esp = 0x08002000;
  tss_task->eflags = 0x202;
  tss_task->esp0 = 0x08003000;
  tss_task->ptl = 0x00;
  tss_task->es = (GDT_ENTRY_DATAS_USER<<3);
  tss_task->ds = (GDT_ENTRY_DATAS_USER<<3);
  tss_task->fs = (GDT_ENTRY_DATAS_USER<<3);
  tss_task->cs = (GDT_ENTRY_CODES_USER<<3);
  tss_task->gs = (GDT_ENTRY_DATAS_USER<<3);
  tss_task->ss = (GDT_ENTRY_DATAS_USER<<3);
  tss_task->ss0 = (GDT_ENTRY_DATAS_KERNEL<<3);
}
