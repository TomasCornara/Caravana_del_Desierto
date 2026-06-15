#ifndef DADO_H_INCLUDED
#define DADO_H_INCLUDED

typedef struct{
    unsigned lados;
    unsigned cara;
}t_dado;

void crearDado(t_dado* d, unsigned cnt_lados);
unsigned contraCara(const t_dado* d);
unsigned tirarDado(t_dado* d);
void destruirDado(t_dado* d);

#endif // DADO_H_INCLUDED
