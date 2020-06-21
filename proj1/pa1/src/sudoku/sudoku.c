#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

/*Function takes in sudoku and checks if it is correct*/
/*To check rows, columns and 3x3 squares I compare the sum and product of all 9 numbers to make sure it is 45 and 362880 respectively*/
int checkSolve(char** sudoku){ 
    int i,j;
    int sum = 0;
    int product = 1;
    /*Traverses rows and makes sure each row has the correct sum and product*/
    for(i = 0;i < 9;i++){
        for(j = 0;j < 9;j++){
            sum = sum + (sudoku[i][j] - '0');/*The - '0' turns the char in the sudoku to an int*/
            product = product * (sudoku[i][j] - '0');
        }
        if(sum != 45 || product != 362880){
            printf("incorrect\n");
            return 0;
        }
        sum = 0;
        product = 1;
    }
    int a,b;
    /*Traverses columns and makes sure each column has the correct sum and product*/
    for(a = 0;a < 9;a++){
        for(b = 0;b < 9;b++){
            sum = sum + (sudoku[b][a] - '0');
            product = product * (sudoku[b][a] - '0');
        }
        if(sum != 45 || product != 362880){
            printf("incorrect\n");
            return 0;
        }
        sum = 0;
        product = 1;
    }
    int k,p,s;
    /*Traverses each 3x3 square and makes sure each column has the correct sum and product*/
    for(k = 0;k < 9;k++){
        for(p = (k / 3) * 3;p < (k / 3) * 3 + 3;p++){
            for(s = (k % 3) * 3;s < (k % 3) * 3 + 3;s++){
                sum = sum + (sudoku[p][s] - '0');
                product = product * (sudoku[p][s] - '0');
            }
        }
        if(sum != 45 || product != 362880){
            printf("incorrect\n");
            return 0;
        }
        sum = 0;
        product = 1;
    }
    printf("correct\n");
    return 0;
}

/*The exact same function as checkSolve expcept it doesn't print anything, but returns 0 if incorrect and 1 if correct*/
/*Used in solve*/
int checkSolveForSolving(char** sudoku){
    int i,j;
    int sum = 0;
    int product = 1;
    for(i = 0;i < 9;i++){
        for(j = 0;j < 9;j++){
            sum = sum + (sudoku[i][j] - '0');
            product = product * (sudoku[i][j] - '0');
        }
        if(sum != 45 || product != 362880){
            return 0;
        }
        sum = 0;
        product = 1;
    }
    int a,b;
    for(a = 0;a < 9;a++){
        for(b = 0;b < 9;b++){
            sum = sum + (sudoku[b][a] - '0');
            product = product * (sudoku[b][a] - '0');
        }
        if(sum != 45 || product != 362880){
            return 0;
        }
        sum = 0;
        product = 1;
    }
    int k,p,s;
    for(k = 0;k < 9;k++){
        for(p = (k / 3) * 3;p < (k / 3) * 3 + 3;p++){
            for(s = (k % 3) * 3;s < (k % 3) * 3 + 3;s++){
                sum = sum + (sudoku[p][s] - '0');
                product = product * (sudoku[p][s] - '0');
            }
        }
        if(sum != 45 || product != 362880){
            return 0;
        }
        sum = 0;
        product = 1;
    }
    return 1;
}

/*Function figures out if sudoku with 1 or 2 missing spots is solvable*/
/*Algorithm simply just tries each combination and checks if it is a valid solution*/
int solve(char** sudoku,int numUnknowns){
    /*If number of unknowns is only 1 it finds that one missing spot*/
    if(numUnknowns == 1){
        int i,j;
        int count2 = 0;
        for(i = 0;i < 9;i++){
            for(j = 0;j < 9;j++){
                if(sudoku[i][j] == ' '){
                    count2 = 1;
                    break;
                }
            }
            if(count2 == 1){
                break;
            }    
        }
        int k;
        /*Tries 1-9 in the spot to see if any valid solution is possible*/
        for(k = 1;k < 10;k++){
            sudoku[i][j] = k + '0';
            if(checkSolveForSolving(sudoku) == 1){
                printf("solvable");
                return 0;
            }
        }
        printf("unsolvable");
        return 0;
    }
    /*If number of unknowns is 2 it finds both missing spots*/
    if(numUnknowns == 2){
        int count = 0;
        int a,b;
        int x,y;
        for(a = 0;a < 9;a++){
            for(b = 0;b < 9;b++){
                if(sudoku[a][b] == ' '){
                    count++;
                    if(count == 2){
                        break;
                    }
                    x = a;
                    y = b;    
                }
            }
            if(count == 2){
                break;
            }
        }
        
        int h,p;
        /*Tries 1-9 in both spots to see if any valid solution is possible*/
        for(h = 1;h < 10;h++){
            sudoku[a][b] = h + '0';
            for(p = 1;p < 10;p++){
                sudoku[x][y] = p + '0';
                if(checkSolveForSolving(sudoku) == 1){
                    printf("solvable");
                    return 0;
                }
            }
        }
        printf("unsolvable");
        return 0;
    }
    return 0;
}

/*Function reads sudoku from file and either sees if it is correct or sees if it can be correct if it has empty slots*/
int main(int argc,char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if(fp == 0){
        printf("error\n");
        return 0;
    }
    /*Creates and allocates space for a 2D array*/
    char **sudoku;
    sudoku = malloc(9 * sizeof(*sudoku));
    int i;
    for(i = 0;i < 9;i++){
        sudoku[i] = malloc(9 * sizeof(sudoku[i]));
    }
    char character;
    int needSolve = 0;
    int c1 = 0;
    int c2 = 0;
    /*Reads in the file character by character and fills up sudoku*/
    while((character = fgetc(fp))){
        if(character == EOF){
            break;
        }
        if(character != ' ' &&  !isdigit(character) && character != '\n'){/*If the file contains anything, but a ' ', number or new line print error*/
            printf("error");
            return 0;
        } 
        if(character == ' '){/*Counts up how many blanks there are*/
            needSolve++;
        }
        if(character == '\n'){
            c1 = 0;
            c2++;
        }
        else{
            sudoku[c2][c1] = character;
            c1++;
        }
    }
    /*If no blanks just check if sudoku is correct*/
    if(needSolve == 0){
        checkSolve(sudoku);
        return 0;
    }
    /*If 1 or 2 blanks see if there any correct possible solutions*/
    else if(needSolve > 0){
        solve(sudoku,needSolve);
        return 0;
    }
    int w;
    for(w = 0;w < 9;w++){
        free(sudoku[w]);
    }
    free(sudoku);
    return 0;
}
