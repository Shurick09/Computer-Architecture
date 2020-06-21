#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*Defines the linked list structure*/
typedef struct node{
    int val;
    struct node* next;
}node;

/*Function that creates and inserts a node if that node doesn't already exist*/
node* insert(int val,node* head){
    node *ptr, *prev;
    ptr = head;
    prev = NULL;
    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = val;
    newNode->next = NULL;
    if(head == NULL){
        return newNode;
    }
    if(val < head->val){/*If the new node needs to be inserted at the beginning of the linked list*/
        newNode->next = head;
        return newNode;
    }
    while(ptr != NULL && ptr->val <= val){/*Looks for the correct spot to put the node*/
        if(ptr->val == val){/*Checks if the node already exists*/
            return head;
        }
        prev = ptr;
        ptr = ptr->next;
    } 
    newNode->next = ptr;/*Puts the new node in the appropriate spot*/
    prev->next = newNode;
    return head;
}

/*Function that deletes a node if that node is in the linked list*/
node* delete(int val, node* head){
    if(head == NULL){
        return head;
    }
    node *ptr;
    node *prev;
    ptr = head;
    if(ptr->val == val){/*If the node that needs to be deleted is the front*/
        head = head->next;
        ptr->next = NULL;
        if(ptr == head){/*If the one that needs to be deleted is the only node of the linked list*/
            head = NULL;
        }
        return head;
    }
    ptr = ptr->next;
    prev = head;
    while(ptr != NULL){/*Looks for the node to delete*/
        if(ptr->val == val){/*If it finds the node then it deletes it*/
            prev->next = ptr->next;
            return head;
        }
        ptr = ptr->next;
        prev = prev->next;
    }
    return head;
}

/*Function that counts and outputs how many nodes are in the linked list*/
void count(node* head){
    int count = 0;
    while(head != NULL){
        count++;
        head = head->next;
    }
    printf("%d\n",count);
}

/*Prints the nodes of the linked list*/
void print(node* head){
    while(head != NULL){
        if(head->next == NULL){
            printf("%d\n",head->val);
            break;
        }
        printf("%d ",head->val);
        head = head->next;
    }
}

/*Main functin that reads in file input and makes the linked list by inserting and deleting nodes*/
int main(int argc, char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if (fp == 0){
        printf("error\n");
        return 0;
    }
    char instruction;
    int value;
    node *head = NULL;
    while(fscanf(fp, " %c %d", &instruction, &value) != EOF) {
        if(instruction == 'i'){
             head = insert(value,head);
        }
        else if(instruction == 'd'){
            head = delete(value,head);
        }
    }
    fclose(fp);
    count(head);
    print(head);
    free(head);
    head = NULL;
    return 0;
}

