#ifndef __SEGMENTATION__
#define __SEGMENTATION__

#include"maxSegmentHandling.h"
#include"segment.h"
#include "HashTable.h"
#include<stdlib.h>
#include<stdbool.h>

int segment(char *plain_txt, char *words);
int compareString(const void *a, const void *b);
size_t naiveEncodeString(const void *key);
MaxCell* maxSegmentation(HashTable* wordsHash, char* text, size_t start, size_t end, MaxTable* recordedMaximums, size_t nbOccursTot);
double brutemaxSegmentation(HashTable* wordsHash, char* text, size_t start, size_t end, size_t nbOccursTot, bool* segmentation);

#endif