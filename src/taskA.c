/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "syscall.h" 
#include "game.h"

void task() {
    int error;

    for(int i=0;i<6;i++)
        syscall_move(1, Right);
        
//while(1) {
	for(int i=0;i<5;i++) {
		syscall_move(1, Down);
        error = (i+1)/i;
	}
	//syscall_move(1, Right);
//}

    int32_t a = syscall_divide();
    if(a==0 && error ) {
        syscall_move(1, Down);
        syscall_move(1, Down);
    } else {
        while(1){
        syscall_move(1, Up);
        syscall_move(1, Up);
        }
    }

    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
