#include<stdlib.h>
#include<stdio.h>
#include"segment.c"

typedef struct{
    char* maxStr;
    double value;
}MaxCell;

typedef struct{
    MaxCell* table;
    size_t tableSize;
    size_t firstAvailable;
}MaxTable;

MaxTable* initMaxTable(size_t initialSize){
    if(initialSize == 0){
        return NULL;
    }
    MaxTable* maxTable = malloc(sizeof(maxTable));
    if(!MaxTable){
        return NULL;
    }
    MaxTable->tableSize = initialSize;
    MaxTable->firstAvailable = 0;
    MaxTable->table = malloc(initialSize *sizeof(MaxCell));
    if(!MaxTable->table){
        free(MaxTable);
        return NULL;
    }
    return MaxTable;
}

static MaxCell* findSegment(MaxTable* maximums, char* text){
    if(!text){
        printf("uninitialized text");
        return;
    }
    for(size_t i = 0; i < maximums->firstAvailable; i++){
        if(strcmp(text, maximums->table[i]->maxStr) == 0){
            //segment has been found
            return &maximums->table[i];
        }
    }
    return NULL;
}


static MaxCell* insertNewMaxCell(MaxTable* recordedMaximums,char* text, double textValue){
    if(!recordedMaximums || !text){
        return;
    }
    //if(findSegment(recordedMaximums, text))
    //    return;//segment is already in the table
    if(firstAvailable+2>=tableSize){
        //table needs to expand
        recordedMaximums->table = realloc(recordedMaximums->table, sizeof(MaxCell)*2*recordedMaximums->tableSize)
        if(recordedMaximums->table == NULL){
            printf("yo momma\n")
            return NULL;//if allocation error
        }
        recordedMaximums->tableSize*=2;
    }

    MaxCell* newCell = malloc(sizeof(MaxCell));
    if(!newCell)
        return NULL;
    newCell->maxStr = text;
    newCell->value = textValue;
    recordedMaximums->table[firstAvailable] = newCell;
    recordedMaximums->firstAvailable ++;

    return newCell;
}
static MaxCell* updateMaxTable(MaxTable* recordedMaximums,CurrentMax* currentMax){
    if(!currentMax || !recordedMaximums){
        return NULL;
    }

    return newCell;
}