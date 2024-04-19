#include <stdio.h>
#include <stdbool.h>

const char INTEGRAL = 'I';
const char BLANCO = 'B';
const char AVENA_Y_MIEL = 'A';
const char QUESO_Y_OREGANO = 'Q';
const char DAMBO = 'D';
const char CHEDDAR = 'C';
const char GRUYEERE = 'G';
const char SIN_QUESO = 'S';
#define ROAST_BEEF 'R'
#define ATUN 'A'
#define SOJA 'S'
#define POLLITO 'P'
#define NADA_DE_PROTE 'N'
const char MINIMA_MEDIDA = 15;
const char MAXIMA_MEDIDA = 30;
const int PRECIO_PAN_BASICO = 5;
const int PRECIO_PAN_ESPECIAL = 8;
const int PRECIO_QUESO_BASICO = 5;
const int PRECIO_QUESO_ESPECIAL = 8;
const int PRECIO__ROAST_BEEF = 7;
const int PRECIO_ATUN = 9;
const int PRECIO_POLLO = 5;
const int PRECIO_SOJA = 3;
const int PRECIO_SIN_AGREGADO = 0;
#define PREGUNTAR_MEDIDA 1
#define PREGUNTAR_TIPO_PAN 2
#define PREGUNTAR_TIPO_QUESO 3
#define PREGUNTAR_TIPO_PROTE 4
#define PREGUNTAR_CALENTAR 5
const float MULTIPLICADOR = 0.3f;

/*
PRE: - tipo_de_pregunta debe ser un valor valido
POST: Imprime por pantalla el mensaje correspondiente al tipo de pregunta
*/
void mostrar_mensaje(int tipo_de_pregunta){

    switch (tipo_de_pregunta)
    {
    case PREGUNTAR_MEDIDA:

        printf("Bienvenido. De que medida vas a querer tu sandwich?.Ingrese la medida sin aclarar la unidad: ");
        break;

    case PREGUNTAR_TIPO_PAN:

        printf("Luego de seleccionar tu medida debes elegir el tipo de pan. Las opciones de panes son:\n Blanco (%c)\n Integral (%c)\n Avena y Miel (%c)\n Queso y Oregano (%c).\n Para elegir la opcion deseada ingrese el caracter correspondiente: ",BLANCO,INTEGRAL,AVENA_Y_MIEL,QUESO_Y_OREGANO );
        break;
    
    case PREGUNTAR_TIPO_QUESO:

        printf("Luego de seleccionar tu pan debes elegir el tipo de queso que quieres en tu sandwich. Las opciones disponibles son:\n Dambo (%c)\n Cheddar (%c)\n Gruyere (%c)\n Sin queso (%c).\n Para elegir la opcion deseada ingrese el caracter correspondiente: ",DAMBO,CHEDDAR,GRUYEERE,SIN_QUESO);
        break;

    case PREGUNTAR_TIPO_PROTE:

        printf("Ahora debes elegir el tipo de proteina que quieres en tu sandwich. Las opciones disponibles son:\n Roast Beef (%c).\n Atun (%c).\n Soja (%c).\n Pollito (%c).\n Ninguna proteina (%c).\n Para elegir la opcion deseada ingrese el caracter correspondiente: ", ROAST_BEEF, ATUN,SOJA,POLLITO,NADA_DE_PROTE);
        break;
    
    case PREGUNTAR_CALENTAR:

        printf("Desea calentar su sandwich?. Ingrese 'S' para si o 'N' para no, para elegir la opcion deseada ingrese el caracter correspondiente: ");
        break;
    }
}

/*
PRE: medida_sandwich debe estar inicializado
POST: Devuelve si medidasandwich esta entre los valores de MINIMA_MEDIDA Y MAXIMA_MEDIDA
*/
bool es_medida_elegida_valida (int medida_sandwich){

    return medida_sandwich > MINIMA_MEDIDA && medida_sandwich < MAXIMA_MEDIDA;
}

/* 
Pre: -
Post: Guarda en medida_sandwich el valor ingresado por el usuario, el cual siemrpe se valida, en caso de ser un valor invalido se vuelve a preguntar
*/
void preguntar_medida(int* medida_sandwich){
   
    mostrar_mensaje(PREGUNTAR_MEDIDA);
    scanf("%i", medida_sandwich);

    while (!es_medida_elegida_valida(*medida_sandwich)){

        printf("Las medidas disponibles son entre 15 a 30. Volve a ingresar tu medida!: ");
        scanf("%i", medida_sandwich);
    }
}

/*
PRE: pan_seleccionado debe estar inicializado
POST: Devuelve si pan_seleccionado es alguna de las opciones presentadas
*/
bool es_eleccion_pan_valida(char pan_seleccionado){
    return pan_seleccionado == BLANCO || pan_seleccionado == INTEGRAL || pan_seleccionado == AVENA_Y_MIEL || pan_seleccionado == QUESO_Y_OREGANO;
}

/*
PRE: -
POST: Guarda en pan_seleccionado el valor ingresado por el usuario, el cual siemrpe se valida, en caso de ser un valor invalido se vuelve a preguntar 
*/
void eleccion_pan(char* pan_seleccionado){

    mostrar_mensaje(PREGUNTAR_TIPO_PAN);
    scanf(" %c", pan_seleccionado);

    while (!es_eleccion_pan_valida(*pan_seleccionado)) {

        printf("Debes elegir entre las opciones dadas, en caso de haber elegido una opcion existente (B,I,A,Q) recorda el uso de mayusculas. Vuelva a ingresar su tipo de pan deseado: ");
        scanf(" %c", pan_seleccionado);
    }
}

/*
PRE: queso debe estar inicializada
POST: devuelve si queso es diferente a todas las opciones presentadas
*/
bool es_eleccion_queso_invalido(char queso){
    return queso != DAMBO && queso != CHEDDAR && queso != GRUYEERE && queso != SIN_QUESO;
}

/*
PRE: -
POST: Guarda en queso_seleccionado el valor ingresado por el usuario,, el cual siemrpe se valida, en caso de ser un valor invalido se vuelve a preguntar
*/
void eleccion_queso(char* queso_seleccionado){

    mostrar_mensaje(PREGUNTAR_TIPO_QUESO);
    scanf(" %c", queso_seleccionado);
    
    while (es_eleccion_queso_invalido(*queso_seleccionado)) {

        printf("Debes elegir entre las opciones dadas, en caso de haber elegido una opcion existente (D,C,G,S) recorda el uso de mayusculas. Vuelva a ingresar su tipo de queso deseado: ");
        scanf(" %c", queso_seleccionado);
    }
}

/*
PRE: proteina_deseada debe estar inicializada
POST: Devuelve si proteina deseada es  diferente a todaas las opciones posibles
*/
bool es_eleccion_proteina_invalido(char proteina_deseada){
    return proteina_deseada != ROAST_BEEF && proteina_deseada != ATUN && proteina_deseada != SOJA && proteina_deseada != POLLITO && proteina_deseada != NADA_DE_PROTE;

}

/*
PRE: -
POST:  Guarda en proteina_seleccionad el valor ingresado por el usuario, , el cual siemrpe se valida, en caso de ser un valor invalido se vuelve a preguntar
*/
void eleccion_proteina(char* proteina_seleccionada){

    mostrar_mensaje(PREGUNTAR_TIPO_PROTE);
    scanf(" %c", proteina_seleccionada);
    
    while (es_eleccion_proteina_invalido(*proteina_seleccionada)) {

        printf("Debes elegir entre las opciones dadas, en caso de haber elegido una opcion existente (R,A,S,P,N) recorda el uso de mayusculas. Vuelva a ingresar su tipo de proteina deseada: ");
        scanf(" %c", proteina_seleccionada);
    }
}

/*
PRE:  
POST: Devuelve si el usuario quiere calentar o no su sandwich,, el cual siemrpe se valida, en caso de ser un valor invalido se vuelve a preguntar 
*/
char calentar_sandwich(char proteina_elegida){

    char respuesta = ' ';
    char afirmativo = 'S';
    char negativo = 'N';

    if (proteina_elegida == ATUN){

        printf("El atun no se puede calentar en este establecimiento.\n");
    }
    else{
    
        mostrar_mensaje(PREGUNTAR_CALENTAR);
        scanf(" %c", &respuesta);

        while (respuesta != afirmativo && respuesta != negativo){
            
            printf("La opcion que elegiste no es valida, Si desea calentar su sandwich ingrese 'S' , en caso contrario ingrese 'N': ");
            scanf(" %c", &respuesta);
        }   
    }
    return respuesta;
}

/*
PRE: 
POST: devuelve el precio del sandwich al hacer la suma del precio del pan + precio del queso + precio proteina 
*/
int obtener_precio_sandwich(int* medida_elegida, char* pan_elegido, char* queso_elegido, char* proteina_elegida){

    int precio = 0;

    if (*pan_elegido == BLANCO || *pan_elegido == INTEGRAL){

        precio += PRECIO_PAN_BASICO;
    }
    else{

        precio += PRECIO_PAN_ESPECIAL; 
    }
    if (*queso_elegido == DAMBO || *queso_elegido == CHEDDAR){

        precio += PRECIO_QUESO_BASICO;
    }
    else if(*queso_elegido == GRUYEERE){

        precio+= PRECIO_QUESO_ESPECIAL;
    }
    else{

        precio += PRECIO_SIN_AGREGADO;
    }

    switch (*proteina_elegida){

        case ROAST_BEEF:
            precio += PRECIO__ROAST_BEEF;
            break;
        case ATUN:
            precio += PRECIO_ATUN;
            break;
        case POLLITO:
            precio += PRECIO_POLLO;
            break;
        case SOJA:
            precio += PRECIO_SOJA;
            break;
        default:
            precio += PRECIO_SIN_AGREGADO;
            break;
    }
    precio = (int) ((float) precio * ( MULTIPLICADOR * (float) *medida_elegida));

    return precio;
}

void calcular_precio_chambuchito(int* precio){

    int medida_elegida = 0;
    preguntar_medida(&medida_elegida);

    char pan_elegido = ' ';
    eleccion_pan(&pan_elegido);

    char queso_elegido = ' ';
    eleccion_queso(&queso_elegido);

    char proteina_elegida = ' ';
    eleccion_proteina(&proteina_elegida);

    calentar_sandwich(proteina_elegida);

    *precio = obtener_precio_sandwich(&medida_elegida, &pan_elegido, &queso_elegido, &proteina_elegida);

}