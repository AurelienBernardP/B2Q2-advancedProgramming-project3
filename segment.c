#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "HashTable.h"

typedef struct{
    char* maxStr;
    double value;
}MaxCell;

typedef struct{
    MaxCell* table;
    size_t tableSize;
}
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
static int compareString(const void *a, const void *b)
{
   return strcmp((char *)a, (char *)b);
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
static size_t naiveEncodeString(const void *key)
{
   const char *str = (char *)key;
   size_t uniqueKey = 0;
   size_t length = strlen(str);
   
   for(size_t i = 0; i < length; i++)
      uniqueKey += pow(27, i+1)*(str[i] - 'a');
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
long double static frequency(HashTable *hashTable, char *word, size_t nbOccursTot)
{
   if (!hashTable)
   {
      printf("vide star\n");
      return 0;
   }
   if (hasKey(hashTable, word))
      return ((size_t)getValue(hashTable, word) / (long double)nbOccursTot);
   else
   {
      size_t power = pow(10, strlen(word));
      long double dec = 10 / (long double)nbOccursTot;
      return dec / power;
   }
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
static long double stringValue(HashTable *hashTable, char *word, size_t nbOccursTot){
    return log(frequency(hashTable, word, nbOccursTot));
}
static HashTable* initWordHash(char* words,size_t* nbOccursTot){
   if (!words){
      printf("Starfoula\n");
      return NULL; //Envoyez à stderr ????
   }

   //Opening the file $words$
   FILE *fp = fopen(words, "r");
   if (!fp)
   {
      printf("Starfoula file\n");
      return NULL; //stderr?
   }

   //Initialisation of a hash table containing the data from $words$
   size_t capacity = 50;
   HashTable *hashTable = createHashTable(capacity, naiveEncodeString, compareString);
   if (!hashTable)
   {
      printf("Starfoula hastable\n");
      return NULL; //stderr?
   }

   //Completing the hash table with every words
   size_t nbOccurs = 0;
   char separator[2] = ";";
   char sizeLine[200];
   char *wordName = malloc(sizeof(char) * 100);
   while (fscanf(fp, "%s", sizeLine) != -1)
   {
      //Separating the line with the separator (first part = word)
      wordName = strtok(sizeLine, separator);
      
      //second part = number of occ
      nbOccurs = strtoul(strtok(NULL, separator), NULL, 10);
      nbOccursTot += nbOccurs;

      //Insert elem in hastable
      insertElement(hashTable, (char **)wordName, (size_t *)nbOccurs);
   }

   printf("num element %lu\n", getNumElements(hashTable));

   char word[50]="goglw";
   printf("freq %Le, nbmax %lu\n", frequency(hashTable, word, nbOccursTot), nbOccursTot);

   fclose(fp);


    return hashTable;
}
static MaxCell* findSengment(MaxTable* maximums, char* text){
    if(!text){
        printf("uninitialized text");
        return;
    }
    for(size_t i = 0; i<maximums->tableSize ; i++){
        if(strcmp(text, maximums->table[i]->maxStr)==){
            //segment has been found
            return maximums->table[i];
        }
    }
    return NULL;
}
double maxSegmentation(HashTable* wordsHash, char* text, size_t start, size_t end, MaxTable* recordedMaxes, size_t nbOccursTot){
    if(!wordsHash || !text || !recordedMaxes){
        printf("uninit args\n");
        return -INFINITY;
    }
    size_t wordSize = end - start;
    if(wordSize == 1){
        //base case

    }
    //init the word
    char* word = malloc((1+wordSize)*sizeof(char));
    if(!tmpText){
        return;
    }
    for(size_t i = start, j = 0; i < end; i++, j++ ){
        word[j] = text[i];
    }
    word[wordSize+1] = '\0';
    
    double maxValue = stringValue(wordsHash, word, size_t nbOccursTot);

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
        return-1;
    }



}


int main(void)
{
   char filename[50] = "words.csv";
   char filename2[50] = "text.txt";

   segment(filename2, filename);
}
