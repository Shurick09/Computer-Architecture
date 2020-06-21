#include <stdio.h>
#include <stdlib.h> //needed for malloc and free

int main (int argc, char **argv) {
    
    int numChars = 20;
    char *ptr = (char *)  malloc(numChars * sizeof(char)); // returns a pointer
    //a chunk of memory

    ptr = "Hello"; //WRONG. Changes the address contained in ptr.

    char buffer[] = "Hello";
    strcpy(ptr, buffer);

    return 0;
