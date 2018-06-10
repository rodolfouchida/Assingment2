#include "auxiliar.h"

int i,j,k;

double **cholesky(double **A, int n){
    double sum;
    double **G;

    /* CREATE THE MATRIX WITH THE SAME SIZE OF A */
    G = createMatrix(n, n);
    
    //printf("[*] Matrix criada!\n");

    for (k = 0; k < n; k++){
        for (i = 0; i <= k; i++){
            //printf("[*] k=%d, i=%d\n", k, i);
            /* THE SUM IS THE SAME FOR BOTH SITUACIOn */
            sum = 0;
            for (j = 0; j < i; j++){
                sum += G[i][j] * G[k][j];
            }
            //printf("[*] sum: %lf\n", sum);
            /* CALCULATE THE Gkk AnD Gjk */
            if (i == k){
                if(A[i][i] - sum <= 0)
                    nrerror("nAO DEFInIDA POSITIVA");
                //printf("[*] A[i][i]: %lf\n", A[i][i]);
                G[i][i] = sqrt(A[i][i] - sum);
                //printf("[*] G[i][i]: %lf\n", G[i][i]);
            }
            else {
                //printf("[*] A[k][i]: %lf\n", A[k][i]);
                G[k][i] = 1.0 / (G[i][i] * (A[k][i] - sum));
                //printf("[*] G[k][i]: %lf\n", G[k][i]);
            }
        }
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

int main(int argc, char **argv)
{
    int count;

    int grau = atoi(argv[2]);
    if (grau < 1){
        printf("[!] Grau do polinomio deve ser no minimo 1!\n");
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
    
    double **A;
    double *b;
    A = createMatrix(count, n);
    b = createArray(count);
    
    i=0; 
    while((line=fgets(buffer,sizeof(buffer),f))!=NULL)
    {
        //printf("Retrievedlineoflength%zu:\n",read);
        //printf("%s",line);
        char *tmp;
        tmp=strdup(line);
        b[i] = atof(getfield(tmp, 3));
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

    showSystem(A, "A", count, n);
    showArray(b,"b", count);

    double **At;
    At = transposeMatrix(A, count, n);
    showSystem(At, "At", n, count);

    double **M;
    M = matrixMultiplication(At, A, n, count, n);
    showSystem(M, "M", n, n);
   
    double *bp;
    bp = arrayMultiplication(At, b, n, count);
    showArray(bp,"bp", n);

    printf("[*] Cholesky\n");

    double **L;
    L = cholesky(M, n);
    showSystem(L, "L", n, n);

    double *y;
    y = createArray(n);
    choleskySolucion(L, bp, y, n);
    showArray(y,"y", n);
    
    double **Lt;
    Lt = transposeMatrix(L, n, n);
    showSystem(Lt, "Lt", n, n);

    double *c;
    c = createArray(n);
    choleskySolucion(Lt, y, c, n);
    showArray(c,"c", n);

    FILE *fp = fopen("results.dat", "w+");
    for(i=0;i<n;i++)
        fprintf(fp, "%lf\n", c[i]);
    fclose(fp);

    return 0;
}
