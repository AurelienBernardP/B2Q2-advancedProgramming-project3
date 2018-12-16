#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "HashTable.h"

static int compareString(const void* a, const void* b){
   return strcmp((char*)a, (char*)b);
}

static size_t naiveEncodeString(const void* key){
   const char* str = (char*)key;
   size_t uniqueKey = 0;
   size_t length = strlen(str);
   for(size_t i = 0; i<length; i++)
      uniqueKey += (str[length-1-i]*pow(31,length-i));
   return uniqueKey;
}

long double frequency(HashTable* hashTable, char* word, size_t nbOccursTot){
   if(!hashTable ){
      printf("vide star\n");
      return 0;
   }
   if(hasKey(hashTable,word))
      return ((size_t)getValue(hashTable, word)/(long double)nbOccursTot);
   else{
      size_t power = pow(10,strlen(word));
      long double dec = 10/(long double)nbOccursTot;
      return dec/power; 
      }


}

int segment(char* plain_txt, char* words){
   if(!words){
      printf("Starfoula\n");
      return -1; //Envoyez à stderr ????
   }

   //Opening the file $words$
   FILE* fp = fopen(words,"r");
   if(!fp){
      printf("Starfoula file\n");
      return -1; //stderr?   
   }

   //Initialisation of a hash table containing the data from $words$
   size_t capacity = 50;
   HashTable* hashTable = createHashTable(capacity,naiveEncodeString,compareString);
   if(!hashTable){
      printf("Starfoula hastable\n");
      return -1; //stderr?
   }


   //Completing the hash table with every words
   size_t nbOccurs, nbOccursTot = 0;
   char separator[2] = ";";
   char sizeLine[200];
   char* wordName = malloc(sizeof(char)*100);
   while(fscanf(fp,"%s",sizeLine) != -1){
      wordName = strtok(sizeLine,separator);
      nbOccurs = strtoul(strtok(NULL, separator),NULL,10);
      nbOccursTot += nbOccurs;
      insertElement(hashTable,(char **)wordName,(size_t*) nbOccurs);
   }
   char word[50] = "0123456789";
   printf("num element %lu\n",getNumElements(hashTable));
   if(hasKey(hashTable,word))
      printf("%lu\n",(size_t)getValue(hashTable, word));
   printf("freq %Le, nbmax %lu\n",frequency(hashTable, word, nbOccursTot), nbOccursTot);

   fclose(fp);
   
   fp = fopen(plain_txt, "r")
   if(!fp){
      freeHashTable(hashTable);
      printf("impossible d'ouvrir le texte.txt\n");
      return -1;
   }
   size_t sizeText = 0;
   while(fgetc(fp) != '\n')
      sizeText++;

   char* text = malloc(sizeof(char)*sizeText);
   if(!text){
      freeHashTable(hashTable);
      printf("Erreur allocation de mémoire\n");
      return -1;
   }

   if(fscanf(fp,"%s",text)<=0){
      printf("Pas de string dans le fichier\n");
      freeHashTable(hashTable);
      return -1;
   }
   

   freeHashTable(hashTable);
}

int main(void){
   char filename[50] = "words.csv";
   char filename2[50] = "text.txt";

   segment(filename2,filename);
}
