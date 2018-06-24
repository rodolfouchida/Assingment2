#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef _AUXILIAR_H_
#define _AUXILIAR_H_

void nrerror(char error_text[]);

void showSystem(double **A, char *c, int rows, int cols);

void showArray(double *X, char *c, int rows);

double *createArray(int n);

double **createMatrix(int rows, int cols);

double **transposeMatrix(double **A, int row, int col);

double **matrixMultiplication(double **A, double **B, int m, int p, int q);

double *arrayMultiplication(double **A, double *B, int m, int p);

int numberOfLines(FILE *f);

const char* getfield(char* line, int num);

#endif
