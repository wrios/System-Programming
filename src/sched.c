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
  for(int i=0; i<cant_tareas; i++)
    scheduler.ya_jugo[i] = true;
  scheduler.ya_jugo[0]=scheduler.ya_jugo[10]=false;
/*
  for(int i=0; i<cant_tareas; i++)
    scheduler.ya_jugo[i] = 0x0;
*/
  for(int i=0; i<cant_tareas; i++)
    scheduler.muertas[i] = true;
  scheduler.muertas[0]=scheduler.muertas[10]=false;

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

  //init algunas frutas (10)
  coord init_frutas[13] = {{0,0}, {10,12}, {10,13}, {10,14}, {10,15}, {5,20}, {5,12}, {6,29}, {29,23}, {29,40}, {29,49}, {0,40}, {30,30}};
  for(int i=0; i<13; i++){
    int x = init_frutas[i].x;
    int y = init_frutas[i].y;
    scheduler.tablero[x][y]=16;
  }
}


int16_t sched_nextTask() {
  int k = 0;

  while(k < 20){
    if( scheduler.muertas[k] == false && scheduler.ya_jugo[k] == false )
      break;
    k++;
  }
  // Paso turno
  if (k == 20){
    /*
    for(int i = 0; i < 50; i++) {
      for(int j = 0; j < 50; j++) {
          char cell = getCell(scheduler.tablero[i][j]);
          print(" ", i, j, cell);
      }
    }*/
   // breakpoint();


    for(int i = 0; i < 50; i++){
      for(int j = 0; j < 50; j++){

        int sum_A_peso = 0;
        int sum_B_peso = 0;
        int sum_A_puntos = 0;
        int sum_B_puntos = 0;

        //Cuenta pesos de puntos de cada tarea en la posición i j
        for(int h = 0; h < 10; h++){
          if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            sum_A_peso += scheduler.peso_por_tarea[h];
            sum_A_puntos += scheduler.puntos_por_tarea[h];
          }
        }

        for(int h = 10; h < 20; h++){
          if(scheduler.muertas[h] != 1 && scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            sum_B_peso += scheduler.peso_por_tarea[h];
            sum_B_puntos += scheduler.puntos_por_tarea[h];
          }
        }

      int ganador = 0;

      // Casos si alguna de las dos tiene mayor peso o son igual en peso
      if(sum_A_peso == sum_B_peso){
        for(int h = 0; h < 20; h++){
          if (scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            scheduler.muertas[h] = true;            
            scheduler.peso_por_tarea[h] = 0;
          }
        }
        if(frutaEn(i,j) > 0){
          scheduler.tablero[i][j] = frutaEn(i,j);
        }else{
          scheduler.tablero[i][j] = 10;
        }
        
      }else if (sum_A_peso > sum_B_peso){
        ganador = 1;
        for(int h = 10; h < 20; h++){
          if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            scheduler.muertas[h] = true;            
            scheduler.peso_por_tarea[h] = 0;
          }
        }
        for(int h = 0; h < 10; h++){
          if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            scheduler.puntos_por_tarea[h] += sum_B_puntos;
            scheduler.peso_por_tarea[h] += frutaEn(i,j);
            break;
          }
        }
        sum_A_puntos+=frutaEn(i,j);
        scheduler.tablero[i][j] = 20;
      }else if (sum_B_peso > sum_A_peso){
        ganador = 2;
        for(int h = 0; h < 10; h++){
          if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            scheduler.muertas[h] = true;            
            scheduler.peso_por_tarea[h] = 0;
          }
        }  

        for(int h = 10; h < 20; h++){
          if(scheduler.coordenadas_siguientes[h].x == i && scheduler.coordenadas_siguientes[h].y == j){
            scheduler.puntos_por_tarea[h] += sum_A_puntos;
            scheduler.peso_por_tarea[h] += frutaEn(i,j);
            break;
          }
        }
        scheduler.tablero[i][j] = 30;
      }      

      if(ganador == 1){
        //breakpoint();
        scheduler.puntosA+=sum_A_puntos;
      }

      if(ganador == 2){
        scheduler.puntosB+=sum_B_puntos;
      }

      }
    }

    for(int h = 0; h < 20; h++){
      scheduler.coordenadas_actuales[h] = scheduler.coordenadas_siguientes[h];
      // setear al inicio del juego coordenadas actuales y siguientes todas iguales.     
    }
    print_dec(scheduler.puntosA, 10, 59, 5, C_BG_GREEN);
    print_dec(scheduler.puntosB, 10, 59, 30, C_BG_CYAN);


    for(int h = 0; h < 20; h++){
      scheduler.ya_jugo[h] = false;
    }
    for(int h = 0; h < 20; h++){
      scheduler.cant_llamadas_a_read_por_tarea[h] = 0;
    }
    
    for(int h = 0; h < 20; h++){
      int x = scheduler.coordenadas_siguientes[h].x;
      int y = scheduler.coordenadas_siguientes[h].y;
      if(scheduler.muertas[h] != 1){
        if (h < 10){
          scheduler.tablero[x][y] = 20;
        }else{
          scheduler.tablero[x][y] = 30;
        }
      }
      
    }

    //Updateo el score en la pantalla
    
  }else{
    scheduler.ya_jugo[k] = true;
  }

/*
  
  
  for(int i = 0; i < 50; i++) {
    for(int j = 0; j < 50; j++) {
      char cell = getCell(scheduler.tablero[i][j]);
      print(" ", i, j, cell);
    }
  }
  
*/
  if (k == 20){
    for(int i = 0; i < 50; i++) {
      for(int j = 0; j < 50; j++) {
        char cell = getCell(scheduler.tablero[i][j]);
        print(" ", i, j, cell);
      }
    } 
  }


  if(k<20){
    Tarea_actual = k;
    print_dec(k+31, 10, 40, 5, C_BG_GREEN);
    breakpoint();
    return k+31;
  }



  k=0;
  while(k < 20){
    if( scheduler.muertas[k] == false && scheduler.ya_jugo[k] == false )
       break;
    k++;
  }
  Tarea_actual = k;
  print_dec(k+31, 10, 40, 5, C_BG_GREEN);
  breakpoint();
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

  //if( scheduler.muertas[Tarea_actual] ) return 0;

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
  //entra <=> (checkear_poder_div_C dio que hay espacio)
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
  
  /*
  tss* nueva_tss =  &tss_array[indice_nueva_tarea];
  tss_inicializar(nueva_tss, indice_nueva_tarea);
  */
  scheduler.muertas[indice_nueva_tarea] = false;
  scheduler.ya_jugo[indice_nueva_tarea] = true;
  scheduler.cant_llamadas_a_read_por_tarea[indice_nueva_tarea] = 0;
  //inicializo informaciones de la nueva tarea
    //empieza donde esta la tarea actual
  scheduler.coordenadas_siguientes[indice_nueva_tarea] = scheduler.coordenadas_siguientes[Tarea_actual];
    //con la mitad del peso
  scheduler.peso_por_tarea[indice_nueva_tarea] = scheduler.peso_por_tarea[Tarea_actual];
    //con 0 puntos
  scheduler.puntos_por_tarea[indice_nueva_tarea] = 0;

  return 1;
}

