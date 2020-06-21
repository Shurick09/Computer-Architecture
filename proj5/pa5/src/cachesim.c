#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*Cache was made up of an array of sets
 * A set was made up of an array of lines
 * A line was made up of a valid bit, the tag, and the age which was used for FIFO
 */

typedef struct line{
    int valid;
    unsigned long int tag;
    int age;
}line;

typedef struct set{
    line* lines;
}set;


typedef struct cache{
    set* sets;
}cache;

/*Tested if number was a power of 2*/
int isPowerOf2(int n){
    while (n != 1){
        if(n%2 != 0){
            return 0;
        }
        n = n/2;
    }
    return 1;
}

/*Found logBase2 of a number (only works if number is power of 2)*/
int logBase2(int n){
    int c = 0;
    while (n != 1){
        n = n / 2;
        c++;
    }
    return c;
}

/*Takes in how many bits for the block offset and set for a memory address and returns the ta
 *Uses bitshifting*/
unsigned long int getTag(unsigned long int memAdd, int littleB, int littleS){
    unsigned long int tag;
    tag = memAdd >> (littleB + littleS);
    return tag;
}

/*Takes in how many bits for the block offset and set for a memory address and returns the set
 * Uses bitshifting and masking*/
unsigned long int getSet(unsigned long int memAdd, int littleB, int littleS){
    unsigned long int setNum;
    setNum = memAdd >> littleB;
    int mask = (1 << littleS) - 1;
    setNum = setNum & mask;
    return setNum;
}

/*For a direct mapped cache checks if the memory address is already stored. Returns 0 if stored and 1 if not stored.
 * If not stored then also adds that memory address to the correct set in the cache*/
int inCacheDirect(cache cache1, unsigned long int tag, unsigned long int setNum){
    if(cache1.sets[setNum].lines[0].valid == 0){
        cache1.sets[setNum].lines[0].tag = tag;
        cache1.sets[setNum].lines[0].valid = 1;
        return 1;
    }
    else{
        if(cache1.sets[setNum].lines[0].tag != tag){
            cache1.sets[setNum].lines[0].tag = tag;
            return 1;
        }
    }
    return 0;
}

/*For a direct mapped nonprefetch cache this function updates the values of cache hits...*/
void directNonPrefetch(char rw, unsigned long int memAdd, cache nonPrefetch, int *res,int numSets,int littleS,int blockSize,int littleB){
    if(rw == 'R'){
        if(inCacheDirect(nonPrefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS)) == 0){/*If in cache*/
            res[0]++;
        }
        else{/*If not in cache*/
            res[1]++;
            res[2]++;
        }   
    }
    else if(rw == 'W'){
        if(inCacheDirect(nonPrefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS)) == 0){
            res[0]++;
            res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
        }
    }
}

/*For a direct mapped prefetch cache this function updates the values of cache hits...*/
void directPrefetch(char rw, unsigned long int memAdd, cache prefetch, int* res, int numSets,int littleS,int blockSize, int littleB){
    if(rw == 'R'){
        if(inCacheDirect(prefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS)) == 0){
            res[0]++;
        }
        else{
            res[1]++;
            res[2]++;
            if(inCacheDirect(prefetch,getTag(memAdd + blockSize,littleB,littleS),getSet(memAdd + blockSize,littleB,littleS)) == 1){/*If next block is not in cache*/
                res[2]++;
            }
        }
    }
    else if(rw == 'W'){
        if(inCacheDirect(prefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS)) == 0){
             res[0]++;
             res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
            if(inCacheDirect(prefetch,getTag(memAdd + blockSize,littleB,littleS),getSet(memAdd + blockSize,littleB,littleS)) == 1){
                res[2]++;
            }
        }
    }
}

/*For a fully associative cache this function checks if memory address is in cache and loads it in if it isn't*/
int inCacheFully(cache nonPrefetch,unsigned long int tag, int linesInSet){
    int i;
    for(i = 0;i < linesInSet;i++){
        if(nonPrefetch.sets[0].lines[i].valid != 0){
            if(nonPrefetch.sets[0].lines[i].tag == tag){//If tag is in cache
                return 0;
            }
        }
    }
    int k;
    for(k = 0;k < linesInSet;k++){//If there is an empty line fill that line
        if(nonPrefetch.sets[0].lines[k].valid == 0){
            nonPrefetch.sets[0].lines[k].tag = tag;
            int o;
            for(o = 0;o < linesInSet;o++){//Updates the age for FIFO
                if(nonPrefetch.sets[0].lines[o].valid != 0){
                    (nonPrefetch.sets[0].lines[o].age)++;
                }
            }
            nonPrefetch.sets[0].lines[k].age = 1;
            nonPrefetch.sets[0].lines[k].valid = 1;
            return 1;
        }
       
    }
    int r;
    for(r = 0;r < linesInSet;r++){//If all lines are filled finds the oldest line and replaces that one
        if(nonPrefetch.sets[0].lines[r].age == linesInSet){
            nonPrefetch.sets[0].lines[r].tag = tag;
            int h;
            for(h = 0;h < linesInSet;h++){
                if(nonPrefetch.sets[0].lines[h].valid != 0){
                    (nonPrefetch.sets[0].lines[h].age)++;
                }
            }
            nonPrefetch.sets[0].lines[r].age = 1;
            nonPrefetch.sets[0].lines[r].valid = 1;
            return 1;
        }
    }
    return 1;
}

/*Updates values for fully associative nonprefetch cache*/
void fullyNonPrefetch(char rw,unsigned long int memAdd,cache nonPrefetch,int* res,int blockSize,int littleB,int linesInSet){
    if(rw == 'R'){
        if(inCacheFully(nonPrefetch,getTag(memAdd,littleB,0),linesInSet) == 0){
            res[0]++;
        }
        else{
            res[1]++;
            res[2]++;
        }
    }
    else if(rw == 'W'){
        if(inCacheFully(nonPrefetch,getTag(memAdd,littleB,0),linesInSet) == 0){
            res[0]++;
            res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
        }
    }
}

/*Updates values for fully associative prefetch cache*/
void fullyPrefetch(char rw,unsigned long int memAdd,cache prefetch,int* res,int blockSize,int littleB,int linesInSet){
    if(rw == 'R'){
        if(inCacheFully(prefetch,getTag(memAdd,littleB,0),linesInSet) == 0){
            res[0]++;
        }
        else{
            res[1]++;
            res[2]++;
            if(inCacheFully(prefetch,getTag(memAdd + blockSize,littleB,0),linesInSet) == 1){
                res[2]++;
            }
        }
    }
    else if(rw == 'W'){
        if(inCacheFully(prefetch,getTag(memAdd,littleB,0),linesInSet) == 0){
             res[0]++;
             res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
            if(inCacheFully(prefetch,getTag(memAdd + blockSize,littleB,0),linesInSet) == 1){
                res[2]++;
            }
        }
    }
}

/*For n-way associative cache function checks if memory address is in cache and loads it in if it isn't
 * Similar to format in function inCacheFully*/
int inCacheNWay(cache nonPrefetch,unsigned long int tag,unsigned long int set, int linesPerSet){
    int i;
    for(i = 0;i < linesPerSet;i++){
        if(nonPrefetch.sets[set].lines[i].tag == tag){
            return 0;
        }
    }
    int j;
    for(j = 0;j < linesPerSet;j++){
        if(nonPrefetch.sets[set].lines[j].valid == 0){
            nonPrefetch.sets[set].lines[j].tag = tag;
            int e;
            for(e = 0;e < linesPerSet;e++){
                if(nonPrefetch.sets[set].lines[e].valid != 0){
                    (nonPrefetch.sets[set].lines[e].age)++;
                }
            }
            nonPrefetch.sets[set].lines[j].age = 1;
            nonPrefetch.sets[set].lines[j].valid = 1;
            return 1;
        }
    }
    int h;
    for(h = 0; h < linesPerSet;h++){
        if(nonPrefetch.sets[set].lines[h].age == linesPerSet){
            nonPrefetch.sets[set].lines[h].tag = tag;
            int f;
            for(f = 0;f < linesPerSet;f++){
                if(nonPrefetch.sets[set].lines[f].valid != 0){
                    (nonPrefetch.sets[set].lines[f].age)++;
                }
            }

            nonPrefetch.sets[set].lines[h].valid = 1;
            nonPrefetch.sets[set].lines[h].age = 1;
            return 1;
        }
    }
    return 0;
}

/*Updates values for an n way associative nonprefetch cache*/
void nWayNonPrefetch(char rw,unsigned long int memAdd,cache nonPrefetch,int* res,int blockSize,int littleB,int numSets,int littleS,int linesPerSet){
    if(rw == 'R'){
        if(inCacheNWay(nonPrefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS),linesPerSet) == 0){
            res[0]++;
        }
        else{
            res[1]++;
            res[2]++;
        }
    }
    else if(rw == 'W'){
        if(inCacheNWay(nonPrefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS),linesPerSet) == 0){
            res[0]++;
            res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
        }
    }
}

/*Updates values for an n way associative prefetch cache*/
void nWayPrefetch(char rw,unsigned long int memAdd,cache prefetch,int *res,int blockSize,int littleB,int numSets,int littleS,int linesPerSet){
    if(rw == 'R'){
        if(inCacheNWay(prefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS),linesPerSet) == 0){
            res[0]++;
        }
        else{
            res[1]++;
            res[2]++;
            if(inCacheNWay(prefetch,getTag(memAdd + blockSize,littleB,littleS),getSet(memAdd + blockSize,littleB,littleS),linesPerSet) == 1){
                res[2]++;
            }
        }
    }
    else if(rw == 'W'){
        if(inCacheNWay(prefetch,getTag(memAdd,littleB,littleS),getSet(memAdd,littleB,littleS),linesPerSet) == 0){
             res[0]++;
             res[3]++;
        }
        else{
            res[1]++;
            res[2]++;
            res[3]++;
            if(inCacheNWay(prefetch,getTag(memAdd + blockSize,littleB,littleS),getSet(memAdd + blockSize,littleB,littleS),linesPerSet) == 1){
                res[2]++;
            }
        }
    }

}

/*Main function:
 * Reads in inputs
 * Figures out what type of cache to create*/
int main(int argc, char** argv){
    int cacheSize = atoi(argv[1]);
    char* assoc = (char*)malloc(20 * sizeof(char));
    assoc = argv[2];
  //char* repPol = (char*)malloc(20 * sizeof(char));
  //repPol = arg[3];
    int blockSize = atoi(argv[4]);
    int i = 6;
    int* nWay = (int*)malloc(5 * sizeof(int));
    if (strncmp(assoc,"assoc:",6) == 0){//Figures out what n is if cache is n way associative
        while (isdigit(assoc[i])){
            int dig = assoc[i] - '0';
            nWay[i - 6] = dig;
            i++;
        }
    }
    int j;
    int n = 0;
    int mult = 1;
    if (i != 6){
        for(j = i - 7; j > -1; j--){
           n = n + mult * nWay[j];
           mult = mult * 10;
        }
    }
    if((isPowerOf2(cacheSize) == 0) || (isPowerOf2(blockSize) == 0)){
        printf("error: not power of 2\n");
        return 1;
    }
    if(n != 0){
        if(isPowerOf2(n) == 0){
            printf("error: not power of 2\n");
            return 1;
        }
    }
    int littleB = logBase2(blockSize);//number of bits for block size
    
    FILE *fp;
    fp = fopen(argv[5],"r");
    if (fp == 0){
        printf("error\n");
        exit(1);
    }
    char* buffer = (char*)malloc(100 * sizeof(char));
    unsigned long int pc;
    char rw;
    unsigned long int memAdd;
    cache nonPrefetch;
    cache prefetch;
    
    int* resNonPrefetch;
    resNonPrefetch = (int*)malloc(4 * sizeof(int));
    int* resPrefetch;
    resPrefetch = (int*)malloc(4 * sizeof(int));
    int d;
    for(d = 0; d < 4;d++){
        resNonPrefetch[d] = 0;
        resPrefetch[d] = 0;
    }
    
    fgets(buffer,100,fp);
    if(strcmp(assoc,"direct") == 0){//Direct
        int numSets = cacheSize / blockSize;
        int littleS = logBase2(numSets);//num of bits for set
        set* set1 = (set*)malloc(numSets * sizeof(set));    
        set* set2 = (set*)malloc(numSets * sizeof(set));
        int k;
        for(k = 0; k < numSets;k++){//Makes each line its own set
            line* line1;
            line1 = (line*)malloc(sizeof(line));
            line1[0].valid = 0;
            line1[0].age = 0;
            line1[0].tag = 0;
            set1[k].lines = line1;
        }
        int r;
        for(r = 0;r < numSets; r++){
            line* line2;
            line2 = (line*)malloc(sizeof(line));
            line2[0].valid = 0;
            line2[0].age = 0;
            line2[0].tag = 0;
            set2[r].lines = line2;
        }
        nonPrefetch.sets = set1;
        prefetch.sets = set2;
        while (buffer[0] != '#'){//Uses fgets and strtok to break file up into parts
             pc =  strtol((strtok(buffer," :")),NULL,16);
             rw = strtok(NULL, " :")[0];
             memAdd = strtol((strtok(NULL," :")),NULL,16);
             directNonPrefetch(rw,memAdd,nonPrefetch,resNonPrefetch,numSets,littleS,blockSize,littleB);
             directPrefetch(rw,memAdd,prefetch,resPrefetch,numSets,littleS,blockSize,littleB);
             fgets(buffer,100,fp);
        }
    }
    else if(n == 0){//Fully associative
         set* set3 = (set*)malloc(sizeof(set));
         set* set4 = (set*)malloc(sizeof(set));
         int linesInSet = cacheSize / blockSize;
         line* line3;
         line* line4;
         line3 = (line*)malloc(sizeof(line) * linesInSet);
         line4 = (line*)malloc(sizeof(line) * linesInSet);
         int g;
         for(g = 0;g < linesInSet;g++){//Makes lines in one set
             line3[g].valid = 0;
             line3[g].age = 0;
             line3[g].tag = 0;
             line4[g].valid = 0;
             line4[g].age = 0;
             line4[g].tag = 0;
         }
         set3[0].lines = line3;
         set4[0].lines = line4;
         nonPrefetch.sets = set3;
         prefetch.sets = set4;
         while (buffer[0] != '#'){
             pc =  strtol((strtok(buffer," :")),NULL,16);
             rw = strtok(NULL, " :")[0];
             memAdd = strtol((strtok(NULL," :")),NULL,16);
             fullyNonPrefetch(rw,memAdd,nonPrefetch,resNonPrefetch,blockSize,littleB,linesInSet);
             fullyPrefetch(rw,memAdd,prefetch,resPrefetch,blockSize,littleB,linesInSet);
             fgets(buffer,100,fp);
        }
    }
    else{//N-way
         int linesPerSet = n;
         int numSets = cacheSize / (blockSize * linesPerSet);
         int littleS = logBase2(numSets);
         set* set5 = (set*)malloc(sizeof(set) * numSets);
         set* set6 = (set*)malloc(sizeof(set) * numSets);
         int q;
         int b;
         for(q = 0;q < numSets;q++){//Nested loops used to set up cache with variable amount of lines in each set
             line* line5 = (line*)malloc(sizeof(line) * linesPerSet);
             line* line6 = (line*)malloc(sizeof(line) * linesPerSet);
             for(b = 0;b < linesPerSet;b++){
                line5[b].valid = 0;
                line5[b].age = 0;
                line5[b].tag = 0;
                line6[b].valid = 0;
                line6[b].age = 0;
                line6[b].tag = 0;
             }
             set5[q].lines = line5;
             set6[q].lines = line6;
         }
         nonPrefetch.sets = set5;
         prefetch.sets = set6;
         while (buffer[0] != '#'){
             pc =  strtol((strtok(buffer," :")),NULL,16);
             rw = strtok(NULL, " :")[0];
             memAdd = strtol((strtok(NULL," :")),NULL,16);
             nWayNonPrefetch(rw,memAdd,nonPrefetch,resNonPrefetch,blockSize,littleB,numSets,littleS,linesPerSet);
             nWayPrefetch(rw,memAdd,prefetch,resPrefetch,blockSize,littleB,numSets,littleS,linesPerSet);
             fgets(buffer,100,fp);
        }
    }
    if(pc == 123){

    }
    /*Final printing of results*/
    printf("no-prefetch\n");
    printf("Cache hits: %d\n",resNonPrefetch[0]);
    printf("Cache misses: %d\n",resNonPrefetch[1]);
    printf("Memory reads: %d\n",resNonPrefetch[2]);
    printf("Memory writes: %d\n",resNonPrefetch[3]);    
    printf("with-prefetch\n");
    printf("Cache hits: %d\n",resPrefetch[0]);
    printf("Cache misses: %d\n",resPrefetch[1]);
    printf("Memory reads: %d\n",resPrefetch[2]);
    printf("Memory writes: %d\n",resPrefetch[3]);
    return 0;
}
