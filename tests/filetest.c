#include <stdio.h>

int main() {

    FILE *f; //pointer to a FILE structure
    
    char buffer[100]; //array of chars

    f = fopen("test.txt", "r"); //opens the file and stores pointer in f
    fscanf(f, "%s", buffer); //read from the file and store into buffer
    printf("%s\n", buffer); // print out the buffer

    fclose(f);   
 
    return 0;
}
