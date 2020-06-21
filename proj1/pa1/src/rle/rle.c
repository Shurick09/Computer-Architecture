#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char* recRle(char* orig,char* comp){
/*Recursive function that takes the original string and the compressed string.
 *Function finds how many consecutive characters there are in the beginning of the string.
 *After it finds a different character it calls itself with the original string minus the letters already inspected and the compressed string.
 *Once the string is empty it returns the final compressed string.*/

    if (orig[0] == '\0'){ /*Base case: returns compressed string when original is traversed*/
        return comp;
    }
    char first = orig[0];
    int c = 0;
    while (orig[0] == first){
        memmove(orig,orig+1,strlen(orig));  /*Cuts the first character*/
        c++;
    }
    char buffer[20];
    char temp[2];
    temp[0] = first;
    temp[1] = '\0';
    sprintf(buffer,"%d",c);
    strcat(comp,temp);
    strcat(comp,buffer); /*Adds the character and how many of them to compressed*/
    return  recRle(orig,comp); /*Recursive call*/
}

int main(int argc, char** argv){
/*Function takes a string and compresses it*/
/* Example aabbcc -> a2b2c2*/
    int i;
    int len = strlen(argv[1]);
    char* input = strdup(argv[1]); 
    char* comp = (char*)malloc(2 * len * sizeof(char) + 1); /*Allocates space for compressed string*/
    for (i = 0; i < strlen(argv[1]); i++){ 
        if (isdigit(argv[1][i])){ /*If string has a digit prints "error"*/
            printf("error \n");
            return 0;
        }
    }
    if (strlen(argv[1]) == 1){ /*If string has one character prints it*/
        printf("%s\n",argv[1]);
        return 0;
    }  
    comp = recRle(argv[1], comp); /*Calls recursive function*/
    if (strlen(comp) > len){ /*If compressed string is longer than original prints original*/
        printf("%s\n", input);
    }
    else{
        printf("%s\n", comp);
    }
    free(comp); /*Frees memory allocated*/
    free(input);
    return 0;
}
