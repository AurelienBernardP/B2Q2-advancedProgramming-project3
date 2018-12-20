#include<stdlib.h>
#include<stdio.h>
#include"segment.c"

typedef struct{
    size_t start;
    size_t end;
    double value;
}MaxCell;

typedef struct{
    MaxCell* table;
    size_t tableSize;
    size_t firstAvailable;
    bool* spacesPlacement;
}MaxTable;

MaxTable* initMaxTable(size_t initialSize, size_t textLength){
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
    MaxTable->spacesPlacement = calloc(textLength ,sizeof(bool));
    if(!MaxTable->spacesPlacement){
        free(MaxTable->table);
        free(MaxTable);        
        return NULL;
    }
    
    return MaxTable;
}

static MaxCell* findSegment(MaxTable* maximums, size_t start, size_t end){
    if(!text){
        printf("uninitialized text");
        return;
    }
    for(size_t i = 0; i < maximums->firstAvailable; i++){
        if(MaxTable->table[i]->start == start && MaxTable->table[i]->end == end){
            return MaxTable->table[i];
        }
    }
    return NULL;
}

void destroyMaxtable(MaxTable* recordedMaximums){
    if(!recordedMaximums){
        return;
    }

}

MaxCell* insertNewMaxCell(MaxTable* recordedMaximums, size_t start, size_t end, double value){
    if(!recordedMaximums || !text){
        return;
    }

    if(findSegment(recordedMaximums, text,start,end))
        return;//segment is already in the table

    if(firstAvailable+2>=tableSize){
        //table needs to expand

        recordedMaximums->table = realloc(recordedMaximums->table, sizeof(MaxCell)*2*recordedMaximums->tableSize);
        if(recordedMaximums->table == NULL){
            printf("yo momma\n");
            return NULL;//if allocation error
        }
        recordedMaximums->tableSize*=2;
    }

    MaxCell* newCell = malloc(sizeof(MaxCell));
    if(!newCell)
        return NULL;
    newCell->start = start;
    newCell->end = end;
    newCell->value = value;
    recordedMaximums->table[firstAvailable] = newCell;
    recordedMaximums->firstAvailable ++;

    return newCell;
}


MaxCell* updateMaxTable(MaxTable* recordedMaximums,CurrentMax* currentMax){
    if(!currentMax || !recordedMaximums){
        return NULL;
    }
    if(!currentMax->isSpliced){
        //new maximum is a whole word
        //insert new found maximum in table of maximums
        MaxCell* newMax = insertNewMaxCell(recordedMaximums, currentMax->start, currentMax->end,currentMax->value)
        if(!newMax){
            printf("error in update!\n");
        }
        //update the sepparations array
        for(size_t i = currentMax ; i < currentMax->end; i++){
            recordedMaximums->spacesPlacement[i] = false;
        }
        recordedMaximums->spacesPlacement[currentMax->end] = true 
    }else{
        //new maximum is a spliced segment
        //insert new maximum
        MaxCell* newMax = insertNewMaxCell(recordedMaximums, currentMax->start, currentMax->end,currentMax->value)
        if(!newMax){
            printf("error in update!\n");
        }
    }
    free(currentMax);
    return newCell;
}