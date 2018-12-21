#ifndef _SEGMENTHANDLING_
#define _SEGMENTHANDLING_

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

MaxCell* updateMaxTable(MaxTable* recordedMaximums,CurrentMax* currentMax);

MaxCell* insertNewMaxCell(MaxTable* recordedMaximums, size_t start, size_t end, double value);

void destroyMaxtable(MaxTable* recordedMaximums);

MaxTable* initMaxTable(size_t initialSize, size_t textLength);

#endif