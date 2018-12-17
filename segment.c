#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "HashTable.h"

static int compareString(const void *a, const void *b)
{
   return strcmp((char *)a, (char *)b);
}

static size_t naiveEncodeString(const void *key)
{
   const char *str = (char *)key;
   size_t uniqueKey = 0;
   size_t length = strlen(str);
   
   for(size_t i = 0; i < length; i++)
      uniqueKey += pow(27, i+1)*(str[i] - 'a');
   return uniqueKey;
}

long double frequency(HashTable *hashTable, char *word, size_t nbOccursTot)
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

int segment(char *plain_txt, char *words)
{
   if (!words)
   {
      printf("Starfoula\n");
      return -1; //Envoyez à stderr ????
   }

   //Opening the file $words$
   FILE *fp = fopen(words, "r");
   if (!fp)
   {
      printf("Starfoula file\n");
      return -1; //stderr?
   }

   //Initialisation of a hash table containing the data from $words$
   size_t capacity = 50;
   HashTable *hashTable = createHashTable(capacity, naiveEncodeString, compareString);
   if (!hashTable)
   {
      printf("Starfoula hastable\n");
      return -1; //stderr?
   }

   //Completing the hash table with every words
   size_t nbOccurs, nbOccursTot = 0;
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

}

int main(void)
{
   char filename[50] = "words.csv";
   char filename2[50] = "text.txt";

   segment(filename2, filename);
}
