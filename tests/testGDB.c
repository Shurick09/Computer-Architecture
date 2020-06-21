#include <stdio.h>
#include <stdlib.h>

void testFunction(size_t  a, int *b){
    int count = 0;
    for(count = 0;count <= a;count++){
        *b += 1; //Derference b and increment what it points to*/
          /*This does not change b*/
    }
}

struct Node{
    int a;
    struct Node *next;
};

int main(int argc, char** argv){
    int x = 0;
    struct Node *node1 = (struct Node *)malloc(sizeof(struct Node));
    struct Node *node2 = (struct Node *)malloc(sizeof(struct Node));

    node1->next = node2;
    node2->next = node1;
  
    /* testFunction(5,&x);*/
    return 0;
}
