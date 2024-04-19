import csv 
import os
import sys

NO_VALIDO = -1
POSICION_HORAS = 2
POSICION_MINIMA_MINUTOS = 3
POSICION_MAXIMA_MINUTOS = 6
MAXIMO_HORAS = 23
MAXIMO_MINUTOS = 59
MINIMO_TIEMPO = 00
MINIMA_CANTIDAD_PERSONAS = 1
AGREGAR = "agregar"
MODIFICAR = "modificar"
ELIMINAR = "eliminar"
NOMBRE = "nombre"
CANTIDAD_DE_PERSONAS = "cant"
HORARIO = "hora"
UBICACION = "ubicacion"
LISTAR = "listar"
AFUERA = 'F'
ADENTRO = 'D'
LONGITUD_RESERVA = 6
LONGITUD_HORARIO = 5
LONGITUD_MINIMA_DATOS = 2
POSICION_ACCION = 1
POSICION_IDS = 0
POSICION_NOMBRE = 1
POSICION_CANTIDAD_PERSONAS = 2
POSICION_HORARIO = 3
POSICION_UBICACION = 4
POSCION_DELIMITADOR_MINUTOS = 2
NOMBRE_ARCHIVO = "reservas.csv"
NOMBRE_ARCHIVO_AUXILIAR = "reservas_aux.csv"
DELIMITADOR = ";"
DELIMITADOR_MINUTOS = ":"
MODO_ESCRITURA = "w"
MODO_AGREGAR = "a"
ERROR_ABRIR_ARCHIVO = 1
ERROR_NOMBRE = 2
ERROR_CANTIDAD_PERSONAS = 3
ERROR_UBICACION = 4
ERORR_ABRIR_ARCHIVO_AUXILIAR = 5
CAMPOS = 6
ERROR_ID = 7
INSUFICIENCIA_PARAMETROS = 8
ID_INICIAL = 1
LISTAR_TODO = 2
LISTAR_POR_RANGO = 4 

#PRE: mensaje debe ser un valor valido
#POST: Imprime por pantalla el mensaje correspondiente 
def mostrar_mensaje(mensaje):
    if mensaje == ERROR_ABRIR_ARCHIVO:
        print("No se pudo abrir el archivo de reservas, puede ser que no se haya agregado ninguna reserva o algun otro Fallo. Vuelve a intentarlo")
    elif mensaje == ERROR_NOMBRE:
        print(f"Error. El {NOMBRE} no puede ser un numero, Debe ingresar el {NOMBRE} a quien se hace la reserva")
    elif mensaje == ERROR_CANTIDAD_PERSONAS:
        print("Error. La cantidad de personas en la mesa debe ser un numero mayor a 0")
    elif mensaje == ERROR_UBICACION:
        print(f"Error. La {UBICACION} de la reserva debe ser ADENTRO ({ADENTRO}) o AFUERA ({AFUERA}). Recorda las mayusculas")
    elif mensaje == ERORR_ABRIR_ARCHIVO_AUXILIAR:
        print("Error al abrir reservas_aux.txt")
    elif mensaje == CAMPOS:
        print(f"Los campos disponibles son el {NOMBRE}, la cantidad de personas ({CANTIDAD_DE_PERSONAS}), la {HORARIO} (ingresar en formato HH:MM) y la {UBICACION} ({ADENTRO} o {AFUERA})")  
    elif mensaje == ERROR_ID:
        print("el id ingresado no es valido")
    elif mensaje == INSUFICIENCIA_PARAMETROS:
        print("Cantidad de parametros incorrecta")

# PRE: El archivo no debe contene lineas vacias 
# POST: Devuelve el id de la reserva
def obtener_id(archivo):
    try:
        reservas = open(archivo)
    except:
        return ID_INICIAL
    id = ID_INICIAL
    lector = csv.reader(reservas, delimiter = DELIMITADOR)  
    for datos in lector:
        if datos[POSICION_IDS].isnumeric():
            identificador = int(datos[POSICION_IDS])
            if identificador >= id:
                id = identificador + 1 
    reservas.close()
    return id

#PRE: La cadena debe estar inicalizada
#POST: Devuelve True si la cadena contiene letras y numeros.
def es_alfanumerico(cadena):
    alfanumerico = False
    numero_presente = False
    letra_presente = False
    for i in range(len(cadena)):
        if cadena[i] == DELIMITADOR_MINUTOS or cadena[i].isnumeric():
            numero_presente = True
        else:
            letra_presente = True
    if(numero_presente == True and letra_presente == True): 
        alfanumerico = True

    return alfanumerico

# PRE: Horario debe estar inicializado
# POST: Devuelve True si el horario y su formato son valido y False en caso contrario
def es_horario_valido(horario):
    horario_valido = True

    if len(horario) != LONGITUD_HORARIO: 
        print("El horario no tiene un formato valido. El formato tiene que ser de la forma HH:MM")
        horario_valido = False

    elif es_alfanumerico(horario) == True: 
        print("El horario debe ser un numero")
        horario_valido = False

    elif horario[POSCION_DELIMITADOR_MINUTOS] != DELIMITADOR_MINUTOS:
        print(f"El horario no tiene un formato valido. debe tener {DELIMITADOR_MINUTOS} entre las horas y los minutos")
        horario_valido = False
                
    elif int(horario[:POSICION_HORAS]) > MAXIMO_HORAS or int(horario[:POSICION_HORAS])< MINIMO_TIEMPO:
        print("Las horas no tiene un formato valido deben ser desde las 00 hasta las 23")
        horario_valido = False
                
    elif int(horario[POSICION_MINIMA_MINUTOS:POSICION_MAXIMA_MINUTOS]) > MAXIMO_MINUTOS or int(horario[POSICION_MINIMA_MINUTOS:POSICION_MAXIMA_MINUTOS]) < MINIMO_TIEMPO: 
        print("Los minutos no tienen un formato valido deben ser desde los 00 hasta los 59")
        horario_valido = False
    return horario_valido

# PRE: El campo debe estar incializado
# Devuelve la posicion del campo, en caso de que le campo no sea valido devuelve NO_VALIDO 
def hallar_posicion_campo(campo):
    posicion = NO_VALIDO
    if campo == NOMBRE:
        posicion = POSICION_NOMBRE
    elif campo == CANTIDAD_DE_PERSONAS:
        posicion = POSICION_CANTIDAD_PERSONAS
    elif campo == HORARIO:
        posicion = POSICION_HORARIO
    elif campo == UBICACION:
        posicion = POSICION_UBICACION
    else:
        mostrar_mensaje(CAMPOS)
    return posicion

# PRE: Los parametros deben estar inicializados
# POST: Devuelve True si los parametros cumplen con el formato pedido y False en caso contrio
def es_reserva_valida(datos_de_la_reserva):
    es_valido = True
    if datos_de_la_reserva[POSICION_NOMBRE].isnumeric():
        mostrar_mensaje(ERROR_NOMBRE)
        es_valido = False

    elif es_horario_valido(datos_de_la_reserva[POSICION_HORARIO]) == False:
        es_valido = False

    elif not datos_de_la_reserva[POSICION_CANTIDAD_PERSONAS].isnumeric() or int(datos_de_la_reserva[POSICION_CANTIDAD_PERSONAS]) < MINIMA_CANTIDAD_PERSONAS:
        mostrar_mensaje(ERROR_CANTIDAD_PERSONAS)
        es_valido = False
    
    elif datos_de_la_reserva[POSICION_UBICACION] != ADENTRO and datos_de_la_reserva[POSICION_UBICACION] != AFUERA:
        mostrar_mensaje(ERROR_UBICACION)
        es_valido = False
    
    return es_valido

# PRE: el id debe estar inicializado
# POST: Agrega la reserva al archivo si es valida 
def cargar_archivo(archivo,id):
    try:
        reservas = open(archivo,MODO_AGREGAR)
    except:
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return

    reservacion = sys.argv[LONGITUD_MINIMA_DATOS:]
    datos_de_la_reserva = []
    datos_de_la_reserva.insert(POSICION_IDS,id)
    for elemento in reservacion:
        datos_de_la_reserva.append(elemento)
        
    if es_reserva_valida(datos_de_la_reserva):
        escritor = csv.writer(reservas, delimiter = DELIMITADOR)
        escritor.writerow(datos_de_la_reserva)
        print("Se ha agregado la reserva con exito")
    
    reservas.close()

# PRE: El campo debe estar incializado
# POST: Devuelve True si el valor es valido y False en caso contrario
def es_modificacion_valida(campo,nuevo_valor):
    es_valido = True

    if hallar_posicion_campo(campo) == POSICION_HORARIO and es_horario_valido(nuevo_valor) == False:
        es_valido = False

    elif hallar_posicion_campo(campo) == POSICION_CANTIDAD_PERSONAS and (not nuevo_valor.isnumeric() or int(nuevo_valor) < MINIMA_CANTIDAD_PERSONAS):
        mostrar_mensaje(ERROR_CANTIDAD_PERSONAS)
        es_valido= False
        
    elif hallar_posicion_campo(campo) == POSICION_UBICACION and nuevo_valor != AFUERA and nuevo_valor != ADENTRO:
        mostrar_mensaje(ERROR_UBICACION)
        es_valido = False 
    
    elif hallar_posicion_campo(campo) == POSICION_NOMBRE and nuevo_valor.isnumeric():
        mostrar_mensaje(ERROR_NOMBRE)
        es_valido = False
    
    return es_valido

# PRE: -
#POST: Devuelve el valor a modificar y la posicion del campo a modificar dentro del archivo
def pedir_modificar_reserva():
    reserva_valida = False
    nuevo_valor = NO_VALIDO
    posicion_campo = NO_VALIDO
    campo = NO_VALIDO
    mostrar_mensaje(CAMPOS)
    while(reserva_valida == False):
        
        modificacion = input("Ingrese el campo a modificar y la modificacion: ")
        modificacion = modificacion.split()

        if(len(modificacion) != LONGITUD_MINIMA_DATOS):
            print("\n")
            mostrar_mensaje(INSUFICIENCIA_PARAMETROS)
        else:
            campo, nuevo_valor = modificacion
            posicion_campo = hallar_posicion_campo(campo)
        
        if(posicion_campo == NO_VALIDO or es_modificacion_valida(campo,nuevo_valor) == False):
            print("\n")
        else:
            reserva_valida = True
    
    return nuevo_valor,posicion_campo

# PRE: Los parametros deben estar inicializados
# POST: Modifca una reserva si el pedido de modificacion es valido
def modificar_reserva(archivo_auxiliar,archivo,id):
    try:
        reservas = open(archivo)
    except:
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return
    try:
        reservas_modificadas = open(archivo_auxiliar,MODO_ESCRITURA)
    except:
        reservas.close()
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return 
    
    modificador = csv.writer(reservas_modificadas, delimiter = DELIMITADOR)
    lector = csv.reader(reservas, delimiter = DELIMITADOR)
    encontrado = False
    for linea in lector:
        if id == linea[POSICION_IDS]:
            nuevo_valor, posicion_campo = pedir_modificar_reserva()
            if(nuevo_valor != NO_VALIDO and posicion_campo != NO_VALIDO):
                linea[posicion_campo] = nuevo_valor
                encontrado = True
        modificador.writerow(linea)

    reservas.close()
    reservas_modificadas.close()

    if encontrado == False:
        mostrar_mensaje(ERROR_ID)
    else:
        print("la modificacion se ha hecho con exito")
        os.rename(archivo_auxiliar,archivo)

# PRE: Los parametros deben estar inicializados
# POST: Elimina una reserva si el id es valido
def eliminar_reserva(archivo_auxiliar,archivo,id):
    try:
        reservas = open(archivo)
    except:
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return
    try:
        reservas_modificadas = open(archivo_auxiliar,MODO_ESCRITURA)
    except:
        reservas.close()
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return 
    lector = csv.reader(reservas, delimiter = DELIMITADOR)
    escritor = csv.writer(reservas_modificadas, delimiter = DELIMITADOR)
    encontrado = False
    for linea in lector:

        if linea[POSICION_IDS].isnumeric() and id.isnumeric():
            if int(id) != int(linea[POSICION_IDS]):
                escritor.writerow(linea)
            else:
                encontrado = True
    reservas.close()
    reservas_modificadas.close()

    if encontrado == True:
        print("Se ha eliminado con exito")
        os.rename(archivo_auxiliar,archivo)
    else:
        mostrar_mensaje(ERROR_ID)

# PRE: El archivo debe estar creado para funcionar
# POST: Muestra las reservas dentro del archivo
def listar_todo_el_contenido(archivo):
    try:
        reservas = open(archivo)
    except:
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return
    lector = csv.reader(reservas, delimiter= DELIMITADOR)
    
    for datos in lector:
        print(f"ID: {datos[POSICION_IDS]}")
        print(f"{NOMBRE}: {datos[POSICION_NOMBRE]}")
        print(f"CANTIDAD DE PERSONAS: {datos[POSICION_CANTIDAD_PERSONAS]}")
        print(f"{HORARIO}: {datos[POSICION_HORARIO]} hs")
        print(f"{UBICACION}: {datos[POSICION_UBICACION]} \n")
    
    reservas.close()

#PRE: El archivo debe estar creado para funcionar
#POST: Muestra todas las reservas dentro de un rango especifco dado
def listar_contenido_especifico(archivo,inicio,fin):
    try:
        reservas = open(archivo)
    except:
        mostrar_mensaje(ERROR_ABRIR_ARCHIVO)
        return
    lector = csv.reader(reservas, delimiter= DELIMITADOR)
    existe = False
    ids_disponibles = []
    for linea in lector:
        ids_disponibles.append(linea[POSICION_IDS])
        if linea[POSICION_IDS].isnumeric() and inicio.isnumeric() and fin.isnumeric() and int(linea[POSICION_IDS]) >= int(inicio) and int(linea[POSICION_IDS]) <= int(fin):  
            existe = True
            print(f"ID: {linea[POSICION_IDS]}")
            print(f"{NOMBRE}: {linea[POSICION_NOMBRE]}")
            print(f"CANTIDAD DE PERSONAS: {linea[POSICION_CANTIDAD_PERSONAS]}")
            print(f"{HORARIO}: {linea[POSICION_HORARIO]} hs")
            print(f"{UBICACION}: {linea[POSICION_UBICACION]} \n")

    reservas.close()

    if existe == False:
        ids_disponibles = DELIMITADOR.join(ids_disponibles)
        print(f"Los ids disponibles son: {ids_disponibles}")
        mostrar_mensaje(ERROR_ID)

def main():
    archivo = NOMBRE_ARCHIVO
    archivo_auxiliar = NOMBRE_ARCHIVO_AUXILIAR 
    id = NO_VALIDO
    if len(sys.argv) < LONGITUD_MINIMA_DATOS:
        mostrar_mensaje(INSUFICIENCIA_PARAMETROS)
        return 
    
    elif len(sys.argv) > LONGITUD_MINIMA_DATOS:
        id = sys.argv[2]
    
    accion = sys.argv[POSICION_ACCION]

    if accion == AGREGAR:
        if len(sys.argv) < LONGITUD_RESERVA or len(sys.argv) > LONGITUD_RESERVA:
            print(f"Cantidad de parametros es incorrecta. Las reservas se hacen en el formato: {NOMBRE},cantidad de personas, {HORARIO}, {UBICACION}")
        else:
            id = obtener_id(archivo)
            cargar_archivo(archivo,id) 

    elif accion == MODIFICAR:
        if len(sys.argv) == 3:
            modificar_reserva(archivo_auxiliar, archivo,id)
        else:
            mostrar_mensaje(INSUFICIENCIA_PARAMETROS)

    elif accion == ELIMINAR:    
        if len(sys.argv) == 3: 
            eliminar_reserva(archivo_auxiliar, archivo,id)
        else:
            mostrar_mensaje(INSUFICIENCIA_PARAMETROS)
    
    elif accion == LISTAR:
        if len(sys.argv) == LISTAR_TODO:
            listar_todo_el_contenido(archivo)  

        elif len(sys.argv) == LISTAR_POR_RANGO:
            inicio = sys.argv[2]
            fin = sys.argv[3]
            listar_contenido_especifico(archivo,inicio,fin)
    
        else:
            print("Cantidad de parametros es incorrecta, para listar todas las reservas ingresar: listar. Para listar reservas especificas ingresar: listar id1 id2")
    else:
        print(f"El accion ingresado no es valido, las acciones permitidos son {AGREGAR} ,{MODIFICAR},{ELIMINAR} o {LISTAR}. Acordate el uso de las minusculas")

main()