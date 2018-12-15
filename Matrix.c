#include <stdlib.h>
#include <stdio.h>
#include "Matrix.h"

/* ------------------------------------------------------------------------- *
 * Matrix structure implemented as a one dimensional array.
 *     M[r, c] = `M->data[r*rowStride + c*colStride]`
 * The rowStride (resp. colStride) indicates the size of the jump in the
 * 1D array to get to the next row (resp. column) with the same column (resp.
 * row).
 * The matrix is supposed to be completely full.
 * ------------------------------------------------------------------------- */
struct matrix_t {
    char* data;             // The actual data of the matrix
    size_t nbrRows;         // The number of rows of the matrix
    size_t nbrColumns;      // The number of columns of the matrix
    size_t rowStride;       // The stride for the row
    size_t colStride;       // The stride for the column
};


// =============================== Accessors ================================ //
/* ------------------------------------------------------------------------- *
 * Computes the index `i` of the element at position `row` x `col` in matrix
 * `matrix`. That is `matrix[row, col] = matrix->data[i]`.
 *
 * PARAMETERS
 * matrix               A valid pointer to a matrix
 * row                  The row number (`row` <= `matrix->nbrRows`)
 * col                  The column number (`col` <= `matrix->nbrColumns`)
 *
 * RETURN
 * i                    The corresponding 1D index
 * ------------------------------------------------------------------------- */
static inline size_t idx_(const Matrix* matrix, size_t row, size_t col)
{
    size_t t = row*matrix->rowStride + col*matrix->colStride,
           size = matrix->nbrRows * matrix->nbrColumns;
    return (t % size) + (t / size);
}


/* ------------------------------------------------------------------------- *
 * Returns the element of the matrix `matrix` at row `row` and column `col`.
 *
 * PARAMETERS
 * matrix               A valid pointer to a matrix
 * row                  The row number (`row` <= `matrix->nbrRows`)
 * col                  The column number (`col` <= `matrix->nbrColumns`)
 *
 * RETURN
 * val                  The value
 * ------------------------------------------------------------------------- */
static inline char get_(const Matrix* matrix, size_t row, size_t col)
{
    return matrix->data[idx_(matrix, row, col)];
}


/* ------------------------------------------------------------------------- *
 * Sets the value `value` in the matrix `matrix` at position (`row`, `col`)
 *
 * PARAMETERS
 * matrix               A valid pointer to a matrix
 * row                  The row number (`row` <= `matrix->nbrRows`)
 * col                  The column number (`col` <= `matrix->nbrColumns`)
 * val                  The value to set
 * ------------------------------------------------------------------------- */
static inline void set_(Matrix* matrix, size_t row, size_t col, char value)
{
    matrix->data[idx_((const Matrix*)matrix, row, col)] = value;
}


// ====================== Creation/deletion of Matrix ======================= //
Matrix* createMatrixForString(char* string, size_t strLength, size_t nbrColumns)
{
    if(!string || !strLength || !nbrColumns || strLength%nbrColumns != 0)
        return NULL;

    Matrix* matrix = malloc(sizeof(Matrix));
    if(!matrix)
        return NULL;

    matrix->rowStride = matrix->nbrColumns = nbrColumns;
    matrix->nbrRows = strLength/nbrColumns;
    matrix->colStride = 1;
    matrix->data = string;

    return matrix;

}


void freeMatrix(Matrix* matrix)
{
    if(matrix != NULL)
    {
        free(matrix->data);
    }
    free(matrix);
}


// =============================== Operations =============================== //
void transpose(Matrix* matrix)
{
    if(!matrix) return;
    // Simply inverse the number of rows and columns and their strides
    *matrix = (Matrix){
        matrix->data,
        matrix->nbrColumns,
        matrix->nbrRows,
        matrix->colStride,
        matrix->rowStride,
    };
}

void flatten(Matrix* matrix)
{
    if(!matrix) return;
    matrix->nbrColumns = matrix->nbrColumns * matrix->nbrRows;
    matrix->nbrRows = 1;
}


bool shiftRows(Matrix* matrix, const int* shifts)
{
    if(!matrix) return true;
    // shifts must be the appropriate size
    char* row = malloc(sizeof(char)*matrix->nbrColumns);
    if(!row)
        return false;

    size_t offset, nbCols = matrix->nbrColumns;
    for(size_t r=0; r<matrix->nbrRows; r++)
    {
        // Offset of the first element
        offset = (shifts[r] < 0)? ((int)nbCols)+shifts[r]: shifts[r];
        // Shift value in row
        for(size_t c=0; c<nbCols; c++)
            row[(c+offset)%nbCols] = get_(matrix, r, c);

        // Copy back in matrix
        for(size_t c=0; c<nbCols; c++)
            set_(matrix, r, c, row[c]);
    }
    free(row);
    return true;
}


// =================================== IO =================================== //
void printMatrix(const Matrix* matrix, FILE* stream)
{
    if(!matrix) return;
    for(size_t r=0; r<matrix->nbrRows; r++)
    {
        for(size_t c=0; c<matrix->nbrColumns; c++)
            fprintf(stream, "%c", get_(matrix, r, c));
        fprintf(stream, "\n");

    }
}
