/* ========================================================================= *
 * IOHandler is a file that contains functions used by the other files to
 * get usable data out of a text file.
 * ========================================================================= */
#ifndef __IO_HANDLER_H_
#define __IO_HANDLER_H_


/* ------------------------------------------------------------------------- *
 * Reads and filters (i.e. remove everything which is not a letter and cast to
 * lower case) the given plain text.
 *
 * The text will be padded so that it can fit into a 2D array of size N x P,
 * where N = ceil (T/P) is the number of rows,
 *       P is the number of columns, and
 *       T is the actual length of the text (after filtering).
 *
 * The text is padded by random chars, so the srand must be seeded before
 * calling this function.
 *
 * PARAMETERS
 * filepath               A readable file-like object containing the text
 * nbrColumns             The number of columns P
 * length                 A pointer to size_t which will be overriden to
 *                        contain the final length of the text
 *
 * RETURN
 * str                    A C string or NULL in case of error
 * ------------------------------------------------------------------------- */
char* readFilterPlainText(const char* filepath, size_t nbrColumns,
                          size_t* length);


/* ------------------------------------------------------------------------- *
 * Reads a ciphered text and removes all the line feed characters, so that
 * it is a one dimensional string with only lowercase letters.
 *
 * The ciphered text must be a matrix of P x N lowercase letters. There is
 * a line feed '\n' at the end of each row (including the last).
 *
 * PARAMETERS
 * filepath               A readable file-like object containing the text
 * length                 A pointer to size_t which will be overriden to
 *                        contain the final length of the text
 * nbrColumns             A pointer to size_t which will be overriden to
 *                        contain the number of lines the ciphered had
 *
 * RETURN
 * str                    A C string or NULL in case of error
 * ------------------------------------------------------------------------- */
char* readCipheredText(const char* filepath, size_t* length, size_t* nbrLines);


/* ------------------------------------------------------------------------- *
 * Reads the key.
 *
 * The key file must have integers on the first line. Integers are separated
 * with exatly one space (no space and the end or the beginning) and the line
 * terminates by a line feed '\n'. The rest is ignored.
 *
 * PARAMETERS
 * filepath               A readable file-like object containing the key
 * length                 A pointer to size_t which will be overriden to
 *                        contain the final length of the key
 *                        (i.e. number of integers)
 *
 * RETURN
 * key                    An array of integer or NULL in case of error
 * ------------------------------------------------------------------------- */
int* readKey(const char* filepath, size_t* length);


#endif // !__IO_HANDLER_H_
