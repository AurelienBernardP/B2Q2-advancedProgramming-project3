#ifndef __SHIFTS__
#define __SHIFTS__
#include"shifts.h"
/* ------------------------------------------------------------------------- *
 * deciphers a text by finding the best shifts vector for each line and prints
 * the text on a single line in a file
 *

 *
 * PARAMETERS
 * maxShifts              an unsigned integer, the maximum shifts that can be 
 *                        done on a line
 * ciphered_text          the path to a ciphered text written as matrix of
 *                        characters
 * bigrams                the path to a table containing the bigrams probability
 *                        for the language in which the text is written in
 *
 * RETURN
 * integer                0 on success other on error
 * ------------------------------------------------------------------------- */
int shifts(unsigned int max_shift, char* ciphered_text, char* bigrams);

#endif 