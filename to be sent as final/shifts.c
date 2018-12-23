#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Matrix.h"
#include"IOHandler.h"
#include"shifts.h"

/* ------------------------------------------------------------------------- *
 * A table of bigrams 
 * ------------------------------------------------------------------------- */
typedef struct{
    unsigned int nbLetters;// number of different letters in this alphabet
    double* table;         // actual table containing the probabilities
}Bigrams;
/* ------------------------------------------------------------------------- *
 * A text implemented as a matrix of characters
 * ------------------------------------------------------------------------- */
typedef struct{
    size_t nbColumns;   // number of columns in the matrix
    size_t nbLines;     // number of lines in the matrix 
    char** text;        //actual matrix containing the characters
}CipheredText;

/* ------------------------------------------------------------------------- *
 * Structure to save and memoize the previous best shift executed on a line
 * ------------------------------------------------------------------------- */
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
/*-------------------------------------------------------------------
*  FUNCTION destroyBigrams :
*  This function frees the allocated memmory for a bigrams structure
*   to each other in a text
*
*  INPUT :
*     - Bigams: a valid pointer to a bigramsHash structure
*
--------------------------------------------------------------------*/
static void destroyBigrams(Bigrams* bigrams){
    if(!bigrams){
        return;
    }
    if(bigrams->table)
        free(bigrams->table);
    free(bigrams);
return;
}
/*-------------------------------------------------------------------
*  FUNCTION destroyCipheredText
*  This function frees a CipheredText structure
*
*  INPUT :
*     - ciphered: a valid pointer to an initialized and empty 
*                 CipheredText structure
*
--------------------------------------------------------------------*/
static void destroyCipheredText(CipheredText* ciphered ){
    if(!ciphered){
        return ;
    }
    if(ciphered->text == NULL){
        free(ciphered);
        return;
    }
    for(size_t i = 0; i < ciphered->nbLines ; i++){
        if(ciphered->text[i] != NULL){
            free(ciphered->text[i]);
        }
    }
    free(ciphered->text);
    free(ciphered);
    return;
}
/*-------------------------------------------------------------------
*  FUNCTION initText :
*  This function places the text located on file in a CipheredText
*   structure
*
*  INPUT :
*     - ciphered: a valid pointer to an initialized and empty 
*                 CipheredText structure
*     - textPath: a valid path to a correctly formated file
*                 the text should be written as a matrix
*
*  OUTPUT :
*     - NULL pointer on error 
*     - ciphered pointer to a CipheredText structure on success
--------------------------------------------------------------------*/
static CipheredText* initText(CipheredText* ciphered ,char* textPath){
    if(!ciphered || !textPath){
        fprintf(stderr,"error uninitialized arguments\n");
        destroyCipheredText(ciphered);
        return NULL;
    }


    FILE* fp= fopen(textPath, "r");
    if(!fp){
        fprintf(stderr,"error opening ciphered text\n");
        destroyCipheredText(ciphered);
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
        fprintf(stderr,"error making one line text\n");
        destroyCipheredText(ciphered);
        return NULL;
    }

    ciphered->text = malloc(ciphered->nbLines * sizeof(char*));
    if(!ciphered->text){
        fprintf(stderr,"error allocating matrix text\n");
        destroyCipheredText(ciphered);
        return NULL;
    }

    
    for(size_t i = 0; i < ciphered->nbLines;i++){
        char* tmpString = malloc(ciphered->nbColumns * sizeof(char));
        if(!tmpString){
            fprintf(stderr,"error allocating tmp string\n");
            destroyCipheredText(ciphered);
            return NULL;
        }
        //will not be freed in this function as createMatrixFrom sting doesn't copy the string it keeps a pointer to first element.
        for(size_t j = 0; j < ciphered->nbColumns; j++){
            tmpString[j] = oneLineText[ciphered->nbColumns*i + j]; 
        }
        ciphered->text[i] = tmpString;
        if(!ciphered->text[i]){
            fprintf(stderr,"error making line matrix\n");
            destroyCipheredText(ciphered);
            return NULL;
        }
        
    }

return ciphered;
}


/* ------------------------------------------------------------------------- *
 * Algorithm to find the integer least sqrt of a number using newton's method
 * 
 * INPUT
 *              -number:  a positive integer 
 * RETURN
 * x              A Integer, the closest integer sqrt of number
 * ------------------------------------------------------------------------- */
static unsigned int intSqrt(unsigned int number){
    unsigned int  y = number;
    unsigned int x = number+1;
    while(y<x){
        x = y;
        y = (x + (number/x))/2;
    }
    return x;
}

/*-------------------------------------------------------------------
*  FUNCTION initBigrams :
*  This function places the table located on file in a Bigrams
*   structure
*
*  INPUT :
*     - bigramsHash: a valid pointer to an initialized and empty 
*                 Bigrams structure
*     - bigramsFile: a valid path to a correctly formated file
*                    containing the bigrams vales
*
*  OUTPUT :
*     - -1 on error 
*     - 0 on success
--------------------------------------------------------------------*/
static int initBigrams(Bigrams* bigramsHash, char* bigramsFile){
    if(!bigramsFile || !bigramsHash){
        fprintf(stderr,"error uninitialized arguments\n");
        return -1;
    }
    FILE* fp = fopen(bigramsFile, "r");// open file and check opening
    if(!fp){
        fprintf(stderr,"error opening bigrams\n");
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
        fprintf(stderr,"error allocating memory");
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
        fprintf(stderr,"%u ? %lu, error computing the number of letter in the bigram's alphabet",nbLettersInAlphabet*nbLettersInAlphabet, fileLength);
        return -1;
    }

    fclose(fp);

return 0;
}

/* ------------------------------------------------------------------------- *
 * Algorithm to find the positive MOD of two numbers
 * 
 * INPUT
 *              -first:  an integer
 *              -second: an integer 
 * RETURN
 * answer              the positive MOD of the two numbers
 * ------------------------------------------------------------------------- */
static size_t MOD(int first, int second){
    int answer = first % second;
    if(answer<0){
        answer+=second;
    }
    return (size_t)answer;
}

/*-------------------------------------------------------------------
*  FUNCTION alignmentQuality :
*  This function measures the alignment quality of two shifted lines by
*   using the values in the bigrams table.
*
*  INPUT :
*     - ciphered: a valid pointer to an initialized CipheredText structure
*     - bigramshash: a valid pointer to an initialized Bigrams structure
*     - line1 & line2: the two line the alignment quality is beeing measured of
*     - shift1 & shift 2: the respective shifts that have been done to each line
*
*  OUTPUT :
*     - -inf on error 
*     - the alignment quality on success
--------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------
*  FUNCTION optimalShiftRec :
*  Recursive function calculating the optimal shifts ro maximize the
*   alignment quality of the matrix
*
*  INPUT :
*     - ciphered: a valid pointer to an initialized CipheredText structure
*     - bigramshash: a valid pointer to an initialized Bigrams structure
*     - maximums: a valid pointer to a matrix of maximums alignments (TopShifts Structures)
*     - maxShifts: maximum of shifts that can be done.
*     - nBrows: the number of lines in which the maximization is taking place
*
*  OUTPUT :
*     - all th best shifts options are stored in maximums
--------------------------------------------------------------------*/
static void optimalShiftRec( Bigrams* bigramsHash, CipheredText* ciphered, topShifts** maximums, size_t nbrRows, int maxShifts){
    if(!maximums || !bigramsHash || !ciphered){
        fprintf(stderr,"error uninitialized arguments\n");
        return ;
    }
    if(nbrRows == 0 || nbrRows >= ciphered->nbLines)
        return;
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

        optimalShiftRec( bigramsHash, ciphered, maximums, ++nbrRows, maxShifts);
    return; 
}

/*-------------------------------------------------------------------
*  FUNCTION destroyTopShiftsMat :
*  This function frees a maxShifts structure matrix
*
*  INPUT :
*     - maximums: a valid pointer to a matrix of maximums alignments (TopShifts Structures)
*     - nbLines: number of lines in the matrix.
--------------------------------------------------------------------*/
static void destroyTopShiftsMat(topShifts** maximums, size_t nbLines){
    if(maximums == NULL){
        return;
    }
    for(size_t i = 0; i<nbLines;i++){
        if(maximums[i]!=NULL){
            free(maximums[i]);
        }
    }
    free(maximums);
    return;
}

/*-------------------------------------------------------------------
*  FUNCTION initTopShifts :
*  This function initializes a matrix of TopShifts structures to 
*   memoize the values calculated with each shift
*   structure
*
*  INPUT :
*     - ciphered: a valid pointer to an initialized CipheredText structure
*     - maxShifts: maximum of shifts that can be done.
*
*  OUTPUT :
*     - NULL pointer on error 
*     - initialized pointer on success
--------------------------------------------------------------------*/
static topShifts** initTopShifts( CipheredText* ciphered,int maxShifts){
    if(!ciphered || maxShifts < 1)
        return NULL;

    topShifts** maximumsRecord = malloc(ciphered->nbLines* sizeof(topShifts*));
    if(!maximumsRecord)
        return NULL;

    for(size_t i = 0; i< ciphered->nbLines ; i++ ){
        maximumsRecord[i] = malloc(((maxShifts*2)+1)* sizeof(topShifts));
        if(!maximumsRecord[i]){
            destroyTopShiftsMat(maximumsRecord, ciphered->nbLines);
            return NULL;
        }
    }

    return maximumsRecord;
}


/*-------------------------------------------------------------------
*  FUNCTION findBestShifts :
*  function that retraces backwards in the topShifts matrix the shifts to be
*   done in order to get the maximum value.
*
*  INPUT :
*     - text: a valid pointer to an initialized CipheredText structure
*     - calculatedShifts: a valid pointer to an initialized integer array where the shifts
*       will be stored
*     - maximums: a valid pointer to a matrix of maximums alignments (TopShifts Structures)
*     - maxShifts: maximum of shifts that can be done.
*
*  OUTPUT :
*     - -inf on error
*     - the updated array and the value of the overall maximum
--------------------------------------------------------------------*/
static double findBestShifts(topShifts** maximums, int* calculatedShifts, CipheredText* text, size_t maxShifts){
    if(!maximums || !calculatedShifts){
        fprintf(stderr,"error uninitialized arguments\n");
        return -INFINITY;
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
        fprintf(stderr,"error uninitialized arguments\n");
        return -1;
    }

    Bigrams* bigramsHash = malloc(sizeof(Bigrams));
    if(initBigrams(bigramsHash,bigrams) != 0){
        fprintf(stderr,"error in bigrams init\n");
        return -1;
    }    
    CipheredText* textToCrack = malloc(sizeof(CipheredText));
    if(!initText(textToCrack, ciphered_text)){
        fprintf(stderr,"error in ciphered text init\n");
        
        destroyBigrams(bigramsHash);
        return-1;
    }
    //allocate structure for shifts memoazing
    topShifts** maximums = initTopShifts( textToCrack,max_shift);
    if(maximums == NULL){
        fprintf(stderr,"error at maximums init\n");
        destroyBigrams(bigramsHash);
        destroyCipheredText(textToCrack);
        return -1;
    }
    //bottom up recursive call to find the shifts to make to get the maximum value
    optimalShiftRec(bigramsHash, textToCrack, maximums, 1, max_shift);

    
    //allocate best shifts array
    int* optimalShifts = calloc(textToCrack->nbLines, sizeof(int));
    if(!optimalShifts){
        fprintf(stderr,"error allocating memory space\n");
        destroyBigrams(bigramsHash);
        destroyTopShiftsMat(maximums,textToCrack->nbLines );
        destroyCipheredText(textToCrack);
        return -1;
    }
    //get back the best shifts scores and fill the optimalShifts array
    double optimalShiftsScore = findBestShifts(maximums, optimalShifts, textToCrack, max_shift);
    if (optimalShiftsScore == -INFINITY){
        fprintf(stderr,"error tracing back top shifts\n");
        destroyBigrams(bigramsHash);
        destroyTopShiftsMat(maximums,textToCrack->nbLines );
        free(optimalShifts);
        destroyCipheredText(textToCrack);
        return -1;
    }
    //print the array for the best shifts and the best score
    for(size_t i = 0; i<textToCrack->nbLines-1; i++ ){
        fprintf(stdout,"%d ",-optimalShifts[i]);
    }
    fprintf(stdout,"%d",-optimalShifts[textToCrack->nbLines-1]);
    fprintf(stdout,"\n");
    fprintf(stdout,"%lf\n",optimalShiftsScore);

    destroyBigrams(bigramsHash);
    destroyTopShiftsMat(maximums,textToCrack->nbLines );
    free(optimalShifts);
    destroyCipheredText(textToCrack);
    return 0;
}



