/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"

void game_init() {

    for (int i = 0; i < 20; i++){
        tss_inicializar(&tss_array[i], i);
        scheduler.tareas_pilas0[i] = tss_array[i].esp0;
        tss_array[i].eax = 0;
    }
    tss_array[0].eax = tss_array[10].eax = 64;

    for(int i = 0; i < 50; i++) {
      for(int j = 0; j < 50; j++) {
          char cell = getCell(scheduler.tablero[i][j]);
          print(" ", i, j, cell);
      }
    }
}

void game_nextStep() {
}


