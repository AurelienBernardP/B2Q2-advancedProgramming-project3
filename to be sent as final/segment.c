#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>
#include <string.h>
#include <math.h>

#include "HashTable.h"
#include "segment.h"
#include "maxSegmentHandling.h"
/*-------------------------------------------------------------------
*  FUNCTION compareString  :
*  compares two strings
*
*  INPUT :
*     - a: valid string
*     - b: valid string
*  OUTPUT :
*     - 0 if they are the same other value otherwise;
--------------------------------------------------------------------*/
int compareString(const void *a, const void *b)
{
   return strcmp((char*)a, (char*)b);
}
/*-------------------------------------------------------------------
*  FUNCTION naiveEncodeString
*  hashing function for a string
*
*  INPUT :
*     - key: valid string pointer
*  OUTPUT :
*     - a key value for the string input
--------------------------------------------------------------------*/
size_t naiveEncodeString(const void *key){
   const char *str = (char *)key;
   size_t uniqueKey = 0;
   size_t length = strlen(str);
   
   for(size_t i = 0; i < length; i++)
      uniqueKey += pow(31, i+1)*(str[i] - 'a');

   return uniqueKey;
}
/*-------------------------------------------------------------------
*  FUNCTION frequency
*  finds the frequency of a string in the words hash table if it is 
*  present
*
*  INPUT :
*     - hashTable : valid hash table of word and their occurence
*     - word : string of the word to find
*     - nbOccursTot : nb of words used to do the has table of occurencies
*  OUTPUT :
*     - a value for the frequency of the word in the table
--------------------------------------------------------------------*/
static double frequency(HashTable *hashTable, char *word, size_t nbOccursTot)
{    
    if (!hashTable){
        printf("empty hashtable\n");
        return 0;
    }
    if (hasKey(hashTable, (void*)word)){
        double value = *(size_t*)getValue(hashTable,(void*) word) / (long double)nbOccursTot;
        return value;
    }else{
        double power = pow((double)10, -(double)strlen(word));
        double value = (((double) 10 / (double)nbOccursTot)) * power;
        return value;
    
   }
   return 0;
}
/*-------------------------------------------------------------------
*  FUNCTION stringValue
*  uses frequency to find the value of the word for a criptographys segmentation
*
*  INPUT :
*     - hashTable : valid hash table of word and their occurence
*     - word : string of the word to find
*     - nbOccursTot : nb of words used to do the has table of occurencies
*  OUTPUT :
*     - a value for the word input
--------------------------------------------------------------------*/
static double stringValue(HashTable *hashTable, char *word, size_t nbOccursTot){
    return log(frequency(hashTable, word, nbOccursTot));
}
static HashTable* initWordHash(char* words,size_t* nbOccursTot){
    if (!words){
        printf("uninit args\n");
        return NULL; //Envoyez à stderr ????
    }

    //Opening the file $words$
    FILE *fp = fopen(words, "r");
    if (!fp)
    {
        printf("not able to open file\n");
        return NULL; //stderr?
    }

    //Initialisation of a hash table containing the data from $words$
    size_t capacity = 500000;
    HashTable *hashTable = createHashTable(capacity, naiveEncodeString, compareString);
    if (!hashTable)
    {
        printf("not init hastable\n");
        return NULL; //stderr?
    }

    //Completing the hash table with every words

    char separator[2] = ";";
    int breaker = 0;
    while (breaker != -1){

        size_t *nbOccurs = calloc(1, sizeof(size_t));
        *nbOccurs = 0;
        char* sizeLine = malloc(sizeof(char)*200);
        breaker = fscanf(fp, "%s", sizeLine);
        if(breaker == -1){
            break;
        }
         //Separating the line with the separator (first part = word)
        char *wordName = strtok(sizeLine, separator);
            
        //second part = number of occ
        *nbOccurs = strtoul(strtok(NULL, separator), NULL, 10);
        *nbOccursTot += *nbOccurs;

        //Insert elem in hashtable
        insertElement(hashTable, wordName, (void*)nbOccurs);

        }
    fclose(fp);
        return hashTable;
}

MaxCell* maxSegmentation(HashTable* wordsHash, char* text, size_t start, size_t end, MaxTable* recordedMaximums, size_t nbOccursTot){
    if(!wordsHash || !text || !recordedMaximums){
        printf("uninit args\n");
        return NULL;
    }
    if(start>end){
        return NULL;
    }
    size_t wordSize = end - start;

    //first see if we already have the answer
    MaxCell* wasCalculated = findSegment(recordedMaximums, start, end);
    if(wasCalculated != NULL){
        return wasCalculated;
    }   


    //init the word
    char* word = malloc((1+wordSize)*sizeof(char));
    if(!word){
        return NULL;
    }
    for(size_t i = start, j = 0; i <= end; i++, j++ ){
        word[j] = text[i];
    }
    // init the current Max
    MaxCell* currentMax = malloc(sizeof(CurrentMax));
    if(!currentMax){

        return NULL;
    }
    //end init of word 
    currentMax->split1 = currentMax->split2 = NULL;
    currentMax->start = start;
    currentMax->end = end;
    currentMax->value = stringValue(wordsHash, word, nbOccursTot);
    // end init current max
    free(word);
       

    if(end == start){
        //base case        
        return insertNewMaxCell(recordedMaximums,currentMax);
    }

    for(size_t i = 1; i <= end-start ; i++){


        if(maxSegmentation(wordsHash, text, start, (end-i), recordedMaximums, nbOccursTot)->value + maxSegmentation(wordsHash, text, (end-i)+1, end, recordedMaximums, nbOccursTot)->value > currentMax->value){
            //a new max has been found
            currentMax->split1 = maxSegmentation(wordsHash, text, start, (end-i), recordedMaximums, nbOccursTot);
            currentMax->split2 = maxSegmentation(wordsHash, text, (end-i)+1, end, recordedMaximums, nbOccursTot);
            currentMax->value = currentMax->split1->value + currentMax->split2->value;
        }
    }

    return insertNewMaxCell(recordedMaximums, currentMax);
}

static void updateSpaces(MaxCell* cell, bool* spaces){
    if(!cell || !spaces){
        return;
    }
    if(cell->split1 == NULL && cell->split2 == NULL){
        //this is a word, segment is not split further
        for(size_t i = cell->start+1; i<cell->end; i++){
            spaces[i]= false;
        }
        spaces[cell->end] = true;
    }else{
        //this segment is splited further
        updateSpaces(cell->split1,spaces);
        //spaces[cell->split1->end] = true;
        updateSpaces(cell->split2,spaces);
    }
}


double brutemaxSegmentation(HashTable* wordsHash, char* text, size_t start, size_t end, size_t nbOccursTot, bool* segmentation){
    if(!wordsHash || !text || start > end ){
        return 0;
    }
    int wordSize = end-start +1;
    char* word = malloc((1+wordSize)*sizeof(char));
    if(!word){
        return 0;
    }
    for(size_t i = start, j = 0; i <= end; i++, j++ ){
        word[j] = text[i];
    }
    double wordValue = stringValue(wordsHash, word, nbOccursTot);
    free(word);
    double value1 = -INFINITY;
    double value2 = -INFINITY;
  //  int bestSegmentation = -1;
    double maxValue = wordValue;
    if(end == start){
        return maxValue;
    }
    for(size_t i = 1; i <= end-start ; i++){

        value1 = brutemaxSegmentation(wordsHash, text, start, end-i, nbOccursTot,segmentation);
        value2 = brutemaxSegmentation(wordsHash, text, end-i+1, end, nbOccursTot,segmentation);
        if((value1+value2)>=maxValue){
            maxValue = value1 + value2;
    //        bestSegmentation = i;
        }
    }/*
    if(bestSegmentation < 0 && start > 0){
        
        for(size_t i = start ; start<end ; i++){
            segmentation[i] = false;
        }
        segmentation[end] = true;
    }else{
        printf("best seg = %d\n",bestSegmentation);
        segmentation[end] = true;
        segmentation[bestSegmentation] = true;
    }
   */
    return maxValue;
}

int segment(char *plain_txt, char *words){
    if(!plain_txt || !words){
        printf("ERROR : unintialized arguments\n");
        return -1;
    }
    size_t nbWords = 0;
    HashTable* wordsHash = initWordHash(words,&nbWords );
    if(!wordsHash || nbWords == 0){
            printf("exit 1\n");
        return-1;
    }

    size_t textLength = strlen(plain_txt);
    if(textLength <=1){
        printf("exit 2\n");
        return-1;
    }

    MaxTable* maxTab = initMaxTable(textLength*textLength, textLength);
    if(!maxTab){
        printf("error at max tab init\n");
        return -1;
    }
    //printf("max value of segemtntation = %lf \n",brutemaxSegmentation(wordsHash, plain_txt, 0, textLength-1, nbWords,maxTab->spacesPlacement));
    
    MaxCell* max = maxSegmentation(wordsHash, plain_txt, 0, textLength,maxTab, nbWords);
    if(!max){
        printf("error\n");
        return -1;
    }
    
    updateSpaces(max, maxTab->spacesPlacement);
    for(size_t i = 0 ;i< textLength+2 ; i++){
        printf("%d", maxTab->spacesPlacement[i]);
    }
    return 0;
}


