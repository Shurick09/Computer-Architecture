#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*All the different circuit commands we need to account for*/
enum opCode{NOT,AND,OR,NAND,NOR,XOR,DECODER,MULTIPLEXER,PASS,IGNORE};

/*Struct for each variable*/
typedef struct operand{
    char *name;
    int val;
}operand;

/*Struct to represent the file as a linked list*/
typedef struct instrNode{
    enum opCode type;
    operand *inputs;
    operand *outputs;
    char checked;
    struct instrNode *next;
}instrNode;

/*Function to print instrNode and make sure program is reading file in properly*/
void print(instrNode* instrNode){
    while(instrNode != NULL){
        printf("%d",instrNode->type);
        printf("%c",instrNode->checked);
        int i,j;
        if(instrNode->inputs != NULL && instrNode->outputs != NULL){
            for(i = 0;i < 1;i++){
                printf("%s ",(instrNode->inputs)[i].name);
                printf("%d ",(instrNode->inputs)[i].val);
            }
            for(j = 0;j < 1;j++){
                printf("%s ",(instrNode->outputs)[j].name);
                printf("%d ",(instrNode->outputs)[j].val);
            }
        }
        printf("\n");
        instrNode = instrNode->next;
    }
}

/*Creates binary representation of a number
 * This is used to go through all combinations to create the input variables of the truth table*/
int* makeBin(int i, int numInputs){
    int j;
    int a = 0;
    int* truthTable;
    truthTable = (int*)malloc(sizeof(int) * numInputs);
    for(j = numInputs - 1; j >= 0;j--){
        truthTable[a] =  (i >> j) & 0x01;
        a++;
    }
    return truthTable;
}

/*Program tests if a variable is in an array*/
int inArray(char* name,operand* inputs,int numInputs){
    int i;
    for (i = 0;i < numInputs;i++){
        if(strcmp(inputs[i].name,name) == 0){
            return 0;
        }
    }
    return 1;
}

/*Program returns the value of a certain variable in an array*/
int getVal(char* name,operand* inputs, int numInputs){
    int i;
    for(i = 0; i < numInputs;i++){
        if(strcmp(inputs[i].name,name) == 0){
            return inputs[i].val;
        }
    }
    return 11;
}

/*This puts a certain value into a variable in an array*/
operand* putVal(char* name,operand* inputs, int numInputs, int tempNot){
    int i;
    for(i = 0;i < numInputs;i++){
        if(strcmp(inputs[i].name,name) == 0){
            inputs[i].val = tempNot;
        }
    }
    return inputs;
}

/*Function creates and prints truth table based on the amount of input variables*/
void makeTruthTable(instrNode* commands,int numInputs,int numOutputs, operand* inputs, operand* outputs, int counter){
    int i;
    int nodesChecked = 0;
    instrNode* ptr = commands;
    int* truthTable; 
    for(i = 0;i < (1 << numInputs); i++){
        truthTable = makeBin(i,numInputs);/*Make row for each combination*/
        int k;
        for(k = 0; k < numInputs;k++){
            printf("%d ",truthTable[k]);
            inputs[k].val = truthTable[k];
        }
        printf("| ");
        nodesChecked = 0;
        while (nodesChecked != counter){ /*Keeps going thrugh the nodes until all are looked at*/
            ptr = ptr->next;
            if (ptr->type == 0){
                 int tempNot;
                 if (inArray((ptr->inputs)[0].name, inputs,numInputs) == 0){
                     tempNot = getVal((ptr->inputs)[0].name, inputs, numInputs);
                     if (tempNot == 0){
                         tempNot = 1;
                     }
                     else if(tempNot == 1){
                         tempNot = 0;
                     }
                     if (inArray((ptr->outputs)[0].name,outputs,numOutputs) == 0){
                         outputs = putVal((ptr->outputs)[0].name,outputs,numOutputs,tempNot);
                     }
                 }
                 ptr->checked = 'y';
                 nodesChecked++;
            }
            else if(ptr->type == 1){

            }
            else if(ptr->type == 2){

            }
            else if(ptr->type == 3){

            }
            else if(ptr->type == 4){

            }
            else if(ptr->type == 5){

            }
            else if(ptr->type == 6){

            }
            else if(ptr->type == 7){

            }
            else if(ptr->type == 8){

            }                     
        }
        int a;
        for (a = 0;a < numOutputs;a++){/*Prints output of each row of truth table*/
                printf("%d ", outputs[a].val);
            }
        
        printf("\n");
    }
}

/*Main function. Takes in file with circuit commands and evaluates them*/
int main(int argc, char** argv){
    FILE *fp;
    fp = fopen(argv[1],"r");
    if (fp == 0){
        printf("error\n");
        exit(1);
    }/*Reads in file into instrNode linked list*/
    char* buff = (char*)malloc(500 * sizeof(char));
    fgets(buff,500,fp);
    strtok(buff,": ");
    char* numOfInputs = (char*)malloc(4 * sizeof(char));
    numOfInputs = strtok(NULL,": ");
    operand* inputs;
    inputs  = (operand*)malloc(atoi(numOfInputs) * sizeof(operand));
    int i;
    for(i = 0; i < atoi(numOfInputs);i++){
        inputs[i].name = strtok(NULL,": \n");
        inputs[i].val = -1;
    }
    int counter = 0;
    char* buff2 = (char*)malloc(500 * sizeof(char));
    fgets(buff2,500,fp);
    strtok(buff2,": ");
    char* numOfOutputs = (char*)malloc(4 * sizeof(char));
    numOfOutputs = strtok(NULL,": ");
    operand* outputs;
    outputs = (operand*)malloc(atoi(numOfOutputs) * sizeof(operand));
    int j;
    for(j = 0; j < atoi(numOfOutputs); j++){
        outputs[j].name = strtok(NULL,": \n");
        outputs[j].val = -1;
    }
    instrNode* commands;
    instrNode* newNode = (instrNode*)malloc(sizeof(instrNode));
    newNode->type = 9;
    newNode->inputs = NULL;
    newNode->outputs = NULL;
    newNode->checked = 'y';
    commands = newNode;
    char* buff3 = (char*)malloc(500 * sizeof(char));
    while(fgets(buff3,500,fp) != NULL){
        char* typeOfOp = (char*)malloc(10 * sizeof(char));
        typeOfOp = strtok(buff3,": ");
        if (strcmp(typeOfOp,"NOT") == 0){/*Different ways to read in input for each command*/
            counter++;
            instrNode* newNode1 = (instrNode*)malloc(sizeof(instrNode));
            newNode1->type = 0;
            operand* input1 = (operand*)malloc(sizeof(operand));
            char* str11 = (char*)malloc(sizeof(char) * 10);
            str11 = strtok(NULL,": \n");
            input1[0].name = str11;
            input1[0].val = -1;
            newNode1->inputs = input1;
            operand* output1 = (operand*)malloc(sizeof(operand));
            char* str12 = (char*)malloc(sizeof(char) * 10);
            str12 = strtok(NULL,": \n");
            output1[0].name = str12;
            output1[0].val = -1;
            newNode1->outputs = output1;
            newNode1->checked = 'n';
            newNode1->next = NULL;
            instrNode* ptr1 = commands;
            while(ptr1->next != NULL){
                ptr1 = ptr1->next;
            }
            ptr1->next = newNode1;
        }
        else if(strcmp(typeOfOp,"AND") == 0){
            counter++;
            instrNode* newNode2 = (instrNode*)malloc(sizeof(instrNode));
            newNode2->type = 1;
            operand* input2 = (operand*)malloc(2 * sizeof(operand));
            input2[0].name = strtok(NULL,": ");
            input2[0].val = -1;
            input2[1].name = strtok(NULL,": ");
            input2[1].val = -1;
            newNode2->inputs = input2;
            operand* output2 = (operand*)malloc(sizeof(operand));
            output2[0].name = strtok(NULL,": ");
            output2[0].val = -1;
            newNode2->outputs = output2;
            newNode2->checked = 'n';
            newNode2->next = NULL;
            instrNode* ptr2 = commands;
            while(ptr2->next != NULL){
                ptr2 = ptr2->next;
            }
            ptr2->next = newNode2;
            
        }
        else if(strcmp(typeOfOp,"OR") == 0){
            counter++;
            instrNode* newNode3 = (instrNode*)malloc(sizeof(instrNode));
            newNode3->type = 2;
            operand* input3 = (operand*)malloc(2 * sizeof(operand));
            input3[0].name = strtok(NULL,": ");
            input3[0].val = -1;
            input3[1].name = strtok(NULL,": ");
            input3[1].val = -1;
            newNode3->inputs = input3;
            operand* output3 = (operand*)malloc(sizeof(operand));
            output3[0].name = strtok(NULL,": ");
            output3[0].val = -1;
            newNode3->outputs = output3;
            newNode3->checked = 'n';
            newNode3->next = NULL;
            instrNode* ptr3 = commands;
            while(ptr3->next != NULL){
                ptr3 = ptr3->next;
            }
            ptr3->next = newNode3;

        }
        else if(strcmp(typeOfOp,"NAND") == 0){
            counter++;
            instrNode* newNode4 = (instrNode*)malloc(sizeof(instrNode));
            newNode4->type = 3;
            operand* input4 = (operand*)malloc(2 * sizeof(operand));
            input4[0].name = strtok(NULL,": ");
            input4[0].val = -1;
            input4[1].name = strtok(NULL,": ");
            input4[1].val = -1;
            newNode4->inputs = input4;
            operand* output4 = (operand*)malloc(sizeof(operand));
            output4[0].name = strtok(NULL,": ");
            output4[0].val = -1;
            newNode4->outputs = output4;
            newNode4->checked = 'n';
            newNode4->next = NULL;
            instrNode* ptr4 = commands;
            while(ptr4->next != NULL){
                ptr4 = ptr4->next;
            }
            ptr4->next = newNode4;

        }
        else if(strcmp(typeOfOp,"NOR") == 0){
            counter++;
            instrNode* newNode5 = (instrNode*)malloc(sizeof(instrNode));
            newNode5->type = 4;
            operand* input5 = (operand*)malloc(2 * sizeof(operand));
            input5[0].name = strtok(NULL,": ");
            input5[0].val = -1;
            input5[1].name = strtok(NULL,": ");
            input5[1].val = -1;
            newNode5->inputs = input5;
            operand* output5 = (operand*)malloc(sizeof(operand));
            output5[0].name = strtok(NULL,": ");
            output5[0].val = -1;
            newNode5->outputs = output5;
            newNode5->checked = 'n';
            newNode5->next = NULL;
            instrNode* ptr5 = commands;
            while(ptr5->next != NULL){
                ptr5 = ptr5->next;
            }
            ptr5->next = newNode5;
        }
        else if(strcmp(typeOfOp,"XOR") == 0){
            counter++;
            instrNode* newNode6 = (instrNode*)malloc(sizeof(instrNode));
            newNode6->type = 5;
            operand* input6 = (operand*)malloc(2 * sizeof(operand));
            input6[0].name = strtok(NULL,": ");
            input6[0].val = -1;
            input6[1].name = strtok(NULL,": ");
            input6[1].val = -1;
            newNode6->inputs = input6;
            operand* output6 = (operand*)malloc(sizeof(operand));
            output6[0].name = strtok(NULL,": ");
            output6[0].val = -1;
            newNode6->outputs = output6;
            newNode6->checked = 'n';
            newNode6->next = NULL;
            instrNode* ptr6 = commands;
            while(ptr6->next != NULL){
                ptr6 = ptr6->next;
            }
            ptr6->next = newNode6;

        }
        else if(strcmp(typeOfOp,"DECODER") == 0){
            counter++;
        }
        else if(strcmp(typeOfOp,"MULTIPLEXER") == 0){
            counter++;
        }
        else if(strcmp(typeOfOp,"PASS") == 0){
            counter++;
        }
    }
    instrNode* ptrToCirc = commands;
    while (ptrToCirc->next != NULL){
        ptrToCirc = ptrToCirc->next;
    }
    ptrToCirc->next = commands;
    makeTruthTable(commands,atoi(numOfInputs),atoi(numOfOutputs),inputs,outputs,counter);/*Send linked list of commands to make a truth table with*/
    return 0;    
}  
