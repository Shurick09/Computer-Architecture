#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Function prints a matrix
 * Used for testing at each step*/
void print(double** matrix,int rows,int cols){
    int i,j;
    for(i = 0;i < rows;i++){
        for(j = 0;j < cols;j++){
            printf("%.6f ",matrix[i][j]);
        }
        printf("\n");
    }
}

/*Prints a matrix that has either 1 row or 1 column
 * Used for testing at different steps*/
void print1D(double* array,int rows){
    int i;
    for(i = 0;i < rows;i++){
        printf("%.6f ",array[i]);
    }
    printf("\n");
}

/*Same as previous function except rounds numbers to nearest digit
 * Used to print list of final house prices*/
void print1DFinal(double* array, int rows){
    int i;
    for(i = 0;i < rows;i++){
        printf("%.0f\n",array[i]);
    }
}

/*Function creates an identity matrix of numRows size
 * Used for Gaussian elimination step*/
double** identityFunc(int numRows,double** identity){
    int i,j;
    for(i = 0;i < numRows;i++){
        for(j = 0;j < numRows;j++){
            if(i == j){/*If element is on diagonal insert a 1*/
                identity[i][j] = 1;
            }
            else{/*All other spots should be 0*/
                identity[i][j] = 0;
            }
        }
    }
    return identity;
}

/*Computes the xSquare^-1 using the Gaussian elimination algorithm*/
double** invert(int attributes, double** xSquare){
    double** identity;
    identity = malloc((attributes + 1) * sizeof(*identity));
    int i;
    for(i = 0;i < attributes + 1;i++){
        identity[i] = malloc((attributes + 1) * sizeof(*identity[i]));
    }
    identity = identityFunc(attributes + 1,identity);/*Creates an identity matrix of the same size as xSquare*/
    int j,k;
    /*First part puts original matrix in upper triangular form*/
    for(j = 0;j < attributes + 1;j++){/*Traverses each column*/
        for(k = j;k < attributes + 1;k++){/*Traverses the rows starting at each column's pivot element*/
            if(k == j){/*If it is pivot element*/
                if(xSquare[k][j] != 1){/*If pivot element is not 1 divide the entire row by that element to make it 1*/
                    double temp = xSquare[k][j];
                    int p;
                    for(p = 0;p < attributes + 1;p++){
                        xSquare[j][p] = xSquare[j][p] / temp;/*All operations are done both to the original matrix and the identity matrix*/
                        identity[j][p] = identity[j][p] / temp;
                    }
                }
            }
            else{/*If it is not pivot element*/
                if(xSquare[k][j] != 0){/*If pivot element is not 0 must multiply pivot point by that element and subtract the two rows*/
                    double temp = xSquare[k][j];
                    int g;
                    for(g = 0;g < attributes + 1;g++){
                        xSquare[k][g] = temp * xSquare[j][g] - xSquare[k][g]; 
                        identity[k][g] = temp * identity[j][g] - identity[k][g];
                    }
                }
            }
        }
    }
    /*Second part makes all the elements above the pivots 0 turning the original matrix into an identity matrix*/
    int a,b;
    for(a = attributes; a >= 0;a--){/*Traverses each column starting from the last one*/
        for(b = a;b >= 0;b--){/*Traverses the rows from the bottom to the top and starts at each column's pivot element*/
            if(a != b){/*If it is not a pivot element*/
                if(xSquare[b][a] != 0){/*If the element is not 0 must multiply pivot point by that element and subtract the two rows*/
                    double temp = xSquare[b][a];
                    int o;
                    for(o = 0;o < attributes + 1;o++){
                        xSquare[b][o] = xSquare[b][o] - (temp * xSquare[a][o]);
                        identity[b][o] = identity[b][o] -  (temp * identity[a][o]);
                    }
                }
            }
        }
    }
    return identity;
}

/*Multiplies matrices xTranspose and x and puts the result in product*/
double** mult(double** xTranspose, double** x, double** product, int numRowsTrans, int numColsTrans, int numRows, int numCols){
    int i,j,k;
    double sum = 0;
    for(i = 0;i < numRowsTrans;i++){
        for(j = 0;j < numCols;j++){
            sum = 0;
            for(k = 0;k < numColsTrans;k++){
                sum = sum + (xTranspose[i][k] * x[k][j]);
            }
            product[i][j] = sum;
        }
    }
    return product;
}

/*Multpiplies a 2D  matrix almostFinal with a 1D matrix y amd puts the result in final*/
double* multWith1D(double** almostFinal, double* y, double* final, int numRows, int numCols){
    int i,k;
    double sum = 0;
     for(i = 0;i < numRows;i++){
            sum = 0;
            for(k = 0;k < numCols;k++){
                sum = sum + (almostFinal[i][k] * y[k]);
            }
            final[i] = sum;    
    }
    return final;

}

/*Finds the transpose of x and puts it into xTranspose*/
double** transpose(int houses, int attributes, double** x, double** xTranspose){
    int i,j;
    for(i = 0;i < houses;i++){
        for(j = 0;j < attributes + 1;j++){
            xTranspose[j][i] = x[i][j];
        }
    }    
    return xTranspose;
}

/*Calculates W or the pseudo-inverse using the formula W = (X^T*X)^-1*X^T*Y
 * Allocates space and computes each matrix in steps*/
double* calcW(int houses,int attributes, double** x, double* y, double* w){
    double** xTranspose;
    xTranspose = malloc((attributes + 1) * sizeof(*xTranspose));
    int s;
    for(s = 0; s < attributes + 1;s++){
        xTranspose[s] = malloc((houses) * sizeof(*xTranspose[s]));
    }
    xTranspose =  transpose(houses,attributes,x,xTranspose);/*Transpose*/
    double** xSquare;
    xSquare = malloc((attributes + 1) * sizeof(*xSquare));
    int u;
    for(u = 0; u < attributes + 1;u++){
        xSquare[u] = malloc((attributes + 1) * sizeof(*xSquare[u]));
    }
    xSquare = mult(xTranspose,x, xSquare,attributes + 1,houses,houses,attributes+1);/*X^T*X*/
    double** xInverted;
    xInverted = malloc((attributes + 1) * sizeof(*xInverted));
    int f;
    for(f = 0;f < attributes + 1;f++){
        xInverted[f] = malloc((attributes + 1) * sizeof(*xInverted[f]));
    }
    xInverted = invert(attributes,xSquare);/*(X^T*X)^-1*/
    double** almostFinal;
    almostFinal = malloc((attributes + 1) * sizeof(*almostFinal));
    int h;
    for(h = 0;h < attributes + 1;h++){
        almostFinal[h] = malloc((houses) * sizeof(*almostFinal[h]));
    }
    almostFinal = mult(xInverted,xTranspose,almostFinal,attributes + 1,attributes + 1,attributes + 1, houses);/*(X^T*X)^-1*X^T*/
    w = multWith1D(almostFinal,y,w,attributes + 1,houses);/*(X^T*X)^-1*X^T*Y*/
    int aa,ab,ac,ad;
    for(aa = 0;aa < attributes + 1;aa++){
        free(xTranspose[aa]);
    }
    free(xTranspose);
    for(ab = 0;ab < attributes + 1;ab++){
        free(xSquare[ab]);
    }
    free(xSquare);
    for(ac = 0;ac < attributes + 1;ac++){
        free(xInverted[ac]);
    }
    free(xInverted);
    for(ad = 0;ad < attributes + 1;ad++){
        free(almostFinal[ad]);
    }
    free(almostFinal);
    return w;
}

/*Main function takes two files. Train has data on houses and their prices. The weight of different attributes needs to be computed.
 * This is a system of equations problem. Once the weights are computed use the data file with different attributes of house to determine their prices.*/
int main(int argc, char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if (fp == 0){
        printf("error\n");
        exit(1);
    }
    char type[6];
    int attributes, houses;
    double** x;
    double* y;
    char train[6];
    char data[5];
    strcpy(train,"train");
    strcpy(data,"data");
    fscanf(fp,"%5s",type);
    if (strcmp(type,train) != 0){/*If first file is not train report an error*/
        printf("error\n");
        exit(1);
    }
    fscanf(fp,"%d",&attributes);
    fscanf(fp,"%d",&houses);
    x = malloc((houses) * sizeof(*x));
    int i;
    for(i = 0; i < houses;i++){
        x[i] = malloc((attributes + 1) * sizeof(*x[i]));
    }
    y = malloc(houses * sizeof(double));
    int a,b;
    /*Fill in x with 1's on the left most column and the attributes on the right
     *Fills in y with the prices*/
    for(a = 0; a < houses;a++){
        for(b = 0; b < attributes + 2;b++){
            if(b == 0){
                x[a][0] = 1;
            }
            else if(b == attributes + 1){
                fscanf(fp,"%lf",&y[a]);
            }
            else{
                fscanf(fp,"%lf",&x[a][b]);
            }
        }
    }
    fclose(fp);
    double* w;
    w = malloc((attributes + 1) * sizeof(double));
    w = calcW(houses,attributes,x,y,w);
    FILE* fp2;
    fp2 = fopen(argv[2],"r");
    if (fp2 == 0){
        printf("error\n");
        exit(1);
    }
    char type2[5];
    fscanf(fp2,"%4s",type2);
    if(strcmp(type2,data) != 0){/*If the second file is not data report an error*/
        printf("error\n");
        exit(1);
    }
    int attributes2;
    int houses2;
    fscanf(fp2,"%d",&attributes2);
    fscanf(fp2,"%d",&houses2);
    if(attributes != attributes2){/*If the attributes for both files is not the same report an error*/
        printf("error");
        exit(1);
    }
    double** dataMatrix;
    dataMatrix = malloc(houses2 * sizeof(*dataMatrix));
    int z;
    for(z = 0;z < houses2;z++){
        dataMatrix[z] = malloc((attributes2 + 1) * sizeof(*dataMatrix[z]));
    }
    int v,n;
    /*Fills in dataMatrix with 1's on the left most column and the attributes to the right*/
    for(v = 0;v < houses2;v++){
        for(n = 0;n < attributes2 + 1;n++){
            if(n == 0){
                dataMatrix[v][0] = 1;
            }
            else{
                fscanf(fp2,"%lf",&dataMatrix[v][n]);
            }
        }
    }
    fclose(fp2);
    double* result;
    result = malloc(houses2 * sizeof(double));
    result = multWith1D(dataMatrix,w,result,houses2,attributes + 1);/*X' * W = Y'*/
    print1DFinal(result,houses2);
    int ba,bb;
    for(ba = 0;ba < houses;ba++){
        free(x[ba]);
    }
    free(x);
    free(y);
    for(bb = 0;bb < houses2;bb++){
        free(dataMatrix[bb]);
    }
    free(dataMatrix);
    free(result);
    free(w);
    return 0;
}
