#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consola.h"
#include "juego.h"



void pausa() {
    int c;
    printf("\n  Presione ENTER para continuar...");
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpiar_buffer(void) {
    fflush(stdin);
}

void pedir_nombre(char *nombre) {
    size_t len;
    size_t i;
    int letras;

    limpiar_buffer();

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
        }

        // Verificar que se haya escrito algo
        letras = 0;
        for (i = 0; i < len; i++) {
            if (isalpha((unsigned char)nombre[i])) {
                letras++;
            }
        }

        if (letras < 3) {
            printf("Nombre invalido: ingrese al menos 3 letras.\n");
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
           "  |             GRUPO 10             |\n"
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
