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
      scheduler.tablero[i][j]=Non;
  scheduler.tablero[10][10]=Playe;
  scheduler.tablero[40][40]=Opp;

  //init algunas frutas (10)
  coord init_frutas[10] = {{0,0}, {2,10}, {5,20}, {5,12}, {6,29}, {29,23}, {29,40}, {29,49}, {0,40}, {30,30}};
  for(int i=0; i<10; i++){
    int x = init_frutas[i].x;
    int y = init_frutas[i].y;
    scheduler.tablero[x][y]=Fruta;
  }

}


int16_t sched_nextTask() {
  int i = 0;
  while(i < 20 &&scheduler.falta_jugar[i] != 1 && scheduler.muertas[i] != 1) 
    i++;

  return i;
  /*
  scheduler.falta_jugar[i] = 0;

  if (i < 20)
    return i + 31;

  for(int i = 0; i < 20; i++){
    if(scheduler.muertas[i] != 1)
      scheduler.falta_jugar[i] = 1;
  }

  for(int i = 0; i < 50; i++){
    for(int j = 0; j < 50; j++){
      int sum_A_peso = 0;
      int sum_B_peso = 0;
      int sum_A_puntos = 0;
      int sum_B_puntos = 0;

      //Cuenta pesos de puntos de cada tarea en la posición i j
      for(int h = 0; h < 10; h++){
        if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          sum_A_peso += scheduler.peso_por_tarea[h];
          sum_A_puntos += scheduler.puntos_por_tarea[h];
        }
      }

      for(int h = 10; h < 20; h++){
        if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          sum_B_peso += scheduler.peso_por_tarea[h];
          sum_B_puntos += scheduler.puntos_por_tarea[h];
        }
      }

    int ganador = 0;

    // Casos si alguna de las dos tiene mayor peso o son igual en peso
    if(sum_A_peso == sum_B_peso){
      for(int h = 0; h < 20; h++){
        if (scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          scheduler.muertas[h] = 1;            
          scheduler.falta_jugar[h] = 0;
          scheduler.peso_por_tarea[h] = 0;
        }
      }
      if(frutaEn(i,j)){
        scheduler.tablero[i][j] = frutaEn(i,j);
      }else{
        scheduler.tablero[i][j] = 10;
      }
      
    }else if (sum_A_peso > sum_B_peso){
      ganador = 1;
      for(int h = 10; h < 20; h++){
        if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          scheduler.muertas[h] = 1;            
          scheduler.falta_jugar[h] = 0;
          scheduler.peso_por_tarea[h] = 0;
        }
      }
      for(int h = 0; h < 10; h++){
        if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          scheduler.puntos_por_tarea[h] += sum_B_puntos;
          scheduler.peso_por_tarea[h] += frutaEn(i,j);
          break;
        }
      }
      scheduler.tablero[i][j] = 20;
    }else if (sum_B_peso > sum_A_peso){
      ganador = 2;
      for(int h = 0; h < 10; h++){
        if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          scheduler.muertas[h] = 1;            
          scheduler.falta_jugar[h] = 0;
          scheduler.peso_por_tarea[h] = 0;
        }
      }  

      for(int h = 10; h < 20; h++){
        if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == i){
          scheduler.puntos_por_tarea[h] += sum_A_puntos;
          scheduler.peso_por_tarea[h] += frutaEn(i,j);
          break;
        }
      }
      scheduler.tablero[i][j] = 30;
    }      

    for(int h = 0; h < 20; h++){
      scheduler.coordenadas_actuales[h] = scheduler.coordenadas_siguientes[h];
      // setear al inicio del juego coordenadas actuales y siguientes todas iguales.     
    }


    }
  }

  return 0;*/
}


//funciones auxiliares de read
uint32_t chequear_vision_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  int distancia_pedida = abs(x-eax)+abs(y-ebx);
  int distancia_maxima = scheduler.peso_por_tarea[Tarea_actual];
  int distancia_efectiva = min(distancia_pedida, distancia_maxima);
  
  return distancia_efectiva > distancia_maxima;
}

uint32_t read_C(int32_t eax, int32_t ebx){
  int x = scheduler.coordenadas_actuales[Tarea_actual].x;
  int y = scheduler.coordenadas_actuales[Tarea_actual].y;

  scheduler.cant_llamadas_a_read_por_tarea[Tarea_actual]++;
  return scheduler.tablero[ ((eax+x)+tam_tablero)%tam_tablero ][ ((ebx+y)+tam_tablero)%tam_tablero ];
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

