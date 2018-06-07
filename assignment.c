#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define n 3

int i,j,k;

void nrerror(char error_text[])
{
    printf("%s\n", error_text);
    exit(0);
};

void showSystem(double **A, char c) {
    printf("\n%*c[%c]\n", 7*n,' ', c);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("|%+02.5lf|\t ", A[i][j]);
        printf("\n");
    }
}

void showArray(double *X, char c) {
    int i;
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c[%d] = %2.5lf\n",c,i, X[i]);
    }
}

double *createArray()
{
    double *array;

    if (( array = malloc( n*sizeof( double ))) == NULL ) {
        nrerror("SEM MEMORIA!"); 
    }

    return array;
}

double **createMatrix()
{
    double **matrix;

    /* n IS THE nUMBER OF ROWS */
    if (( matrix = malloc( n*sizeof( double* ))) == NULL ) {
        nrerror("SEM MEMORIA!"); 
    }

    /* n IS THE nUMBER OF COLUMnS */
    for ( i = 0; i < n; i++ )
    {
        if (( matrix[i] = malloc( n*sizeof( double ))) == NULL )
        {
            nrerror("SEM MEMORIA!");
        }
    }

    /* nxn MATRIX */
    return matrix;
}

double **cholesky(double **A)
{
    double sum;
    double **G;

    /* CREATE THE MATRIX WITH THE SAME SIZE OF A */
    G = createMatrix();

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

void choleskySolucion(double **a, double b[], double x[])
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


void luDecomposition(double **mat, double **lower, double **upper)
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


int main()
{
    int count;

    double **arr;
    double **lower;
    double **upper;
    
    double **G;

    arr = createMatrix();
    lower = createMatrix();
    upper = createMatrix();
    // note that arr[i][j] is same as *(*(arr+i)+j)
    count = 0;
    for (i = 0; i <n; i++){
        for (j = 0; j < n; j++){
            arr[i][j] = 1;
            lower[i][j] = 0;
            upper[i][j] = 0;
        }
    }

    arr[0][0] = 4; arr[0][1] = 12; arr[0][2] = -16;
    arr[1][0] = 12; arr[1][1] = 37; arr[1][2] = -43;
    arr[2][0] = -16; arr[2][1] = -43; arr[2][2] = 98;

    luDecomposition(arr, lower, upper);

    G = cholesky(arr);
    showSystem(G, 'G');

    showSystem(arr, 'A');
    showSystem(lower, 'L');
    showSystem(upper, 'U');
    printf("\n");

    return 0;
}
