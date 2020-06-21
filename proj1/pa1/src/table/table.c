#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Defines linked list*/
typedef struct node{
    int val;
    struct node* next;
}node;

/*Hashing function*/
int hashCode(int val){
    return val % 10000;
}

/*Function inserts values into the hashtable*/
node* insert(node* list, int val){
    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = val;
    node *ptr,*prev;
    ptr = list;
    prev = NULL;
    if(list == NULL){/*If spot in array has no values make the value the first node*/
        newNode->next = NULL;
        list = newNode;
        printf("inserted\n");
        return list;
    }
    else{
        while(ptr != NULL){/*If linked list has other values traverse until the end and then add the node*/
            if(ptr->val == val){/*If value already exists print duplicate*/
                printf("duplicate\n");
                return list;
            }
            prev = ptr;
            ptr = ptr->next;
        }
        prev->next = newNode;
        newNode->next = NULL;
        printf("inserted\n");
        return list;
    }
}

/*Function searches to see if the value exists in the hashtable*/
int search(node* hashTable[10000],int val){
    int pos = hashCode(val);
    node *ptr;
    ptr = hashTable[pos];
    while(ptr != NULL){/*Traverses hashtable looking for val*/
        if(ptr->val == val){
            printf("present\n");
            return 0;
        }
        ptr = ptr->next;
    }
    printf("absent\n");
    return 0;
}

/*Main function that creates a hashtable. The hashtable is an array of linked lists and is filled by reading from a file*/
int main(int argc, char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if (fp == 0){
        printf("error\n");
        return 0;
    }
    char instruction;
    int value;
    node* hashTable[10000];/*Initializes the hashtable by making an array of 10,000 linked lists*/
    int i;
    for(i = 0;i < 10000; i++){/*Allocates memory for each linked list in array*/
        hashTable[i] = (node*)malloc(sizeof(node));
        hashTable[i] = NULL;
    }
    while(fscanf(fp," %c %d",&instruction,&value) != EOF){/*Reads from the file line by line either inserting or searching the value*/ 
        if(instruction == 'i'){
            hashTable[hashCode(value)] = insert(hashTable[hashCode(value)],value);
            
        }
        else if(instruction == 's'){
            search(hashTable,value);
        }
    }
    fclose(fp);
    int j;
    for(j = 0;j < 10000;j++){/*Frees all the memory allocated*/
        free(hashTable[j]);
    }
    return 0;
}
