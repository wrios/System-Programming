/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "syscall.h" 

void task() {

    //int a = syscall_read(-1,0);
    while(1) {
        syscall_move(1, Down);
        syscall_move(1, Right);
    }


    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
