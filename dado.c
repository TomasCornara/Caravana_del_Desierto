#include <stdlib.h>
#include "dado.h"

void crearDado(t_dado* d, unsigned cnt_lados)
{
    if (!d) return;
    d->cara = 1;
}

unsigned contraCara(const t_dado* d){
    if(!d) return 0;

    return (d->lados + 1) - d->cara;
}

unsigned tirarDado(t_dado* d){
    if (!d || d->lados <= 0) return 0;
    return d->cara = rand()% d->lados + 1;
}

void destruirDado(t_dado* d){
    d->cara = 0;
    d->lados = 0;
}
