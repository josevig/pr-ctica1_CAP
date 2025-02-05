#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Función para inicializar una matriz con valores aleatorios en [0,1) */
void init_matrix(double *M, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            M[i * cols + j] = (double)rand() / (double)RAND_MAX;
        }
    }
}

/* Función para poner en cero una matriz */
void zero_matrix(double *M, int rows, int cols) {
    memset(M, 0, rows * cols * sizeof(double));
}

/* Función para imprimir una matriz (solo si es pequeña) */
void print_matrix(double *M, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%6.3f ", M[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Producto de matrices usando la ordenación ijk */
void mult_ijk(double *A, double *B, double *C, int m, int k, int n) {
    int i, j, l;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            double sum = 0.0;
            for (l = 0; l < k; l++) {
                sum += A[i * k + l] * B[l * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

/* Producto de matrices usando la ordenación jki */
void mult_jki(double *A, double *B, double *C, int m, int k, int n) {
    int i, j, l;
    for (j = 0; j < n; j++) {
        for (l = 0; l < k; l++) {
            for (i = 0; i < m; i++) {
                C[i * n + j] += A[i * k + l] * B[l * n + j];
            }
        }
    }
}

/* Producto de matrices usando la ordenación kji */
void mult_kji(double *A, double *B, double *C, int m, int k, int n) {
    int i, j, l;
    for (l = 0; l < k; l++) {
        for (j = 0; j < n; j++) {
            for (i = 0; i < m; i++) {
                C[i * n + j] += A[i * k + l] * B[l * n + j];
            }
        }
    }
}

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
