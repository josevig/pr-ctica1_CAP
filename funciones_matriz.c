// Este archivo contiene las funciones para crear, imprimir y multiplicar matrices.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones_matriz.h"

/* Función para inicializar una matriz con valores aleatorios en [0,1) */
void init_matrix(double *M, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        printf("Las dimensiones de la matriz deben ser un valor superior a 0\n");
        exit(1);
    }
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            M[i * cols + j] = (double)rand() / (double)RAND_MAX;
        }
    }
}

/* Función para poner en cero una matriz */
void zero_matrix(double *M, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        printf("Las dimensiones de la matriz deben ser un valor superior a 0\n");
        exit(1);
    }
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
            for (l = 0; l < k; l++) {
                C[i * n + j] += A[i * k + l] * B[l * n + j];
            }
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