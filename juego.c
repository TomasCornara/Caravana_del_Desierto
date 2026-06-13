#include "juego.h"
#include "manejo_archivos.h"
#include "dado.h"
#include "lista_circular_doble.h"
#include "indice_jugadores.h"

void jugar_partida(t_mapa *mapa, t_config *config)
{
    #if DEBUG_JUGADOR
        unsigned numero_turno = 0;
    #endif // DEBUG_JUGADOR
    t_arbol arbol_indice;
    int cant_movs=0;
    t_indice reg_idx;
    unsigned id_jugador;
    t_movimientos cola_movimientos_jugador;
    t_movimientos cola_turno;
    t_movimiento movi;
    t_jugador jugador;
    t_dado dado;
    bool caso_oasis = false;
    char direccion;
    unsigned cantidad_casilleros,
            posicion_salida;
    FILE *archivo_caravana;

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
    archivo_caravana = abrir_txt(ARCHIVO_MAPA, "w");
    if (archivo_caravana)
    {
        printCaravana(archivo_caravana, mapa);
        fclose(archivo_caravana);
    }


    while ((jugador.vidas > 0) && (jugador.pos_en_mapa != posicion_salida))
    {
        limpiar_pantalla();
        #if DEBUG_JUGADOR
            printf("DEBUG - Numero de turno: %d\n",numero_turno);
            numero_turno++;
        #endif // DEBUG_JUGADOR

        mostrarEstadisticas(&jugador);
        printCaravana(stdout, mapa);
        /*
        if (jugador.efectoTormenta)
        {
            jugador.efectoTormenta = false;
            printf("La tormenta te hace perder este turno.\n");
            mostrarFooter();
            pausa();

        }
        else
        {
        */
            unsigned pos_final_j;
            direccion = AVANZAR; //Por defecto se avanza

            tirarDado(&dado);
            printf("DADO - Sacaste un: %d\n",dado.cara);
            mostrarFooter();

            #if DEBUG_JUGADOR
                printf("DEBUG - Estado jugador:\n jugador.pos_en_mapa: %d\n jugador.efecto_tormenta: %s\n jugador.efecto_oasis: %s\n",
                    jugador.pos_en_mapa, jugador.efectoTormenta ? "VERDADERO" : "FALSO", jugador.efectoOasis ? "VERDADERO" : "FALSO");
                printf("DEBUG - Sobreescribir valor del dado. Ingrese un valor y presione ENTER: ");
                scanf("%d", &dado.cara);
                limpiar_buffer();
            #endif // DEBUG_JUGADOR

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

            pausa();

            pos_final_j = calcular_pos_final_del_jugador(jugador.pos_en_mapa,
                                                         cantidad_casilleros,
                                                         dado.cara,direccion);
            if(jugador.efectoTormenta == false){
                guardar_movimiento(&cola_movimientos_jugador,
                                   jugador.pos_en_mapa,
                                   pos_final_j,
                                   direccion,
                                   dado.cara,true);
            }else{

               printf("\nLA TORMENTA TE HACE PERDER EL TURNO");
               pausa();
            }

            guardar_movimiento(&cola_turno,
                               jugador.pos_en_mapa,
                               pos_final_j,
                               direccion,
                               dado.cara,true);

            calcular_bandidos(mapa,&cola_turno);

            while(!colaVacia(&cola_turno)){
                desacolar(&cola_turno,&movi,sizeof(t_movimiento));

                #if DEBUG_BANDIDOS
                    printf("\n\nCANTIDAD MOVIMIENTO: %u", movi.cantidad_movimiento);
                    printf("\nORIENTACION: %c", movi.orientacion);
                    printf("\nPOS INICIAL: %u", movi.pos_inicial);
                    printf("\nPOS FINAL: %u", movi.pos_final);
                #endif // DEBUG_BANDIDOS

                if(movi.jugador_humano){
                    if(jugador.efectoTormenta == false){
                        mover_jugador(mapa,&jugador,&movi);
                    }else{
                       jugador.efectoTormenta = false;
                    }
                    //mover_jugador(mapa,&jugador,&movi);
                    //resolver_casillero_actual(mapa,&jugador);
                }else{
                    //t_casillero *casillero_jugador;
                    mover_bandido(mapa,&movi);

                    //Le paso donde esta el jugador para que pueda chequear si le puede hacer daño
                    //casillero_jugador = obtener_de_lista_dir_dato(mapa, &(jugador.pos_en_mapa), comparar_clave_casillero);
                    //resolver_bandido_en_casillero(mapa, &jugador, casillero_jugador);
                }
                resolver_casillero_actual(mapa,&jugador);
            }

            if( jugador.efectoOasis && caso_oasis == false ){
                caso_oasis = true;
            }else{
                if(caso_oasis == true){
                    jugador.efectoOasis = false;
                    caso_oasis = false;
                }
            }

            #if DEBUG_JUGADOR
                limpiar_pantalla();
                printf("DEBUG - Direccion calculada/elegida: %c\n", direccion);
                printf("DEBUG - Posición%s calculada para el jugador: %d\n",(jugador.pos_en_mapa == pos_final_j)? " final":"", pos_final_j);
                if(jugador.pos_en_mapa != pos_final_j){
                    printf("DEBUG - Jugador atacado por una bandido. Retrocede al inicio. Posicion final: %d",jugador.pos_en_mapa);
                }
                pausa();
            #endif // DEBUG_UUGADOR
            limpiar_pantalla();
        //}
    }

    ///Fin del juego
    limpiar_pantalla();
    jugador.vidas?  victoria() : gameOver();


    printf("Puntos: %d\n",jugador.puntos);
    #if DEBUG_JUGADOR
        printf("DEBUG - Ingrese cuantos puntos quiere tener: ");
        scanf("%d", &jugador.puntos);
    #endif // DEBUG_JUGADOR
    //bajarindice(&arbol_indice,"indice.idx")
    cant_movs = resultado_partida(&cola_movimientos_jugador);
    pausa();
    crearArbol(&arbol_indice);
    file_a_arbolIndice(& arbol_indice,ARCHIVO_IDX);
    ///SI LO ENCUENTRA NOS DEVUELVE 1 SINO 0
    if (indice_buscar(&arbol_indice, jugador.nombre, &reg_idx)) {
        id_jugador=buscar_id(&reg_idx);
        //printf("%u",id_jugador);
    } else {
        FILE *arch;
        id_jugador        = jugadores_proximo_id();
        jugadores_agregar(id_jugador, jugador.nombre);
        arch=fopen(ARCHIVO_JUGADORES,"rb");///reemplazar
        if(!arch)return;
        crear_indice(arch);
    }

    partidas_agregar(id_jugador, jugador.puntos, cant_movs);
    destruirArbol(&arbol_indice);
    //Limpiar estructuras
    //Limpiar estructuras
    vaciarCola(&cola_movimientos_jugador);
    vaciarCola(&cola_turno);
    destruirDado(&dado);

    return;
}

int resultado_partida(t_movimientos * cola ){
   t_movimiento movimiento_desacolado;
   unsigned acumulador_avanzar = 0;
   unsigned acumulador_retroceder = 0;

   while( !colaVacia(cola) ){
      desacolar(cola,&movimiento_desacolado,sizeof(t_movimiento));
      if(movimiento_desacolado.orientacion == AVANZAR){
         acumulador_avanzar += movimiento_desacolado.cantidad_movimiento;
      }else{
         acumulador_retroceder += movimiento_desacolado.cantidad_movimiento;
      }
   }
   printf("\n-----------------MOVIMIENTOS JUGADOR----------------------\n");

   printf("F: %u      B: %u", acumulador_avanzar, acumulador_retroceder);

   printf("\n----------------------------------------------------------\n");

   return acumulador_avanzar + acumulador_retroceder;
}

unsigned calcular_pos_final_del_jugador(unsigned pos_inicial_del_jugador,
                                   unsigned cantidad_nodos_lista,
                                   unsigned cantidad_pasos,
                                   char direccion){
    unsigned pos;

    if(!cantidad_nodos_lista ||
       pos_inicial_del_jugador > cantidad_nodos_lista ||
       (direccion != AVANZAR && direccion != RETROCEDER))
    {
        return cantidad_nodos_lista;
    }

    pos = pos_inicial_del_jugador;

    while(cantidad_pasos > 0)
    {
        if(direccion == AVANZAR)
        {
            if(pos == cantidad_nodos_lista)
            {
                direccion = RETROCEDER;
                pos--;
            }
            else
            {
                pos++;
            }
        }
        else
        {
            if(pos == 0)
            {
                direccion = AVANZAR;
                pos++;
            }
            else
            {
                pos--;
            }
        }
        cantidad_pasos--;
    }

    return pos;
}

void calcular_bandidos(t_mapa * mapa, t_movimientos * cola)
{
    map_lista(mapa, situar_bandidos, (void*)cola);
}

void situar_bandidos(void* a, void* parametro_extra)
{
    t_dado dado;
    t_casillero casillero_actual = *(t_casillero*)a;
    t_movimiento movimiento_bandido;
    unsigned signo,cantidad_movimiento;

    if(casillero_actual.cant_bandidos > 0){
       crearDado(&dado,CARAS_DADO);

       signo = tirarDado(&dado);
       #if DEBUG_BANDIDOS_LENTOS
            cantidad_movimiento = 1;
       #else
            cantidad_movimiento = tirarDado(&dado);
       #endif

       movimiento_bandido.orientacion = (signo%2 == 0)? AVANZAR : RETROCEDER;
       movimiento_bandido.cantidad_movimiento = cantidad_movimiento;
       movimiento_bandido.pos_inicial = casillero_actual.nro_posicion;
       movimiento_bandido.pos_final = 0; //en este punto no se calculo la posicion final del bandido
       movimiento_bandido.jugador_humano = false; //porque es un movimiento de un bandido

       acolar((t_cola*)parametro_extra,&movimiento_bandido,sizeof(t_movimiento));
    }
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

int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final,char orientacion,unsigned cant_movimiento,bool jugador_humano)
{
    t_movimiento mov;
    mov.pos_inicial = pos_inicial;
    mov.pos_final = pos_final;
    mov.jugador_humano = jugador_humano;
    mov.orientacion = orientacion;
    mov.cantidad_movimiento = cant_movimiento;
    return acolar(cola, &mov, sizeof(t_movimiento));
}

void mover_jugador(t_mapa*mapa,t_jugador *jugador,t_movimiento* movimiento)
{
  modificar_elemento_segun_clave(mapa, &(jugador->pos_en_mapa),
                                 comparar_posicion_del_jugador_en_mapa,
                                 quitar_jugador, NULL);

  jugador->pos_en_mapa = movimiento->pos_final;

  modificar_elemento_segun_clave(mapa, &(jugador->pos_en_mapa),
                                 comparar_posicion_del_jugador_en_mapa,
                                 ponerlo_jugador, NULL);

}

//comparar la posicion en mapa, o si jugador = true;
 int comparar_posicion_del_jugador_en_mapa(const void * a,const void * b){
   unsigned posicion;
   t_casillero casillero;

   posicion = *(unsigned*)a;
   casillero = *(t_casillero*)b;
   return posicion - casillero.nro_posicion;
 }

void quitar_jugador(void* a,void* parametro_extra){
    t_casillero* casillero;

    casillero = (t_casillero*)a;
    if(casillero->presencia_jugador){
        casillero->presencia_jugador = false;
    }

    return;
}

void ponerlo_jugador(void* a,void* parametro_extra){
    t_casillero* casillero;

    casillero = (t_casillero*)a;
    casillero->presencia_jugador = true;
    return;
}

void resolver_casillero_actual(t_mapa * mapa,t_jugador *jugador)
{
    bool caso_oasis = false;
    t_casillero* casillero_actual;

    casillero_actual = obtener_de_lista_dir_dato(mapa,&(jugador->pos_en_mapa),comparar_clave_casillero);
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
    /*
    else if (casillero_actual->tipo_casillero != TIPO_TORMENTA && jugador->efectoOasis)
    {
        jugador->efectoOasis = false;
        limpiar_pantalla();
        printf("Pierdes el efecto Oasis.\n");
        pausa();
    }
    */
    if (casillero_actual->animacion)
    {
        limpiar_pantalla();
        casillero_actual->animacion();//aca exploto

        if(caso_oasis)
        {
            caso_oasis = false;
            printf("El Oasis te protege\n");
            jugador->efectoOasis = false;
        }

        pausa();
    }

    //Cuando el jugador cae en un casillero, lo normnaliza
    if(casillero_actual->tipo_casillero != TIPO_INICIO && casillero_actual->tipo_casillero != TIPO_FIN)
    {
        casillero_actual->tipo_casillero = TIPO_NORMAL;
        casillero_actual->animacion = NULL;
    }

    resolver_bandido_en_casillero(mapa,jugador, casillero_actual);
}

void resolver_bandido_en_casillero(t_mapa*mapa,t_jugador *jugador, t_casillero *casillero_actual)
{
    if (!jugador || !casillero_actual) return;

    //No hay bandidos
    if(!casillero_actual->cant_bandidos) return;


    limpiar_pantalla();
    printBandido();
    pausa();

    if (jugador->vidas > 0 && jugador->efectoOasis != true)
    {
        limpiar_pantalla();
        t_movimiento movimiento_jugador;
        jugador->vidas--;
        limpiar_pantalla();
        printf("El bandido te quita 1 vida.\n");
        movimiento_jugador.pos_inicial = jugador->pos_en_mapa;
        movimiento_jugador.pos_final = 0;
        movimiento_jugador.cantidad_movimiento = 0;
        mover_jugador(mapa,jugador,&movimiento_jugador);
    }else{
        printf("El Oasis te protege del bandido\n");
        //jugador->efectoOasis = false;
        limpiar_buffer();
    }
    casillero_actual->cant_bandidos--; //Elimina al bandido que ataco
    pausa();
}

void mover_bandido(t_mapa *mapa, t_movimiento* movimiento_bandido)
{
    t_casillero *bandido_pos_inicial;
    int cantidad_nodo,
        pos_final;


    cantidad_nodo = cantidad_elementos_lista(mapa);
    pos_final = movimiento_bandido->cantidad_movimiento + movimiento_bandido->pos_inicial;

    if(pos_final == cantidad_nodo){
        pos_final++;
    }

    pos_final = pos_final%cantidad_nodo;

    if(pos_final == 0){
        pos_final++;
    }

    //Compruebo que haya un bandido vivo que mover
    bandido_pos_inicial = obtener_de_lista_dir_dato(mapa, &(movimiento_bandido->pos_inicial), comparar_clave_casillero);
    if (bandido_pos_inicial && bandido_pos_inicial->cant_bandidos > 0) {
        modificar_elemento_segun_clave(mapa, &(movimiento_bandido->pos_inicial),
                                     comparar_clave_casillero,
                                     quitar_bandido, NULL);

        modificar_elemento_segun_clave(mapa, &pos_final,
                                     comparar_clave_casillero,
                                     poner_bandido, NULL);
    }

}

void poner_bandido(void* a, void* parametro_extra){
    t_casillero *casillero;

    casillero = (t_casillero*)a;
    casillero->cant_bandidos++;
    return;
}

void quitar_bandido(void *a, void* parametro_extra){
    t_casillero *casillero;

    casillero = (t_casillero*)a;

    if(casillero->cant_bandidos){
       casillero->cant_bandidos--;
    }

    return;
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
    nuevo_casillero.animacion = NULL;

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
    FILE *f;
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

    f = abrir_txt(ARCHIVO_CONFIG, "r");
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
    int total_especiales;

    total_especiales = config->maximo_oasis + config->maximo_tormentas +
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

char inicialTipoCasillero(unsigned tipo)
{
    switch(tipo)
    {
        case TIPO_INICIO:     return 'I';
        case TIPO_FIN:        return 'F';
        case TIPO_OASIS:      return 'O';
        case TIPO_TORMENTA:   return 'T';
        case TIPO_VIDA_EXTRA: return 'V';
        case TIPO_PREMIO:     return 'P';
        default:              return '.';
    }
}

void printCasillero(void* casillero, void* file){
    FILE* arch = (FILE*)file;
    t_casillero* cas = (t_casillero*)casillero;
    fprintf(arch,"[%02d|%c|%c|B:%d]\n",
            cas->nro_posicion,
            inicialTipoCasillero(cas->tipo_casillero),
            (cas->presencia_jugador)? 'J':'.',
            cas->cant_bandidos);
}

void printCaravana(FILE* archivo, t_mapa* mapa){
    map_lista(mapa,printCasillero,archivo);
}

///MI PRINT CARAVANA
/*
void printCaravana(FILE *archivo, t_mapa *mapa)
{
    t_casillero *cas;
    unsigned pos;
    int contador_casilleros;
    int cantidad_casilleros;

    //t_casillero casillero_actual;

    if (!archivo || !mapa || !*mapa) return;

    contador_casilleros = 0;
    cantidad_casilleros = cantidad_elementos_lista(mapa);
    printf("\n");
    while (contador_casilleros <= cantidad_casilleros )
    {
        int num_elems = 0;
        char elems[20][3];
        //obtener_de_lista(mapa,&contador_casilleros,&casillero_actual,sizeof(t_casillero),comparar_clave_casillero);

        cas = obtener_de_lista_dir_dato(mapa,&contador_casilleros,comparar_clave_casillero);
        pos = cas->nro_posicion;

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
}*/

///FUNCIONES DE COMPARACION

int comparar_posicion_casilleros(const void* elem_a,
                                 const void* elem_b)
{
    const t_casillero* a = elem_a;
    const t_casillero* b = elem_b;

    if(a->nro_posicion < b->nro_posicion)
        return -1;

    if(a->nro_posicion > b->nro_posicion)
        return 1;

    return 0;
}

int comparar_clave_casillero(const void* elem_a,const void* elem_b){

    unsigned clave;
    t_casillero casillero_b;

    clave = *(unsigned*)elem_a;
    casillero_b = *(t_casillero*)elem_b;

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
    int letras_guardadas;
    int letras_totales;
    int c;
    char buffer[MAX_NOMBRE];

    do {
        printf("Ingrese su nombre (3 letras): ");

        letras_guardadas = 0;
        letras_totales = 0;

        while ((c = getchar()) != '\n' && c != EOF) {
            if (isalpha((unsigned char)c)) {
                letras_totales++;
                if (letras_guardadas < 3) {
                    buffer[letras_guardadas++] = (char)toupper((unsigned char)c);
                }
            }
        }
        buffer[letras_guardadas] = '\0';

        if (letras_totales != 3) {
            printf("\nNombre invalido: Los nombres deben tener solo 3 letras.\n");
        }

    } while (letras_totales != 3);

    strcpy(nombre, buffer);
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

void mostrarEstadisticas(t_jugador * jugador){

    char efecto_tormenta[12];
    char efecto_oasis[12];

    if(jugador->efectoTormenta){
        strcpy(efecto_tormenta,"ACTIVO");
    }else{
        strcpy(efecto_tormenta,"DESACTIVADO");
    }
    if(jugador->efectoOasis){
        strcpy(efecto_oasis,"ACTIVO");
    }else{
        strcpy(efecto_oasis,"DESACTIVADO");
    }

    printf(
    "                                                      VIDAS: %d                                                         \n"
    "   EFECTO TORMENTA: %s      EFECTO OASIS: %s           PUNTOS: %d                           JUGADOR: %s                \n"
    "#########################################################################################################################\n",
    jugador->vidas,efecto_tormenta,efecto_oasis, jugador->puntos,jugador->nombre
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

    while (desacolar(cola, &mov, sizeof(t_movimiento))) {
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
