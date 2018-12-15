#include <stdlib.h>
#include <stdio.h>
#include "IOHandler.h"
#include "Matrix.h"

/* ------------------------------------------------------------------------- *
 * Decipher the given ciphered text according to the given key.
 *
 * PARAMETERS
 * ciphered_text        A string holding the location of the ciphered text file
 * key                  A string holding the location of the key file
 * RETURN
 * returnCode           EXIT_FAILURE if an error occured or EXIT_SUCCESS if
 *                      all went fine
 * ------------------------------------------------------------------------- */
int main(int argc, char** argv)
{
    // ------------------- Parsing command line arguments ------------------- //
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <path> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }


    size_t keyLength = 0;
    int* key = readKey(argv[2], &keyLength);
    if(!key || !keyLength)
    {
        fprintf(stderr, "Error while loading the key. Exiting.\n");
        return EXIT_FAILURE;
    }

    size_t nbrRows = 0, length = 0, nbrColumns = 0;
    char* cipheredText = readCipheredText(argv[1], &length, &nbrRows);
    if(!cipheredText)
    {
        fprintf(stderr, "Error while loading the ciphered text. Exiting.\n");
        free(key);
        return EXIT_FAILURE;
    }
    nbrColumns = length / nbrRows;

    if (keyLength != nbrRows){
        fprintf(stderr, "The key must have the same length as the number of "
                        "rows in the ciphered text. Exiting.\n");
        free(cipheredText);
        free(key);
        return EXIT_FAILURE;
    }

    Matrix* matrix = createMatrixForString(cipheredText, length, nbrColumns);
    if(!matrix)
    {
        fprintf(stderr, "Error while creating the matrix. Exiting\n");
        free(cipheredText);
        free(key);
        return EXIT_FAILURE;
    }

    // ------------------------- Actual deciphering ------------------------- //
    // Inverting the key
    for(size_t i=0; i<keyLength; i++)
        key[i] = -key[i];

    shiftRows(matrix, key);
    transpose(matrix);
    flatten(matrix);
    printMatrix(matrix, stdout);


    // ------------------------- Freeing resources ------------------------- //
    freeMatrix(matrix);
    free(key);
    return EXIT_SUCCESS;
}
