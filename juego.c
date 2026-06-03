#include "juego.h"
#include "manejo_archivos.h"
#include "dado.h"
#include "consola.h"

void jugar_partida(t_mapa *mapa, t_config *config){
    t_movimientos cola_movimientos;
    t_jugador jugador;
    t_dado dado;
    t_casillero *casillero_actual;
    char c_lado;
    int lado = 1;

    limpiar_pantalla();


    //Inicializacion
    crearCola(&cola_movimientos);
    crearDado(&dado, CARAS_DADO);
    inicializar_jugador(&jugador, config, *mapa);
    pedir_nombre(jugador.nombre); //INICIALIZAR NOMBRE DEL JUGADOR

    limpiar_buffer();

    // Generacion caravana.txt
    FILE *archivo_caravana = abrir_txt(ARCHIVO_MAPA, "w");
    if (archivo_caravana) {
        printCaravana(archivo_caravana, mapa);
        fclose(archivo_caravana);
    }

    ///Juego
    while (jugador.vidas && ((t_casillero*)jugador.pos->info)->tipo_casillero != TIPO_FIN) {
        limpiar_pantalla();
        mostrarEstadisticas(jugador.vidas, jugador.puntos, jugador.nombre);
        printCaravana(stdout, mapa);

        if (jugador.efectoTormenta) {
            jugador.efectoTormenta = false;
            printf("La tormenta te hace perder este turno.\n");
            mostrarFooter();
            pausa();

        } else {
            tirarDado(&dado);
            printf("DADO - Sacaste un: %d\n",dado.cara);
            mostrarFooter();
            c_lado = 'A';
            lado = 1;

            // PARA DEBUGGEAR
            printf("Ingresa un valor para debuggear el dado y presiona ENTER: ");
            scanf("%d", &dado.cara);

            // (((t_casillero*)jugador.pos->info)->nro_posicion) = numero de casillero actual del jugador
            if(dado.cara < (((t_casillero*)jugador.pos->info)->nro_posicion) +1){
                do{
                    printf("\nIngresa 'A' para avanzar y 'R' para retroceder y presiona ENTER: ");
                    limpiar_buffer();
                    scanf("%c", &c_lado);

                    if(c_lado != 'A' && c_lado != 'R')
                            printf("\nIngrese un término válido.");
                } while(c_lado != 'A' && c_lado != 'R');
            }

            if(c_lado == 'R')
                lado *= -1;

            limpiar_buffer();
            pausa();

            mover_jugador(&jugador, dado.cara, &cola_movimientos, lado);

            casillero_actual = (t_casillero*)jugador.pos->info;

            //Aplica los los buffeos y debuffos + bandidos
            resolver_casillero_actual(&jugador, casillero_actual, &cola_movimientos);
            limpiar_pantalla();
        }

        ///Logica del bandido
        mover_bandido(mapa, &cola_movimientos);
    }

    ///Fin del juego
    limpiar_pantalla();
    if(jugador.vidas){
       victoria();
    } else {
        gameOver();
    }

    printf("Puntos: %d\n",jugador.puntos);

    ///ACA FALTA BAJAR LOS RESULTADOS AL INDICE Y LOS MOVIMIENTOS A UN ARCHIVO

    //Limpiar estructuras
    vaciarCola(&cola_movimientos);
    destruirDado(&dado);

    return;
}

void inicializar_jugador(t_jugador *jugador, t_config *config, t_mapa mapa) {
    tNodo *inicio;

    strcpy(jugador->nombre, "");
    jugador->vidas = config->vidas_inicio;
    jugador->puntos = 0;
    jugador->efectoTormenta = false;
    jugador->efectoOasis = false;
    inicio = mapa;
    while (inicio != NULL && inicio->ant != NULL) {
        inicio = inicio->ant;
    }
    jugador->pos = inicio;
}

int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final, bool jugador_humano) {
    t_movimiento mov;
    mov.pos_inicial = pos_inicial;
    mov.pos_final = pos_final;
    mov.jugador_humano = jugador_humano;
    return acolar(cola, &mov, sizeof(t_movimiento));
}

void mover_jugador(t_jugador *jugador, unsigned pasos, t_movimientos *cola_movimientos, int lado) {
    tNodo *actual;
    unsigned pos_inicial;
    unsigned pos_final;

    if (!jugador || !jugador->pos) {
        return;
    }

    actual = jugador->pos;
    pos_inicial = ((t_casillero *)actual->info)->nro_posicion;

    while (pasos != 0) {
        if (actual->sig == NULL)
            lado *= -1;

        actual = ( lado > 0 ) ? actual->sig : actual->ant;
        pasos--;
    }

    ((t_casillero *)jugador->pos->info)->presencia_jugador = false;
    ((t_casillero *)actual->info)->presencia_jugador = true;
    jugador->pos = actual;

    pos_final = ((t_casillero *)actual->info)->nro_posicion;
    guardar_movimiento(cola_movimientos, pos_inicial, pos_final, true);
}

void resolver_casillero_actual(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos) {
    bool caso_oasis = false;

    if (!jugador || !casillero_actual) {
        return;
    }

    if (casillero_actual->tipo_casillero == TIPO_OASIS) {
        jugador->efectoOasis = true;
    } else if (casillero_actual->tipo_casillero == TIPO_TORMENTA){
        if(jugador->efectoOasis){
            caso_oasis = true;

        } else {
            jugador->efectoTormenta = true;
        }
    } else if (casillero_actual->tipo_casillero == TIPO_PREMIO) {
        jugador->puntos++;
    } else if (casillero_actual->tipo_casillero == TIPO_VIDA_EXTRA) {
        jugador->vidas++;
    } else if (casillero_actual->tipo_casillero != TIPO_TORMENTA && jugador->efectoOasis){
        jugador->efectoOasis = false;
        limpiar_pantalla();
        printf("Pierdes el efecto Oasis.\n");
        pausa();
        limpiar_buffer();
    }

    if (casillero_actual->animacion) {
        limpiar_pantalla();
        casillero_actual->animacion();

        if(caso_oasis){
            caso_oasis = false;
            printf("El Oasis te protege\n");
            jugador->efectoOasis = false;
            limpiar_buffer();
        }

        pausa();
    }

    //Cuando el jugador cae en un casillero, lo normnaliza
    if(casillero_actual->tipo_casillero != TIPO_INICIO && casillero_actual->tipo_casillero != TIPO_FIN){
        casillero_actual->tipo_casillero = TIPO_NORMAL;
        casillero_actual->animacion = NULL;
    }

    resolver_bandido_en_casillero(jugador, casillero_actual, cola_movimientos);
}

void resolver_bandido_en_casillero(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos) {
    if (!jugador || !casillero_actual) return;

    //No hay bandidos
    if(!casillero_actual->cant_bandidos) return;

    limpiar_pantalla();
    printBandido();
    pausa();

    if (jugador->vidas > 0) {
        jugador->vidas--;
        limpiar_pantalla();
        printf("El bandido te quita 1 vida.\n");
        mover_jugador(jugador, ((t_casillero*)jugador->pos->info)->nro_posicion, cola_movimientos, -1);
    }

    casillero_actual->cant_bandidos--; //Elimina al bandido que ataco
    pausa();
}

void mover_bandido(t_mapa *mapa, t_movimientos *cola_movimientos) { ///REVISAR ESTA FUNCION, ESTA BASTANTE FEA
    tNodo *inicio;
    tNodo *temp;
    tNodo **nodos;
    unsigned *cantidades;
    unsigned cantidad_nodos;
    unsigned i;

    if (!mapa || !*mapa) {
        return;
    }

    inicio = *mapa;
    while (inicio->ant != NULL) {
        inicio = inicio->ant;
    }

    cantidad_nodos = 0;
    temp = inicio;
    while (temp != NULL) {
        cantidad_nodos++;
        temp = temp->sig;
    }

    if (cantidad_nodos < 2) {
        return;
    }

    nodos = malloc(cantidad_nodos * sizeof(tNodo *));
    cantidades = malloc(cantidad_nodos * sizeof(unsigned));
    if (!nodos || !cantidades) {
        free(nodos);
        free(cantidades);
        return;
    }

    temp = inicio;
    for (i = 0; i < cantidad_nodos; i++) {
        nodos[i] = temp;
        cantidades[i] = ((t_casillero *)temp->info)->cant_bandidos;
        temp = temp->sig;
    }

    for (i = 0; i < cantidad_nodos; i++) {
        unsigned bandido;
        unsigned destino = (i + 1) % cantidad_nodos;
        t_casillero *cas_origen = (t_casillero *)nodos[i]->info;
        t_casillero *cas_destino = (t_casillero *)nodos[destino]->info;

        for (bandido = 0; bandido < cantidades[i]; bandido++) {
            guardar_movimiento(cola_movimientos, cas_origen->nro_posicion, cas_destino->nro_posicion, false);
        }

        cas_origen->cant_bandidos -= cantidades[i];
        cas_destino->cant_bandidos += cantidades[i];
    }

    free(nodos);
    free(cantidades);
}

void poner_bandidos_random(t_casillero *temp, int total, int cantidad) {
    int puestos = 0;
    while (puestos < cantidad) {
        t_casillero *cas = temp + 1 + rand() % (total - 2);

        if (cas->tipo_casillero!= TIPO_INICIO &&
            cas->tipo_casillero!= TIPO_FIN &&
            cas->cant_bandidos == 0) {

            cas->cant_bandidos = 1;
            puestos++;
        }
    }
}

void poner_tipo_random(t_casillero *temp, int total, unsigned tipo, int cantidad) {
    int puestos = 0;
    while (puestos < cantidad) {
        t_casillero *cas = temp + 1 + rand() % (total - 2);

        if (cas->tipo_casillero == TIPO_NORMAL) {
            cas->tipo_casillero = tipo;

            // Asignar función de animación según tipo
            switch(tipo) {
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

void poner_casilleros_especiales(t_casillero *temp, int total, t_config *config) {
    poner_tipo_random(temp, total, TIPO_OASIS, config->maximo_oasis);
    poner_tipo_random(temp, total, TIPO_TORMENTA, config->maximo_tormentas);
    poner_tipo_random(temp, total, TIPO_PREMIO, config->maximo_premios);
    poner_tipo_random(temp, total, TIPO_VIDA_EXTRA, config->maximo_vidas_extra);
}

int juego_generar_mapa(t_config *config, t_mapa *mapa) {
    t_casillero *p,
                *fin;
    unsigned cant;
    t_casillero *temp;

    cant = config->cantidad_posiciones;
    temp = calloc(cant, sizeof(t_casillero));
    crear_lista(mapa);

    if (!temp) return 0;

    fin = temp + cant;
    // Inicializar casilleros como normales y sin bandidos
    for (p = temp; p < fin; p++) {
        p->nro_posicion = p - temp;
        p->tipo_casillero = TIPO_NORMAL;
        p->presencia_jugador = false;
        p->cant_bandidos = 0;
    }

    // Colocar casilleros especiales y bandidos
    temp->tipo_casillero = TIPO_INICIO;
    temp->presencia_jugador = true;
    (temp + cant - 1)->tipo_casillero = TIPO_FIN;

    poner_casilleros_especiales(temp, cant, config);
    poner_bandidos_random(temp, cant, config->maximo_bandidos);

    // Pasar el arreglo a la lista
    for (p = temp; p < fin; p++) {
        if (agregar_final_lista(mapa, p, sizeof(t_casillero))!= 1) {
            free(temp);
            return 0;
        }
    }

    free(temp);
    return 1;
}

int juego_cargar_config(t_config *config) {
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
    if (!f) {
        printf("[Aviso] config.txt no encontrado. Se usaran todos los valores por defecto.\n");
        return 0;
    }

    while (fscanf(f, "%63[^:]:%d\n", clave, &valor) == VALORES_POR_LINEA_CONFIG) {
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

int juego_validar_config(t_config *config) {
    int total_especiales = config->maximo_oasis + config->maximo_tormentas +
                           config->maximo_premios + config->maximo_vidas_extra;
    if (config->cantidad_posiciones < 2) {
        printf("Error: cantidad_posiciones debe ser mayor a 2. Valor actual: %u\n",
               config->cantidad_posiciones);
        return 0;
    }
    if (config->vidas_inicio < 1 || config->vidas_inicio > 10) {
        printf("Error: vidas_inicio debe estar entre 1 y 10. Valor actual: %u\n",
               config->vidas_inicio);
        return 0;
    }

    if (config->maximo_bandidos > config->cantidad_posiciones / 2) {
        printf("Error: maximo_bandidos no puede superar la mitad de casilleros.\n");
        printf("Casilleros: %u | Max bandidos permitidos: %u | Valor actual: %u\n",
               config->cantidad_posiciones, config->cantidad_posiciones / 2, config->maximo_bandidos);
        return 0;
    }
    if (total_especiales > config->cantidad_posiciones - 2) {
        printf("Error: demasiados casilleros especiales.\n");
        printf("Total especiales: %d | Casilleros disponibles: %u\n",
               total_especiales, config->cantidad_posiciones - 2);
        return 0;
    }

    return 1; // Todo OK
}

void printCaravana(FILE *archivo, t_mapa *mapa) {
    tNodo *temp;
    t_casillero *cas;
    unsigned pos;

    if (!archivo || !mapa || !*mapa) return;

    temp = *mapa;
    while (temp->ant != NULL) {
        temp = temp->ant;
    }

    while (temp != NULL) {
        cas = (t_casillero *)temp->info;
        pos = cas->nro_posicion;

        int num_elems = 0;
        char elems[20][3];

        if (cas->tipo_casillero != TIPO_NORMAL || (!cas->presencia_jugador && cas->cant_bandidos == 0)) {
            switch (cas->tipo_casillero) {
                case TIPO_INICIO: strcpy(elems[num_elems++], "I"); break;
                case TIPO_FIN: strcpy(elems[num_elems++], "F"); break;
                case TIPO_NORMAL: strcpy(elems[num_elems++], "."); break;
                case TIPO_OASIS: strcpy(elems[num_elems++], "O"); break;
                case TIPO_TORMENTA: strcpy(elems[num_elems++], "T"); break;
                case TIPO_VIDA_EXTRA: strcpy(elems[num_elems++], "V"); break;
                case TIPO_PREMIO: strcpy(elems[num_elems++], "P"); break;
                default: strcpy(elems[num_elems++], "?"); break;
            }
        }

        if (cas->presencia_jugador) {
            strcpy(elems[num_elems++], "J");
        }

        for (unsigned i = 0; i < cas->cant_bandidos; i++) {
            if (num_elems < 20) strcpy(elems[num_elems++], "B");
        }

        fprintf(archivo, "%02u", pos);
        if (num_elems == 1) {
            fprintf(archivo, ":%s\n", elems[0]);
        } else if (num_elems > 1) {
            fprintf(archivo, ":[");
            for (int i = 0; i < num_elems; i++) {
                fprintf(archivo, "%s%s", elems[i], i == num_elems - 1 ? "" : " ");
            }
            fprintf(archivo, "]\n");
        } else {
            fprintf(archivo, ":.\n");
        }

        temp = temp->sig;
    }

    fprintf(archivo, "\n");
}
