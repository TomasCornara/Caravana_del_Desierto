# Caravana del Desierto - Algoritmos y Estructuras de Datos

Este repositorio es una implementación en C de un juego interactivo de consola llamado "Caravana del Desierto", donde los jugadores emprenden una peligrosa travesía por el desierto. 

El juego de Caravana del Desierto es una simulación basada en casilleros por los cuales uno se va desplazando. La travesía en el tablero está determinada por las siguientes reglas:

- Un jugador que cae en un oasis recupera ventaja, pausando las inclemencias del viaje.
- Un jugador atrapado en una tormenta de arena sufre penalizaciones en el mapa.
- Un jugador asaltado por bandidos pierde vidas si estos lo alcanzan.
- Casilleros con premios y vidas extra aumentan la puntuación final y las probabilidades de éxito.

## Tabla de Contenidos
- [Descripción del Proyecto](#descripción-del-proyecto)
- [Instrucciones de Uso](#instrucciones-de-uso)
- [Integrantes](#integrantes)

## Descripción del Proyecto
Este proyecto está escrito en C y construye un motor de juego utilizando TDA (Tipos de Datos Abstractos). Utiliza **Listas Circulares Dobles** para la representación de los casilleros del mapa y permitir el movimiento en un circuito cerrado. El accionar de los bandidos y del jugador se ejecutan almacenando sus distintas acciones en **Colas Dinámicas**. Por último, cuenta con un sistema de ranking el cual guarda a los jugadores con sus respectivos puntos ordenados en un **Árbol Binario de Búsqueda**. El programa logra carga un tablero de juego y las reglas leyendo un archivo de texto de configuración.

## Instrucciones de Uso

### Compilacion
Para compilar y ejecutar el proyecto:

1. Clonar este repositorio.
2. Compilar usando GCC:
```bash
gcc -o Caravana_del_Desierto main.c consola.c dado.c manejo_archivos.c cola_dinamica.c arbolBinario.c juego.c lista_circular_simple.c -Wall
```
*(De forma opcional, si utiliza Code::Blocks, puede abrir directamente el archivo `Caravana_del_Desierto.cbp` para buildear el proyecto).*

### Archivos de Configuración
Antes de iniciar, puede modificar el archivo de texto `config.txt` que debe encontrarse en el mismo directorio que el ejecutable. Debe estar formateado de la siguiente manera:

```text
cantidad_posiciones:25
vidas_inicio:3
maximo_bandidos:2
maximo_premios:3
maximo_vidas_extra:1
maximo_oasis:2
maximo_tormentas:3
```
*(Si no se encuentra el archivo `config.txt`, el motor cargará estos valores por defecto).*

### Utilizar
Para usar el programa, debe abrir una consola de comandos donde se encuentre el binario compilado y ejecutarlo:
```bash
Caravana_del_Desierto.exe
```
1. Seleccione la opción númerica en el menú principal.
2. Elija "Jugar nueva partida" para comenzar o "Ver ranking" para leer el historial de puntajes, el cual es deserializado del archivo binario integrado.
3. Siga las instrucciones en pantalla y presione la tecla indicada para lanzar el dado.

## Integrantes
- Valentín Nievas 
- Divano, Matías 
- Gauto, Gastón
- Cornara, Tomás
- Lazarte Ulises

