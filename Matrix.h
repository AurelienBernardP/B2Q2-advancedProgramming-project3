/* ========================================================================= *
 * cipheringUtils is a file that contains functions used to (de)cipher texts
 * ========================================================================= */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdbool.h>
/* ------------------------------------------------------------------------- *
 * Opaque structure of a matrix (of chars)
 * ------------------------------------------------------------------------- */
typedef struct matrix_t Matrix;

/* ------------------------------------------------------------------------- *
 * Allocates a `strLength/nbrColums` by `nbrColumns` char matrix for the
 * given string
 *
 * PARAMETERS
 * string               A valid pointer to a char*
 * strLength            The length of `string`
 * nbrColumns           The number of columns the matrix will have
 *
 * RETURN
 * matrix               A matrix or NULL if an error occured
 * ------------------------------------------------------------------------- */
Matrix* createMatrixForString(char* string, size_t strLength, size_t nbrColumns);


/* ------------------------------------------------------------------------- *
 * Frees a matrix. If a NULL pointer is given, does nothing
 *
 * PARAMETERS
 * matrix               The matrix to be freed (including its data)
 * ------------------------------------------------------------------------- */
void freeMatrix(Matrix* matrix);


/* ------------------------------------------------------------------------- *
 * Transposes the matrix inplace (O(1))
 *
 * PARAMETERS
 * matrix               A valid pointer to the matrix to transpose
 * ------------------------------------------------------------------------- */
void transpose(Matrix* matrix);


/* ------------------------------------------------------------------------- *
 * Flattens a `r` x `c` matrix inplace (O(1)). The resulting matrix is of
 * shape `1` x `r*c`.
 *
 * PARAMETERS
 * matrix               A valid pointerto the matrix to flatten
 * ------------------------------------------------------------------------- */
void flatten(Matrix* matrix);


/* ------------------------------------------------------------------------- *
 * Shifts each row `r_i` of the matrix by `shifts[i]` inplace. The shift is
 * circular.
 *
 * PARAMETERS
 * matrix               A valid pointer to the matrix to reorder
 * shifts               A valid array of ints of length equal to the number of
 *                      of rows in the matrix
 *
 * RETURN
 * wentFine             A boolean indicating the everything went fine
 * ------------------------------------------------------------------------- */
bool shiftRows(Matrix* matrix, const int* shifts);


/* ------------------------------------------------------------------------- *
 * Prints the matrix on the given stream
 *
 * PARAMETERS
 * matrix               A valid pointer to the matrix to print
 * stream               A writable file-like object
 * ------------------------------------------------------------------------- */
void printMatrix(const Matrix* matrix, FILE* stream);

#endif // !_MATRIX_H_
