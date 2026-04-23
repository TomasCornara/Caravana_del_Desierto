#include <stdio.h>
#include <stdlib.h>
#include "consola.h"

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

void mostrarEstadisticas(unsigned vidas, unsigned puntos){
    printf(
    "                                                      VIDAS: %d                                                         \n"
    "                                                      PUNTOS: %d                                                        \n"
    "########################################################################################################################\n",
    vidas, puntos
    );
}

void printCaravana(void){
    printf(
    "                                                                                                                          \n"
    "                                         sSSSSSs                                                                          \n"
    "                                      sSSsSSSSSSss                                                                        \n"
    "                                      sSSSSSSSSSSSs                                                                       \n"
    "                                       ssSSSSSSSSSs                                                                       \n"
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
        "           ............     (((((           ....)))))))))                              \n"
        "                          ........(((((((   ..  ((....(((((         ))).......))))         )))))                       \n"
        "                    ((((...   ((((      ...   ....       ...))))..............   )).............)))))                  \n"
        "                 (((...     (((.......(((((((.                .))))))        .....)).))))     .......)))               \n"
        "               ((  ..    .(((..    (((    .             ......   ....))    )     ....   )))))).        ))).            \n"
        "              ((    .. .((      .((        ...(((........            ..)...)))   )) ....     .)))        ))..          \n"
        "              ( .......(    ..(((       ..((((                        .).....)....)))  ...      .))       ))           \n"
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
        "                              ......                                                                                   \n"
        "                          .............                                                                                \n"
        "                         ...... ..........                                        ***                                  \n"
        "                       ..................        ######                              **             ****               \n"
        "                      ............... ..########## ## #######                         *****       ***                  \n"
        "                ###########...... #######     #####         ########                     OO*    **OOO                  \n"
        "           #####  ####    #########     #######                    #####                OOO**  ** OOO                  \n"
        "        #####                          ###                            ###              **********  ***O**              \n"
        " ########    0**    *00              ##                                #########   ***** *********** OO *****          \n"
        "               *  ** 0               ##                           ######          **   OOO    *A*******     *          \n"
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

void mostrarFooter(void){
        printf(
        "########################################################################################################################\n"
        "\n"
        "                                                    GRUPO 10                                                            \n"
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
