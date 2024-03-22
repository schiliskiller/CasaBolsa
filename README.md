# CasaBolsa

Programa que imprime de manera asimetrica y aproximada los costos de apertura y los costos de cierre 

## Sintaxis

```bash
./CasaBolsa dir/.../[nombre de archivo .csv]
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
