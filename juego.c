#include "juego.h"
#include "manejo_archivos.h"

void poner_tipo_random(t_casillero *temp, int total, unsigned tipo, int cantidad) {
    int puestos = 0;
    while (puestos < cantidad) {
        // pos entre 1 y total-2, no INICIO ni FIN
        t_casillero *cas = temp + 1 + rand() % (total - 2);

        if (cas->tipo_casillero == TIPO_NORMAL) {
            cas->tipo_casillero = tipo;
            puestos++;
        }
    }
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

int juego_generar_mapa(t_config *config, t_mapa *mapa) {
    crear_lista(mapa);
    srand(time(NULL));
    t_casillero *p, *fin;
    unsigned cant = config->cantidad_posiciones;
    t_casillero *temp = calloc(cant, sizeof(t_casillero));
    if (!temp) return 0;

    fin = temp + cant;

    for (p = temp; p < fin; p++) {
        p->tipo_casillero = TIPO_NORMAL;
        p->presencia_jugador = false;
        p->cant_bandidos = 0;
    }

    temp->tipo_casillero = TIPO_INICIO;
    temp->presencia_jugador = true;
    (temp + cant - 1)->tipo_casillero = TIPO_FIN;

    poner_tipo_random(temp, cant, TIPO_OASIS, config->maximo_oasis);
    poner_tipo_random(temp, cant, TIPO_TORMENTA, config->maximo_tormentas);
    poner_tipo_random(temp, cant, TIPO_PREMIO, config->maximo_premios);
    poner_tipo_random(temp, cant, TIPO_VIDA_EXTRA, config->maximo_vidas_extra);

    poner_bandidos_random(temp, cant, config->maximo_bandidos);


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
    FILE *f = abrir_txt(ARCHIVO_CONFIG, "r");
    if (!f) {
        /* Valores por defecto si no existe el archivo */
        config->cantidad_posiciones = 25;
        config->vidas_inicio        = 3;
        config->maximo_bandidos     = 2;
        config->maximo_premios      = 3;
        config->maximo_vidas_extra  = 1;
        config->maximo_oasis        = 2;
        config->maximo_tormentas    = 3;
        printf("[Aviso] config.txt no encontrado. Usando valores por defecto.\n");
        return 0;
    }
    char clave[64];
    int  valor;
    while (fscanf(f, "%63[^:]:%d\n", clave, &valor) == 2) {
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
