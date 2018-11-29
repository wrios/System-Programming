/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del schedulerler
*/

#include "sched.h"

#define abs(x) (x<0? -x : x)
#define min(a, b) (a<b? a : b)
#define max(a, b) (a>b? a : b)
#define true 1
#define false 0

schedu scheduler;
tablero tablero_sched;
int Tarea_actual;

void sched_init() {

  //init struct sheduler
  for(int i=0; i<cant_tareas; i++)
    scheduler.falta_jugar[i] = 0x0;
  scheduler.falta_jugar[0]=scheduler.falta_jugar[10]=0x1;

  for(int i=0; i<cant_tareas; i++)
    scheduler.ya_jugo[i] = 0x0;

  for(int i=0; i<cant_tareas; i++)
    scheduler.muertas[i] = 0x1;
  scheduler.muertas[0]=scheduler.muertas[10]=0x0;

  for(int i=0; i<cant_tareas; i++)
    scheduler.puntos_por_tarea[i]=0;
  //scheduler.puntos_por_tarea[0]=scheduler.puntos_por_tarea[10]=64;

  for(int i=0; i<cant_tareas; i++)
    scheduler.peso_por_tarea[i]=0;
  scheduler.peso_por_tarea[0]=scheduler.peso_por_tarea[10]=64;

  for(int i=0; i<cant_tareas; i++)
    scheduler.cant_llamadas_a_read_por_tarea[i]=0;

  coord coor_init = {0,0};
  for(int i=0; i<cant_tareas; i++)
    scheduler.coordenadas_actuales[i]=coor_init;
  coord a = {10,10};
  coord b = {40,40};
  scheduler.coordenadas_actuales[0] = a;
  scheduler.coordenadas_actuales[10] = b;

  for(int i=0; i<cant_tareas; i++)
    scheduler.coordenadas_siguientes[i]=coor_init;

  //init struct tablero_sched_juego
  for(int i=0; i<tam_tablero; i++)
    for(int j=0; j<tam_tablero; j++)
      tablero_sched.info[i][j]=Non;
  tablero_sched.info[10][10]=Playe;
  tablero_sched.info[40][40]=Opp;

  //init algunas frutas (10)
  coord init_frutas[10] = {{0,0}, {2,10}, {5,20}, {5,12}, {6,29}, {29,23}, {29,40}, {29,49}, {0,40}, {30,30}};
  for(int i=0; i<10; i++){
    int x = init_frutas[i].x;
    int y = init_frutas[i].y;
    tablero_sched.info[x][y]=Fruta;
  }
}


int16_t sched_nextTask() {
  return 0;
}


//funciones auxiliares de read
uint32_t chequear_vision_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  int distancia_pedida = abs(x-eax)+abs(y-ebx);
  int distancia_maxima = scheduler.peso_por_tarea[Tarea_actual];
  int distancia_efectiva = min(distancia_pedida, distancia_maxima);
  
  return distancia_efectiva > distancia_maxima;
  return 0;
}

uint32_t read_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  scheduler.cant_llamadas_a_read_por_tarea[Tarea_actual]++;
  return tablero_sched.info[ ((eax+x)+tam_tablero)%tam_tablero ][ ((ebx+y)+tam_tablero)%tam_tablero ];
}

//funciones auxiliares de move
uint32_t move_actualizar_C(uint32_t distancia, uint32_t dir){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  int maxima_distancia_moverse = 64/scheduler.peso_por_tarea[Tarea_actual];
  int distancia_efectiva = min(maxima_distancia_moverse, distancia);

  coord a_donde_me_muevo = {x, y};
  if( dir == 1 )
    a_donde_me_muevo.x = (a_donde_me_muevo.x + distancia_efectiva)%tam_tablero;
  else if( dir == 2 )
    a_donde_me_muevo.x = ((a_donde_me_muevo.x - distancia_efectiva)+tam_tablero)%tam_tablero;
  else if( dir == 3 )
    a_donde_me_muevo.y = (a_donde_me_muevo.y + distancia_efectiva)%tam_tablero;
  else
    a_donde_me_muevo.y = ((a_donde_me_muevo.y - distancia_efectiva)+tam_tablero)%tam_tablero;

  scheduler.coordenadas_siguientes[Tarea_actual] = a_donde_me_muevo;

  return distancia_efectiva;
  return 0;
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

  scheduler.peso_por_tarea[Tarea_actual] /= 2;

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
  tss_inicializar(nueva_tss, indice_nueva_tarea);

  //inicializo informaciones de la nueva tarea
    //empieza donde esta la tarea actual
  scheduler.coordenadas_siguientes[indice_nueva_tarea] = scheduler.coordenadas_siguientes[Tarea_actual];
    //con la mitad del peso
  scheduler.peso_por_tarea[indice_nueva_tarea] = scheduler.peso_por_tarea[Tarea_actual];
    //con 0 puntos
  scheduler.puntos_por_tarea[indice_nueva_tarea] = 0;

  return 1;
}

