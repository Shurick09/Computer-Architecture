#include <stdio.h>
#include <string.h>

int main (int argc, char** argv){
/*Takes a  string and sorts the letters
 * Uses insertion sort */ 

    int i,c,tr,r;
    int len = strlen(argv[1]);
    char fin[len];
    fin[0] = argv[1][0];/*Makes the first letter of the input the first letter of the ordered string*/
    for (i = 1; i < len; i++){
        c = 0;
        tr = 0;     
        while(c!= i){
            if(argv[1][i] < fin[c]){ /*Traverses the final string until it finds place for current letter*/
                int j;
                for(j = len-1; j > c; j--){/*Moves the letters one spot to the right to make room for the current letter*/
                fin[j] = fin[j-1];
                }
                fin[c] = argv[1][i];
                tr = 1;
                break;        
            }
            c++;
        }
        if(tr == 0){
             fin[c] = argv[1][i];
        }
    }
    for(r = 0; r < len; r++){/*Prints the final string letter by letter*/
        printf("%c",fin[r]);
    }
    printf("\n");
    return 0;
}
