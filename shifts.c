#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Matrix.h"
#include"IOHandler.h"

typedef struct{
    unsigned int nbLetters;
    double* table;

}Bigrams;
unsigned int getBigramValue(Bigrams* bigramsHash, char firstLetter, char secondLetter){
    unsigned int valueFirstLetter = firstLetter - 'a';
    unsigned int valueSecondLetter = secondLetter - 'a' + 1 ;
    unsigned int position = valueFirstLetter * bigramsHash->nbLetters + valueSecondLetter;

return bigramsHash->table[position];
}


Matrix** initText(char* cipheredText){
    if(!cipheredText){
        printf("error");
        return NULL;
    }


    FILE* fp= fopen(cipheredText, "r");
    if(!fp){
        printf("error opening ciphered text\n");
        return NULL;
    }

    size_t lengthText = 0;//count the lines and columns in the file
    size_t nbLines = 0;
    while(!feof(fp)){
        char c = getc(fp);
        if(c == '\n'){
            nbLines ++;
        }else{
            lengthText++;
        }
    }
    fclose(fp);
    size_t nbColumns = lengthText/nbLines; 

    char* oneLineText = readCipheredText(cipheredText, lengthText, nbLines);
    if(!oneLineText){
        printf("error making one line text\n");
        return NULL;
    }

    Matrix** text = malloc(nbLines * sizeof(void*));
    if(!text){
        printf("error allocating matrix text\n");
        return NULL;
    }
    char* tmpString = malloc(nbColumns * sizeof(char));
    if(!tmpString){
        printf("error allocating tmp string\n");
        return NULL;
    }    
    for(size_t i = 0; i < nbLines;i++){
        

        for(size_t j = 0; j<nbColumns; j++){
            tmpString[j] = oneLineText[nbColumns*i + j]; 
        }
        text[i] = createMatrixForString(tmpString, nbColumns, nbColumns);
        if(!text[i]){
            printf("error making line matrix\n");
            return NULL;
        }
        
    }
    free(tmpString);
return text;
}


/*algorithm to find the integer sqrt of a number using newton's method

*/
static unsigned int intSqrt(unsigned int number){
    unsigned int  y = number;
    unsigned int x = number+1;
    while(y<x){
        x = y;
        y = (x + (number/x))/2;
    }
    return x;
}

static int initBigrams(Bigrams* bigramsHash, char* bigramsFile){
    if(!bigramsFile || !bigramsHash){
        return -1;
    }
    printf("in\n");
    FILE* fp = fopen(bigramsFile, "r");// open file and check opening
    if(!fp){
        printf("error opening bigrams\n");
        return -1;
    }
    char unusedStr[3];
    double unusedVal;
    size_t fileLength = 1;//find the length of the bigrams file, hence the number of combinations of 2 letters
    printf("%d",fscanf(fp, "%3c%lf\n",unusedStr,&unusedVal ) );
    printf("%lf\n", unusedVal);
    while ( fscanf(fp, "%3c%lf\n",unusedStr,&unusedVal ) == 2){
        printf("%lf\n", unusedVal);
        fileLength ++;
    }

    bigramsHash->table = malloc(fileLength * sizeof(double));//allocate and check the allocation of the hash table for the bigram values
    if(!bigramsHash->table){
        printf("error allocating memory");
        return -1;
    }

    rewind(fp);//put the values of the bigrams in the table
    for(size_t i = 0; i< fileLength; i++){
        
        fscanf(fp, "%3c%lf\n",unusedStr,&bigramsHash->table[i]);
    }

    unsigned int nbLettersInAlphabet = intSqrt(fileLength);//calculate the number of 
    if (nbLettersInAlphabet*nbLettersInAlphabet == fileLength){
        bigramsHash->nbLetters = nbLettersInAlphabet;
    }else{
        printf("%u ? %lu, error computing the number of letter in the bigram's alphabet",nbLettersInAlphabet*nbLettersInAlphabet, fileLength);
        return -1;
    }

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
    Matrix** textToCrack = initText(ciphered_text);
    if(!textToCrack){
        printf("error in text to crack init\n");
        free(bigrams);
        return -1;
    }


    return;
}