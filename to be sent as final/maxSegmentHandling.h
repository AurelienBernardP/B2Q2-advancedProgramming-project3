#ifndef _SEGMENTHANDLING_
#define _SEGMENTHANDLING_

typedef struct MaxCell_t MaxCell;
struct MaxCell_t{
    size_t start;
    size_t end;
    MaxCell* split1;
    MaxCell* split2;
    double value;
};

typedef struct{
    MaxCell** table;
    size_t tableSize;
    size_t firstAvailable;
    bool* spacesPlacement;
}MaxTable;

typedef struct{
    bool isSpliced;
    MaxCell* split1;
    MaxCell* split2;
    size_t wordStart;
    size_t wordEnd;
    double value;
}CurrentMax;

MaxCell* findSegment(MaxTable* maximums, size_t start, size_t end);

MaxCell* updateMaxTable(MaxTable* recordedMaximums,MaxCell* currentMax);

MaxCell* insertNewMaxCell(MaxTable* recordedMaximums, MaxCell* newCell);

void destroyMaxtable(MaxTable* recordedMaximums);

MaxTable* initMaxTable(size_t initialSize, size_t textLength);

#endif