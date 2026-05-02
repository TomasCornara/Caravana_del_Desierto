**REGLAS:**



Asignado: (*\[Nombre]*)

Terminado: ✓

A revisar: \*



**Integracion/Juego:**



\- Estructura configuracion

\- Estructura de movimientos

\- Estructura para jugador

\- Estructura para bandido

\- Estructura de un casillero

\- Estructura de una mano

\- Funcion para definir si lo desapilado es un t\_bandido o t\_jugador

\- Crear bandido

\- Asignarle a un bandido la direccion de un t\_casillero

\- Calcular movimiento bandido (Tirar dado, elegir mejor direccion, moverse en el mapa)

\- Crear jugador

\- Jugar mano (Fijarse si hay que aplicar efectos, premios, etc y como, tirar dado, pedir direccion, desplazarlo. Todas estas podrian ser sub funciones asociadas a esta)

\- Cargar mano en el sistema de manos

\- Bajar un archivo al sistema de pre-preparacion del mapa

\- Bajar del sistema de pre-preparacion del mapa al sistema de mapa

\- Funciones wrapper para los sistemas (Ejemplo: nuestro sistema de mapa usa t\_nodos en una lista circular, pero nosotros necesitamos una funcion wrapper que pongan en cola el caso especifico nuestro, que seria t\_casillero. O sea, se necesita funciones como "crearMapa" que envuelvan a funciones como "crearColaCircular" o "cargarCasillero" que envuelvan a "ponerEncola")





**Sistema de archivos:**



\- Abrir archivo ✓

\- Cerrar archivo

\- Hacer lectura de una linea del arch de configuracion y bajarla dentro del sistema de un t\_configuracion + t\_casillero (y un bandido asociado a ese casillero, segun corresponda)

\- Bajar t\_mano a archivo





**Estructura para el Sistema de pre-preparacion del mapa:**



\- Estructura del nodo de un vector

\- Crear vector

\- Destruir vector

\- Poner al final

\- Vaciar vector

\- Ver posicion



**Estructura para el Sistema de Turnos:**



\- Estructura del nodo de lista circular

\- Crear Lista Circular

\- Vaciar Lista

\- Lista llena

\- Lista vacia

\- Poner un elemento al principio

\- Mostrar elemento actual

\- Quitar elemento

\- Desplazar al siguiente nodo



**Sistema de dados:** ✓



\- Estructura dado ✓

\- Crear dado ✓

\- Destruir ✓

\- Mostrar cara ✓

\- Mostrar Contra-cara ✓

\- Lanzar dado (Cambiar el valor) ✓



**Estructura para el Sistema de mapa:**



\- Estructura del nodo de lista circular doble

\- Crear Lista circular doble

\- vaciar lista circular doble

\- lista llena

\- lista vacia

\- vaciar lista

\- poner elemento al final

\- ver elemento actual

\- Desplazar al siguiente nodo

\- Desplazar al anterior nodo

\- Desplazar N nodos



**Estructura para el Sistema movimientos:**



\- Estructura del nodo de cola

\- Crear Cola

\- Destruir Cola

\- Ver primero

\- Cola llena

\- Cola vacia

\- Desencolar

