/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef enum e_direction {
    Right = 1,
    Left = 2,
    Up = 3,
    Down = 4,
} e_direction_t;

typedef enum e_datatype {
    Null = 0, // Si no se llega a ver
    None = 10, // Si no hay nada
    Player = 20, // Si esta el jugador
    Opponent = 30, // Si esta el oponente
    Food = 40, // Si hay fruta  (puede ser de 16, 32 ó 64)
} e_datatype_t;

void game_init();
void game_nextStep();

#endif  /* !__GAME_H__ */
