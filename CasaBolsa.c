#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALLOC(x) if (!((x) = malloc(sizeof(*(x))))) {                   \
                     fprintf(stderr, "ERROR: espacio no disponible\n"); \
                     exit(1);                                           \
                 }

#define ALLOC_SIZE(x, n) if (!((x) = malloc((n) * sizeof(*(x))))) {             \
                             fprintf(stderr, "ERROR: espacio no disponible\n"); \
                             exit(1);                                           \
                         }

typedef enum {COMPRADO, PENDIENTE} Estado;

struct fecha {
    int dia, mes, anio;
};

struct costos {
    struct fecha fech_dato;
    float  apertura,
           cierre;
    Estado estado_accion;
    struct costos *sig;
};

typedef struct {
    struct costos *lista;
    int cant_datos;
} Costos;

typedef struct {
    char **mat;
    int m, n;
} Matriz;

void aniadirCostos(Costos *, FILE *);
void liberarCostos(Costos *);
void saltarLinea(FILE **);
void pasarCostoArr(Costos *, float *, float *);

void graficarHistoria(Costos);
void initGraf(Matriz *);
void imprGraf(Matriz, float *);
void dibujarGraf(Matriz *, float *, float *);

float max(float *, int n);
struct costos *ultimoElem(struct costos *);

void initMatriz(Matriz *);
void liberarMatriz(Matriz m);

int
main(int argc, char const **argv) {
    Costos cost = {.lista = NULL, .cant_datos = 0};
    FILE *datos = NULL;
    char *arch = NULL;

    if (strstr(argv[1], ".csv"))
        arch = (char *) argv[1];
    else {
        arch = malloc(sizeof(char) * (strlen(argv[1]) + 4));
        strcpy(arch, argv[1]);
        strcat(arch, ".csv");
    }

    if (!(datos = fopen(arch, "r"))) {
        fprintf(stderr, "ERROR: archivo no existe o no tiene extension .csv\n");
        exit(1);
    }

    saltarLinea(&datos);
    while (!feof(datos))
        aniadirCostos(&cost, datos);

    fclose(datos);

    graficarHistoria(cost);

    if (!strstr(argv[1], ".csv"))
        free(arch);
    
    return 0;
}

void
initMatriz(Matriz *m) {
    ALLOC_SIZE(m->mat, m->m);
    for (int i = 0; i < m->m; i++)
        ALLOC_SIZE(m->mat[i], m->n);
}

void
liberarMatriz(Matriz m) {
    for (int i = 0; i < m.m; i++)
        free(m.mat[i]);
    free(m.mat);
}

void
saltarLinea(FILE **arch) {
    char buff[256];

    fgets(buff, 254, *arch);
}

void
aniadirCostos(Costos *costos, FILE *datos) {
    struct costos *nuevoElem = NULL;
    float f;

    ALLOC(nuevoElem);

    // Para fecha y costo de apertura
    fscanf(datos, "%4d%*c%2d%*c%2d%*c%f", &nuevoElem->fech_dato.anio,
                                          &nuevoElem->fech_dato.mes,
                                          &nuevoElem->fech_dato.dia,
                                          &nuevoElem->apertura);
    // Para costo de cierre
    fscanf(datos, "%*c%f%*c%f%*c%f", &f, &f, &nuevoElem->cierre);
    // Para saltar a la siguiente linea
    saltarLinea(&datos);

    if (!nuevoElem->fech_dato.anio) {
        free(nuevoElem);
        return;
    }

    nuevoElem->sig = costos->lista;
    costos->lista = nuevoElem;
    costos->cant_datos++;
}

void
graficarHistoria(Costos costos) {
    Matriz grafica = {
        NULL,
        costos.cant_datos + 1,
        costos.cant_datos * 2
    };
    float apert[costos.cant_datos],
          cierr[costos.cant_datos];

    initMatriz(&grafica);
    initGraf(&grafica);

    pasarCostoArr(&costos, apert, cierr);

    dibujarGraf(&grafica, apert, cierr);

    struct costos *i = ultimoElem(costos.lista);
    
    for (int i = 0; i < costos.cant_datos - 8; i++) putchar(' ');
    printf("Del %.2d-%.2d-%.4d hasta %.2d-%.2d-%.4d\n\n", i->fech_dato.dia,
                                                          i->fech_dato.mes,
                                                          i->fech_dato.anio,
                                                          costos.lista->fech_dato.dia,
                                                          costos.lista->fech_dato.mes,
                                                          costos.lista->fech_dato.anio);
    imprGraf(grafica, max(apert, costos.cant_datos) < max(cierr, costos.cant_datos) ? cierr : apert);
    liberarMatriz(grafica);
}

float
max(float *arr, int n) {
    int max = 0;
    
    for (int i = 0; i < n; i++)
        if (max < arr[i]) max = arr[i];

    return max;
}

struct costos
*ultimoElem(struct costos *c) {
    if (c->sig) return ultimoElem(c->sig);

    return c;
}

void
pasarCostoArr(Costos *c, float *apert, float *cierr) {
    static int i = 0, cond = 0;
    static struct costos *it = NULL;
    if (!cond) {
        i = c->cant_datos - 1;
        it = c->lista;
        cond++;
    }
    if (!it) {
        i = 0;
        it = NULL;
        return;
    }

    apert[i] = it->apertura;
    cierr[i--] = it->cierre;
    it = it->sig;

    pasarCostoArr(c, apert, cierr);
}

void
ordenar(float *arr, int n) {
    for (int i = 0; i < n; i++) {
    	float aux;
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                aux    = arr[i];
                arr[i] = arr[j];
                arr[j] = aux;
            }
    }
}


void
dibujarGraf(Matriz *graf, float *apert, float *cierr) {
    float arr_orden[graf->m - 1];
    int i = 0;

    memcpy(arr_orden, apert, graf->m * sizeof(float));
    ordenar(arr_orden, graf->m - 1);

    for (int k = 1; k < graf->m - 1; k++) {
        for (i = 0; i < graf->m - 1 && arr_orden[k] != apert[i]; i++)
            ;

        if (!i)
            i++;

        graf->mat[graf->m - (i + 1)][2 * k] = '.';
    }

    memcpy(arr_orden, cierr, graf->m * sizeof(float));
    ordenar(arr_orden, graf->m - 1);

    for (int k = 1; k < graf->m - 1; k++) {
        for (i = 0; i < graf->m - 1 && arr_orden[k] != cierr[i]; i++)
            ;
        if (graf->mat[graf->m - (i + 1)][2 * k] == '.') {
            graf->mat[graf->m - (i + 1)][2 * k] = '*';
            continue;
        }

        if (!i)
            i++;
        
        graf->mat[graf->m - (i + 1)][2 * k] = ',';
    }
}

void
initGraf(Matriz *graf) {
    for (int i = 0; i < graf->m; i++) {
        if (i == graf->m - 1) {
            graf->mat[i][0] = '`';
            for (int k = 1; k < graf->n; k++)
                graf->mat[i][k] = '-';
            break;
        }

        graf->mat[i][0] = '|';
        for (int j = 1; j < graf->n; j++)
            graf->mat[i][j] = ' ';

    }
}

void
imprGraf(Matriz graf, float *cost_ordenado) {
    int k = graf.m - 1;
    ordenar(cost_ordenado, k--);

    for (int i = 0; i < graf.m; i++) {
        if (k >= 0)
            printf("%3.2f ", cost_ordenado[k--]);
        else
            printf("       ");

        for(int j = 0; j < graf.n; j++)
            putchar(graf.mat[i][j]);
        putchar('\n');
    }

    printf("\n       . => costo de apertura\n"
           "       , => costo de cierre\n"
           "       * => costos en interseccion (aproximadamente)\n");
}
