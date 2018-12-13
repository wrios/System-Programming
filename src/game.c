/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"

void game_init() {

    // inicializamos todas las tareas
    for (int i = 0; i < 20; i++){
        tss_inicializar(&tss_array[i], i);
        tss_array[i].eax = 64;
    }

}

void game_nextStep() {
}


