#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Matrix.h"
#include"IOHandler.h"
#include"shifts.h"

typedef struct{
    unsigned int nbLetters;
    double* table;

}Bigrams;

typedef struct{
    size_t nbColumns;
    size_t nbLines;
    char** text;
}CipheredText;

typedef struct{
    int previousShift;
    double cumulativeQuality;
}topShifts;
/*-------------------------------------------------------------------
*  FUNCTION getLBigramValue :
*  This function returns the probability of two letters beeing next
*   to each other in a text
*
*  INPUT :
*     - BigamsHash : a valid pointer to a bigramsHash structure
*     - firstLetter: a letter in the language of the bigrams table
*     - firstLetter: a letter in the language of the bigrams table
*
*  OUTPUT :
*     - the bigram value
--------------------------------------------------------------------*/
static long double getBigramValue(Bigrams* bigramsHash, char firstLetter, char secondLetter){
    unsigned int valueFirstLetter = firstLetter - 'a';
    unsigned int valueSecondLetter = secondLetter - 'a';
    unsigned int position = valueFirstLetter * bigramsHash->nbLetters + valueSecondLetter;
    //printf("bigram = %c %c value = %lf\n ",firstLetter,secondLetter,bigramsHash->table[position]);

return bigramsHash->table[position];
}

static void destroyBigrams(Bigrams* bigrams){
    if(!bigrams){
        return;
    }
    if(bigrams->table)
        free(bigrams->table);
    free(bigrams);
return;
}

static CipheredText* initText(CipheredText* ciphered ,char* textPath){
    if(!ciphered || !textPath){
        printf("error");
        return NULL;
    }


    FILE* fp= fopen(textPath, "r");
    if(!fp){
        printf("error opening ciphered text\n");
        return NULL;
    }

    size_t lengthText = 0;//count the lines and columns in the file
    ciphered->nbLines = 0;
    while(!feof(fp)){
        char c = getc(fp);
        if(c == '\n'){
            ciphered->nbLines ++;
        }else{
            lengthText++;
        }
    }
    fclose(fp);
    ciphered->nbColumns = (lengthText+1)/ciphered->nbLines; //upper bound of decimal, that is the reason for the +1
    size_t nbLinesTmp = ciphered->nbLines;

    char* oneLineText = readCipheredText(textPath, &lengthText, &nbLinesTmp);
    if(!oneLineText){
        printf("error making one line text\n");
        return NULL;
    }

    ciphered->text = malloc(ciphered->nbLines * sizeof(char*));
    if(!ciphered->text){
        printf("error allocating matrix text\n");
        return NULL;
    }

    
    for(size_t i = 0; i < ciphered->nbLines;i++){
        char* tmpString = malloc(ciphered->nbColumns * sizeof(char));
        if(!tmpString){
            printf("error allocating tmp string\n");
            return NULL;
        }
        //will not be freed in this function as createMatrixFrom sting doesn't copy the string it keeps a pointer to first element.
        for(size_t j = 0; j < ciphered->nbColumns; j++){
            tmpString[j] = oneLineText[ciphered->nbColumns*i + j]; 
        }
        ciphered->text[i] = tmpString;
        if(!ciphered->text[i]){
            printf("error making line matrix\n");
            return NULL;
        }
        
    }

return ciphered;
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
    FILE* fp = fopen(bigramsFile, "r");// open file and check opening
    if(!fp){
        printf("error opening bigrams\n");
        return -1;
    }
    char unusedStr[3];
    double unusedVal;
    size_t fileLength = 0;//find the length of the bigrams file, hence the number of combinations of 2 letters

    while ( fscanf(fp, "%3c%lf\n",unusedStr,&unusedVal ) == 2){
        
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
       // bigramsHash->table[i] *=1000;

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

static size_t MOD(int first, int second){
    int answer = first % second;
    if(answer<0){
        answer+=second;
    }
    return (size_t)answer;
}

static double alignmentQuality(CipheredText* ciphered,Bigrams* bigramsHash, size_t line1, size_t line2, int shiftLine1, int shiftLine2){
    if(!bigramsHash || !ciphered || line1>ciphered->nbLines || line2>ciphered->nbLines){
        printf("uninitialized arguments in alignmentQuality\n");
        return -INFINITY;
    }

    double quality = 0.0;
    for(size_t i= 0; i < ciphered->nbColumns ;i++){
        quality += log(getBigramValue(bigramsHash, ciphered->text[line1][MOD((i-shiftLine1),ciphered->nbColumns)], ciphered->text[line2][MOD(i-shiftLine2,ciphered->nbColumns)]));
    }
    return quality;
}


static char* flattenText(char** text, size_t nbColumns, size_t nbLines){
    if(!text){
        printf("text not initialized\n");
        return NULL;
    }
    char* flatText = malloc(nbColumns*nbLines*sizeof(char));
    if(!flatText){
        printf("error allocating memory\n");
        return NULL;
    }
    for(size_t i = 0; i < nbLines; i++){
        for(size_t j = 0; j<nbColumns; j++){
            flatText[i*nbColumns+j] = text[i][j];
        }
    }
    for(size_t i = 0 ;i< nbColumns*nbLines; i++){
        printf("%c",flatText[i] );
    }
    printf("\n");


return flatText;
}

static void printText(char* fileName, char** text, size_t nbColumns, size_t nbLines){
    if(!text || !fileName){
        printf("error\n");
        return;
    }
    FILE* fp = fopen(fileName, "w");
    if(!fp){
        printf("error opening file\n");
        return;
    }
    for(size_t i = 0; i < nbLines ; i++){
        for(size_t j = 0; j<nbColumns ; j++){
            fprintf(fp,"%c",text[i][j]);
        }
        fprintf(fp,"\n");
    }

    return;
}

static void shiftText(char** text,int* shifts,  size_t nbColumns, size_t nbLines){
    if(!text || !shifts){
        return;
    }


    for(size_t i = 0; i<nbLines ; i++){
        char * tmpString = malloc(nbColumns*sizeof(char));
        if(!tmpString){
            printf("error\n");
            return;
        }
        for(size_t j = 0; j<nbColumns ; j++){
            tmpString[j] = text[i][MOD(j-shifts[i],nbColumns)];

        }
        free(text[i]);
        text[i] = tmpString;
    }
    return;
}



static void optimalShiftRec( Bigrams* bigramsHash, CipheredText* ciphered, topShifts** maximums, size_t nbrRows, int maxShifts){
    if(!maximums || !bigramsHash || !ciphered){
        printf("error\n");
        return ;
    }
    if(nbrRows == 0 || nbrRows >= ciphered->nbLines)
        return;
    printf("working on row %lu\n",nbrRows);
    double localMaxQuality = -INFINITY;
    int previousLineShifts = 0;

    if(nbrRows == 1){
        for (int i = -maxShifts; i <= maxShifts; i++){

            localMaxQuality = -INFINITY;
            previousLineShifts = 0;

            for(int j = -maxShifts; j <= maxShifts; j++){
                double currentQ = alignmentQuality(ciphered, bigramsHash, nbrRows-1,nbrRows, j, i);
                
                if(currentQ > localMaxQuality){
                    localMaxQuality = currentQ;
                    previousLineShifts = j;
                }

            }
            maximums[nbrRows][i+maxShifts].previousShift = previousLineShifts;
            maximums[nbrRows][i+maxShifts].cumulativeQuality = localMaxQuality;
        }
        optimalShiftRec( bigramsHash, ciphered, maximums, ++nbrRows, maxShifts);
        return ;
    }

    for (int i = -maxShifts; i <= maxShifts; i++){
        localMaxQuality = -INFINITY;
        previousLineShifts = 0;
        for(int j = -maxShifts; j <= maxShifts; j++){
            double currentQ = alignmentQuality(ciphered, bigramsHash, nbrRows-1, nbrRows, j,i ) + maximums[nbrRows-1][j+maxShifts].cumulativeQuality;

            if(currentQ > localMaxQuality){
                localMaxQuality = currentQ;
                previousLineShifts = j;
            }
        }
        maximums[nbrRows][i+maxShifts].previousShift = previousLineShifts;
        maximums[nbrRows][i+maxShifts].cumulativeQuality = localMaxQuality;
    }
    printf("row %lu done\n",nbrRows);
        optimalShiftRec( bigramsHash, ciphered, maximums, ++nbrRows, maxShifts);
    return; 
}

static topShifts** initTopShifts( CipheredText* ciphered,int maxShifts){
    if(!ciphered || maxShifts < 1)
        return NULL;

    topShifts** maximumsRecord = malloc(ciphered->nbLines* sizeof(topShifts*));
    if(!maximumsRecord)
        return NULL;

    for(size_t i = 0; i< ciphered->nbLines ; i++ ){
        maximumsRecord[i] = malloc(((maxShifts*2)+1)* sizeof(topShifts));
        if(!maximumsRecord[i])
            return NULL;
    }

    return maximumsRecord;
}

static double findBestShifts(topShifts** maximums, int* calculatedShifts, CipheredText* text, size_t maxShifts){
    if(!maximums || !calculatedShifts){
        return -INFINITY;
    }

    for(size_t i = 1; i < text->nbLines ;i++){
        printf("************current row = %lu*****************\n",i);
        for(size_t j = 0; j<=2*maxShifts; j++){
            printf("###for shift %d: \n    previous best shift = %d \n    cumsum with that shift = %lf\n",j-maxShifts, maximums[i][j].previousShift, maximums[i][j].cumulativeQuality );
        }
    }
    //find the maximum quality previously calculated
    double localMaximum = -INFINITY;
    for(size_t i = 0; i <= maxShifts*2 ; i++){
        double currentQuality = maximums[text->nbLines-1][i].cumulativeQuality;
        if(currentQuality > localMaximum){
            localMaximum = currentQuality;
            calculatedShifts[text->nbLines-1] = i-maxShifts;
        }
    }

    //put the shifts that made that maximum in an array 
    for(size_t i = text->nbLines-1; i >= 1; i--){
        calculatedShifts[i-1] = maximums[i][calculatedShifts[i]+maxShifts].previousShift;
    }

return localMaximum;
}


int shifts(unsigned int max_shift, char* ciphered_text, char* bigrams){
    if (!ciphered_text || !bigrams){
        printf("error\n");
        return -1;
    }

    Bigrams* bigramsHash = malloc(sizeof(Bigrams));
    if(initBigrams(bigramsHash,bigrams) != 0){
        printf("error in bigrams init\n");
        return -1;
    }    
    CipheredText* textToCrack = malloc(sizeof(CipheredText));
    if(!initText(textToCrack, ciphered_text)){
        printf("error in ciphered text init\n");
        
        destroyBigrams(bigramsHash);
        return-1;
    }
    //allocate structure for shifts memoazing
    topShifts** maximums = initTopShifts( textToCrack,max_shift);
    if(maximums == NULL){
        printf("error at maximums init\n");
        destroyBigrams(bigramsHash);
        return -1;
    }
    //bottom up recursive call to find the shifts to make to get the maximum value
    optimalShiftRec(bigramsHash, textToCrack, maximums, 1, max_shift);
    printf("out\n");
    
    //allocate best shifts array
    int* optimalShifts = calloc(textToCrack->nbLines, sizeof(int));
    if(!optimalShifts){
        printf("error allocating memory space\n");
        return -1;
    }
    //get back the best shifts scores and fill the optimalShifts array
    double optimalShiftsScore = findBestShifts(maximums, optimalShifts, textToCrack, max_shift);
    if (optimalShiftsScore == -INFINITY){
        return -1;
    }
    //print the array for the best shifts and the best score
    for(size_t i = 0; i<textToCrack->nbLines-1; i++ ){
        fprintf(stdout,"%d ",optimalShifts[i]);
    }
    fprintf(stdout,"%d",optimalShifts[textToCrack->nbLines-1]);
    fprintf(stdout,"\n");
    fprintf(stdout,"%lf\n",optimalShiftsScore);

    printText("copyOfTest.txt",textToCrack->text, textToCrack-> nbColumns, textToCrack->nbLines);
    shiftText(textToCrack->text,optimalShifts, textToCrack-> nbColumns, textToCrack->nbLines);
    printText("testDescifered.txt",textToCrack->text, textToCrack-> nbColumns, textToCrack->nbLines);
    return 0;
}



