#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "funciones_matriz.h"

int main(int argc, char **argv) {
    int m, k, n;
    double *A, *B, *C;
    clock_t inicio, fin;
    double duration;

    if (argc != 4) {
        printf("Error de sintaxis. Uso: %s m k n\n", argv[0]);
        exit(1);
    }

    m = atoi(argv[1]);
    k = atoi(argv[2]);
    n = atoi(argv[3]);

    if (m <= 0 || k <= 0 || n <= 0) {
        printf("Las dimensiones deben ser mayores que 0\n");
        exit(1);
    }

    A = (double *)malloc(m * k * sizeof(double));
    B = (double *)malloc(k * n * sizeof(double));
    C = (double *)malloc(m * n * sizeof(double));

    if (!A || !B || !C) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));
    init_matrix(A, m, k);
    init_matrix(B, k, n);
    zero_matrix(C, m, n);

    printf("Dimensiones: A(%d x %d) * B(%d x %d) = C(%d x %d)\n\n", m, k, k, n, m, n);

    zero_matrix(C, m, n);
    inicio = clock();
    mult_ijk(A, B, C, m, k, n);
    fin = clock();
    duration = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Version ijk: %2.3f segundos\n", duration);
    if (m <= 8 && k <= 8 && n <= 8) {
        printf("Resultado C (ijk):\n");
        print_matrix(C, m, n);
    }

    zero_matrix(C, m, n);
    inicio = clock();
    mult_jki(A, B, C, m, k, n);
    fin = clock();
    duration = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Version jki: %2.3f segundos\n", duration);
    if (m <= 8 && k <= 8 && n <= 8) {
        printf("Resultado C (jki):\n");
        print_matrix(C, m, n);
    }

    zero_matrix(C, m, n);
    inicio = clock();
    mult_kji(A, B, C, m, k, n);
    fin = clock();
    duration = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Version kji: %2.3f segundos\n", duration);
    if (m <= 8 && k <= 8 && n <= 8) {
        printf("Resultado C (kji):\n");
        print_matrix(C, m, n);
    }

    if (m <= 8 && k <= 8 && n <= 8) {
        printf("Matriz A:\n");
        print_matrix(A, m, k);
        printf("Matriz B:\n");
        print_matrix(B, k, n);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
