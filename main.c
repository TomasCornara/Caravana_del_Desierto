#include <stdio.h>
#include <stdlib.h>

#include "manejo_archivos.h"
#include "consola.h"

int main()
{
    FILE* arch;


    arch = abrir_txt("ejemplo","w");
    if(arch){
        printf("Todo en orden");
        fclose(arch);
    }
    return 0;
}
