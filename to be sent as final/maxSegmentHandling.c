#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"segment.h"
#include"maxSegmentHandling.h"


MaxTable* initMaxTable(size_t initialSize, size_t textLength){
    if(initialSize == 0){
        return NULL;
    }
    MaxTable* maxTable = malloc(sizeof(maxTable));
    if(!maxTable){
        return NULL;
    }
    maxTable->tableSize = initialSize;
    maxTable->firstAvailable = 0;
    maxTable->table = malloc(initialSize *sizeof(MaxCell*));
    if(!maxTable->table){
        free(maxTable);
        return NULL;
    }
    maxTable->spacesPlacement = calloc(textLength+3 ,sizeof(bool));
    if(!maxTable->spacesPlacement){
        free(maxTable->table);
        free(maxTable);        
        return NULL;
    }
    maxTable->spacesPlacement[0] = maxTable->spacesPlacement[textLength+1]=true;
    
    return maxTable;
}

MaxCell* findSegment(MaxTable* maximums, size_t start, size_t end){
if(!maximums){
    printf("error1\n");
}
    for(size_t i = 0; i < maximums->firstAvailable; i++){
        if(maximums->table[i]->start == start && maximums->table[i]->end == end){
            return maximums->table[i];
        }
    }
    return NULL;
}

void destroyMaxtable(MaxTable* recordedMaximums){
    if(!recordedMaximums){
        return;
    }

}

MaxCell* insertNewMaxCell(MaxTable* recordedMaximums, MaxCell* newCell){
    if(!recordedMaximums ){
        return NULL;
    }

 //   if(findSegment(recordedMaximums, newCell->start,newCell->end))
 //       return NULL;//segment is already in the table

    if(recordedMaximums-> firstAvailable + 2 >= recordedMaximums-> tableSize){
        //table needs to expand

        recordedMaximums->table = realloc(recordedMaximums->table, sizeof(MaxCell*)*2*recordedMaximums->tableSize);
        if(recordedMaximums->table == NULL){
            printf("error\n");
            return NULL;//if allocation error
        }
        recordedMaximums->tableSize*=2;
    }
    
    recordedMaximums->table[recordedMaximums-> firstAvailable] = newCell;
    recordedMaximums->firstAvailable ++;

    return newCell;
}


