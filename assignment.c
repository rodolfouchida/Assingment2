#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int i,j,k;

void nrerror(char error_text[])
{
    printf("%s\n", error_text);
    exit(0);
};

void showSystem(double **A, char c, int rows, int cols) {
    printf("\n%*c[%c]\n", 7*cols,' ', c);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            printf("|%+02.5lf|\t ", A[i][j]);
        printf("\n");
    }
}

void showArray(double *X, char c, int rows) {
    int i;
    printf("\n");
    for (i = 0; i < rows; i++) {
        printf("%c[%d] = %2.5lf\n",c,i, X[i]);
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
    for ( i = 0; i < rows; i++ )
    {
        if (( matrix[i] = malloc( cols*sizeof( double ))) == NULL )
        {
            nrerror("SEM MEMORIA!");
        }
    }

    /* nxn MATRIX */
    return matrix;
}

double **cholesky(double **A, int n){
    double sum;
    double **G;

    /* CREATE THE MATRIX WITH THE SAME SIZE OF A */
    G = createMatrix(n, n);

    for (k = 0; k < n; k++)
        for (i = 0; i <= k; i++)
        {
            /* THE SUM IS THE SAME FOR BOTH SITUACIOn */
            sum = 0;
            for (j = 0; j < i; j++)
            {
                sum += G[i][j] * G[k][j];
            }

            /* CALCULATE THE Gkk AnD Gjk */
            if (i == k)
            {
                if(A[i][i] - sum <= 0)
                    nrerror("nAO DEFInIDA POSITIVA");
                G[i][i] = sqrt(A[i][i] - sum);
            }
            else
                G[k][i] = 1.0 / G[i][i] * (A[k][i] - sum);
        }

    /* RETURn THE DECOMPOSITIOn */
    return G;
}

void choleskySolucion(double **a, double b[], double x[], int n)
{
    double sum;

    /* SOLVE THE SYSTEM AND STORE THE ANSWER Y IN X */
    for(i = 0; i < n; ++i) {
        sum = 0;

        for(j = 0; j < i; ++j)
            sum += a[i][j] * x[j];

        x[i] = (b[i] - sum) / a[i][i];
    }

    /* showArray(x,n,'y'); */

    /* SOLVE THE SYSTEM USING 'Y' CALCULATED BEFORE, AND STORE THE ANSWER IN X */
    for (i=n-1;i>=0;i--) {
        sum=0;
        for(j = i+1; j < n; j++)
            sum += a[j][i] * x[j];

        x[i] = (x[i] - sum) / a[i][i];
    }

    /* showArray(x,n,'x'); */

}


void luDecomposition(double **mat, double **lower, double **upper, int n)
{
    // Decomposing matrix into Upper and Lower
    // triangular matrix
    for (int i = 0; i < n; i++) {
        // Upper Triangular
        for (int k = i; k < n; k++) {
        // Summation of L(i, j) * U(j, k)
            int sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);
            // Evaluating U(i, k)
            upper[i][k] = mat[i][k] - sum;
        }
 
        // Lower Triangular
        for (int k = i; k < n; k++) {
            if (i == k)
                lower[i][i] = 1; // Diagonal as 1
            else {
                // Summation of L(k, j) * U(j, i)
                int sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);
                    // Evaluating L(k, i)
                    lower[k][i] = (mat[k][i] - sum) / upper[i][i];
            }
        }
    }
}

void transposeMatrix(double **A, double **T, int row, int col)
{
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            T[i][j] = A[j][i];
        }
    }
}

void matrixMultiplication(double **A, double **B, double **R, int m, int p, int q)
{
    // [A]*[B] = [R]
    int sum = 0;
    int c, d, k;
    for (c = 0; c < m; c++) {
        for (d = 0; d < q; d++) {
            for (k = 0; k < p; k++) {
                sum = sum + A[c][k]*B[k][d];
            }
            R[c][d] = sum;
            sum = 0;
        }
    }
}

int numberOfLines(FILE *f) {
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

int main(int argc, char **argv)
{
    int count;

    double **A;
    double **B;
    double **arr;
    double *z;

    int grau = atoi(argv[2]);
    if (grau < 1){
        printf("[!] Grau do polinomio deve ser no minimo 1!");
        exit(EXIT_FAILURE);
    }
    
    int n = 2*grau + 2;

    FILE *f = fopen(argv[1], "r");
    char *record, *line, buffer[1024];
    size_t len=0;
    ssize_t read;

    if(f==NULL)
        exit(EXIT_FAILURE);

    count = numberOfLines(f);
    
    A = createMatrix(count, n);
    B = createMatrix(n, count);
    arr = createMatrix(count, count);
    z = createArray(count);
    
    i=0; 
    while((line=fgets(buffer,sizeof(buffer),f))!=NULL)
    {
        //printf("Retrievedlineoflength%zu:\n",read);
        //printf("%s",line);
        char *tmp;
        tmp=strdup(line);
        z[i] = atof(getfield(tmp, 3));
        free(tmp);
        A[i][0] = 1.0;
        tmp=strdup(line);
        A[i][1] = atof(getfield(tmp, 1));
        free(tmp);
        tmp=strdup(line);
        A[i][2] = atof(getfield(tmp, 2));
        free(tmp);
        A[i][3] = A[i][1]*A[i][2];
        for(j=4; j < n; j++){
            if(j % 2 == 0){
                A[i][j] = A[i][j-1]*A[i][1];
            } else {
                A[i][j] = A[i][j-1]*A[i][2];
            }
        };
        i++;
    }

    fclose(f);
    if(line)
        free(line);

    showSystem(A, 'A', count, n);

    transposeMatrix(A, B, n, count);
    showSystem(B, 'B', n, count);

    matrixMultiplication(B, A, arr, n, count, n);
    showSystem(arr, 'M', n, n);
    
//    double **lower;
//    double **upper;
    double **G;

//    lower = createMatrix(n, n);
//    upper = createMatrix(n, n);

//    luDecomposition(arr, lower, upper, n);
//    showSystem(lower, 'L', n, n);
//    showSystem(upper, 'U', n, n);

    G = cholesky(arr, n);
    showSystem(G, 'G', n, n);

    return 0;
}
