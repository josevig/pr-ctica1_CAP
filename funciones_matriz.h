#ifndef FUNCIONES_MATRIZ_H
#define FUNCIONES_MATRIZ_H

void init_matrix(double *M, int rows, int cols);
void zero_matrix(double *M, int rows, int cols);
void print_matrix(double *M, int rows, int cols);
void mult_ijk(double *A, double *B, double *C, int m, int k, int n);
void mult_jki(double *A, double *B, double *C, int m, int k, int n);
void mult_kji(double *A, double *B, double *C, int m, int k, int n);

#endif