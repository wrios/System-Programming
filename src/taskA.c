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

//while(1) {
	for(int i=0;i<5;i++) {
		syscall_move(1, Down);
        i = i/i;
	}
	//syscall_move(1, Right);
//}

    //breakpoint();

    int32_t a = syscall_divide();
    if(a==0) {
        syscall_move(1, Down);
        syscall_move(1, Down);
        a = a/a;
    } else {
        while(1){
        syscall_move(1, Up);
        syscall_move(1, Up);
        }
    }

    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
