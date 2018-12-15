#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "IOHandler.h"
#include "Matrix.h"

/* ------------------------------------------------------------------------- *
 * Cipher the given plain text according to the given key.
 *
 * PARAMETERS
 * plain_text      A string holding the loaction of the plain text file
 * key             A string holding the location of the key file
 *
 * RETURN
 * returnCode      EXIT_FAILURE if an error occured or EXIT_SUCCESS if
 *                 all went fine
 * ------------------------------------------------------------------------- */
int main(int argc, char** argv)
{
    // Random seed initialization
    srand(time(NULL));

    // ------------------- Parsing command line arguments ------------------- //
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <plain text> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Load the key
    // Rem.: the key length is equal to the number of rows after transposition.
    // It is also equal to the number of columns before transposition
    size_t keyLength = 0;
    int* key = readKey(argv[2], &keyLength);
    if(!key || !keyLength)
    {
        fprintf(stderr, "Error while loading the key. Exiting.\n");
        return EXIT_FAILURE;
    }

    // Load and filter the plain text
    size_t filteredTextLength = 0;
    char* filteredText =  readFilterPlainText(argv[1], keyLength,
                                              &filteredTextLength);
    if(!filteredText || !filteredTextLength)
    {
        fprintf(stderr, "Error while loading the plain text. Exiting.\n");
        free(key);
        return EXIT_FAILURE;
    }

    // ---------------- Ciphering: tranposition and shifts ------------------ //
    Matrix* matrix = createMatrixForString(filteredText, filteredTextLength,
                                           keyLength);
    transpose(matrix);
    shiftRows(matrix, key);
    printMatrix(matrix, stdout);

    // ------------------------- Freeing resources ------------------------- //
    freeMatrix(matrix);
    free(key);
    return EXIT_SUCCESS;
}
