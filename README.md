# CasaBolsa

Programa que imprime de manera asimetrica y aproximada los costos de apertura y los costos de cierre. 

Respecto al codigo, se usaron listas enlazadas para capturar tanto las fechas de cada dato como los costos mencionados con anterioridad.
Asimismo, se implemento un tipo-enumeracion que permite guardar el estado de las acciones de manera que esta pueda guardar si las acciones fueron compradas o no.

Con posterioridad se brindaran los headers y las funciones por separado para aquello que necesite utilizarlo con propositos educativos o academicos.

(este programa se realizo con ayuda de un debugger (gdb) y un depurador de memoria (valgrind))

## Sintaxis

```bash
./CasaBolsa dir/.../[nombre de archivo .csv]
# o tambien
./CasaBolsa dir/.../[nombre de archivo].csv
```

## Compilacion

```bash
$ gcc -lm -g -Wall -o CasaBolsa CasaBolsa.c

```

## Ejemplo:

```bash
$ ./CasaBolsa hist/GOOG_Monthly

     Del 01-04-2023 hasta 01-03-2024

143.69 |
139.61 |         ,         .
139.60 |                 ,   .
138.43 |                 .   ,
133.32 |             .     ,
132.15 |       .     ,
130.85 |     ,     .
125.34 |       ,       .
123.50 |         .     ,
120.32 |   .       ,
107.72 | ,   .
102.67 | . ,
       `-----------------------

       . => costo de apertura
       , => costo de cierre
       * => costos aproximados
```

## Creditos
Martinez Schleske Alan. Estudiante de Ingenieria en Informatica (Universidad Veracruzana)
