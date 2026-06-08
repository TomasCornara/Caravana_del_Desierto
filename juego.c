#include "juego.h"
#include "manejo_archivos.h"
#include "dado.h"
#include "lista_circular_doble.h"

#define DEBUG_ACTIVO 1

void jugar_partida(t_mapa *mapa, t_config *config)
{
    #if DEBUG_ACTIVO
        unsigned numero_turno = 0;
    #endif // DEBUG_ACTIVO

    t_movimientos cola_movimientos_jugador;
    t_movimientos cola_turno;
    //t_movimiento movi;
    t_jugador jugador;
    t_dado dado;
    //t_casillero *casillero_actual;
    char direccion;
    unsigned cantidad_casilleros,
             posicion_salida;
    unsigned      cant_movs = 0;
    t_raking      arbol_indice;
    t_reg_indice  reg_idx;
    unsigned      id_jugador;
    limpiar_pantalla();

    //Inicializacion
    cantidad_casilleros = cantidad_elementos_lista(mapa);
    posicion_salida = (config->cantidad_posiciones) - 1;
    crearCola(&cola_movimientos_jugador);
    crearCola(&cola_turno);
    crearDado(&dado, CARAS_DADO);
    inicializar_jugador(&jugador, config);
    pedir_nombre(jugador.nombre);

    // Generacion caravana.txt
    FILE *archivo_caravana = abrir_txt(ARCHIVO_MAPA, "w");
    if (archivo_caravana)
    {
        printCaravana(archivo_caravana, mapa);
        fclose(archivo_caravana);
    }

    ///Juego
    while ((jugador.vidas > 0) && (jugador.pos_en_mapa != posicion_salida))
    {
        limpiar_pantalla();
        #if DEBUG_ACTIVO
            printf("DEBUG - Numero de turno: %d\n",numero_turno);
            numero_turno++;
        #endif // DEBUG_ACTIVO

        mostrarEstadisticas(jugador.vidas, jugador.puntos, jugador.nombre);
        printCaravana(stdout, mapa);

        if (jugador.efectoTormenta)
        {
            jugador.efectoTormenta = false;
            printf("La tormenta te hace perder este turno.\n");
            mostrarFooter();
            pausa();

        }
        else
        {
            int pos_final_j;
            direccion = AVANZAR; //Por defecto se avanza

            tirarDado(&dado);
            printf("DADO - Sacaste un: %d\n",dado.cara);
            mostrarFooter();

            #if DEBUG_ACTIVO
                printf("DEBUG - Estado jugador:\n jugador.pos_en_mapa: %d\n jugador.efecto_tormenta: %s\n jugador.efecto_oasis: %s\n",
                    jugador.pos_en_mapa, jugador.efectoTormenta ? "VERDADERO" : "FALSO", jugador.efectoOasis ? "VERDADERO" : "FALSO");
                printf("DEBUG - Sobreescribir valor del dado. Ingrese un valor y presione ENTER: ");
                scanf("%d", &dado.cara);
                limpiar_buffer();
            #endif // DEBUG_ACTIVO

            //Si hay lugar suficiente, se le permite al usuario regresar
            if(dado.cara < jugador.pos_en_mapa + 1)
            {
                do
                {
                    printf("\nIngresa 'A' para avanzar y 'R' para retroceder y presiona ENTER: ");
                    scanf("%c", &direccion);
                    limpiar_buffer();

                    if(direccion != AVANZAR && direccion != RETROCEDER)
                        printf("\nIngrese un término válido.");
                }
                while(direccion != AVANZAR && direccion != RETROCEDER);
            }

            pos_final_j = calcular_pos_final_del_jugador(jugador.pos_en_mapa, cantidad_casilleros, dado.cara, direccion);
            guardar_movimiento(&cola_movimientos_jugador, jugador.pos_en_mapa, pos_final_j, true);
            guardar_movimiento(&cola_turno, jugador.pos_en_mapa, pos_final_j, true);

            #if DEBUG_ACTIVO
                printf("DEBUG - Direccion calculada/elegida: %c\n", direccion);
                printf("DEBUG - Posición final calculada para el jugador: %d\n", pos_final_j);
            #endif // DEBUG_ACTIVO

            pausa();
            limpiar_pantalla();
        }

        ///Logica de los bandidos
        //calcular_bandidos(mapa,&cola_turno);

        ///Logica del turno - ACA FALTARIA IMPLEMENTAR ESTA FUNCION (CREO QUE SERIA ALGO ASI)
        //resolver_turno(mapa,&cola_turno,%jugador);
    }

    ///Fin del juego
    limpiar_pantalla();
    jugador.vidas?  victoria() : gameOver();


    printf("Puntos: %d\n",jugador.puntos);
    #if DEBUG_ACTIVO
        printf("DEBUG - Ingrese cuantos puntos quiere tener: ");
        scanf("%d", &jugador.puntos);
    #endif // DEBUG_ACTIVO

    cant_movs=mostrar_movimientos(&cola_movimientos_jugador, jugador.nombre);

    crearArbol(&arbol_indice);
    indice_cargar(&arbol_indice);
    ///SI LO ENCUENTRA NOS DEVUELVE 1 SINO 0
    if (indice_buscar(&arbol_indice, jugador.nombre, &reg_idx)) {
        ///id_jugador = reg_idx.id;
        id_jugador=buscar_id(&reg_idx);
        printf("%u",id_jugador);
    } else {
        id_jugador        = jugadores_proximo_id();
        reg_idx.posicion   = (long)((id_jugador - 1) * sizeof(t_reg_jugador));
        strncpy(reg_idx.nombre, jugador.nombre, MAX_NOMBRE - 1);
        *(reg_idx.nombre + MAX_NOMBRE - 1) = '\0';
        jugadores_agregar(id_jugador, jugador.nombre);
        indice_insertar(&arbol_indice, &reg_idx);
        indice_guardar(&arbol_indice);
        printf("%u",id_jugador);
    }

    partidas_agregar(id_jugador, jugador.puntos, cant_movs);
    destruirArbol(&arbol_indice);
    //Limpiar estructuras
    vaciarCola(&cola_movimientos_jugador);
    vaciarCola(&cola_turno);
    destruirDado(&dado);

    return;
}

void calcular_bandidos(t_mapa * mapa, t_movimientos * cola)
{
    //recorrer_adelante_accion(mapa,situar_bandidos, (void*)cola);
}

void situar_bandidos(void* a, void* parametro_extra)
{
    tNodo * nodoactual = (tNodo*)a;
    t_casillero casillero_lista = *(t_casillero*)nodoactual->info;

    tNodo * pivot = (tNodo*)a;
    t_casillero casillero_pivot;

    t_dado dado;
    int signo;
    int contador = 0;

    if(casillero_lista.cant_bandidos < 1) return;

    crearDado(&dado,CARAS_DADO);
    signo = tirarDado(&dado);


    casillero_pivot = *(t_casillero*)pivot->info;

    while(signo > contador)
    {
        pivot = (signo%2 == 0) ? pivot->sig : pivot->ant;
        casillero_pivot = *(t_casillero*)pivot->info;
        contador++;
    }

    guardar_movimiento((t_movimientos*)parametro_extra,casillero_lista.nro_posicion,casillero_pivot.nro_posicion,false);
}


void inicializar_jugador(t_jugador *jugador, t_config *config)
{
    strcpy(jugador->nombre, "");
    jugador->vidas = config->vidas_inicio;
    jugador->puntos = 0;
    jugador->efectoTormenta = false;
    jugador->efectoOasis = false;
    jugador->pos_en_mapa = 0;
}

int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final, bool jugador_humano)
{
    t_movimiento mov;
    mov.pos_inicial = pos_inicial;
    mov.pos_final = pos_final;
    mov.jugador_humano = jugador_humano;
    return acolar(cola, &mov, sizeof(t_movimiento));
}

void mover_jugador(t_jugador *jugador, unsigned pasos, int lado)
{

}
int calcular_pos_final_del_jugador(unsigned pos_inicial_del_jugador,
                                   unsigned cantidad_nodos_lista,
                                   unsigned cantidad_pasos,
                                   char direccion){
    unsigned pos;



    if(!cantidad_nodos_lista || pos_inicial_del_jugador > cantidad_nodos_lista || (direccion != AVANZAR && direccion != RETROCEDER))
    {
        return -1;
    }

    pos = pos_inicial_del_jugador;

    while(cantidad_pasos > 0)
    {
        if(direccion == AVANZAR)
        {
            if(pos == cantidad_nodos_lista){
                direccion = RETROCEDER; //Aca rebota
                pos--;
            }
            else{
                pos++;
            }
        }
        else //Va para atras si no llega 1
        {
            if(pos>1)
                pos--;
            //Si pos es 1 se queda ahí
        }
        cantidad_pasos--;
    }
    return pos;
}

void resolver_casillero_actual(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos)
{
    bool caso_oasis = false;

    if (!jugador || !casillero_actual)
    {
        return;
    }

    if (casillero_actual->tipo_casillero == TIPO_OASIS)
    {
        jugador->efectoOasis = true;
    }
    else if (casillero_actual->tipo_casillero == TIPO_TORMENTA)
    {
        if(jugador->efectoOasis)
        {
            caso_oasis = true;

        }
        else
        {
            jugador->efectoTormenta = true;
        }
    }
    else if (casillero_actual->tipo_casillero == TIPO_PREMIO)
    {
        jugador->puntos++;
    }
    else if (casillero_actual->tipo_casillero == TIPO_VIDA_EXTRA)
    {
        jugador->vidas++;
    }
    else if (casillero_actual->tipo_casillero != TIPO_TORMENTA && jugador->efectoOasis)
    {
        jugador->efectoOasis = false;
        limpiar_pantalla();
        printf("Pierdes el efecto Oasis.\n");
        pausa();
        limpiar_buffer();
    }

    if (casillero_actual->animacion)
    {
        limpiar_pantalla();
        casillero_actual->animacion();

        if(caso_oasis)
        {
            caso_oasis = false;
            printf("El Oasis te protege\n");
            jugador->efectoOasis = false;
            limpiar_buffer();
        }

        pausa();
    }

    //Cuando el jugador cae en un casillero, lo normnaliza
    if(casillero_actual->tipo_casillero != TIPO_INICIO && casillero_actual->tipo_casillero != TIPO_FIN)
    {
        casillero_actual->tipo_casillero = TIPO_NORMAL;
        casillero_actual->animacion = NULL;
    }

    resolver_bandido_en_casillero(jugador, casillero_actual, cola_movimientos);
}

void resolver_bandido_en_casillero(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos)
{
    if (!jugador || !casillero_actual) return;

    //No hay bandidos
    if(!casillero_actual->cant_bandidos) return;

    limpiar_pantalla();
    printBandido();
    pausa();

    if (jugador->vidas > 0)
    {
        jugador->vidas--;
        limpiar_pantalla();
        printf("El bandido te quita 1 vida.\n");
        //mover_jugador(jugador, ((t_casillero*)jugador->pos->info)->nro_posicion, -1);
    }

    casillero_actual->cant_bandidos--; //Elimina al bandido que ataco
    pausa();
}

void mover_bandido(t_mapa *mapa, t_movimientos *cola_movimientos)   ///REVISAR ESTA FUNCION, ESTA BASTANTE FEA
{
    tNodo *inicio;
    tNodo *temp;
    tNodo **nodos;
    unsigned *cantidades;
    unsigned cantidad_nodos;
    unsigned i;

    if (!mapa || !*mapa)
    {
        return;
    }

    inicio = *mapa;
    while (inicio->ant != NULL)
    {
        inicio = inicio->ant;
    }

    cantidad_nodos = 0;
    temp = inicio;
    while (temp != NULL)
    {
        cantidad_nodos++;
        temp = temp->sig;
    }

    if (cantidad_nodos < 2)
    {
        return;
    }

    nodos = malloc(cantidad_nodos * sizeof(tNodo *));
    cantidades = malloc(cantidad_nodos * sizeof(unsigned));
    if (!nodos || !cantidades)
    {
        free(nodos);
        free(cantidades);
        return;
    }

    temp = inicio;
    for (i = 0; i < cantidad_nodos; i++)
    {
        nodos[i] = temp;
        cantidades[i] = ((t_casillero *)temp->info)->cant_bandidos;
        temp = temp->sig;
    }

    for (i = 0; i < cantidad_nodos; i++)
    {
        unsigned bandido;
        unsigned destino = (i + 1) % cantidad_nodos;
        t_casillero *cas_origen = (t_casillero *)nodos[i]->info;
        t_casillero *cas_destino = (t_casillero *)nodos[destino]->info;

        for (bandido = 0; bandido < cantidades[i]; bandido++)
        {
            guardar_movimiento(cola_movimientos, cas_origen->nro_posicion, cas_destino->nro_posicion, false);
        }

        cas_origen->cant_bandidos -= cantidades[i];
        cas_destino->cant_bandidos += cantidades[i];
    }

    free(nodos);
    free(cantidades);
}

void poner_bandidos_random(t_mapa *mapa, int total, int cantidad)
{
    t_dado dado;
    t_casillero *cas;
    crearDado(&dado,(total - 1));
    int valor_random;
    int puestos = 0;

    while (puestos < cantidad)
    {
        valor_random = tirarDado(&dado);
        cas = obtener_de_lista_dir_dato(mapa,&valor_random,comparar_clave_casillero);

        if (cas->tipo_casillero != TIPO_INICIO &&
                cas->tipo_casillero != TIPO_FIN &&
                cas->cant_bandidos == 0)
        {

            cas->cant_bandidos = 1;
            puestos++;
        }
    }
}

void poner_tipo_random(t_mapa *mapa, int total, unsigned tipo, int cantidad)
{
    t_dado dado;
    int puestos = 0;
    int valor_random;
    crearDado(&dado, (total - 1) );

    while (puestos < cantidad)
    {
        valor_random = tirarDado(&dado);
        t_casillero *cas = obtener_de_lista_dir_dato(mapa,&valor_random,comparar_clave_casillero);

        if (cas->tipo_casillero == TIPO_NORMAL)
        {
            cas->tipo_casillero = tipo;

            // Asignar función de animación según tipo
            switch(tipo)
            {
            case TIPO_OASIS:
                cas->animacion = printOasis;
                break;
            case TIPO_TORMENTA:
                cas->animacion = printTormenta;
                break;
            case TIPO_PREMIO:
                cas->animacion = printPremio;
                break;
            case TIPO_VIDA_EXTRA:
                cas->animacion = printVidaExtra;
                break;
            default:
                cas->animacion = NULL;
                break;
            }

            puestos++;
        }
    }
}

void poner_casilleros_especiales(t_mapa *mapa, t_config *config)
{
    poner_tipo_random(mapa, config->cantidad_posiciones, TIPO_OASIS, config->maximo_oasis);
    poner_tipo_random(mapa, config->cantidad_posiciones, TIPO_TORMENTA, config->maximo_tormentas);
    poner_tipo_random(mapa, config->cantidad_posiciones, TIPO_PREMIO, config->maximo_premios);
    poner_tipo_random(mapa, config->cantidad_posiciones, TIPO_VIDA_EXTRA, config->maximo_vidas_extra);
}

int juego_generar_mapa(t_config *config, t_mapa *mapa)
{
    t_casillero nuevo_casillero;
    crear_lista(mapa);
    unsigned nro_casillero = 0;

    nuevo_casillero.cant_bandidos = 0;
    nuevo_casillero.nro_posicion = nro_casillero;
    nuevo_casillero.presencia_jugador = false;
    nuevo_casillero.tipo_casillero = TIPO_NORMAL;

    while( nro_casillero < config->cantidad_posiciones ){
        if(nro_casillero == 0){
            nuevo_casillero.tipo_casillero = TIPO_INICIO;
            nuevo_casillero.presencia_jugador = true;
        }
        if(nro_casillero == (config->cantidad_posiciones - 1) ){
            nuevo_casillero.tipo_casillero = TIPO_FIN;
        }
        agregar_ord_en_lista(mapa,&nuevo_casillero,sizeof(t_casillero),comparar_posicion_casilleros);

        nro_casillero++;
        nuevo_casillero.tipo_casillero = TIPO_NORMAL;
        nuevo_casillero.presencia_jugador = false;
        nuevo_casillero.nro_posicion = nro_casillero;
    }

    poner_casilleros_especiales(mapa, config);
    poner_bandidos_random(mapa, config->cantidad_posiciones, config->maximo_bandidos);

    return 1;
}

int juego_cargar_config(t_config *config)
{
    char clave[MAX_CLAVE_CONFIG];
    int  valor;

    // Inicializar con valores por defecto (En caso de que al archivo le falte alguna clave, o no exista)
    config->cantidad_posiciones = CONFIG_CANTIDAD_POSICIONES_DEFAULT;
    config->vidas_inicio        = CONFIG_VIDAS_INICIO_DEFAULT;
    config->maximo_bandidos     = CONFIG_MAXIMO_BANDIDOS_DEFAULT;
    config->maximo_premios      = CONFIG_MAXIMO_PREMIOS_DEFAULT;
    config->maximo_vidas_extra  = CONFIG_MAXIMO_VIDAS_EXTRA_DEFAULT;
    config->maximo_oasis        = CONFIG_MAXIMO_OASIS_DEFAULT;
    config->maximo_tormentas    = CONFIG_MAXIMO_TORMENTAS_DEFAULT;

    FILE *f = abrir_txt(ARCHIVO_CONFIG, "r");
    if (!f)
    {
        printf("[Aviso] config.txt no encontrado. Se usaran todos los valores por defecto.\n");
        return 0;
    }

    while (fscanf(f, "%63[^:]:%d\n", clave, &valor) == VALORES_POR_LINEA_CONFIG)
    {
        if      (!strcmp(clave, "cantidad_posiciones")) config->cantidad_posiciones = valor;
        else if (!strcmp(clave, "vidas_inicio"))        config->vidas_inicio        = valor;
        else if (!strcmp(clave, "maximo_bandidos"))     config->maximo_bandidos     = valor;
        else if (!strcmp(clave, "maximo_premios"))      config->maximo_premios      = valor;
        else if (!strcmp(clave, "maximo_vidas_extra"))  config->maximo_vidas_extra  = valor;
        else if (!strcmp(clave, "maximo_oasis"))        config->maximo_oasis        = valor;
        else if (!strcmp(clave, "maximo_tormentas"))    config->maximo_tormentas    = valor;
    }
    fclose(f);
    return 1;
}

int juego_validar_config(t_config *config)
{
    int total_especiales = config->maximo_oasis + config->maximo_tormentas +
                           config->maximo_premios + config->maximo_vidas_extra;
    if (config->cantidad_posiciones < 2)
    {
        printf("Error: cantidad_posiciones debe ser mayor a 2. Valor actual: %u\n",
               config->cantidad_posiciones);
        return 0;
    }
    if (config->vidas_inicio < 1 || config->vidas_inicio > 10)
    {
        printf("Error: vidas_inicio debe estar entre 1 y 10. Valor actual: %u\n",
               config->vidas_inicio);
        return 0;
    }

    if (config->maximo_bandidos > config->cantidad_posiciones / 2)
    {
        printf("Error: maximo_bandidos no puede superar la mitad de casilleros.\n");
        printf("Casilleros: %u | Max bandidos permitidos: %u | Valor actual: %u\n",
               config->cantidad_posiciones, config->cantidad_posiciones / 2, config->maximo_bandidos);
        return 0;
    }
    if (total_especiales > config->cantidad_posiciones - 2)
    {
        printf("Error: demasiados casilleros especiales.\n");
        printf("Total especiales: %d | Casilleros disponibles: %u\n",
               total_especiales, config->cantidad_posiciones - 2);
        return 0;
    }

    return 1; // Todo OK
}
///MI PRINT CARAVANA
void printCaravana(FILE *archivo, t_mapa *mapa)
{
    t_casillero *cas;
    unsigned pos;
    //t_casillero casillero_actual;

    if (!archivo || !mapa || !*mapa) return;

    int contador_casilleros = 0;
    int cantidad_casilleros = cantidad_elementos_lista(mapa);

    while (contador_casilleros <= cantidad_casilleros )
    {

        //obtener_de_lista(mapa,&contador_casilleros,&casillero_actual,sizeof(t_casillero),comparar_clave_casillero);

        cas = obtener_de_lista_dir_dato(mapa,&contador_casilleros,comparar_clave_casillero);
        pos = cas->nro_posicion;

        int num_elems = 0;
        char elems[20][3];

        if (cas->tipo_casillero != TIPO_NORMAL || (!cas->presencia_jugador && cas->cant_bandidos == 0))
        {
            switch (cas->tipo_casillero)
            {
            case TIPO_INICIO:
                strcpy(elems[num_elems++], "I");
                break;
            case TIPO_FIN:
                strcpy(elems[num_elems++], "S");
                break;
            case TIPO_NORMAL:
                strcpy(elems[num_elems++], ".");
                break;
            case TIPO_OASIS:
                strcpy(elems[num_elems++], "O");
                break;
            case TIPO_TORMENTA:
                strcpy(elems[num_elems++], "T");
                break;
            case TIPO_VIDA_EXTRA:
                strcpy(elems[num_elems++], "V");
                break;
            case TIPO_PREMIO:
                strcpy(elems[num_elems++], "P");
                break;
            default:
                strcpy(elems[num_elems++], "?");
                break;
            }
        }

        if (cas->presencia_jugador)
        {
            strcpy(elems[num_elems++], "J");
        }

        for (unsigned i = 0; i < cas->cant_bandidos; i++)
        {
            if (num_elems < 20) strcpy(elems[num_elems++], "B");
        }

        fprintf(archivo, "%02u", pos);
        if (num_elems == 1)
        {
            fprintf(archivo, ":%s\n", elems[0]);
        }
        else if (num_elems > 1)
        {
            fprintf(archivo, ":[");
            for (int i = 0; i < num_elems; i++)
            {
                fprintf(archivo, "%s%s", elems[i], i == num_elems - 1 ? "" : " ");
            }
            fprintf(archivo, "]\n");
        }
        else
        {
            fprintf(archivo, ":.\n");
        }
        contador_casilleros++;
    }
    fprintf(archivo, "\n");
}
/*
void printCaravana(FILE *archivo, t_mapa *mapa)
{
    tNodo *temp;
    tNodo *inicio = (*mapa);

    t_casillero *cas;
    t_casillero *ultimocasillero = (t_casillero*)(inicio->ant->info);
    unsigned pos;

    if (!archivo || !mapa || !*mapa) return;

    temp = *mapa;

    int contador_casilleros = 0;
    int finalcas = ultimocasillero->nro_posicion;

    while (contador_casilleros <= finalcas )
    {
        cas = (t_casillero *)temp->info;
        pos = cas->nro_posicion;

        int num_elems = 0;
        char elems[20][3];

        if (cas->tipo_casillero != TIPO_NORMAL || (!cas->presencia_jugador && cas->cant_bandidos == 0))
        {
            switch (cas->tipo_casillero)
            {
            case TIPO_INICIO:
                strcpy(elems[num_elems++], "I");
                break;
            case TIPO_FIN:
                strcpy(elems[num_elems++], "S");
                break;
            case TIPO_NORMAL:
                strcpy(elems[num_elems++], ".");
                break;
            case TIPO_OASIS:
                strcpy(elems[num_elems++], "O");
                break;
            case TIPO_TORMENTA:
                strcpy(elems[num_elems++], "T");
                break;
            case TIPO_VIDA_EXTRA:
                strcpy(elems[num_elems++], "V");
                break;
            case TIPO_PREMIO:
                strcpy(elems[num_elems++], "P");
                break;
            default:
                strcpy(elems[num_elems++], "?");
                break;
            }
        }

        if (cas->presencia_jugador)
        {
            strcpy(elems[num_elems++], "J");
        }

        for (unsigned i = 0; i < cas->cant_bandidos; i++)
        {
            if (num_elems < 20) strcpy(elems[num_elems++], "B");
        }

        fprintf(archivo, "%02u", pos);
        if (num_elems == 1)
        {
            fprintf(archivo, ":%s\n", elems[0]);
        }
        else if (num_elems > 1)
        {
            fprintf(archivo, ":[");
            for (int i = 0; i < num_elems; i++)
            {
                fprintf(archivo, "%s%s", elems[i], i == num_elems - 1 ? "" : " ");
            }
            fprintf(archivo, "]\n");
        }
        else
        {
            fprintf(archivo, ":.\n");
        }
        contador_casilleros++;
        temp = temp->sig;
    }
    (*mapa) = inicio;
    fprintf(archivo, "\n");
}
*/
///FUNCIONES DE COMPARACION

int comparar_posicion_casilleros(const void* elem_a,const void* elem_b){
    t_casillero casillero_a = *(t_casillero*)elem_a;
    t_casillero casillero_b = *(t_casillero*)elem_b;

    return casillero_b.nro_posicion - casillero_a.nro_posicion;
}

int comparar_clave_casillero(const void* elem_a,const void* elem_b){

    unsigned clave = *(unsigned*)elem_a;
    t_casillero casillero_b = *(t_casillero*)elem_b;

    return clave - casillero_b.nro_posicion;
}

///FUNCIONES DE CONSOLA
void pausa() {
    printf("\n  Presione ENTER para continuar...");
    limpiar_buffer();
}

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pedir_nombre(char *nombre) {
    size_t len;
    size_t i;
    int letras;

    do {
        printf("Ingrese su nombre (al menos 3 letras): ");
        if (fgets(nombre, MAX_NOMBRE, stdin) == NULL) {
            nombre[0] = '\0';
            return;
        }

        // Eliminar el salto de linea
        len = strlen(nombre);
        if (len > 0 && nombre[len - 1] == '\n') {
            nombre[len - 1] = '\0';
            len--;
        } else {
            limpiar_buffer();
        }

        // Verificar que se haya escrito algo
        letras = 0;
        for (i = 0; i < len; i++) {
            if (isalpha((unsigned char)nombre[i])) {
                letras++;
            }
        }

        if (letras < 3) {
            printf("\nNombre invalido: ingrese al menos 3 letras.\n");
        }
    } while (letras < 3);

    // Normalizar a mayusculas
    for (i = 0; i < len; i++) {
        nombre[i] = (char)toupper((unsigned char)nombre[i]);
    }
}










void mostrar_menu() {
    printf("\n"
           "  ------------------------------------\n"
           "  |     CARAVANA DEL DESIERTO        |\n"
           "  ------------------------------------\n"
           "  |  1. Jugar nueva partida          |\n"
           "  |  2. Ver ranking                  |\n"
           "  |  3. Salir                        |\n"
           "  ------------------------------------\n"
           "  |              GRUPO 1             |\n"
           "  ------------------------------------\n"
           "  Opcion: ");
}


void mostrarHeader(void){
    printf(
    "\n"
    "                                                CARAVANA DEL DESIERTO                                                   \n"
    "\n"
    );
}

void mostrarBienvenida(void){
    printf(
    "                                     BIENVENIDO! TOQUE CUALQUIER TECLA PARA CONTINUAR                                   \n"
    "\n"
    "########################################################################################################################\n"
    );
}

void mostrarEstadisticas(unsigned vidas, unsigned puntos, char* nombre){
    printf(
    "                                                      VIDAS: %d                                                         \n"
    "                                                      PUNTOS: %d                             JUGADOR: %s                \n"
    "########################################################################################################################\n",
    vidas, puntos, nombre
    );
}


void gameOver(void){
    printf(
    "                                                                                                                          \n"
    "                                                                                  sSSSSSs                                 \n"
    "                                                                               sSSs                                       \n"
    "             SUCUMBES ANTE LOS                                                 sSS                                        \n"
    "                BANDIDOS...                                                     ssSS     S                                \n"
    "                                                                                  ssSSSSS                                 \n"
    "                                                                                                                          \n"
    "                                               cc                                                                         \n"
    "                               ccCCC     cccccccccc cc                                                                    \n"
    "                                 cCCc  ccccc ccccccccccc                                                                  \n"
    "                                  c cccccccccccccccCCCCc··                                                                \n"
    "                                        cccccccccccccccc                                                                  \n"
    "                                          ccc       cc  cc                                                                \n"
    "                                          c  cc     cc   cc                                                               \n"
    "                                         Xc c       XX   XX                                                               \n"
    );

}


void victoria(void){
    printf(
    "                                                                                                                          \n"
    "                                         sSSSSSs                                                                          \n"
    "                                      sSSsSSSSSSss                                                                        \n"
    "             ESCAPAS CON VIDA         sSSSSSSSSSSSs                                                                       \n"
    "                DE LAS ARENAS...       ssSSSSSSSSSs                                                                       \n"
    "                                         ssSSSSSs                                                                         \n"
    "                                                                                                                          \n"
    "                                               cc                         cccc                   ccccc                    \n"
    "                               ccCCC     cccccccccc cc    ccCCC        ccccccccc    ·cCCC     cccc cccccccc               \n"
    "                    cPP  ∙∙∙∙∙∙  cCCc  ccccc ccccccccccc ··  cCcccc  ccccccccccccc ···ccCcc cccccccc cccc cc              \n"
    "                  b∙ccc∙∙∙        c cccccccccccccccCCCCc··      ccccccc ccc cccc cc     cccccccccc cccccccccc             \n"
    "                  bbOOO                 cccccccccccccccc              cccccccc cccc        ccccccccccccc  ccc             \n"
    "                    OOO                   ccc       cc  cc             ccc      c  c           cc cc     c  cc            \n"
    "                   0XX0                   c  cc     cc   cc          cc  cc    cc  c           c   cc       cc            \n"
    "                  d0  d0                 Xc c       XX   XX          xc  Xcc   Xc  X          Xc   Xc       Xc            \n"
    );

}

void printBandido(void){
        printf(
    "                                                 $$0000000000000000003$$                                                  \n"
    "                                             ####00000000000000000000000####                                              \n"
    "                                           $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$00                                           \n"
    "                                         #########################################0                                       \n"
    "                                        0$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$00            BANDIDOS!!                 \n"
    "                                         ##########################################                                       \n"
    "                                        ## V   44##   ##################   ##4   ##                                       \n"
    "    ..... ..                           # VV    4##  @@@@@@          @@@@@   ##   ##               .  .  .. .              \n"
    " . .         . ...                     # VV V 4###   <·∙≥>{   } <≥∙·>    #####  Q###             ..  .     ..             \n"
    "                    ...                 VV  V 4####      {{   }}       #### #   QQ##   .........             ....         \n"
    "                        .. ..... .  D# VVV V  #AAAAA#    {      }    ###AAAA#    Q  ####      .. .....          ......    \n"
    "                                 DDD##VVV  V ## AAAAA##################AAAAA#    QQ ####  .. ..                      .....\n"
    "                             ##DDDFF##V V V $    4AAAAAAAA AAAAAAAAAAAAAAAA  #$ QQQ #D###                                 \n"
    "                          ####  FF  ###  $VVV $   4AAAAAAAAAAAAAAAA AAAAAA$ #$ QQQQ ####DDD                               \n"
    "                      #####   FFFF  ####  $ $ V $  54 AAAAAAAAAAAA AAAAAA4 #$ QQQQQ########DDDDDDDD                       \n"
    "                  #####    FFFFFFF  #### V $ VV #   4#AAAAAAAAAAAAA AA $  #$ QQQQ  ####   ########DDDDD                   \n"
    "                 ##    FF{FFF FF F F##### $  $VV V#  4############## $  4$ QQQQQ #####  Q   QQ   #########                \n"
    "               # ##  FFFFF  FF F F F #####  $  VVV V    $ #####$$$44  $QQQQQQQ$  ###   Q   QQ Q QQ ########               \n"
    "              ## # FFFF   FFF  FFF F ######  $  $$VVVV##   $$$$ #55  QQ QQQQQ $  ##  Q   QQQQQQQQQQQQQQQQQQQQ             \n"
    "            ###### FF F FFF   FF F F #######VVV $ $VVVVVV###  ### QQQQQQQQQQ $ ####QQ  QQQQQ QQQQQQ  QQQQQQQQ             \n"
    "           #######FFFFFFFF    F  F  F # ##    VV   $VVVVVVV ##   #Q#QQQQQQ $ $ ### Q  QQQQQQQQQQQQQQQ QQQQQQQQQQ          \n"
    );
}


void printTormenta(void){
        printf(
        "           ............     (((((           ....)))))))))                                                              \n"
        "                          ........(((((((   ..  ((....(((((         ))).......))))         )))))                       \n"
        "                    ((((...   ((((      ...   ....       ...))))..............   )).............)))))                  \n"
        "                 (((...     (((.......(((((((.                .))))))        .....)).))))     .......)))               \n"
        "               ((  ..    .(((..    (((    .             ......   ....))    )     ....   )))))).        ))).            \n"
        "              ((    .. .((      .((        ...(((........            ..)...)))   )) ....     .)))        ))..          \n"
        "              ( .......(    ..(((  TORMENTA DE ARENA!!!               .).....)....)))  ...      .))       ))           \n"
        "             ((..   ..(    ..((        .(((                 )       ..))     )     .))).          )).    ..)           \n"
        "             (..    . (    .((        .((       )           )      ))))     ))        ).....       )..    )).          \n"
        "             (..   .  (    .((        .(      ..))    ))   .)   ))))       )) ..      )              ...               \n"
        "               ..  ..        ((.     ..(        ..))))).   ))   ...     )))    .....xxxXXXXXXXXXXXXXXXXX               \n"
        "                ..  ..xxxXXxxxxxxxxxx  (.xxxxxxxxxxxxxxx.))).....   ))))         xxxx                  XXXXXX          \n"
        "               XXXXXXX..        ....X  xxxxxx          xxxxxXXXXXXX           xxxxX                         XXXXXXXXX  \n"
        "        XXXXXXXXX.              X xxxxxxxx           xxxxxxxx  XXXXXXXXXXXXXXxxxxxxxxx xx                           XXX\n"
        "XXXXXXXXXX                 xxXXXX        xxxxxxxx                                   xxxxx                              \n"
        "                         Xxx x               xxxx ...                                         x                        \n"
        "                       XX                        xxxx                                                                  \n"
        "                                                     X x x                                                             \n"
        );
}

void printOasis(void){
        printf(
        "                             ......                                                                                   \n"
        "                          .............                                                                                \n"
        "                         ...... ..........                                        ***                                  \n"
        "                       ..................        ######                              **             ****               \n"
        "                      ............... ..########## ## #######                         *****       ***                  \n"
        "                ###########...... #######     #####         ########                     OO*    **OOO                  \n"
        "           #####  ####    #########     #######                    #####                OOO**  ** OOO                  \n"
        "        #####                          ###                            ###              **********  ***O**              \n"
        " ########    0**    *00              ##                                #########   ***** *********** OO *****          \n"
        "               *  ** 0               ##      OASIS!               ######          **   OOO    *A*******     *          \n"
        "            *******                                            ###                *     OO    AA  OOO***               \n"
        "         0**  0******                                                                       AA    OO  **               \n"
        "         00    * 0AA  **                                                                     A          **     ######  \n"
        "              **   AA  *00        ((((.....................      .........   . ))))))        A                       # \n"
        "              *0    A   0((((((((((....~~~~~~~~~~~~~~~~~~~........~~~~~~~............))))    AA                        \n"
        "              0     AA  ((.....~~~~~~~.~~~~~~~~ ~~......           ...        ~~~~~~.....)))) A                        \n"
        "                     A ((..           ........~~~      ..........  ~~...~~~....          ~.~))AA                       \n"
        "                    QQQ(((~.........       ..  ~~~     ~~~~~~          ....        ~~......))Q AAQQQ                   \n"
        "                   QQQQQ (((((( ~~~~.......      ~~~~~~~     ~~              ~.......   ))))QQQ AA Q                   \n"
        "                               (((((((((((~....................................   ))))))    QQQQQQQQ                   \n"
        );
}

void printVidaExtra(void){
    printf(
    "                                                     @@@@@@@@@@        @@@@@@@@@@                    \n"
    "                                   ####            @@@@        @@@     @@@       @@@  \n"
    "                                 #aaaa#           @@              @@@@@            @  \n"
    "                  ###          #aaa##a#           @               @@@              @@@\n"
    "                  ###         ###aa #a#           @             @@                  @ \n"
    "                  ###               #a#           @           @@                   @@ \n"
    "                  ###              #aa#           @                               @@  \n"
    "           ##################      #a #           @                              @@   \n"
    "           ##################      #a #           @@                            @@    \n"
    "                  ###              #a #            @@@                         @@     \n"
    "                  ###              #a #              @@@@                    @@       \n"
    "                  ###          #####a # ##              @@@@              @@@         \n"
    "                  ###         ############                 @@@         @@@@           \n"
    "                                                             @@     @@@@              \n"
    "                                                              @@@#@@                  \n"
    "       VIDA EXTRA!                                            ###                     \n"
    "                                                             #                        \n"
    );
}

void printPremio(void){
    printf(
    "                     ####################################                                            \n"
    "                 XXXXXXXXXXXXXXXXXXX      XXXXXXXXXXXXXXXXXXXX                                       \n"
    "              XXXX  XXX TTTTTTTTTT X      X TTTTTTTTTTTTXXX  XXX         PUNTO EXTRA!                \n"
    "             XX   XzzzzzzzzzzzzzzzzzX     XzzzzzzzzzzzzzzzXX   XX                                    \n"
    "            XX   XX TTTTTTTTTTTTTT  X     X TTTTTTTTTTTTTT XX   X                                    \n"
    "           XX   XXzzzzzzzzzzzzzzzzzzX     XzzzzzzzzzzzzzzzzzzX  XX                                   \n"
    "           X    X TTTTTTTTTTTTTTTT  X     X TTTTTTTTTTTTTTTT X   XX                 ###       ###    \n"
    "           X    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    X                 ###     ## ##    \n"
    "           XzzzzzzzzzzzzzzzzzzzzzXnUnnXXnnUnXzzzzzzzzzzzzzzzzzzzzzXX             ########      ##    \n"
    "           X                     XnUnXXXXnUnX                     X              ########      ##    \n"
    "           X    XXXXXXXXXXXXXXXX XnUUX  XUUXXXXXXXXXXXXXXXXXXX    X                 ###        ##    \n"
    "           X    X TTTTTTTTTTTTTT XXXUXXXXUXX TTTTTTTTTTTTTTT X    X                 ###        ##    \n"
    "           XX   X  TTTTTTTTTTTTTT  XXXXXXXX  TTTTTTTTTTTTTTT X   XX                          ######  \n"
    "            X   XzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzX   X                                   \n"
    "            XX  XX TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT XX  XX                                   \n"
    "             X   XzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzX   X                                    \n"
    "             XXXXXXXTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT XXXXX                                    \n"
    "              X   UXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXU  UX                                    \n"
    "              XU   UUXX                                 XXX   UXX                                    \n"
    "              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                                     \n"
    );
}


void mostrarFooter(void){
        printf(
        "########################################################################################################################\n"
        "\n"
        "                                                     GRUPO 1                                                            \n"
        "\n"
        );
}

void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int mostrar_movimientos(t_movimientos *cola, const char* nombre_jugador) {
    t_movimiento mov;
    int total = 0;
    int col   = 0;

    printf("\n=== Movimientos de %s ===\n", nombre_jugador);

    while (desacolar(cola, &mov, sizeof(t_movimiento)) == OK) {
        if (mov.jugador_humano) {
            int delta = (int)mov.pos_final - (int)mov.pos_inicial;
            char tipo  = (delta >= 0) ? 'F' : 'B';
            int  pasos = (delta >= 0) ? delta : -delta;
            printf("%c%d ", tipo, pasos);
            total++;
            col++;
            if (col % 15 == 0) printf("\n");
        }
    }
    printf("\nTotal: %d movimientos\n", total);
    return total;
}
unsigned buscar_id(t_reg_indice* punt)
{
    t_reg_jugador reg;
    // Inicializamos por seguridad en caso de que falle la lectura
    reg.id = 0;

    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");

    // 1. Validar que el archivo se abrió con éxito
    if (f == NULL) {
        printf("Error: No se pudo abrir el archivo de jugadores.\n");
        return 0;
    }

    // 2. Posicionarse y leer
    fseek(f, punt->posicion, SEEK_SET);

    // Validar si realmente se pudo leer el registro completo
    if (fread(&reg, sizeof(t_reg_jugador), 1, f) != 1) {
        printf("Error: No se pudo leer el registro en la posicion %ld\n", punt->posicion);
    }

    // 3. ¡SIEMPRE cerrar el archivo!
    fclose(f);

    printf("%u dentro de funcion\n", reg.id);
    return reg.id;
}

unsigned jugadores_proximo_id(void) {
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");
    unsigned cant = 0;
    fseek(f, 0, SEEK_END);
    cant = ftell(f) / sizeof(t_reg_jugador);
    fclose(f);
    return cant + 1;
}
int jugadores_agregar(unsigned id, const char *nombre) {
    t_reg_jugador reg;
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "ab");
    if (!f) return 0;
    reg.id = id;
    strncpy(reg.nombre, nombre, MAX_NOMBRE - 1);
    *(reg.nombre + MAX_NOMBRE - 1) = '\0';
    fwrite(&reg, sizeof(t_reg_jugador), 1, f);
    fclose(f);
    return 1;
}
int jugadores_buscar_nombre(unsigned id, char *nombre_dest) {
    t_reg_jugador reg;
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");
    if (!f) return 0;
    while (fread(&reg, sizeof(t_reg_jugador), 1, f) == 1) {
        if (reg.id == id) {
            strncpy(nombre_dest, reg.nombre, MAX_NOMBRE - 1);
            *(nombre_dest + MAX_NOMBRE - 1) = '\0';
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
int indice_comparar(const void *a, const void *b) {
    const t_reg_indice *ra = (const t_reg_indice*)a;
    const t_reg_indice *rb = (const t_reg_indice*)b;
    return strcmp(ra->nombre, rb->nombre);
}
int indice_buscar(t_raking *arbol, const char *nombre, t_reg_indice *dest) {
    t_reg_indice busq;
    strncpy(busq.nombre, nombre, MAX_NOMBRE - 1);
    *(busq.nombre + MAX_NOMBRE - 1) = '\0';
    busq.posicion = 0;
    if (!buscarEnArbol(arbol, &busq, sizeof(t_reg_indice), indice_comparar))
        return 0;
    *dest = busq;
    return 1;
}
void indice_insertar(t_raking *arbol, t_reg_indice *reg) {
    ponerEnArbol(arbol, reg, sizeof(t_reg_indice), indice_comparar);
}
void indice_guardar(t_raking *arbol) {
    FILE *f = abrir_bin(ARCHIVO_INDICE, "wb");
    if (!f) { perror("indice_guardar"); return; }///modificar fprintf (stderr,"error");
    guardarArbolBin(arbol, f, sizeof(t_reg_indice));
    fclose(f);
}

void indice_cargar(t_raking *arbol) {
    FILE *f = abrir_bin(ARCHIVO_INDICE, "rb");
    if (!f) return;
    cargarArbolBin(arbol, f, sizeof(t_reg_indice), indice_comparar);
    fclose(f);
}
int partidas_agregar(unsigned id_jugador, unsigned puntos, unsigned cant_movimientos) {
    t_reg_partida reg;
    FILE *f = abrir_bin(ARCHIVO_PARTIDAS, "ab");
    if (!f) return 0;
    reg.id_jugador       = id_jugador;
    reg.puntos           = puntos;
    reg.cant_movimientos = cant_movimientos;
    fwrite(&reg, sizeof(t_reg_partida), 1, f);
    fclose(f);
    return 1;
}
void ranking_mostrar(void) {
    FILE *f;
    t_reg_partida reg;
    t_acum acum[512];
    int cant_acum = 0;
    int i, j, k;
    t_acum top[TOP_RANKING];
    int cant_top;
    char nombre[MAX_NOMBRE];

    f = abrir_bin(ARCHIVO_PARTIDAS, "rb");
    if (!f) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    while (fread(&reg, sizeof(t_reg_partida), 1, f) == 1) {
        int encontrado = 0;
        for (i = 0; i < cant_acum; i++) {
            if ((acum + i)->id == reg.id_jugador) {
                (acum + i)->puntos_totales += reg.puntos;
                (acum + i)->cant_movimientos_t += reg.cant_movimientos;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado && cant_acum < 512) {
            (acum + cant_acum)->id             = reg.id_jugador;
            (acum + cant_acum)->puntos_totales = reg.puntos;
            (acum + cant_acum)->cant_movimientos_t=reg.cant_movimientos;
            cant_acum++;
        }
    }
    fclose(f);

    if (cant_acum == 0) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    cant_top = (cant_acum < TOP_RANKING) ? cant_acum : TOP_RANKING;
    for (i = 0; i < cant_top; i++) {
        int idx_max = 0;
        for (j = 1; j < cant_acum; j++) {
            if ((acum + j)->puntos_totales > (acum + idx_max)->puntos_totales)
                idx_max = j;
        }
        *(top + i) = *(acum + idx_max);
        (acum + idx_max)->puntos_totales = 0;
        (acum + idx_max)->id = 0;
        (acum + idx_max)->cant_movimientos_t=0;
    }

    printf("\n  %-4s %-20s %8s %8s\n", "Pos.", "Jugador", "Puntos", "T_Movs");
    printf("  %-4s %-20s %8s  %8s\n", "----", "--------------------", "--------" , "--------");
    for (k = 0; k < cant_top; k++) {
        if (jugadores_buscar_nombre((top + k)->id, nombre))
            printf("  %-4d %-20s %8u %8u\n", k + 1, nombre, (top + k)->puntos_totales,(top + k)->cant_movimientos_t);
        else
            printf("  %-4d %-20s %8u\n", k + 1, "???", (top + k)->puntos_totales);
    }
    printf("\n");
}
