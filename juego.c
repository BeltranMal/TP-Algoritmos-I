#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chambuchito.h"
#include "cocineritos.h"

int main(){
    srand((unsigned)time(NULL));
    juego_t juego;
    int precio = 0;
    char movimiento = ' ';
    calcular_precio_chambuchito(&precio);
    printf("El precio total es -%i-\n",precio);
    inicializar_juego(&juego,precio);
    
    while (estado_juego(juego) == 0){
        imprimir_terreno(juego);
        realizar_jugada(&juego, movimiento);
        system("clear");
    }
    
    if(estado_juego(juego) == - 1){
        printf("Perdiste\n");
    }
    else{
        printf("Ganaste\n");
    }   
    return 0;
}



