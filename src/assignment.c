#include "auxiliar.h"

/*
 * Num de coeficientes do polinomio
 * Grau 1 = 4
 * Grau 2 = 8
 * Grau 3 = 10
 */

int i,j,k;

double **cholesky(double **A, int n){
    double sum;
    double **G;

    G = createMatrix(n, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < (i+1); j++) {
            double s = 0;
            for (int k = 0; k < j; k++){
                s += G[i][k] * G[j][k];
            }
            if(i == j){
                G[i][j] = sqrt(A[i][i] - s);
            } else {
                G[i][j] = (1.0 / G[j][j] * (A[i][j] - s));
            }
        }
    }
    return G;
}

void linearSolucion(double **a, double b[], double x[], int n)
{
    double sum;
    for(i = 0; i < n; ++i) {
        sum = 0;
        for(j = 0; j < i; ++j)
            sum += a[i][j] * x[j];
        x[i] = (b[i] - sum) / a[i][i];
    }
}

void choleskySolucion(double **a, double b[], double x[], int n)
{
    double sum;
    for (i=n-1;i>=0;i--) {
        sum=0;
        for(j=n-1;j>i;j--)
            sum += a[i][j] * x[j];
        x[i] = (b[i] - sum) / a[i][i];
    }
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

void choleskyTest(){
    double **M;
    M = createMatrix(3, 3);
    M[0][0] = 4.0;
    M[0][1] = 12.0;
    M[0][2] = -16.0;
    M[1][0] = 12.0;
    M[1][1] = 37.0;
    M[1][2] = -43.0;
    M[2][0] = -16.0;
    M[2][1] = -43.0;
    M[2][2] = 98.0;
    double **L;
    L = cholesky(M, 3);
    //showSystem(M, "M", 3, 3);
    //showSystem(L, "L", 3, 3);
}

int main(int argc, char **argv)
{
    choleskyTest();

    int count;
    int grau = 0;
    if(argc == 1){
        printf("Usage: ./output <arquivo> <grau do polinomio>\n");
        exit(EXIT_FAILURE);
    }
    if(argc == 2){
        printf("[*] Grau de polinomio padrao = 3\n");
        grau = 3;
    } else{
        grau = atoi(argv[2]);
    }
    
    if (grau > 3){
        printf("[!] Grau do polinomio deve ser no maximo 3! Mudando grau para 3!\n");
        grau = 3;
    }
    if (grau < 1){
        printf("[!] Grau do polinomio deve ser no minimo 1! Mudando grau para 1!\n");
        grau = 1;
    }
   
    int n = grau+1; 
    int m = 0;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if(i+j < n){
                m++;
            }
        }
    }
    /*
    int m = 2*grau + 2;
    */

    char *data, *result;
    data = malloc(strlen(argv[1]) + 1);
    result = malloc(strlen(argv[1]) + 1);
    strcpy(data, argv[1]);
    strcpy(result, argv[1]);
    
    FILE *f = fopen(strcat(data, ".csv"), "r");
    char *record, *line, buffer[1024];
    size_t len=0;
    ssize_t read;

    if(f==NULL)
        exit(EXIT_FAILURE);

    count = numberOfLines(f);
    
    double **A;
    double *b;
    A = createMatrix(count, m);
    b = createArray(count);

    int l=0; 
    while((line=fgets(buffer,sizeof(buffer),f))!=NULL)
    {
        //printf("Retrievedlineoflength%zu:\n",read);
        //printf("%s",line);
        char *tmp;
        tmp=strdup(line);
        b[l] = atof(getfield(tmp, 3));
        free(tmp);
        
        tmp=strdup(line);
        double x = atof(getfield(tmp, 1));
        free(tmp);
        tmp=strdup(line);
        double y = atof(getfield(tmp, 2));
        free(tmp);
 
        int w = 0;
        for(i=0; i<n; i++){
            for(j=0; j<n; j++){
                if(i+j < n){
                    A[l][w] = pow(x, i)*pow(y, j);
                    w++;
                }
            }
        }
        /*
        A[l][0] = 1.0;
        A[l][1] = x;
        A[l][2] = y;
        A[l][3] = x*y;
        for(j=4; j < m; j++){
            if(j % 2 == 0){
                A[l][j] = A[l][j-1]*A[l][1];
            } else {
                A[l][j] = A[l][j-1]*A[l][2];
            }
        }
        */
        l++;
    }

    fclose(f);
    if(line)
        free(line);
    
    //if(grau < 3) showSystem(A, "A", count, m);
    //if(grau < 10) showArray(b,"b", count);

    double **At;
    At = transposeMatrix(A, count, m);
    //if(grau < 3) showSystem(At, "At", m, count);

    double **M;
    M = matrixMultiplication(At, A, m, count, m);
    //if(grau < 3) showSystem(M, "M", m, m);
   
    double *bp;
    bp = arrayMultiplication(At, b, m, count);
    //if(grau < 10) showArray(bp,"bp", m);

    //printf("[*] Cholesky\n");

    double **L;
    L = cholesky(M, m);
    //if(grau < 3) showSystem(L, "L", m, m);
    
    double *y;
    y = createArray(m);
    linearSolucion(L, bp, y, m);
    //if(grau < 10) showArray(y,"y", m);
    
    double **Lt;
    Lt = transposeMatrix(L, m, m);
    //if(grau < 3) showSystem(Lt, "Lt", m, m);

    double *c;
    c = createArray(m);
    choleskySolucion(Lt, y, c, m);
    //if(grau < 10) showArray(c,"c", m);

    double *r;
    r = arrayMultiplication(A, c, count, m);
    //if(grau < 10) showArray(r, "r", count);
    //if(grau < 10) showArray(b,"b", count);

    FILE *fp = fopen(strcat(result,".dat"), "w+");
    for(i=0;i<m;i++)
        fprintf(fp, "%lf;\n", c[i]);
    fclose(fp);

    return 0;
}
