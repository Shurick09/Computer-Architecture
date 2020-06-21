#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Program creates an identity matrix of dimensions sizexsize*/
/*Is called when the exponent is 0*/
void identity(int size){
    int ident[size][size];
    int i,j;
    /*Fills diagonal with 1's and everything else with 0's*/
    for(i = 0;i < size; i++){
        for(j = 0;j < size;j++){
            if(i == j){
                ident[i][j] = 1;
                /*This printing makes the spacing write for the needed output*/
                if(j == size - 1){
                    printf("%d",ident[i][j]);
                }
                else{
                    printf("%d ",ident[i][j]);
                }
            }
            else{
                ident[i][j] = 0;
                if(j == size - 1){
                    printf("%d",ident[i][j]);
                }
                else{
                    printf("%d ",ident[i][j]);
                }
            }
        }
        printf("\n");
    }
}

/*Function multiplies the matrix by itself exp - 1 times*/
void multiply (int** matrix,int size,int exp){
     int i,j,r,q;
     int sum = 0;
     int final[size][size];
     int a,b;
     int buffer[size][size];
     /*Creates deep copy of matrix in buffer*/
     for(a = 0;a < size;a++){
         for(b = 0;b < size;b++){
             buffer[a][b] = matrix[a][b];
         }
     }
     /*Outer loop makes the matrix multiply by itself exp - 1 times*/
     for(q = 1;q < exp;q++){
         /*Three inner loops allow for the row and column multiplication to multiply the matrix*/
         for(i = 0;i < size;i++){
             for(j = 0;j < size;j++){
                 sum = 0;
                 for(r = 0;r < size;r++){
                     sum = sum + (buffer[i][r] * matrix[r][j]);
                 }
                 final[i][j] = sum;
             }
         }
         int e,w;
         /*Makes a new copy of buffer for each multiplication*/
         for(e = 0; e < size;e++){
             for(w = 0; w < size;w++){
                 buffer[e][w] = final[e][w];
             }
         }
    }
    int n,m;
    /*Prints the final matrix*/
    for(n = 0;n < size;n++){
        for(m = 0;m < size;m++){
            if(m == size - 1){
                printf("%d",final[n][m]);
            }
            else{
                printf("%d ",final[n][m]);
           }
        }
        printf("\n");
    }    
}

/*Main function reads a file with size of a matrix, the matrix, and the exponent*/
/*Then raises the matrix to the exponent*/
int main(int argc, char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if(fp == 0){
        printf("error\n");
    }
    int size;
    fscanf(fp,"%d",&size);
    int **matrix;
    /*Allocates space for matrix*/
    matrix = malloc(size * sizeof(*matrix));
    int d;
    for(d = 0;d < size;d++){
        matrix[d] = malloc(size * sizeof(*matrix[d]));
    }
    int i,j;
    for(i = 0; i < size;i++){
        for(j = 0; j < size;j++){
            fscanf(fp,"%d",&matrix[i][j]);
        }
    }
    int exp;
    fscanf(fp,"%d",&exp);
    fclose(fp);
    /*Prints matrix if exponent is 1*/
    if(exp == 1){
        int r,k;
        for(r = 0; r < size;r++){
            for(k = 0; k < size;k++){
                if(k == size - 1){
                    printf("%d",matrix[r][k]);
                }
                else{
                    printf("%d ",matrix[r][k]);
                }
             }
             printf("\n");
        }
        int m;
 
       for(m = 0;m < size;m++){
            free(matrix[m]);
        }
        free(matrix);
        return 0;
    }
    /*Prints identity of that matrix if exponent is 0*/
    if(exp == 0){
        identity(size);
        int l;
        for(l = 0;l < size;l++){
            free(matrix[l]);
        }
        free(matrix);
        return 0;
    }
    multiply(matrix,size,exp);
    int y;
    /*Frees allocated space*/
    for(y = 0;y < size;y++){
        free(matrix[y]);
    }
    free(matrix);
    return 0;
}
                                           
