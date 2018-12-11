/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"

void game_init() {

    tss_inicializar(&tss_array[0], 0);
    tss_inicializar(&tss_array[10], 10);
    tss_array[0].eax = 64;
    tss_array[10].eax = 64;

}

void game_nextStep() {
}


