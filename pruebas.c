#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "funciones_matriz.h"

/* Tipo de puntero a función para las funciones de multiplicación */
typedef void (*mult_func)(double*, double*, double*, int, int, int);

/* Función para calcular la media de un arreglo de doubles */
double mean(double *data, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += data[i];
    return sum / n;
}

/* Función para calcular la desviación estándar de un arreglo de doubles */
double stddev(double *data, int n, double mean_val) {
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += (data[i] - mean_val) * (data[i] - mean_val);
    return sqrt(sum / n);
}

/* Función que realiza 'runs' ejecuciones para una función de multiplicación dada,
   almacenando los tiempos en el arreglo 'times'. */
void run_benchmark(mult_func func, int m, int k, int n, int runs, double *times) {
    for (int r = 0; r < runs; r++) {
        double *A = (double *)malloc(m * k * sizeof(double));
        double *B = (double *)malloc(k * n * sizeof(double));
        double *C = (double *)malloc(m * n * sizeof(double));
        if (!A || !B || !C) {
            printf("Error al reservar memoria.\n");
            exit(1);
        }
        /* Inicialización de las matrices */
        init_matrix(A, m, k);
        init_matrix(B, k, n);
        zero_matrix(C, m, n);

        clock_t start = clock();
        func(A, B, C, m, k, n);
        clock_t end = clock();
        times[r] = (double)(end - start) / CLOCKS_PER_SEC;

        free(A);
        free(B);
        free(C);
    }
}

int main(void) {
    /* Dimensiones a probar: ejemplo con matrices cuadradas.
       Se crean archivos CSV separados para cada dimensión. */
    int sizes[] = {128, 256, 512, 1024, 2048, 4096};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int runs = 10;

    /* Inicializar la semilla aleatoria */
    srand((unsigned int)time(NULL));

    printf("Pruebas de rendimiento para multiplicacion de matrices\n");
    printf("-----------------------------------------------------\n\n");

    for (int s = 0; s < num_sizes; s++) {
        int dim = sizes[s];
        int m = dim, k = dim, n = dim;
        printf("Probando con matrices de dimension %d x %d\n", dim, dim);

        /* Arreglos para almacenar los tiempos de cada algoritmo */
        double times_ijk[10], times_jki[10], times_kji[10];
        /* Arreglos para almacenar GFLOPS por ejecución */
        double gflops_ijk[10], gflops_jki[10], gflops_kji[10];
        /* Se calcula el número de operaciones de punto flotante (FLOPS): aprox. 2*m*k*n */
        double ops = 2.0 * m * k * n;

        /* Ejecutar las tres versiones */
        run_benchmark(mult_ijk, m, k, n, runs, times_ijk);
        run_benchmark(mult_jki, m, k, n, runs, times_jki);
        run_benchmark(mult_kji, m, k, n, runs, times_kji);

        /* Calcular GFLOPS para cada ejecución */
        for (int i = 0; i < runs; i++) {
            gflops_ijk[i] = ops / (times_ijk[i] * 1e9);
            gflops_jki[i] = ops / (times_jki[i] * 1e9);
            gflops_kji[i] = ops / (times_kji[i] * 1e9);
            //Para evitar divisiones por 0 (Se asume un valor de tiempo minimo es de 0.000001)
            if (times_ijk[i]<=1e-6)
                gflops_ijk[i] = ops / 1e3;
            if (times_jki[i]<=1e-6)
                gflops_jki[i] = ops / 1e3;
            if (times_kji[i]<=1e-6)
                gflops_kji[i] = ops / 1e3;
        }

        /* Calcular medias y desviaciones de tiempos */
        double mean_ijk = mean(times_ijk, runs);
        double mean_jki = mean(times_jki, runs);
        double mean_kji = mean(times_kji, runs);
        double std_ijk = stddev(times_ijk, runs, mean_ijk);
        double std_jki = stddev(times_jki, runs, mean_jki);
        double std_kji = stddev(times_kji, runs, mean_kji);

        /* Calcular medias de GFLOPS */
        double mean_gflops_ijk = mean(gflops_ijk, runs);
        double mean_gflops_jki = mean(gflops_jki, runs);
        double mean_gflops_kji = mean(gflops_kji, runs);

        /* Calcular la eficiencia relativa: expresada en % respecto a la mejor media en GFLOPS */
        double best_gflops = mean_gflops_ijk;
        if (mean_gflops_jki > best_gflops) best_gflops = mean_gflops_jki;
        if (mean_gflops_kji > best_gflops) best_gflops = mean_gflops_kji;

        double eff_ijk = (mean_gflops_ijk / best_gflops) * 100.0;
        double eff_jki = (mean_gflops_jki / best_gflops) * 100.0;
        double eff_kji = (mean_gflops_kji / best_gflops) * 100.0;

        /* Mostrar resultados en consola */
        printf("Resultados para matriz de dimension %d x %d:\n", dim, dim);
        for (int i = 0; i < runs; i++) {
            printf("  Ejecucion %2d: tiempo (ijk) = %f s, GFLOPS = %f; tiempo (jki) = %f s, GFLOPS = %f; tiempo (kji) = %f s, GFLOPS = %f\n",
                   i + 1, times_ijk[i], gflops_ijk[i],
                   times_jki[i], gflops_jki[i],
                   times_kji[i], gflops_kji[i]);
        }
        printf("  Medias de tiempo: ijk = %f s, jki = %f s, kji = %f s\n", mean_ijk, mean_jki, mean_kji);
        printf("  Desv.Std de tiempo: ijk = %f s, jki = %f s, kji = %f s\n", std_ijk, std_jki, std_kji);
        printf("  Medias de GFLOPS: ijk = %f, jki = %f, kji = %f\n", mean_gflops_ijk, mean_gflops_jki, mean_gflops_kji);
        printf("  Eficiencia relativa: ijk = %.2f%%, jki = %.2f%%, kji = %.2f%%\n\n", eff_ijk, eff_jki, eff_kji);

        /* Escribir resultados en un archivo CSV.
           Se crea un archivo por dimensión, por ejemplo: resultados_128.csv */
        char filename[50];
        sprintf(filename, "resultados_%d.csv", dim);
        FILE *fp = fopen(filename, "w");
        if (fp == NULL) {
            perror("Error al abrir el archivo CSV");
            exit(1);
        }

        /* Encabezado del CSV */
        fprintf(fp, "Ejecucion;Time_ijk(s);Time_jki(s);Time_kji(s);GFLOPS_ijk;GFLOPS_jki;GFLOPS_kji\n");
        for (int i = 0; i < runs; i++) {
            fprintf(fp, "%d;%f;%f;%f;%f;%f;%f\n",
                    i + 1, times_ijk[i], times_jki[i], times_kji[i],
                    gflops_ijk[i], gflops_jki[i], gflops_kji[i]);
        }
        /* Líneas opcionales: promedios y desviaciones */
        fprintf(fp, "Media;%f;%f;%f;%f;%f;%f\n",
                mean_ijk, mean_jki, mean_kji, mean_gflops_ijk, mean_gflops_jki, mean_gflops_kji);
        fprintf(fp, "DesvStd;%f;%f;%f;;;\n", std_ijk, std_jki, std_kji);
        /* Eficiencia relativa */
        fprintf(fp, "Eficiencia Relativa (%%);%f;%f;%f;;;\n", eff_ijk, eff_jki, eff_kji);

        fclose(fp);
        printf("Archivo CSV '%s' creado.\n", filename);
        printf("======================================================\n\n");
    }

    return 0;
}
