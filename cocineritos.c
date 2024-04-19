#include "cocineritos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const int DIMENSION_DE_TABLERO = 21;
const int MITAD_DE_TABLERO = 10;
const int PRIMERA_FILA = 0;
const int PRIMERA_COLUMNA = 0;
const int ULTIMA_FILA = 20;
const int ULTIMA_COLUMNA = 20;
const int AMPLITUD_CUADRANTE_EN_FILAS = 9;
const int MINIMO_FILAS_CUADRANTE_STITCH = 1;
const int MINIMO_FILAS_CUADRANTE_REUBEN = 11;
const int AMPLITUD_CUADRANTE_EN_COLUMNAS = 19;
const int MINIMO_COLUMNAS = 1;
const int JUEGO_GANADO = 1;
const int JUEGO_PERDIDO = -1;

#define MOVER_ARRIBA 'W'
#define MOVER_ABAJO 'S'
#define MOVER_IZQUIERDA 'A'
#define MOVER_DERECHA 'D'
#define CORTAR 'C'
#define COCINAR 'H'
#define AGARRAR_O_SOLTAR_INGREDIENTE 'R'  
#define PASAR_INGREDIENTE 'T'
#define ACTIVAR_MATAFUEGO 'M'
#define CAMBIO_DE_JUGADOR 'X'

const char SALIDA ='P';
const char VACIO = ' ';
const char MANO_VACIA = 'V';
const char MESA = '_';
const char PARED = '#';
const char AGUJEROS = 'A';
const char FUEGO = 'F';
const char MATAFUEGOS = 'M';
const char CUCHILLOS = 'C';
const char HORNO = 'H';

const char LECHUGA = 'L';
const char TOMATE = 'T';
const char MILANESA = 'I';
const char CARNE = 'B';
const char PAN = 'N';
const char JAMON = 'J';

const char QUESO = 'Q';
const char MASA = 'O';

const char ENSALADA = 'E';
const char PIZZA = 'P';
const char HAMBURGUESA = 'H';
const char SANDWICH = 'S';
const char STITCH = 'S';
const char REUBEN = 'R';

const int CANTIDAD_CUCHILLOS = 2;
const int CANTIDAD_HORNOS = 2;
const int CANTIDAD_PERSONAJES = 2;
const int CANTIDAD_MAXIMA_AGUJEROS = 20;
const int CANTIDAD_MATAFUEGOS = 1;
const int POSICION_INVALIDA = -1;
const int DISTANCIA_1 = 1;
const int DISTANCIA_2 =2;
const int MODULO = -1;
const int CANTIDAD_FUEGOS = 1;
const int MOVIMIENTOS_MAXIMOS = 15;
const int CANTIDAD_INGREDIENTES_PIZZA = 3;
const int CANTIDAD_INGREDIENTES_ENSALADA = 2;
const int CANTIDAD_INGREDIENTES_HAMBURGUESA = 4;
const int CANTIDAD_INGREDIENTES_SANDWICH = 6;
const int PRECIO_PRIMER_NIVEL = 100;
const int PRECIO_SEGUNDO_NIVEL = 150;


/*
PRE: -
POST: Devuelve true si la fila y columna coinciden con los bordes del terreno
*/
bool hay_pared(int filas,int columnas){
    return filas == PRIMERA_FILA || (filas == MITAD_DE_TABLERO && columnas != MITAD_DE_TABLERO) || filas == ULTIMA_FILA || columnas == PRIMERA_COLUMNA || columnas == ULTIMA_COLUMNA;
}

/*
PRE: -
POST: Devuelve true si la fila y columna son la mitad de tablero.
*/
bool hay_mesa(int filas,int columnas){
    return filas == MITAD_DE_TABLERO && columnas == MITAD_DE_TABLERO;
}

/*
PRE: -
POST: devuelve un entero entre 11 y 19
*/
int conseguir_numero_aleatorio_cuadrante_reuben(){
    return rand() % AMPLITUD_CUADRANTE_EN_FILAS + MINIMO_FILAS_CUADRANTE_REUBEN;
}

/*
PRE: -
POST: devuelve un entero entre 1 y 9
*/
int conseguir_numero_aleatorio_cuadrante_stitch(){
    
    return rand() % AMPLITUD_CUADRANTE_EN_FILAS + MINIMO_FILAS_CUADRANTE_STITCH;
}

/*
PRE: -
POST: devuelve un entero entre 1 y 19
*/
int conseguir_numero_aleatorio_columna(){
    return rand() % AMPLITUD_CUADRANTE_EN_COLUMNAS + MINIMO_COLUMNAS;
}

/*
PRE: Los vectores deben estar inicializados y vacios
POST: Guarda las posiciones de la mesa y las paredes, las cuales son validas 
*/
void inicializar_tablero(juego_t* juego){
    for (int filas = 0; filas < DIMENSION_DE_TABLERO; filas++){
        for (int columnas = 0; columnas < DIMENSION_DE_TABLERO; columnas++){

            if(hay_pared(filas,columnas)){
                
                juego->paredes[juego->tope_paredes].fil = filas;
                juego->paredes[juego->tope_paredes].col = columnas;
                juego->tope_paredes++;
            }
            if(hay_mesa(filas,columnas)){
                juego->mesa.fil = filas;
                juego->mesa.col = columnas;
            }   
        }
    }
}

/*
PRE: -
POST: Devuelve true si la fila y columna ya estan en el vector obstaculos.
*/
bool posicion_obstaculos(int fila, int columna, juego_t juego){
    bool lugar_ocupado = false;

    for (int i = 0; i < juego.tope_obstaculos && !lugar_ocupado; i++){
        if(juego.obstaculos[i].posicion.fil == fila && juego.obstaculos[i].posicion.col == columna){
            lugar_ocupado = true;
        }
    }
    return lugar_ocupado;
}

/*
PRE: -
POST: Le asigna una fila,columna y tipo al objeto en el vector obstaculos
*/
void asignar_obstaculos(juego_t* juego, int filas,int columnas, char objeto){

    juego->obstaculos[juego->tope_obstaculos].tipo = objeto;
    juego->obstaculos[juego->tope_obstaculos].posicion.fil = filas;
    juego->obstaculos[juego->tope_obstaculos].posicion.col = columnas;
    juego->tope_obstaculos++;
}

/*
PRE: -
POST: Guarda las posiciones de los obstaculos en el vector obstaculos, las cuales son validas
*/
void inicializar_agujeros(juego_t* juego){
    int fila_agujero = 0;
    int columna_agujero = 0;
    int agujeros_asignados = 0;

    while( agujeros_asignados < (CANTIDAD_MAXIMA_AGUJEROS/2)){
    
        fila_agujero = conseguir_numero_aleatorio_cuadrante_stitch();
        columna_agujero = conseguir_numero_aleatorio_cuadrante_stitch();

        if(!posicion_obstaculos(fila_agujero,columna_agujero,*juego)){
            asignar_obstaculos(juego,fila_agujero,columna_agujero,AGUJEROS);
            agujeros_asignados++;
        }
    }
    while (agujeros_asignados < CANTIDAD_MAXIMA_AGUJEROS){
    
        fila_agujero = conseguir_numero_aleatorio_cuadrante_reuben();
        columna_agujero = conseguir_numero_aleatorio_cuadrante_reuben();

        if(!posicion_obstaculos(fila_agujero,columna_agujero,*juego)){
            asignar_obstaculos(juego,fila_agujero,columna_agujero,AGUJEROS);
            agujeros_asignados++;
        }
    }
}

/*
PRE: -
POST: Devuelve true si la fila y columna esta en otro vector.
*/
bool posicion_herramienta(int fila, int columna, juego_t juego){
    bool lugar_ocupado = false;

    for (int i = 0; i < juego.tope_herramientas && !lugar_ocupado; i++){
        if(juego.herramientas[i].posicion.fil == fila && juego.herramientas[i].posicion.col == columna){
            lugar_ocupado = true;
        }
    }
    return lugar_ocupado;
}

/*
PRE: -
POST: Le asigna una fila,columna y tipo al objeto en el vector herramientas
*/
void asignar_herramienta(juego_t* juego, int filas,int columnas, char objeto){
    
    juego->herramientas[juego->tope_herramientas].tipo = objeto;
    juego->herramientas[juego->tope_herramientas].posicion.fil = filas;
    juego->herramientas[juego->tope_herramientas].posicion.col = columnas;
    juego->tope_herramientas++;
}

/*
PRE: -
POST: Guarda las posiciones de las herramientas en el vector herramientas, las cuales son validas
*/
void inicializar_herramientas(juego_t* juego){
    int fila_herramienta = 0;
    int columna_herramienta = 0;
    int cantidad_herramienta_asignada = 0;

    while (juego->tope_herramientas < CANTIDAD_CUCHILLOS){
    
        fila_herramienta = conseguir_numero_aleatorio_cuadrante_stitch();
        columna_herramienta = conseguir_numero_aleatorio_cuadrante_stitch();

        if(!posicion_herramienta(fila_herramienta,columna_herramienta,*juego) && !posicion_obstaculos(fila_herramienta,columna_herramienta,*juego)){

            asignar_herramienta(juego,fila_herramienta,columna_herramienta,CUCHILLOS);
            cantidad_herramienta_asignada++;
        }
    }
    while(juego->tope_herramientas < CANTIDAD_CUCHILLOS + CANTIDAD_HORNOS){
        
        fila_herramienta = conseguir_numero_aleatorio_cuadrante_reuben();
        columna_herramienta = conseguir_numero_aleatorio_cuadrante_reuben();

        if(!posicion_herramienta(fila_herramienta,columna_herramienta,*juego) && !posicion_obstaculos(fila_herramienta,columna_herramienta,*juego)){

            asignar_herramienta(juego,fila_herramienta,columna_herramienta,HORNO);
            cantidad_herramienta_asignada++;
        }
    }
}

/*
PRE: -
POST: Devuelve true si la fila y columna esta en otro vector.
*/
bool posicion_ingredientes(int fila, int columna, juego_t juego){
    bool lugar_ocupado = false;
    
    for (int i = 0; i < juego.comida[juego.tope_comida-1].tope_ingredientes && !lugar_ocupado; i++){
        
        if((juego.comida[juego.tope_comida-1].ingrediente[i].posicion.fil == fila && juego.comida[juego.tope_comida-1].ingrediente[i].posicion.col == columna)){
            lugar_ocupado = true;
        }
    }
    return lugar_ocupado;
}

/*
PRE: -
POST: Le asigna una fila,columna y tipo al objeto en el vector comidas
*/
void asignar_ingrediente(juego_t* juego, int filas,int columnas, char objeto){
    
    juego->comida[juego->tope_comida-1].ingrediente[juego->comida[juego->tope_comida-1].tope_ingredientes].tipo = objeto;

    juego->comida[juego->tope_comida-1].ingrediente[juego->comida[juego->tope_comida-1].tope_ingredientes].posicion.fil = filas;

    juego->comida[juego->tope_comida-1].ingrediente[juego->comida[juego->tope_comida-1].tope_ingredientes].posicion.col = columnas;

    juego->comida[juego->tope_comida-1].ingrediente[juego->comida[juego->tope_comida-1].tope_ingredientes].esta_cocinado = false;

    juego->comida[juego->tope_comida-1].ingrediente[juego->comida[juego->tope_comida-1].tope_ingredientes].esta_cortado = false; 

    juego->comida[juego->tope_comida-1].tope_ingredientes++;

}

/*
PRE: -
POST: Guarda las posiciones de los ingredientes de la ensalada en el vector comidas, las cuales son validas
*/
void inicializar_ingredientes_ensalada(juego_t* juego){

    int coordenada_fila_lechuga = 0;   
    int coordenada_columna_lechuga = 0;
    int coordenada_fila_tomate = 0;
    int coordenada_columna_tomate = 0;
    int ingredientes_asignados = 0;
    juego->comida[juego->tope_comida].tope_ingredientes = 0;
    juego->tope_comida++;
    juego->comida[juego->tope_comida-1].tipo = ENSALADA;

    while (ingredientes_asignados < CANTIDAD_INGREDIENTES_ENSALADA){

       coordenada_fila_lechuga = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_lechuga = conseguir_numero_aleatorio_columna();

        if (!posicion_ingredientes(coordenada_fila_lechuga,coordenada_columna_lechuga,*juego) && !posicion_obstaculos(coordenada_fila_lechuga,coordenada_columna_lechuga,*juego) && !posicion_herramienta(coordenada_fila_lechuga,coordenada_columna_lechuga, *juego) && ingredientes_asignados == 0){
                
            asignar_ingrediente(juego,coordenada_fila_lechuga,coordenada_columna_lechuga,LECHUGA);
            ingredientes_asignados++;
        }

        coordenada_fila_tomate = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_tomate = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_tomate,coordenada_columna_tomate,*juego) && !posicion_herramienta(coordenada_fila_tomate,coordenada_columna_tomate,*juego) && !posicion_obstaculos(coordenada_fila_tomate,coordenada_columna_tomate,*juego) && ingredientes_asignados == 1){
                
            asignar_ingrediente(juego,coordenada_fila_tomate,coordenada_columna_tomate,TOMATE);
            ingredientes_asignados++;
        }      
    }
}

/*
PRE: -
POST: Guarda las posiciones de los ingredientes de la pizza en el vector comidas, las cuales son validas
*/
void inicializar_ingredientes_pizza(juego_t* juego){

    int coordenada_fila_jamon = 0;
    int coordenada_columna_jamon = 0;
    int coordenada_fila_queso = 0;
    int coordenada_columna_queso = 0;
    int coordenada_fila_masa = 0; 
    int coordenada_columna_masa = 0;
    int ingredientes_asignados = 0;
    juego->comida[juego->tope_comida].tope_ingredientes = 0;
    juego->tope_comida++;
    juego->comida[juego->tope_comida-1].tipo = PIZZA;
    juego->tope_comida_lista = 0;

    while(ingredientes_asignados < CANTIDAD_INGREDIENTES_PIZZA){

        coordenada_fila_jamon = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_jamon = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_jamon,coordenada_columna_jamon,*juego) && ingredientes_asignados == 0 && !posicion_herramienta(coordenada_fila_jamon,coordenada_columna_jamon,*juego) && !posicion_obstaculos(coordenada_fila_jamon,coordenada_columna_jamon,*juego)){
            
            asignar_ingrediente(juego,coordenada_fila_jamon,coordenada_columna_jamon,JAMON);
            ingredientes_asignados++;
        }
        coordenada_fila_queso = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_queso = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_queso,coordenada_columna_queso,*juego) && ingredientes_asignados == 1 &&!posicion_herramienta(coordenada_fila_queso,coordenada_columna_queso,*juego) && !posicion_obstaculos(coordenada_fila_queso,coordenada_columna_queso,*juego) ){
                
            asignar_ingrediente(juego,coordenada_fila_queso,coordenada_columna_queso,QUESO);
            ingredientes_asignados++;
        }   

        coordenada_fila_masa = conseguir_numero_aleatorio_cuadrante_reuben();
        coordenada_columna_masa = conseguir_numero_aleatorio_columna();
   
        if(!posicion_ingredientes(coordenada_fila_masa,coordenada_columna_masa,*juego) && ingredientes_asignados == 2 && !posicion_herramienta(coordenada_fila_masa,coordenada_columna_masa,*juego) && !posicion_obstaculos(coordenada_fila_masa,coordenada_columna_masa,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_masa,coordenada_columna_masa,MASA);
            ingredientes_asignados++;            
        }
    }
    
}

/*
PRE: -
POST: Guarda las posiciones de los ingredientes de la hamburguesa en el vector comidas, las cuales son validas
*/
void inicializar_ingredientes_hamburguesa(juego_t* juego){
    int coordenada_fila_pan = 0;
    int coordenada_columna_pan = 0;
    int coordenada_fila_carne = 0;
    int coordenada_columna_carne = 0;
    int ingredientes_asignados = 0;
    inicializar_ingredientes_ensalada(juego);
    juego->comida[juego->tope_comida].tope_ingredientes = 0;
    juego->comida[juego->tope_comida-1].tipo = HAMBURGUESA;
    juego->tope_comida_lista = 0;

    while(ingredientes_asignados < (CANTIDAD_INGREDIENTES_HAMBURGUESA - CANTIDAD_INGREDIENTES_ENSALADA)){

        coordenada_fila_pan = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_pan = conseguir_numero_aleatorio_columna();
        
        if(!posicion_ingredientes(coordenada_fila_pan,coordenada_columna_pan,*juego) && ingredientes_asignados == 0 && !posicion_herramienta(coordenada_fila_pan,coordenada_columna_pan,*juego) && !posicion_obstaculos(coordenada_fila_pan,coordenada_columna_pan,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_pan,coordenada_columna_pan,PAN);
            ingredientes_asignados++;
        }
        coordenada_fila_carne = conseguir_numero_aleatorio_cuadrante_reuben();
        coordenada_columna_carne = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_carne,coordenada_columna_carne,*juego)  && ingredientes_asignados == 1 && !posicion_herramienta(coordenada_fila_carne,coordenada_columna_carne,*juego) && !posicion_obstaculos(coordenada_fila_carne,coordenada_columna_carne,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_carne,coordenada_columna_carne,CARNE);
            ingredientes_asignados++;
        }   
    }
}

/*
PRE: -
POST: Guarda las posiciones de los ingredientes del sandwich en el vector comidas, las cuales son validas
*/
void inicializar_ingredientes_sandwich(juego_t* juego){
    int coordenada_fila_pan = 0;
    int coordenada_columna_pan = 0;
    int coordenada_fila_jamon = 0;
    int coordenada_columna_jamon = 0;
    int coordenada_fila_queso = 0;
    int coordenada_columna_queso = 0;
    int coordenada_fila_milanesa = 0;
    int coordenada_columna_milanesa = 0;
    int ingredientes_asignados = 0;    
    inicializar_ingredientes_ensalada(juego);
    juego->comida[juego->tope_comida].tope_ingredientes = 0;
    juego->comida[juego->tope_comida-1].tipo = SANDWICH;
    juego->tope_comida_lista = 0;

    while(ingredientes_asignados < (CANTIDAD_INGREDIENTES_SANDWICH-CANTIDAD_INGREDIENTES_ENSALADA)){

        coordenada_fila_pan = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_pan = conseguir_numero_aleatorio_columna();
        
        if(!posicion_ingredientes(coordenada_fila_pan,coordenada_columna_pan,*juego) && ingredientes_asignados == 0 && !posicion_herramienta(coordenada_fila_pan,coordenada_columna_pan,*juego) && !posicion_obstaculos(coordenada_fila_pan,coordenada_columna_pan,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_pan,coordenada_columna_pan,PAN);
            ingredientes_asignados++;
        }
        coordenada_fila_jamon = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_jamon = conseguir_numero_aleatorio_columna();
        
        if(!posicion_ingredientes(coordenada_fila_jamon,coordenada_columna_jamon,*juego) && ingredientes_asignados == 1 && !posicion_herramienta(coordenada_fila_jamon,coordenada_columna_jamon,*juego) && !posicion_obstaculos(coordenada_fila_jamon,coordenada_columna_jamon,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_jamon,coordenada_columna_jamon,JAMON);
            ingredientes_asignados++;
        }
        coordenada_fila_queso = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_columna_queso = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_queso,coordenada_columna_queso,*juego) && ingredientes_asignados == 2 && !posicion_herramienta(coordenada_fila_queso,coordenada_columna_queso,*juego) && !posicion_obstaculos(coordenada_fila_queso,coordenada_columna_queso,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_queso,coordenada_columna_queso,QUESO);
            ingredientes_asignados++;
        }   
        coordenada_fila_milanesa = conseguir_numero_aleatorio_cuadrante_reuben();
        coordenada_columna_milanesa = conseguir_numero_aleatorio_columna();

        if(!posicion_ingredientes(coordenada_fila_milanesa,coordenada_columna_milanesa,*juego) && ingredientes_asignados == 3 && !posicion_herramienta(coordenada_fila_milanesa,coordenada_columna_milanesa,*juego) && !posicion_obstaculos(coordenada_fila_milanesa,coordenada_columna_milanesa,*juego)){
                
            asignar_ingrediente(juego,coordenada_fila_milanesa,coordenada_columna_milanesa,MILANESA);
            ingredientes_asignados++;
        }  
    }
}

/*
PRE: el tope debe estar inicializado y cargado
POST: Inicializa los ingredientes de la comida segun el tope
*/
void inicializar_ingredientes_primer_nivel(juego_t* juego){

    if(juego->tope_comida == 0){
        juego->comida_actual = ENSALADA;
        inicializar_ingredientes_ensalada(juego);
 
    }
    else if(juego->tope_comida == 1){
        juego->comida_actual = PIZZA;
        inicializar_ingredientes_pizza(juego); 
    }
}

/*
PRE: el tope debe estar inicializado y cargado
POST: Inicializa los ingredientes de la comida segun el tope
*/
void inicializar_ingredientes_segundo_nivel(juego_t* juego){

    if(juego->tope_comida == 0){
        juego->comida_actual = ENSALADA;
        inicializar_ingredientes_ensalada(juego);  
    }
    else if(juego->tope_comida == 1){
        juego->comida_actual = PIZZA;
        inicializar_ingredientes_pizza(juego);  
    }
    else if(juego->tope_comida == 2){
        juego->comida_actual = HAMBURGUESA;
        inicializar_ingredientes_hamburguesa(juego);
    }
}

/*
PRE: el tope debe estar inicializado y cargado
POST: Inicializa los ingredientes de la comida segun el tope
*/
void inicializar_ingredientes_tercer_nivel(juego_t* juego){

    if(juego->tope_comida == 0){
        juego->comida_actual = ENSALADA;
        inicializar_ingredientes_ensalada(juego);
        
    }
    else if(juego->tope_comida == 1){
        juego->comida_actual = PIZZA;
        inicializar_ingredientes_pizza(juego);
        
    }

    else if(juego->tope_comida == 2){
        juego->comida_actual = HAMBURGUESA;
        inicializar_ingredientes_hamburguesa(juego);
        
    }
    else if(juego->tope_comida == 3){
        juego->comida_actual = SANDWICH;
        inicializar_ingredientes_sandwich(juego);
        
    }
}

/*
PRE: -
POST: Le da una posicion y tipo valido al personaje
*/
void asignar_personajes(juego_t* juego, int filas,int columnas, char personaje){
    
    if (personaje == STITCH){ 
        juego->stitch.tipo = STITCH;
        juego->stitch.posicion.fil = filas;
        juego->stitch.posicion.col = columnas;
    }
    else{
        juego->reuben.tipo = REUBEN;
        juego->reuben.posicion.fil = filas;
        juego->reuben.posicion.col = columnas;
    }
}

/*
PRE: -
POST: Verifica si la fila y columna pasada por parametro estan en otro vector, si lo estan devuelve true
*/
bool posicion_objetos(int filas, int columnas, juego_t juego){
    
    bool lugar_ocupado = false;
    
    if((posicion_obstaculos(filas,columnas,juego) || posicion_herramienta(filas,columnas,juego)) || (posicion_ingredientes(filas,columnas,juego))){

        return true;
    }
    return lugar_ocupado;
}

/*
PRE: Los paramentros deben estar inicializados
POST: Agrega la posicion de los personajes al struct coordenada_t
*/
void inicializar_personajes(juego_t* juego){

    int coordenada_stich_fila = 0;
    int coordenada_stich_columna = 0;
    int coordenada_reuben_fila = 0;
    int coordenada_reuben_columna = 0;

    int personajes_asignados = 0;
    juego->personaje_activo = STITCH;

    while(personajes_asignados < CANTIDAD_PERSONAJES){

        coordenada_stich_fila = conseguir_numero_aleatorio_cuadrante_stitch();
        coordenada_stich_columna = conseguir_numero_aleatorio_columna();

        if(!posicion_objetos(coordenada_stich_fila, coordenada_stich_columna, *juego) &&personajes_asignados == 0){

            asignar_personajes(juego,coordenada_stich_fila,coordenada_stich_columna, STITCH);
            personajes_asignados++;
        }

        coordenada_reuben_fila = conseguir_numero_aleatorio_cuadrante_reuben();   
        coordenada_reuben_columna = conseguir_numero_aleatorio_columna();

        if (!posicion_objetos(coordenada_reuben_fila, coordenada_reuben_columna, *juego) && personajes_asignados == 1){

            asignar_personajes(juego,coordenada_reuben_fila,coordenada_reuben_columna, REUBEN);
            personajes_asignados++;
        }
    }        
}

/*
PRE: -
POST: Verifica si la fila y columna pasada por parametro estan en otro vector o coinciden con la posicion del personaje, si lo estan devuelve true
*/
bool posicion_salida(int filas, int columnas, juego_t juego){
    
    if(posicion_objetos(filas, columnas, juego) && juego.reuben.posicion.fil == filas && juego.reuben.posicion.col == columnas){

        return true;
    }
    return false;

}

/*
PRE: Los paramentros deben estar inicializados
POST: Le da una posicion valida a la salida
*/
void inicializar_salida(juego_t* juego){

    bool posicion_valida = false; 
    int coordenada_fila_salida = 0;
    int coordenada_columna_salida = 0;

    while(!posicion_valida){

        coordenada_fila_salida = conseguir_numero_aleatorio_cuadrante_reuben();
        coordenada_columna_salida = conseguir_numero_aleatorio_columna();

        if(!posicion_salida(coordenada_fila_salida,coordenada_columna_salida,*juego)){
            juego->salida.fil = coordenada_fila_salida;
            juego->salida.col = coordenada_columna_salida;
            posicion_valida = true;
        }
    }
}


void imprimir_terreno(juego_t juego){

    int fila, columna;
    char objeto;
    char tablero[21][21];
    for (int i = 0; i < 21; i++){
        for (int j = 0; j < 21; j++){
            tablero[i][j] = VACIO;
            fila = juego.mesa.fil;
            columna = juego.mesa.col;
            tablero[fila][columna] = MESA;
        }
    }

    for (int i = 0; i < juego.tope_paredes; i++){

        fila = juego.paredes[i].fil;
        columna = juego.paredes[i].col;
        tablero[fila][columna] = PARED;
    }
    
    for (int i = 0; i < juego.tope_obstaculos; i++){

        fila = juego.obstaculos[i].posicion.fil;
        columna = juego.obstaculos[i].posicion.col;
        objeto = juego.obstaculos[i].tipo;
        tablero[fila][columna] = objeto;
    }
    for (int i = 0; i < juego.tope_herramientas; i++){

        fila = juego.herramientas[i].posicion.fil;
        columna = juego.herramientas[i].posicion.col;
        objeto = juego.herramientas[i].tipo;
        tablero[fila][columna] = objeto;
    }
    
    for (int i = 0; i < juego.comida[juego.tope_comida-1].tope_ingredientes; i++){
        
        fila = juego.comida[juego.tope_comida-1].ingrediente[i].posicion.fil;
        columna = juego.comida[juego.tope_comida-1].ingrediente[i].posicion.col;
        objeto = juego.comida[juego.tope_comida-1].ingrediente[i].tipo;
        tablero[fila][columna] = objeto;
    }

    fila = juego.stitch.posicion.fil;
    columna = juego.stitch.posicion.col;
    tablero[fila][columna] = STITCH;
    fila = juego.reuben.posicion.fil;
    columna = juego.reuben.posicion.col;
    tablero[fila][columna] = REUBEN;
    fila = juego.salida.fil;
    columna = juego.salida.col;
    tablero[fila][columna] = SALIDA;

    for (int i = 0; i < 21; i++){
        for (int j = 0; j < 21; j++){
            printf(" %c", tablero[i][j]);
        }
        printf("\n");
    }
}

void inicializar_juego(juego_t* juego, int precio){
    juego->tope_obstaculos = 0;
    juego->tope_paredes = 0;
    juego->tope_herramientas = 0;
    juego->tope_comida = 0;
    juego->tope_comida_lista = 0;
    juego->movimientos = 0;
    juego->stitch.objeto_en_mano = MANO_VACIA; 
    juego->reuben.objeto_en_mano = MANO_VACIA;
    juego->precio_total = precio;
    
    inicializar_tablero(juego);
    inicializar_agujeros(juego);
    inicializar_herramientas(juego);

    if(precio <= 100){
        inicializar_ingredientes_primer_nivel(juego);  
    }

    else if(precio <=150 && precio > 100){
        inicializar_ingredientes_segundo_nivel(juego);
    }

    else{
        inicializar_ingredientes_tercer_nivel(juego);
    }
    inicializar_personajes(juego);
    inicializar_salida(juego);
    
    printf("Ahora es turno de preparar tu comida, acuerdate que te mueves con W,A,S,D, cambias el personaje con la X, agarras las cosas con la R, cortas con la C, cocinas con la H, utilizas el matafuegos con la M y pasas los alimentos con la T. Si caes en un agujero pierdes. Buena suerte!\n");
}

/*
PRE: -
POST: devuelve la coordenada fila del personaje seleccionado
*/
int posicion_fila_jugador_activo(juego_t juego, char personaje){
    int filas = 0;

    if(personaje == STITCH){
        filas = juego.stitch.posicion.fil;
    } 
    else if(personaje == REUBEN){
        filas = juego.reuben.posicion.fil;
    }
    return filas;
}

/*
PRE: -
POST: devuelve la coordenada columna del personaje seleccionado
*/
int posicion_columna_jugador_activo(juego_t juego, char personaje){
    int columnas = 0;
    if(personaje == STITCH){    
        columnas = juego.stitch.posicion.col;
    } 
    else if(personaje == REUBEN){
        
        columnas = juego.reuben.posicion.col;
    }
    return columnas;
}

/*
PRE: -
POST: Devuelve true si esta el HORNO en el vector de herramientas.
*/
bool hay_horno(juego_t juego, int filas, int columnas){
    bool hay_horno = false;
    for (int i = 0; i < juego.tope_herramientas && !hay_horno; i++){

        if(juego.herramientas[i].tipo == HORNO && juego.herramientas[i].posicion.fil == filas && juego.herramientas[i].posicion.col == columnas){
            hay_horno = true;
        }
    }
    return hay_horno;
}

/*
PRE: -
POST: Devuelve true si esta el FUEGO en el vector de obstaculos
*/
bool hay_fuego(juego_t juego, int filas, int columnas){
    bool hay_fuego = false;
    for (int i = 0; i < juego.tope_obstaculos && !hay_fuego; i++){

        if(juego.obstaculos[i].tipo == FUEGO && juego.obstaculos[i].posicion.fil == filas && juego.obstaculos[i].posicion.col == columnas){
            hay_fuego = true;
        }
    }
    return hay_fuego;
}

/*
PRE: Personaje activo debe estar inicializado
POST: Devuelve el personaje activo en la jugada
*/
char personaje_activo(juego_t juego){
    char personaje = VACIO;
    if (juego.personaje_activo == STITCH){
        personaje = STITCH;
    }
    else{
        personaje = REUBEN;
    }
    return personaje;
}

int estado_juego(juego_t juego){
    char personaje = personaje_activo(juego);
    int filas = posicion_fila_jugador_activo(juego,personaje);
    int columnas = posicion_columna_jugador_activo(juego,personaje);
    int estado = 0;

    if(posicion_obstaculos(filas,columnas,juego)){
        estado = JUEGO_PERDIDO;
    }

    else if((juego.tope_comida == 2 && juego.tope_comida_lista == 3)||(juego.tope_comida == 3 && juego.tope_comida_lista == 4) || (juego.tope_comida == 4 && juego.tope_comida_lista == 6)){
        estado = JUEGO_GANADO;
    }
    
    return estado;
}

/*
PRE: -
POST: Devuelve true si el movimiento es (W,A,S,D)y false en caso contrario
*/
bool movimiento_permitido(char movimiento){
    bool movimiento_valido = false;

    switch(movimiento){
    case MOVER_ARRIBA:
    case MOVER_ABAJO:
    case MOVER_IZQUIERDA:
    case MOVER_DERECHA:
        movimiento_valido = true;
        break;
    }
    return movimiento_valido;
}

/*
PRE: -
POST: Devuelve true si esta permitido el comando y false en caso contrario
*/
bool comando_permitido(char movimiento){
    bool comando_valido = false;
    switch (movimiento){
    case MOVER_ARRIBA:
    case MOVER_ABAJO:
    case MOVER_IZQUIERDA:
    case MOVER_DERECHA:
    case CAMBIO_DE_JUGADOR:
    case ACTIVAR_MATAFUEGO:
    case AGARRAR_O_SOLTAR_INGREDIENTE:
    case CORTAR:
    case COCINAR:
    case PASAR_INGREDIENTE:
        comando_valido = true;
        break;
    }
    if(comando_valido == false){
        printf("Los comandos validos son (W,A,S,D) Para moverse, X para cambiar de personaje, H y C para cocinar y cortar respectivamente, M para apagar el fuego y T para interactuar con la mesa. No te olvides de las mayusculas!\n");
    }
    return comando_valido;
}

/*
PRE: Movimiento debe ser un valor valido
POST: Verifica si hay una puerta, mesa, horno o fuego en la posicion a la cual se quiere mover, si lo hay devuelve false
*/
bool posicion_valida_para_moverse(juego_t juego, char personaje, char movimiento){
    int filas = posicion_fila_jugador_activo(juego,personaje);
    int columnas = posicion_columna_jugador_activo(juego,personaje);
    
    bool posicion_permitida = true;
    switch (movimiento){
    case MOVER_ARRIBA:
        if(hay_pared(filas-DISTANCIA_1,columnas) || hay_mesa(filas-DISTANCIA_1,columnas) || hay_horno(juego,filas-DISTANCIA_1,columnas) || hay_fuego(juego,filas-DISTANCIA_1,columnas)){
            posicion_permitida = false;
        }
        
        break;
    case MOVER_ABAJO:
        if(hay_pared(filas+DISTANCIA_1,columnas) || hay_mesa(filas+DISTANCIA_1,columnas)|| hay_horno(juego,filas+DISTANCIA_1,columnas) || hay_fuego(juego,filas+DISTANCIA_1,columnas)){
            posicion_permitida = false;
        }
        break;
    case MOVER_IZQUIERDA:
        if(hay_pared(filas,columnas-DISTANCIA_1) || hay_mesa(filas,columnas-DISTANCIA_1)|| hay_horno(juego,filas,columnas-DISTANCIA_1) || hay_fuego(juego,filas,columnas-DISTANCIA_1)){
            posicion_permitida = false;
        }
        break;
    case MOVER_DERECHA:
        if(hay_pared(filas,columnas+DISTANCIA_1) || hay_mesa(filas,columnas+DISTANCIA_1)|| hay_horno(juego,filas,columnas+DISTANCIA_1) || hay_fuego(juego,filas,columnas+DISTANCIA_1)){
            posicion_permitida = false;
        }
        break;
    }
    return posicion_permitida;
}

/*
PRE: Movimiento debe ser un valor valido.
POST: Modifica la coordenada fila o columna de Stitch
*/
void mover_stitch(juego_t* juego, char movimiento){

    switch (movimiento){

    case MOVER_ARRIBA:
        juego->stitch.posicion.fil  -= DISTANCIA_1;    
        break;
    
    case MOVER_ABAJO:
        juego->stitch.posicion.fil  += DISTANCIA_1;
        break;

    case MOVER_DERECHA:
        juego->stitch.posicion.col  += DISTANCIA_1;
        break;
     
    case MOVER_IZQUIERDA:
        juego->stitch.posicion.col  -= DISTANCIA_1;
        break;
    }
}

/*
PRE: Movimiento debe ser un valor valido.
POST: Modifica la coordenada fila o columna de Reuben
*/
void mover_reuben(juego_t* juego, char movimiento){
    switch (movimiento){
    case MOVER_ARRIBA:
        juego->reuben.posicion.fil  -= DISTANCIA_1;
        break;
    
    case MOVER_ABAJO:
        juego->reuben.posicion.fil  += DISTANCIA_1;
        break;

    case MOVER_DERECHA:
        juego->reuben.posicion.col  += DISTANCIA_1;
        break;
     
    case MOVER_IZQUIERDA:
        juego->reuben.posicion.col  -= DISTANCIA_1;
        break;
    }
}

/*
PRE: -
POST: Verifica si la fila y columna dada esta ocupada y devuelve false si lo esta
*/
bool posicion_valida_fuego_o_matafuego(int filas, int columnas, juego_t juego){
    
    if ((juego.reuben.posicion.fil == filas && juego.reuben.posicion.col == columnas) || (juego.stitch.posicion.fil == filas && juego.stitch.posicion.col == columnas) || posicion_objetos(filas,columnas,juego)){
        return false;
    }
    return true; 
}

/*
PRE: -
POST: Agrega FUEGO al vector de obstaculos en una posicion valida. 
*/
void inicializar_fuego(juego_t* juego){

    int coordenada_filas_fuego = 0;
    int coordenada_columnas_fuego = 0; 
    int fuego_asignado = 0; 
    
    while (fuego_asignado != CANTIDAD_FUEGOS){

        if(juego->personaje_activo == STITCH){

            coordenada_filas_fuego = conseguir_numero_aleatorio_cuadrante_stitch();
            coordenada_columnas_fuego = conseguir_numero_aleatorio_columna();
            
            if(posicion_valida_fuego_o_matafuego(coordenada_filas_fuego, coordenada_columnas_fuego, *juego)){

                asignar_obstaculos(juego, coordenada_filas_fuego, coordenada_columnas_fuego, FUEGO);
                fuego_asignado++;
            }
        }
        else{
            coordenada_filas_fuego = conseguir_numero_aleatorio_cuadrante_reuben();
            coordenada_columnas_fuego = conseguir_numero_aleatorio_columna();

            if(posicion_valida_fuego_o_matafuego(coordenada_filas_fuego, coordenada_columnas_fuego, *juego)){
            
                asignar_obstaculos(juego,coordenada_filas_fuego,coordenada_columnas_fuego,FUEGO);
                fuego_asignado++;
            }
        }
    }
}

/*
PRE: Debe estar el vector herramientas inicializado.
POST: Agrega el MATAFUEGOS al vector de herramientas en una posicion valida
*/
void inicializar_matafuegos(juego_t* juego){
    int coordenada_filas_matafuego = 0;
    int coordenada_columnas_matafuego = 0;
    int matafuegos_asignados = 0;
    
    while(matafuegos_asignados != CANTIDAD_MATAFUEGOS){
        if(juego->personaje_activo == STITCH){

            coordenada_filas_matafuego = conseguir_numero_aleatorio_cuadrante_stitch();
            coordenada_columnas_matafuego = conseguir_numero_aleatorio_columna();

            if(posicion_valida_fuego_o_matafuego(coordenada_filas_matafuego, coordenada_columnas_matafuego, *juego)){
            
                asignar_herramienta(juego, coordenada_filas_matafuego,coordenada_columnas_matafuego, MATAFUEGOS);
                matafuegos_asignados++;

            }
        }
        else{
            if(posicion_valida_fuego_o_matafuego(coordenada_filas_matafuego, coordenada_columnas_matafuego, *juego)){
                coordenada_filas_matafuego = conseguir_numero_aleatorio_cuadrante_reuben();
                coordenada_columnas_matafuego = conseguir_numero_aleatorio_columna();


                asignar_herramienta(juego,coordenada_filas_matafuego,coordenada_columnas_matafuego,MATAFUEGOS);
                matafuegos_asignados++;
            }
        }
    }
    printf("Has llegado a los 15 movimientos, se ha creado un fuego y un matafuego,las acciones de preparacion de comidas quedan inhabilitadas hasta que lo apagues!\n");
}

/*
PRE: Movimiento debe ser un valor valido
POST: desplaza la coordenada fila o columna del personaje segun corresponda
*/
void mover_personaje(juego_t* juego, char movimiento, char personaje, bool fuego_activo){

    if(posicion_valida_para_moverse(*juego, personaje, movimiento)){
    if(!fuego_activo){

        juego->movimientos++;
    }
        if(personaje == STITCH){           
            mover_stitch(juego,movimiento);                          
        } 
        else{
            mover_reuben(juego,movimiento); 
        }
    }     
}

/*
PRE: -
POST: Modifica el campo de personaje activo.
*/
char cambiar_personaje(juego_t* juego, char personaje_actual){

    if (personaje_actual == REUBEN){
        juego->personaje_activo = STITCH;
    }
    if(personaje_actual == STITCH){
        juego->personaje_activo = REUBEN;
    }
    return personaje_actual;
} 

/*
PRE: -
POST: Llena el cambo objeto en mano de los personajes y pone la posicion del ingrediente a una posicion invalida
*/
void agarrar_ingredientes(juego_t* juego, char personaje){
    int filas = posicion_fila_jugador_activo(*juego,personaje);
    int columnas = posicion_columna_jugador_activo(*juego,personaje);
    char objeto = ' ';
    
    for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){
        if(juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil == filas && juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col == columnas){

            objeto = juego->comida[juego->tope_comida-1].ingrediente[i].tipo;
            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = POSICION_INVALIDA;
            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = POSICION_INVALIDA;

            if(personaje == STITCH && juego->stitch.objeto_en_mano == MANO_VACIA){
                juego->stitch.objeto_en_mano = objeto;
            }
            if(personaje == REUBEN && juego->reuben.objeto_en_mano == MANO_VACIA){
                juego->reuben.objeto_en_mano = objeto;
            }
        }
    }
}

/*
PRE: -
POST: Vacia el objeto en mano y ubica el ingrediente en esa posicion
*/
void soltar_ingredientes(juego_t* juego, char personaje){
    int filas = 0;
    int columnas = 0; 
    char ingrediente = ' ';
    int coordenada_columna_jugador = posicion_columna_jugador_activo(*juego,personaje);
    int coordenada_fija_jugador = posicion_fila_jugador_activo(*juego,personaje);

    if (!posicion_herramienta(coordenada_fija_jugador,coordenada_columna_jugador,*juego) && !posicion_ingredientes(coordenada_fija_jugador,coordenada_columna_jugador,*juego)){
    
        if(personaje == STITCH){
            filas = juego->stitch.posicion.fil;
            columnas = juego->stitch.posicion.col;
            ingrediente = juego->stitch.objeto_en_mano;
            juego->stitch.objeto_en_mano = MANO_VACIA;
            
        } 
        else{
            filas = juego->reuben.posicion.fil;
            columnas = juego->reuben.posicion.col;
            ingrediente = juego->reuben.objeto_en_mano;
            juego->reuben.objeto_en_mano = MANO_VACIA;
        }

        for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){
            if(juego->comida[juego->tope_comida-1].ingrediente[i].tipo == ingrediente ){
                juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = filas;
                juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = columnas;
            }
        }
    }
}

/*
PRE: Los parametros deben estar inicializados y cargados.
POST: Devuelve a la distancia manhattan que dos elementos se encuentran
*/
int distancia_manhattan(int fila1, int fila2, int columna1, int columna2){
    int coordenada_en_x, coordenada_en_y,resultado;
    coordenada_en_x = (fila1- fila2);
    coordenada_en_y = (columna1 - columna2);
    
    if(coordenada_en_x < 0){
        coordenada_en_x *= MODULO;
    }
    if(coordenada_en_y < 0){
        coordenada_en_y *= MODULO;
    }
    resultado = coordenada_en_x + coordenada_en_y;
    return resultado;
}

/*
PRE: El tope y el vector de herramientas debe estar incializado
POST: Devuelve true si estas arriba de un cuchillo y con las manos ocupadas y false en caso contrario 
*/
bool posicion_valida_para_cortar(juego_t juego){

    int filas = juego.stitch.posicion.fil;
    int columnas = juego.stitch.posicion.col;
    bool valido = false;

    if(juego.stitch.objeto_en_mano != MANO_VACIA){

        for (int i = 0; i < juego.tope_herramientas; i++){
            if (juego.herramientas[i].posicion.fil == filas && juego.herramientas[i].posicion.col == columnas && juego.herramientas[i].tipo == CUCHILLOS){
                valido = true;
            }
        }
    }
    return valido;
}

/*
PRE: -.
POS: Transforma en true el valor de cortado de cada ingrediente.
*/
bool valido_para_cocinar(juego_t juego, char objeto_en_mano){

    if(objeto_en_mano == MASA || objeto_en_mano == MILANESA || objeto_en_mano == CARNE){
        return true;
    }
    return false;
}

/*
PRE: debe estar inicializado el vector de ingredientes.
POS: Transforma en true el valor de cortado de cada ingrediente.
*/
void cortar_ingredientes(juego_t* juego){
    char objeto_en_mano = juego->stitch.objeto_en_mano;

    if (posicion_valida_para_cortar(*juego) && !valido_para_cocinar(*juego,objeto_en_mano)){
        
        for(int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){
            
            if(juego->comida[juego->tope_comida-1].ingrediente[i].tipo == objeto_en_mano){

                juego->comida[juego->tope_comida-1].ingrediente[i].esta_cortado = true;
                printf("Has cortado el ingrediente con exito\n");
            }
        }
    }
    else{
        printf("Asi no funciona el cuchillo. No puedes cortar un ingrediente sino estas arriba de la herramienta y recuerda que los ingredientes proveniente del cuadrante de Reuben no se pueden cortar\n");
    }
}        

/*
PRE: Debe estar inicializado el vector de herramientas.
POS: devuelve true si esta a distancia 1 del horno.
*/
bool posicion_valida_para_cocinar(juego_t juego){
    bool valido = false;
    int fila_jugador = juego.reuben.posicion.fil;
    int columna_jugador = juego.reuben.posicion.col;

    if(juego.reuben.objeto_en_mano != MANO_VACIA){
        
        for (int i = 0; i < juego.tope_herramientas; i++){

            if(distancia_manhattan(fila_jugador,juego.herramientas[i].posicion.fil,columna_jugador,juego.herramientas[i].posicion.col) == DISTANCIA_1 && juego.herramientas[i].tipo == HORNO){
            valido = true;
            }
        }
    }
    return valido;
}

/*
PRE: -.
POS: Transforma en true el valor de cocinado de cada ingrediente.
*/
void cocinar_ingredientes(juego_t* juego){
    char objeto_en_mano = juego->reuben.objeto_en_mano;

    if(posicion_valida_para_cocinar(*juego) && valido_para_cocinar(*juego, objeto_en_mano)){

        for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){

            if(juego->comida[juego->tope_comida-1].ingrediente[i].tipo == objeto_en_mano && juego->comida[juego->tope_comida-1].ingrediente[i].esta_cortado != true){

                juego->comida[juego->tope_comida-1].ingrediente[i].esta_cocinado = true;
                printf("Has cocinado el ingrediente con exito\n");
            }
        }   
    }
    else{
        printf("Asi no funciona el horno. No puedes cocinar un ingrediente estando a mas de distancia manhattan 1 del horno y recuerda que los ingredientes proveniente del cuadrante de stitch no se pueden cocinar\n");
    }
    
}

/*
PRE: -.
POS: Lo que esta en la mano del personaje lo pone en la mesa y lo saca de la mano
*/
void dejar_sobre_mesa_stitch(juego_t* juego, int filas, int columnas){

    for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){

        if(juego->stitch.objeto_en_mano == juego->comida[juego->tope_comida-1].ingrediente[i].tipo){

            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = filas + DISTANCIA_1;
            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = columnas;
            juego->stitch.objeto_en_mano = MANO_VACIA;                
        }
    }
}

/*
PRE: -.
POS: Lo que esta en la mano de reuben lo pone en la mesa y lo saca de la mano
*/
void dejar_sobre_mesa_reuben(juego_t* juego, int filas, int columnas){

    for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){

        if(juego->reuben.objeto_en_mano == juego->comida[juego->tope_comida-1].ingrediente[i].tipo){

            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = filas - DISTANCIA_1;
            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = columnas;
            juego->reuben.objeto_en_mano = MANO_VACIA;                
        }
    }
}

/*
PRE: -.
POS: Lo que esta en la mano del personaje lo pone en la mesa y lo saca de la mano
*/
void poner_en_mesa(juego_t* juego, char personaje, bool fuego_creado, int fila_mesa, int columna_mesa){
    int fila_stitch = juego->stitch.posicion.fil;
    int columna_stitch = juego->stitch.posicion.col;
    int fila_reuben = juego->reuben.posicion.fil;
    int columna_reuben = juego->reuben.posicion.col;

    if(distancia_manhattan(fila_stitch, fila_mesa, columna_stitch, columna_mesa) == DISTANCIA_1 && personaje == STITCH){
        if(!fuego_creado){

            dejar_sobre_mesa_stitch(juego,fila_stitch,columna_stitch);
            
        }
        else{
            printf("No puedes dejar ingredientes con el fuego activo en tu cuadrante\n");
        }
    }
        
    if(distancia_manhattan(fila_reuben, fila_mesa, columna_reuben, columna_mesa) == DISTANCIA_1 && personaje == REUBEN){
        if(!fuego_creado){
            dejar_sobre_mesa_reuben(juego,fila_reuben,columna_reuben);
        }
        else{
            printf("No puedes dejar ingredientes con el fuego activo en tu cuadrante\n");
        }
    }
}

/*
PRE: Debe haber puesto algo en la mesa.
POS: Lo que esta en la mesa lo pone en la mano de stitch y lo saca de la mesa 
*/
void agarrar_de_mesa_stitch(juego_t* juego, int filas, int columnas){
    for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){

        if(juego->stitch.objeto_en_mano == MANO_VACIA && juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil == (filas + 1) && juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col == columnas){

            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = POSICION_INVALIDA;
            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = POSICION_INVALIDA;

            juego->stitch.objeto_en_mano = juego->comida[juego->tope_comida-1].ingrediente[i].tipo;    
            printf("Has agarrado un %c\n", juego->stitch.objeto_en_mano);            
        }
    }
}

/*
PRE: Debe haber puesto algo en la mesa.
POS: Lo que esta en la mesa lo pone en la mano de reuben y lo saca de la mesa 
*/
 void agarrar_de_mesa_reuben(juego_t* juego, int filas, int columnas){
    
    for (int i = 0; i < juego->comida[juego->tope_comida-1].tope_ingredientes; i++){

        if(juego->reuben.objeto_en_mano == MANO_VACIA && juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil == (filas -1) && juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col == columnas){

            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.fil = -1;

            juego->comida[juego->tope_comida-1].ingrediente[i].posicion.col = -1;
            juego->reuben.objeto_en_mano = juego->comida[juego->tope_comida-1].ingrediente[i].tipo;       
            printf("Has agarrado un %c\n", juego->reuben.objeto_en_mano);         
        }
    }
}

/*
PRE: Debe estar el juego inicializado.
POS: Lo que esta en la mesa lo pone en la mano del personaje y lo saca de la mesa 
*/
void sacar_de_mesa(juego_t* juego, char personaje, bool fuego_creado,int fila_mesa, int columna_mesa){
    
    int fila_stitch = juego->stitch.posicion.fil;
    int columna_stitch = juego->stitch.posicion.col;
    int fila_reuben = juego->reuben.posicion.fil;
    int columna_reuben = juego->reuben.posicion.col;

    if(distancia_manhattan(fila_stitch, fila_mesa, columna_stitch, columna_mesa) == DISTANCIA_1 && personaje == STITCH){
        if(!fuego_creado){

            agarrar_de_mesa_stitch(juego,fila_stitch,columna_stitch);
        }
    }
    
    if(distancia_manhattan(fila_reuben, fila_mesa, columna_reuben, columna_mesa) == DISTANCIA_1 && personaje == REUBEN){
        if(!fuego_creado){
            agarrar_de_mesa_reuben(juego,fila_reuben,columna_reuben);
        }
    }
}

/*
PRE: debe estar el matafuego inicializado.
POS: Devuelve true si el personaje esta en la misma posicion del matafuego.
*/
bool posicion_agarrar_matafuego(juego_t* juego, int filas, int columnas){
    bool posicion_valida = false;

    for (int i = 0; i < juego->tope_herramientas; i++){
 
        if(juego->herramientas[i].posicion.fil == filas && juego->herramientas[i].posicion.col == columnas && juego->herramientas[i].tipo == MATAFUEGOS){
 
            juego->herramientas[i].posicion.fil = POSICION_INVALIDA;
            juego->herramientas[i].posicion.col = POSICION_INVALIDA;
            posicion_valida = true;
        }
    }
    return posicion_valida;
}

/*
PRE: Debe estar el matafuego inicializado
POS: pone el matafuego en la mano del personaje
*/
void agarrar_matafuego(juego_t* juego, char personaje){
    int filas = posicion_fila_jugador_activo(*juego,personaje);
    int columnas = posicion_columna_jugador_activo(*juego,personaje);
    
    if(personaje == STITCH && juego->stitch.objeto_en_mano == MANO_VACIA){
        if (posicion_agarrar_matafuego(juego,filas,columnas)){
            juego->stitch.objeto_en_mano = MATAFUEGOS;
            printf("Has agarrado el matafuego, ve a apagar el fuego\n");
        }
    }
    if(personaje == REUBEN && juego->reuben.objeto_en_mano == MANO_VACIA){
        if (posicion_agarrar_matafuego(juego,filas,columnas)){
            juego->reuben.objeto_en_mano = MATAFUEGOS;
            printf("Has agarrado el matafuego, ve a apagar el fuego\n");
        }
    }
}

/*
PRE: -
POS: Devuelve la coordenada fila de la posicion del fuego
*/
int poisicion_fila_fuego(juego_t juego){
    int fila_obstaculo = -1;
    int fila_fuego = 0;

    for (int i = 0; i < juego.tope_obstaculos && fila_fuego != fila_obstaculo; i++){
            
        fila_obstaculo = juego.obstaculos[i].posicion.fil;
    
        if(juego.obstaculos[i].tipo == FUEGO){
            fila_fuego = fila_obstaculo;
        }
    }
    return fila_fuego;
}

/*
PRE: -
POS: Devuelve la coordenada columna de la posicion del fuego
*/
int poisicion_columna_fuego(juego_t juego){
    int columna_obstaculo = -1;
    int columna_fuego = 0;

    for (int i = 0; i < juego.tope_obstaculos && columna_fuego != columna_obstaculo; i++){
            
        columna_obstaculo = juego.obstaculos[i].posicion.col;
    
        if(juego.obstaculos[i].tipo == FUEGO){
            columna_fuego = columna_obstaculo;
        }
    }
    return columna_fuego;
}

/*
PRE: -
POS: elimina el matafuego del vector herramientay pone el objeto en mano en MANO_VACIA.
*/
void eliminar_matafuego(juego_t* juego,char personaje){

    for (int i = 0; i < juego->tope_herramientas; i++){

        if (juego->herramientas[i].tipo == MATAFUEGOS){
            
            juego->herramientas[i].tipo = juego->herramientas[juego->tope_herramientas-1].tipo;
            juego->tope_herramientas--;

            if (personaje == STITCH){
                juego->stitch.objeto_en_mano = MANO_VACIA;
            }
            else{
                juego->reuben.objeto_en_mano = MANO_VACIA;
            }
        }
    }
}

/*
PRE: -
POS: elimina el fuego del vector obstaculos.
*/
void eliminar_fuego(juego_t* juego){
    for (int i = 0; i < juego->tope_obstaculos; i++){

        if (juego->obstaculos[i].tipo == FUEGO){
            
            juego->obstaculos[i].tipo = juego->obstaculos[juego->tope_obstaculos-1].tipo;
            juego->tope_obstaculos--;
        }
    }
}

/*
PRE: el fuego debe estar inicializado y el matafuego tambien.
POS: elimina el fuego y el matafuego de sus respectivos vectores
*/
void apagar_fuego(juego_t* juego, char personaje){
    
    if(personaje == STITCH && juego->stitch.objeto_en_mano == MATAFUEGOS){
        eliminar_fuego(juego); 
        eliminar_matafuego(juego,personaje);
        printf("Has apagado el fuego, dentro de 15 movimientos se inicializara de nuevo!\n");

    }
    else if((personaje == REUBEN && juego->reuben.objeto_en_mano == MATAFUEGOS)){
        eliminar_fuego(juego);
        eliminar_matafuego(juego,personaje);
        printf("Has apagado el fuego, dentro de 15 movimientos se inicializara de nuevo!\n");
    }
}

/*
PRE: El vector comida_lista debe estar cargado y el tope de comida_lista inicializado.
POS: Devuelve true si los elementos en comida_lista tienen su posicion en la salida y false en caso contrario
*/
bool ingrediente_entregado(juego_t juego, char ingrediente){

    bool entregado = false;

    for(int i = 0; i < juego.tope_comida_lista; i++){

        if (juego.comida_lista[i].posicion.fil == juego.salida.fil && juego.comida_lista[i].posicion.col == juego.salida.col && ingrediente == juego.comida_lista[i].tipo){
            entregado = true;  
        }
    }
    return entregado;
}

/*
PRE: El vector ingredientes debe estar cargado y el tope de ingredientes inicializado.
POS: Devuelve si esta cortado o no cortado el objeto en mano
*/
bool esta_cortado(juego_t juego, char objeto_en_mano){
    bool cortado = false;
    for (int i = 0; i < juego.comida[juego.tope_comida-1].tope_ingredientes && !cortado; i++){
        if((objeto_en_mano == juego.comida[juego.tope_comida-1].ingrediente[i].tipo && juego.comida[juego.tope_comida-1].ingrediente[i].esta_cortado == true)){

            cortado = true;
        }
    }
    return cortado;
}

/*
PRE: El vector ingredientes debe estar cargado y el tope de ingredientes inicializado.
POS: Devuelve si esta o no cocinado el obeto en mano
*/
bool esta_cocinado(juego_t juego, char objeto_en_mano){
    bool cocinado = false;

    for (int i = 0; i < juego.comida[juego.tope_comida-1].tope_ingredientes && !cocinado; i++){
        if((objeto_en_mano == juego.comida[juego.tope_comida-1].ingrediente[i].tipo && juego.comida[juego.tope_comida-1].ingrediente[i].esta_cocinado == true)){

            cocinado = true;
        }
    }
    return cocinado;
}

/*
PRE: El vector ingredientes debe estar cargado y el tope de ingredientes inicializado.
POS: aumenta el tope de comida lista y carga el vector comida_lista
*/
void preparar_comida(juego_t* juego){

    if(juego->reuben.objeto_en_mano != MANO_VACIA && (esta_cocinado(*juego, juego->reuben.objeto_en_mano) || esta_cortado(*juego, juego->reuben.objeto_en_mano))){

        juego->comida_lista[juego->tope_comida_lista].tipo = juego->reuben.objeto_en_mano;
        juego->comida_lista[juego->tope_comida_lista].posicion.fil = juego->salida.fil;
        juego->comida_lista[juego->tope_comida_lista].posicion.col = juego->salida.col;
        juego->comida_lista[juego->tope_comida_lista].esta_cocinado = esta_cocinado(*juego, juego->reuben.objeto_en_mano);
        juego->comida_lista[juego->tope_comida_lista].esta_cortado = esta_cortado(*juego, juego->reuben.objeto_en_mano);
        (juego->tope_comida_lista)++;
        juego->reuben.objeto_en_mano = MANO_VACIA; 
        printf("Has emplatado el ingrediente\n");
    }
}

/*
PRE: El vector ingredientes debe estar cargado y el tope de inicializado inicializado.
POS: Devuelve si se puede pasar el nivel o no.
*/
bool nivel_terminado(juego_t* juego){
    
    bool terminado = false;
    
    if ((ingrediente_entregado(*juego,TOMATE) && ingrediente_entregado(*juego,LECHUGA)) && juego->comida_actual == ENSALADA){   
        terminado = true;
    }
    else if (ingrediente_entregado(*juego, JAMON) && ingrediente_entregado(*juego,QUESO) && ingrediente_entregado(*juego,MASA) && juego->comida_actual == PIZZA){
        terminado = true;
        
    }
    else if (ingrediente_entregado(*juego, PAN) && ingrediente_entregado(*juego, CARNE) && ingrediente_entregado(*juego,LECHUGA) && ingrediente_entregado(*juego, TOMATE) && juego->comida_actual == HAMBURGUESA){
        terminado = true;
    }
    else if (ingrediente_entregado(*juego, PAN) && ingrediente_entregado(*juego, MILANESA) && ingrediente_entregado(*juego, LECHUGA) && ingrediente_entregado(*juego, TOMATE) && ingrediente_entregado(*juego,JAMON) && ingrediente_entregado(*juego,QUESO) && juego->comida_actual == SANDWICH){
        
        terminado = true;
    }
    return terminado;
}

/*
PRE: El vector obstaculos debe estar cargado y el tope de obstaculos inicializado.
POS: Devuelve si hay o no un fuego en el vector de obstaculos 
*/
bool fuego_activo(juego_t* juego){
    for(int i = 0; i < juego->tope_obstaculos; i++){
        if(juego->obstaculos[i].tipo == FUEGO){

            return true;
        }
    }
    return false;
}

/*
pre: -
POST: devuelve true si a la posicion que se va a mover esta ubicada la salida, false en caso contrario
*/
bool posicion_entrega(juego_t* juego, char personaje, char movimiento){

    int filas = posicion_fila_jugador_activo(*juego,personaje);
    int columnas = posicion_columna_jugador_activo(*juego,personaje);
    
    bool posicion_entrega = false;
    switch (movimiento){
    case MOVER_ARRIBA:
        if(juego->salida.fil == filas-1 && juego->salida.col == columnas){
            posicion_entrega = true;
        }
        
        break;
    case MOVER_ABAJO:
        if(juego->salida.fil == filas+1 && juego->salida.col == columnas){
            posicion_entrega = true;
        }
        break;
    case MOVER_IZQUIERDA:
        if(juego->salida.fil == filas && juego->salida.col == columnas - 1 ){
            posicion_entrega = true;
        }
        break;
    case MOVER_DERECHA:
        if(juego->salida.fil == filas && juego->salida.col == columnas + 1){
            posicion_entrega = true;
        }
        break;
    }
    return posicion_entrega;
}

/*
PRE: -
POST: Carga la variable movimiento con lo Que ingrese el usuario
*/
char pedir_movimiento(char movimiento){
    printf("Ingrese un movimiento: %c", movimiento);
    scanf(" %c", &movimiento);
    return movimiento;
}

/*
PRE: -
POST: en caso de recibir un movimiento valido, lo efectua.
*/
void realizar_movimiento(juego_t* juego, char movimiento, char personaje, bool fuego_creado){
    

    if(movimiento_permitido(movimiento)){
            
        mover_personaje(juego,movimiento,personaje, fuego_creado);            
    }
    if(!comando_permitido(movimiento) && !(movimiento_permitido(movimiento))){
        printf("El movimiento ingresado es invalido, acuerdate que los movimientos permitidos son (W,A,S,D). No te olvides de las mayusculas\n");
    }

}

/*
PRE: El juego debe estar inicializado
POST: Incializa los ingredientes del siguiente nivel
*/
void pasar_de_nivel(juego_t* juego){
    if(nivel_terminado(juego)){
        if(juego->tope_comida == 1 && juego->precio_total <= PRECIO_PRIMER_NIVEL){
            inicializar_ingredientes_primer_nivel(juego);
        }
        else if(juego->tope_comida <= 2 && juego->precio_total > PRECIO_PRIMER_NIVEL && juego->precio_total <= PRECIO_SEGUNDO_NIVEL){
                
            inicializar_ingredientes_segundo_nivel(juego);
        }
        else if(juego->tope_comida <= 3 && juego->precio_total > PRECIO_SEGUNDO_NIVEL){
                
            inicializar_ingredientes_tercer_nivel(juego);
        }   
        printf("Has pasado de nivel!\n");         
    }
}

/*
PRE: Fuego_creado debe estar cargado
POST: Si no hay ningun fuego en el vector de obstaculos y se hizo 15 movimientos se crea uno
*/
void crear_fuego(juego_t* juego, bool fuego_creado){
    if(juego->movimientos == MOVIMIENTOS_MAXIMOS && !fuego_creado){   
        inicializar_fuego(juego);
        inicializar_matafuegos(juego);
    }
}

/*
PRE: -
POST: Elimina el fuego del vector de obstaculos y el matafuegos del de herramientas si se ingreso el movimiento correcto.
*/
void destruir_fuego(juego_t* juego, char movimiento, char personaje, int fila_jugador, int columna_jugador, int fila_fuego, int columna_fuego){

    if(movimiento == ACTIVAR_MATAFUEGO){
        if(distancia_manhattan(fila_jugador, fila_fuego ,columna_jugador, columna_fuego) <= DISTANCIA_2){
            apagar_fuego(juego,personaje);
            juego->movimientos = 0;
        }
    }
}

/*
PRE: Movimiento, fuego_creado, fila_fuego,fila_mesa y columna_mesa deben estar inicializadas
POST: Segun el movimiento ingresado agarra o suelta el ingrediente
*/
void manipular_ingredientes(juego_t* juego, char movimiento, bool fuego_creado, char personaje, int fila_fuego, int fila_mesa, int columna_mesa){
    if(movimiento == AGARRAR_O_SOLTAR_INGREDIENTE){
        if(personaje == STITCH){  
            if (juego->stitch.objeto_en_mano == MANO_VACIA){
                if((fuego_creado && fila_fuego > MITAD_DE_TABLERO) || !fuego_creado ){       

                    agarrar_ingredientes(juego,personaje); 
                    printf("Has agarrado el ingrediente %c\n", juego->stitch.objeto_en_mano);       
                }
            }
            else if(juego->stitch.objeto_en_mano != MATAFUEGOS){            
                soltar_ingredientes(juego, personaje);  
                printf("Has soltado el ingrediente\n");  
            }     
        }        
        else{
            if(juego->reuben.objeto_en_mano == MANO_VACIA){
                if((fuego_creado && fila_fuego < MITAD_DE_TABLERO) || !fuego_creado){       
                    
                    agarrar_ingredientes(juego,personaje);
                    printf("Has agarrado el ingrediente %c\n", juego->reuben.objeto_en_mano);
                }
            }   
            else if(juego->reuben.objeto_en_mano != MATAFUEGOS){
                    
                soltar_ingredientes(juego, personaje); 
                printf("Has soltado el ingrediente\n"); 
            }     
        }
    }


}

/*
PRE: Movimiento ya debe estar cargado.
POST: prepara los ingredientes para su entrega, cortandolos o cocinandolos segun corresponda
*/
void preparar_ingredientes(juego_t* juego, char movimiento, int fila_fuego, bool fuego_creado){
    if(movimiento == CORTAR){
        if((fuego_creado && fila_fuego > MITAD_DE_TABLERO) || !fuego_creado){ 
            cortar_ingredientes(juego);
        }
    }

    else if(movimiento == COCINAR){
        if((!fuego_creado && fila_fuego < MITAD_DE_TABLERO) || !fuego_creado){        
            cocinar_ingredientes(juego);
        }
    }
}

void realizar_jugada(juego_t *juego, char movimiento){ 
    char personaje = juego->personaje_activo;  
    int fila_mesa = juego->mesa.fil;
    int columna_mesa = juego->mesa.col;
    int fila_jugador = posicion_fila_jugador_activo(*juego,personaje);
    int columna_jugador = posicion_columna_jugador_activo(*juego,personaje);
    int fila_fuego = poisicion_fila_fuego(*juego);
    int columna_fuego = poisicion_columna_fuego(*juego);
    bool fuego_creado = fuego_activo(juego);
    movimiento = pedir_movimiento(movimiento);

    if(posicion_entrega(juego,personaje,movimiento)){   
        preparar_comida(juego);
    }

    realizar_movimiento(juego,movimiento,personaje,fuego_creado);
    
    crear_fuego(juego,fuego_creado);
    
    agarrar_matafuego(juego,personaje);
    
    destruir_fuego(juego,movimiento,personaje,fila_jugador,columna_jugador,fila_fuego,columna_fuego);
    
    pasar_de_nivel(juego);

    manipular_ingredientes(juego,movimiento,fuego_creado,personaje,fila_fuego,fila_mesa,columna_mesa);
    
    preparar_ingredientes(juego,movimiento,fila_fuego,fuego_creado);
    
    if(movimiento == PASAR_INGREDIENTE){
        if(!posicion_ingredientes(fila_mesa,columna_mesa,*juego)){
                    
            poner_en_mesa(juego,personaje,fuego_creado, fila_mesa, columna_mesa);

        }
        else{
            sacar_de_mesa(juego,personaje,fuego_creado, fila_mesa, columna_mesa);
        }
    }
        
    if (movimiento == CAMBIO_DE_JUGADOR){    
        cambiar_personaje(juego,personaje);
        personaje = juego->personaje_activo;
    }
    
}