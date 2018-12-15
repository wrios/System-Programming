/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del schedulerler
*/

#include "sched.h"
#include "screen.h"

#define abs(x) (x<0? -x : x)
#define min(a, b) (a<b? a : b)
#define max(a, b) (a>b? a : b)
#define true 1
#define false 0

schedu scheduler;
int Tarea_actual;


void sched_init() {

  //init struct sheduler
  scheduler.termino_el_juego = false;
  scheduler.debug_mode = false;

  for(int i=0; i<cant_tareas; i++)
    scheduler.ya_jugo[i] = true;
  scheduler.ya_jugo[0]=scheduler.ya_jugo[10]=false;

  for(int i=0; i<cant_tareas; i++)
    scheduler.muertas[i] = true;
  scheduler.muertas[0]=scheduler.muertas[10]=false;

  scheduler.puntosA = scheduler.puntosB = 0;

  for(int i=0; i<cant_tareas; i++)
    scheduler.peso_por_tarea[i]=0;
  scheduler.peso_por_tarea[0]=scheduler.peso_por_tarea[10]=64;

  for(int i=0; i<cant_tareas; i++)
    scheduler.cant_llamadas_a_read_por_tarea[i]=0;

  coord coor_init = {0,0};
  for(int i=0; i<cant_tareas; i++)
    scheduler.coordenadas_actuales[i]=coor_init;
  coord a = {10,10};
  coord b = {10,2};
  scheduler.coordenadas_actuales[0] = a;
  scheduler.coordenadas_actuales[10] = b;

  for(int i=0; i<cant_tareas; i++)
    scheduler.coordenadas_siguientes[i]=coor_init;
  scheduler.coordenadas_siguientes[0] = a;
  scheduler.coordenadas_siguientes[10] = b;

  //init struct tablero_sched_juego
  for(int i=0; i<tam_tablero; i++)
    for(int j=0; j<tam_tablero; j++)
      scheduler.tablero[i][j]=Non;
  scheduler.tablero[a.x][a.y]=Playe;
  scheduler.tablero[b.x][b.y]=Opp;

  //init algunas frutas
  coord init_frutas[13] = {{0,0}, {10,12}, {10,13}, {10,14}, {10,15}, {5,20}, {5,12}, {6,29}, {29,23}, {29,40}, {29,49}, {0,40}, {30,30}};
  for(int i=0; i<13; i++){
    int x = init_frutas[i].x;
    int y = init_frutas[i].y;
    scheduler.tablero[x][y]= i%2? 16 : 32;
  }
}

uint32_t termino_el_juego(){
  //Termina el juego sii no hay frutas o 
  //no hay mas tareas de al menos un jugador
  
  uint32_t hay_fruta = false;
  for(int i=0; i<50; i++)
    for(int j=0; j<50; j++)
      if( frutaEn(i, j) )
        hay_fruta = true;

  uint32_t murio_A = true;
  for(int i=0; i<10; i++)
    if( scheduler.muertas[i] == false )
      murio_A = false;

  uint32_t murio_B = true;
  for(int i=10; i<20; i++)
    if( scheduler.muertas[i] == false )
      murio_B = false;

  if( hay_fruta == false || murio_A || murio_B ){
    scheduler.termino_el_juego = true;
    return true;
  }

  return false;
}

int16_t sched_nextTask() {
  
  if( scheduler.termino_el_juego || scheduler.debug_mode ) return 28;
  
  int k = 0;
  while(k < 20){
    if( scheduler.muertas[k] == false && scheduler.ya_jugo[k] == false )
      break;
    k++;
  }
  //Paso un turno
  if (k == 20){

    //Resuelvo para cada casilla del tablero
    for(int i = 0; i < 50; i++){
      for(int j = 0; j < 50; j++){

        int sum_A_peso, sum_B_peso;
        sum_A_peso = sum_B_peso = 0;

        //Calculo pesos de tareas que caigan (i, j)
        for(int h = 0; h < 10; h++)
          if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            sum_A_peso += scheduler.peso_por_tarea[h];
          }
        for(int h = 10; h < 20; h++)
          if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            sum_B_peso += scheduler.peso_por_tarea[h];
          }

        //Resuelvo todos los conflictos
        if(sum_A_peso == sum_B_peso){
          //Caso "ambos pesos iguales" --> mueren todas
          for(int h = 0; h < 20; h++){
            if (scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
              scheduler.muertas[h] = true;            
            }
          }
          //Queda lo que habia en ese (i, j) (fruta o nada)
          if(frutaEn(i,j) > 0)
            scheduler.tablero[i][j] = frutaEn(i,j);
          else
            scheduler.tablero[i][j] = 10;

        }else if (sum_A_peso > sum_B_peso){
          //Caso "A gana a B" --> mueren las de B
          for(int h = 10; h < 20; h++){
            if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
              scheduler.muertas[h] = true;            
            }
          }
          for(int h = 0; h < 10; h++){
            if(scheduler.muertas[h]==false && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
              scheduler.puntosA += sum_B_peso;
              scheduler.puntosA += frutaEn(i,j);
              break;//lo que se gana va al primero que encuentra el for
            }
          }
          //Queda A en ese (i, j)
          scheduler.tablero[i][j] = 20;

        }else if (sum_B_peso > sum_A_peso){
          //Caso "B gana a A" --> mueren las de A
          for(int h = 0; h < 10; h++){
            if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
              scheduler.muertas[h] = true;            
            }
          }  
          for(int h = 10; h < 20; h++){
            if(scheduler.muertas[h]==false && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
              scheduler.puntosB += sum_A_peso;
              scheduler.puntosB += frutaEn(i,j);
              break;//lo que se gana va al primero que encuentra el for
            }
          }
          //Queda B en ese (i, j)
          scheduler.tablero[i][j] = 30;
        }      
      }
    }

    //Como ya termino el turno...
    // Actualizo todas las coordenadas -> las siguientes pasan a ser las actuales
    // Actualizo .ya_jugo diciendo que le falta jugar a todas
    // Actualizo las # llamadas a read -> todas en 0
    // Actualizo el tablero
    // Acutalizo los puntajes de A y B (ya lo hice)
    for(int h=0; h<20; h++){
      scheduler.coordenadas_actuales[h] = scheduler.coordenadas_siguientes[h];
      scheduler.ya_jugo[h] = false;
      scheduler.cant_llamadas_a_read_por_tarea[h] = 0;
      //tablero..
      int x = scheduler.coordenadas_siguientes[h].x;
      int y = scheduler.coordenadas_siguientes[h].y;
      if(scheduler.muertas[h] != 1){
        if (h < 10)
          scheduler.tablero[x][y] = 20;
        else
          scheduler.tablero[x][y] = 30;
      }
    }

    //Printeo todo
    for(int i = 0; i < 50; i++) {
      for(int j = 0; j < 50; j++) {
        char cell = getCell(scheduler.tablero[i][j]);
        print(" ", i, j, cell);
      }
    } 
    print("Jugador A", 61, 4, C_FG_WHITE);
    print_dec(scheduler.puntosA, 10, 60, 6, C_FG_WHITE);
    print("Jugador B", 61, 29, C_FG_WHITE);
    print_dec(scheduler.puntosB, 10, 60, 31, C_FG_WHITE);

    //Si termino el juego..
    if( termino_el_juego() ){

      print("Fin del juego", 17, 22, C_BG_GREEN);
      
      if( scheduler.puntosA > scheduler.puntosB ){
        //gano A
        print("Gano el jugador A :D", 14, 24, C_BG_GREEN);
      }else if( scheduler.puntosA < scheduler.puntosB ){
        //gano B
        print("Gano el jugador B :c", 14, 24, C_BG_GREEN);
      }else{
        //Empate
        print("Hubo un empate :o", 14, 24, C_BG_GREEN);
      }
      return 28;//idle
    }
    
    k=0;
    while(k < 20){
      if( scheduler.muertas[k] == false && scheduler.ya_jugo[k] == false )
        break;
      k++;
    }

  }else{
    scheduler.ya_jugo[k] = true;
  }

  Tarea_actual = k;
  //print_dec(k+31, 10, 40, 5, C_BG_GREEN);//debugger
  return k+31;
}

//funciones auxiliares de read
uint32_t chequear_vision_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  int distancia_pedida = abs(x-eax)+abs(y-ebx);
  int distancia_maxima = scheduler.peso_por_tarea[Tarea_actual];
  int distancia_efectiva = min(distancia_pedida, distancia_maxima);
  scheduler.cant_llamadas_a_read_por_tarea[Tarea_actual]++;
  if (scheduler.cant_llamadas_a_read_por_tarea[Tarea_actual] > scheduler.peso_por_tarea[Tarea_actual]){
    return 0;
  }
  return distancia_efectiva < distancia_maxima;
}

uint32_t read_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  return scheduler.tablero[ ((eax+x)+tam_tablero)%tam_tablero ][ ((ebx+y)+tam_tablero)%tam_tablero ];
}

//funciones auxiliares de move
uint32_t move_actualizar_C(uint32_t distancia, uint32_t dir){

  int x = scheduler.coordenadas_siguientes[Tarea_actual].x;
  int y = scheduler.coordenadas_siguientes[Tarea_actual].y;

  int maxima_distancia_moverse = 64/scheduler.peso_por_tarea[Tarea_actual];
  int distancia_efectiva = min(maxima_distancia_moverse, distancia);

  coord a_donde_me_muevo = {x, y};
  if( dir == 1 )
    a_donde_me_muevo.x = (a_donde_me_muevo.x + distancia_efectiva)%tam_tablero;
  else if( dir == 2 )
    a_donde_me_muevo.x = ((a_donde_me_muevo.x - distancia_efectiva)+tam_tablero)%tam_tablero;
  else if( dir == 3 )
    a_donde_me_muevo.y = ((a_donde_me_muevo.y - distancia_efectiva) + tam_tablero)%tam_tablero;
  else if(dir == 4)
    a_donde_me_muevo.y = (a_donde_me_muevo.y + distancia_efectiva)%tam_tablero;

  scheduler.ya_jugo[Tarea_actual] = true;
  scheduler.coordenadas_siguientes[Tarea_actual] = a_donde_me_muevo;

  return distancia_efectiva;
}

uint32_t frutaEn(int i, int j){
  if (scheduler.tablero[i][j] == 16 || scheduler.tablero[i][j] == 32 || scheduler.tablero[i][j] == 64){
    return scheduler.tablero[i][j];
  }else{
    return 0;
  }
}

//funciones auxiliares de divide
uint32_t checkear_poder_div_C(){
  uint32_t peso_tarea_actual = scheduler.peso_por_tarea[Tarea_actual];
  uint32_t peso_valido = peso_tarea_actual > 1;
  
  uint32_t espacio_tss_libre = false;
  //Me fijo si la Tarea_actual es de A o B
  if(Tarea_actual < 10){
    for(int i = 0; i < 10; i++)
      if( scheduler.muertas[i] )
        espacio_tss_libre = true;
  }else{
    for(int i=10; i<20; i++)
      if(scheduler.muertas[i] )
        espacio_tss_libre = true;
  }
    
  return peso_valido && espacio_tss_libre;    
}

uint32_t copiar_tarea_C(){
  //Pre: checkear_poder_div_C dio true
  scheduler.peso_por_tarea[Tarea_actual] /= 2;
  scheduler.ya_jugo[Tarea_actual] = true;

  int indice_nueva_tarea;
  //Me fijo si la Tarea_actual es de A o B 
  if(Tarea_actual < 10){
    for(int i = 0; i < 10; i++)
      if( scheduler.muertas[i] )
        {indice_nueva_tarea = i; break;}
  }else{
    for(int i=10; i<20; i++)
      if( scheduler.muertas[i] )
        {indice_nueva_tarea = i; break;}
  }

  //existe una biyeccion entre indices tareas e indices de tss
  tss* nueva_tss =  &tss_array[indice_nueva_tarea];
  nueva_tss->eip = TASK_CODE;
  nueva_tss->esp0 = scheduler.tareas_pilas0[indice_nueva_tarea];
  nueva_tss->ebp = 0x8002000;
  nueva_tss->esp = 0x8002000;

  scheduler.muertas[indice_nueva_tarea] = false;
  scheduler.ya_jugo[indice_nueva_tarea] = true;
  scheduler.cant_llamadas_a_read_por_tarea[indice_nueva_tarea] = 0;
  
  //inicializo informaciones de la nueva tarea
    //-> empieza donde esta la tarea actual
  scheduler.coordenadas_siguientes[indice_nueva_tarea] = scheduler.coordenadas_siguientes[Tarea_actual];
    //-> con la mitad del peso
  scheduler.peso_por_tarea[indice_nueva_tarea] = scheduler.peso_por_tarea[Tarea_actual];

  return 1;
}

void debug_modeC(){
  if( scheduler.debug_mode == true ){
    for(int i = 0; i < 50; i++) {
      for(int j = 0; j < 50; j++) {
        char cell = getCell(scheduler.tablero[i][j]);
        print(" ", i, j, cell);
      }
    } 
    if( termino_el_juego() ){
      print("Fin del juego", 17, 22, C_BG_GREEN);
      if( scheduler.puntosA > scheduler.puntosB ){
        print("Gano el jugador A :D", 14, 24, C_BG_GREEN);
      }else if( scheduler.puntosA < scheduler.puntosB ){
        print("Gano el jugador B :c", 14, 24, C_BG_GREEN);
      }else{
        print("Hubo un empate :o", 14, 24, C_BG_GREEN);
      }
    }
    scheduler.debug_mode = false;
  }else{
    /* limpio */
    for(int i = 0; i < 50; i++)
      for(int j = 0; j < 50; j++)
        print(" ", i, j, C_BG_LIGHT_GREY);
    print("MODO DEBUG", 69, 1, C_FG_WHITE);  
    uint32_t x = 5;
    uint32_t y = 2;
    /* eax, ebx, ecx, edx, esi, edi, ebp, esp, eip */
    print("eax", 6+x, 5+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].eax, 8, 10+x, 5+y, C_FG_WHITE);
    print("ebx", 6+x, 7+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ebx, 8, 10+x, 7+y, C_FG_WHITE);
    print("ecx", 6+x, 9+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ecx, 8, 10+x, 9+y, C_FG_WHITE);
    print("edx", 6+x, 11+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].edx, 8, 10+x, 11+y, C_FG_WHITE);
    print("esi", 6+x, 13+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].esi, 8, 10+x, 13+y, C_FG_WHITE);
    print("edi", 6+x, 15+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].edi, 8, 10+x, 15+y, C_FG_WHITE);
    print("ebp", 6+x, 17+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ebp, 8, 10+x, 17+y, C_FG_WHITE);
    print("esp", 6+x, 19+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].esp, 8, 10+x, 19+y, C_FG_WHITE);
    print("eip", 6+x, 21+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].eip, 8, 10+x, 21+y, C_FG_WHITE);
    /* cs, ds, es, fs, gs, ss */
    print("cs", 7+x, 23+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].cs, 4, 10+x, 23+y, C_FG_WHITE);
    print("ds", 7+x, 25+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ds, 4, 10+x, 25+y, C_FG_WHITE);
    print("es", 7+x, 27+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].es, 4, 10+x, 27+y, C_FG_WHITE);
    print("fs", 7+x, 29+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].fs, 4, 10+x, 29+y, C_FG_WHITE);
    print("gs", 7+x, 31+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].gs, 4, 10+x, 31+y, C_FG_WHITE);
    print("ss", 7+x, 33+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ss, 4, 10+x, 33+y, C_FG_WHITE);
    /* eflags */
    print("eflags", 7+x, 37+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].ss, 8, 14+x, 37+y, C_FG_WHITE);
    /* cr0, cr2, cr3, cr4 */  //ESTA BIEN DE DONDE LOS SACO ??
    print("cr0", 22+x, 6+y, C_BG_LIGHT_GREY);
    print_dec(rcr0(), 8, 26+x, 6+y, C_FG_WHITE);
    print("cr2", 22+x, 8+y, C_BG_LIGHT_GREY);
    print_dec(rcr2(), 8, 26+x, 8+y, C_FG_WHITE);
    print("cr3", 22+x, 10+y, C_BG_LIGHT_GREY);
    print_dec(rcr3(), 8, 26+x, 10+y, C_FG_WHITE);
    print("cr4", 22+x, 12+y, C_BG_LIGHT_GREY);
    print_dec(rcr4(), 8, 26+x, 12+y, C_FG_WHITE);
    /* stack */ //ESTO ESTA BIEN QUE IMPRIMA LOS esp ????
    print("stack", 22+x, 23+y, C_BG_LIGHT_GREY);
    print_dec(tss_array[Tarea_actual].esp, 8, 22+x, 26+y, C_FG_WHITE);
    print_dec(tss_array[Tarea_actual].esp0, 8, 22+x, 27+y, C_FG_WHITE);
    print_dec(tss_array[Tarea_actual].esp1, 8, 22+x, 28+y, C_FG_WHITE);
    print_dec(tss_array[Tarea_actual].esp2, 8, 22+x, 29+y, C_FG_WHITE);
    
    //horizontales
    for(x=8; x<42; x++){
      print(" ", x, 42, C_FG_DARK_GREY);
      print(" ", x, 3, C_FG_DARK_GREY);
    }
    //verticales
    for(y=3; y<43; y++){
      print(" ",7, y, C_FG_DARK_GREY);
      print(" ",42, y, C_FG_DARK_GREY);
    }
    
    scheduler.debug_mode = true;
  }
    
}

void remove_taskC(){
  scheduler.muertas[Tarea_actual] = true;
  scheduler.peso_por_tarea[Tarea_actual] = 0;
  if( termino_el_juego() ){
    scheduler.termino_el_juego = true;
    print("Fin del juego", 17, 22, C_BG_GREEN);
    if( scheduler.puntosA > scheduler.puntosB ){
      print("Gano el jugador A :D", 14, 24, C_BG_GREEN);
    }else if( scheduler.puntosA < scheduler.puntosB ){
      print("Gano el jugador B :c", 14, 24, C_BG_GREEN);
    }else{
      print("Hubo un empate :o", 14, 24, C_BG_GREEN);
    }
  }
}