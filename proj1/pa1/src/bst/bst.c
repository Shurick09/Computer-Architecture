#include <stdio.h>
#include <stdlib.h>

/*Defines a node of a BST*/
typedef struct node{
    int val;
    struct node* left;
    struct node* right;
}node;

/*Recursive algorithm prints bst in parentheses with left to right nodes*/
int print(node* bst){
    if(bst == NULL){
        return 0;
    }
    node* ptr;
    ptr = bst;
    printf("(");
    print(ptr->left);
    printf("%d",ptr->val);
    print(ptr->right);
    printf(")");
    return 0;
}

/*Inserts val into the bst*/
/*The left child is smaller than its parent and the right child is bigger than its parent*/
node* insert(node* bst,int val){
    node* newNode;
    newNode = (node*)malloc(sizeof(node));
    node *ptr,*prev;
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    /*If bst is empty make the bst just one node*/
    if(bst == NULL){
        bst = newNode;
        printf("inserted\n");
        return bst;
    }
    ptr = bst;
    prev = NULL;
    char dir;
    /*Look for the correct spot to insert the node*/
    while(ptr != NULL){
        prev = ptr;
        /*If val already exists print duplicate and don't add another*/
        if(ptr->val == val){
            printf("duplicate\n");
            return bst;
        }
        /*Go left or right depending on if the val is smaller or greater than the current node*/
        else if(val < ptr->val){
            dir = 'l';
            ptr = ptr->left;
        }
        else if(val > ptr->val){
            dir = 'r';
            ptr = ptr->right;
        }
    }
    /*Depending on if node is smaller or bigger than parent add it in its left or right subtree*/
    if(dir == 'l'){
        prev->left = newNode;
        printf("inserted\n");
    }
    else if(dir == 'r'){
        prev->right = newNode;
        printf("inserted\n");
    }
    return bst;
}

/*Searches for val in bst*/
int search(node* bst,int val){
    node *ptr;
    ptr = bst;
    if(bst == NULL){
        printf("absent\n");
        return 1;
    }
    /*Traverses bst looking for val*/
    while(ptr != NULL){
        if(ptr->val == val){
            printf("present\n");
            return 0;
        }
        else if(val < ptr->val){
            ptr = ptr->left;
        }
        else if(val > ptr->val){
            ptr = ptr->right;
        }
    }
    printf("absent\n");
    return 1;
}

/*Same function as search except does not print anything*/
/*Used in delete*/
int searchForDelete(node* bst,int val){
    node *ptr;
    ptr = bst;
    if(bst == NULL){
        return 1;
    }

    while(ptr != NULL){
        if(ptr->val == val){
            return 0;
        }
        else if(val < ptr->val){
            ptr = ptr->left;
        }
        else if(val > ptr->val){
            ptr = ptr->right;
        }
    }
    return 1;
}

/*Deletes val from bst
 * Three deletion cases:
 * If node has no children just delete that node
 * If node has one child just delete it and replace it by its one child
 * If node has two children replace with maximum node in left subtree*/
node* delete(node* bst,int val){
    /*If val is not in tree just print absent*/
    if(searchForDelete(bst,val) == 1){
        printf("absent\n");
        return bst;
    }
    node *ptr,*prev;
    ptr = bst;
    prev = NULL;
    char dir;
    /*Traverses tree looking for node to delete*/
    while(ptr != NULL){
        if(val == ptr->val){
            /*First case when node has no children*/
            if(ptr->left == NULL && ptr->right == NULL){
                if(dir == 'l'){
                    prev->left = NULL;
                }
                else if(dir == 'r'){
                    prev->right = NULL;
                }
                printf("deleted\n");
                return bst;
            }
           /*Second case when node has one child*/
            else if(ptr->left == NULL || ptr->right == NULL){
                if(ptr->left == NULL){
                    if(dir == 'l'){
                        prev->left = ptr->right;
                        printf("deleted\n");
                        return bst;
                    }
                    else if(dir == 'r'){
                        prev->right = ptr->right;
                        printf("deleted\n");
                        return bst;
                    }
                }
                else if(ptr->right == NULL){
                    if(dir == 'l'){
                        prev->left = ptr->left;
                        printf("deleted\n");
                        return bst;
                    }
                    else if(dir == 'r'){
                        prev->right = ptr->left;
                        printf("deleted\n");
                        return bst;
                    }
                }
            }
            /*Third case when node has two children*/
            else if(ptr->left != NULL && ptr->right != NULL){
                prev = ptr;
                ptr = ptr->left;
                while(ptr->right != NULL){
                    ptr = ptr->right;
                }
                prev->val = ptr->val;
                ptr = NULL;
                printf("deleted\n");
                return bst;
            }
        }
        else if(val < ptr->val){
            prev = ptr;
            ptr = ptr->left;
            dir = 'l';
        }
        else if(val > ptr->val){
            prev = ptr;
            ptr = ptr->right;
            dir = 'r';
        }
    }
    return bst;
}

/*Main function that reads in instructions of making a bst
 * Inserting, deleting, searching, and printing are options*/
int main(int argc,char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if(fp == 0){
        printf("error\n");
    }
    node *bst = NULL; 
    int val;
    char instruction;
    while(fscanf(fp,"%c ",&instruction) != EOF){
        if(instruction == 'i'){
            fscanf(fp,"%d",&val);
            bst = insert(bst,val);
        }
        else if(instruction == 's'){
            fscanf(fp,"%d",&val);
            search(bst,val);
        }

        else if(instruction == 'd'){
            fscanf(fp,"%d",&val);
            bst = delete(bst,val);
        }
        else if(instruction == 'p'){
            print(bst);
            printf("\n");
        }
    }
    free(bst);    
    return 0;
}
