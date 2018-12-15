#include<stdio.h>
#include<stlib.h>

typedef struct{
    unsigned int nbLetters;
    double* table;

}Bigrams;
unsigned int getBigramValue(Bigrams* bigramsHash, char firstLetter){
    unsigned int valueFirstLetter = firstLetter - 'a';
    unsigned int valueSecondLetter = secondLetter - 'a' + 1 ;
    unsigned int position = valueFirstLetter * bigramsHash->nbLetters + valueSecondLetter;

return bigramsHash->table[position];
}


char** initText(char* cipheredText){
    if(!cipheredText)


return text;
}

int initBigrams(Bigrams* bigramsHash, char* bigramsFile){
    if(!bigramsFile || !bigramsHash){
        return -1;
    }

    FILE* fp = fopen(bigramsFile, "r");
    if(!fp){
        printf("error oening bigrams\n");
        return -1;
    }

    //find the lenght of the bigrams file?
    // alocate ana array of size of the bigrams elements
    //is format always the same ? letters in order?

    fclose(fp);

return 0;
}


double costFunction(Bigrams* bigramsHash,char* firstLine, char* secondLine, unsigned int lenghtOfLine ){
    if(!bigramsHash || !firstLine || !secondLine){
        printf("error\n")
        return -1.0;
    }
    

return cost;
}

int shifts(unsigned int max_shift, char* ciphered_text, char* bigrams){
    if (!ciphered_text || !bigrams){
        printf("error\n");
        return;
    }

    Bigrams* bigramsHash = malloc(sizeof(Bigrams));
    initBigrams(bigramsHash,bigrams);
    if(initBigrams(bigramsHash,bigrams != 0){
        printf("error in bigrams init\n");
        return -1;
    }
    char** textToCrack = initText(ciphered_text);
    if(!textToCrack){
        printf("error in text to crack init\n");
        free(bigrams);
        return -1;
    }


    return;
}