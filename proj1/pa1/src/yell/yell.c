#include <stdio.h>
#include <string.h>
#include <ctype.h>

int lc(char c){
/* Checks if char is a lowercase letter*/
    if (c >= 'a' && c <= 'z'){
        return 1;
    }
    else{
        return 0;
    }
}


int main(int argc, char **argv){
/* Prints the input statement only with each letter capitalized*/
    int i,j;
    for (i = 1; i < argc; i++){
        for (j = 0; j < strlen(argv[i]); j++){
            if (lc(argv[i][j])){ /*If char is lowercase letter print it capitalized*/
                printf("%c",toupper(argv[i][j]));
            }
            else{ /*If char is not a letter or already capitalized print it as is*/
                printf("%c",argv[i][j]);
            }
        }
        printf(" ");    
    }   
    printf("\n");              
    return 0;
}








