#include "auxiliar.h"

int i,j,k;

void nrerror(char error_text[])
{
    printf("%s\n", error_text);
    exit(0);
};

void showSystem(double **A, char *c, int rows, int cols) 
{
    printf("\n%*c[%s]\n", 7*cols,' ', c);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            printf("|%+02.5lf|\t ", A[i][j]);
        printf("\n");
    }
}

void showArray(double *X, char *c, int rows) 
{
    int i;
    printf("\n");
    for (i = 0; i < rows; i++) {
        printf("%s[%d] = %2.5lf\n",c,i, X[i]);
    }
}

double *createArray(int n)
{
    double *array;
    if (( array = malloc( n*sizeof( double ))) == NULL ) {
         nrerror("SEM MEMORIA!"); 
    }
    return array;
}

double **createMatrix(int rows, int cols)
{
    double **matrix;
    /* n IS THE nUMBER OF ROWS */
    if (( matrix = malloc( rows*sizeof( double* ))) == NULL ) {
        nrerror("SEM MEMORIA!"); 
    }
    /* n IS THE nUMBER OF COLUMnS */
    for ( i = 0; i < rows; i++ ){
        if (( matrix[i] = malloc( cols*sizeof( double ))) == NULL ){
            nrerror("SEM MEMORIA!");
        }
    }
    /* nxn MATRIX */
    return matrix;
}

double **transposeMatrix(double **A, int row, int col)
{
    double **T;
    T = createMatrix(col, row);
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            T[j][i] = A[i][j];
        }
    }
    return T;
}

double **matrixMultiplication(double **A, double **B, int m, int p, int q)
{
    // [A]*[B] = [R]
    double **R;
    R = createMatrix(m, q);
    double sum;
    int c, d, k;
    for (c = 0; c < m; c++) {
        for (d = 0; d < q; d++) {
            sum = 0;
            for (k = 0; k < p; k++) {
                sum = sum + A[c][k]*B[k][d];
            }
            R[c][d] = sum;
        }
    }
    return R;
}

double *arrayMultiplication(double **A, double *B, int m, int p)
{
    // [A]*[B] = [R]
    double *R;
    R = createArray(m);
    double sum;
    int c, k;
    for (c = 0; c < m; c++) {
        sum = 0;
        for (k = 0; k < p; k++) {
            sum = sum + A[c][k]*B[k];
        }
        R[c] = sum;
    }
    return R;
}

int numberOfLines(FILE *f) 
{
    char c;
    int count = 0;
    // Extract characters from file and store in character c
    for (c = getc(f); c != EOF; c = getc(f))
        if(c == '\n') // Increment count if this character is newline
            count = count + 1;
    // Close the file
    //printf("The file has %d lines\n ", count);
    fseek(f, 0, SEEK_SET);

    return count;
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
        tok && *tok;
        tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
