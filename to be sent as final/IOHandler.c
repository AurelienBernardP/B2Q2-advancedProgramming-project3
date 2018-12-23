#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "IOHandler.h"

// ========================= String data structure ========================== //
/* ------------------------------------------------------------------------- *
 * A buffer to read one integer
 * ------------------------------------------------------------------------- */
static const size_t BUFFER_SIZE = 1024;
static const size_t CAPACITY = 256;

/* ------------------------------------------------------------------------- *
 * A string (implemented as a vector)
 * ------------------------------------------------------------------------- */
typedef struct string_t {
    char* data;                 // The content of the string(no '\0' final char)
    size_t length;              // The actual length of the string
    size_t capacity;            // The total capacity (>= length+1)
} String;


/* ------------------------------------------------------------------------- *
 * Allocates a `String` of `length` 0 (with a `capacity` of at least 1)
 *
 * RETURN
 * str               A `String` or NULL if an error occured
 * ------------------------------------------------------------------------- */
static String* createEmptyString()
{
    char* data = calloc(CAPACITY, sizeof(char));
    String* str = malloc(sizeof(String));
    if(!str || !data)
    {
        free(data);
        free(str);
        return NULL;
    }

    str->data = data;
    str->capacity = CAPACITY;
    str->length = 0;

    return str;
}


/* ------------------------------------------------------------------------- *
 * Frees a `String` (and possibly its content)
 *
 * PARAMETERS
 * str               The `String` to be freed (or NULL)
 * freeContent       Whether to free the content as well
 * ------------------------------------------------------------------------- */
static void freeString(String* str, bool freeContent)
{
    if(!str) return;
    if(freeContent)
        free(str->data);
    free(str);
}


/* ------------------------------------------------------------------------- *
 * Append the given C string of length `toAppendLength` to the given `String`
 *
 * PARAMETERS
 * str               A valid pointer to a `String`
 * toAppend          A valid pointer to a C string
 * toAppendLength    The length of `toAppend`
 *
 * RETURN
 * wentFine          False if there was an allocation error. True otherwise
 * ------------------------------------------------------------------------- */
static bool appendToString(String* str, const char* toAppend,
                           size_t toAppendLength)
{
    // Size/capacity of the string
    size_t room = str->capacity - str->length,
           newCapacity = str->capacity;

    // Need to enlarge the vector ?
    while(toAppendLength+1 > room) // +1 to enforce capacity >= length+1
    {
        newCapacity *= 2;
        room = newCapacity - str->length;
    }

    // Enlarge the vector
    if(str->capacity != newCapacity)
    {
        char* data = realloc(str->data, sizeof(char)*newCapacity);
        if(!data)
            return false; // the String is not lost
        str->data = data;
        str->capacity = newCapacity;
    }

    // Append
    for(size_t i=0; i<toAppendLength; i++, str->length++)
        str->data[str->length] = toAppend[i];

    return true;
}


/* ------------------------------------------------------------------------- *
 * Filter the given `String` in place.
 *
 * Note: the given function should be of type bool(*)(char), but this is not
 * the convention of the char predicate of the standard library, so we stick
 * to it.
 *
 * PARAMETERS
 * str               A valid pointer to a `String`
 * f                 A predicate on char
 *
 * RETURN
 * str
 * ------------------------------------------------------------------------- */
static String* filterString(String* str, int (*f)(int))
{

    size_t j = 0;
    for(size_t i=0; i<str->length; i++)
        if((*f)(str->data[i]))
            str->data[j++] = str->data[i];

    str->length = j;

    return str;
}


/* ------------------------------------------------------------------------- *
 * Apply a function elementwise on  the given `String` (in place).
 *
 * Note: the given function should be of type char(*)(char), but this is not
 * the convention of the char predicate of the standard library, so we stick
 * to it.
 *
 * PARAMETERS
 * str               A valid pointer to a `String`
 * f                 A function from char to char
 *
 * RETURN
 * str
 * ------------------------------------------------------------------------- */
 static String* mapString(String* str, int (*f)(int))
{
    for(size_t i=0; i<str->length; i++)
        str->data[i] = (*f)(str->data[i]);

    return str;
}


/* ------------------------------------------------------------------------- *
 * Converts the `String` to a C string and free the `String`
 *
 * PARAMETERS
 * str               A valid pointer to a `String`
 * length            A pointer to size_t which will be overriden to
 *                   contain the final length of `str`
 *
 * RETURN
 * cStr              The C string corresponding to `str`
 * ------------------------------------------------------------------------- */
static char* stringToCharArray(String* str, size_t* length)
{
    // Append '\0' at the end to have a traditional C string
    // since capacity >= length+1, we have room for '\0'
    str->data[str->length] = '\0';

    char* toReturn = str->data;
    *length = str->length;
    freeString(str, false);
    return toReturn;
}


/* ------------------------------------------------------------------------- *
 * Converts the first line of the `String` containing only integers (with
 * no space before the first interger or after the last integer).
 * Frees the `String`
 *
 * PARAMETERS
 * str               A valid pointer to a `String`
 * length            A pointer to size_t which will be overriden to
 *                   contain the final length of the array
 *
 * RETURN
 * intArray          The array of integers
 * ------------------------------------------------------------------------- */
static int* stringToIntArray(String* str, size_t* length)
{
    // Count number of integers on the first line. There is no whitespace before
    // or after the array of ints, therefore the number of ints, is the number
    // of whitespaces + 1
    size_t i = 0, nbrNumbers = 1;
    while(str->data[i] != '\n')
        if(isspace(str->data[i++]))
            nbrNumbers++;

    int* toReturn = malloc(sizeof(int) * nbrNumbers);
    if(!toReturn)
    {
        freeString(str, true);
        *length = 0;
        return NULL;
    }

    // Parse integers
    char* start = str->data;
    char* end;
    long number;
    for(i=0; i<nbrNumbers; i++)
    {
        number = strtol(start, &end, 10);
        if(number == LONG_MAX || number == LONG_MIN)
        {
            // Got something which is not an integer
            freeString(str, true);
            *length = 0;
            free(toReturn);
            return NULL;
        }
        toReturn[i] = number;
        start = end;
    }

    freeString(str, true);
    *length = nbrNumbers;
    return toReturn;
}


// ============================ Local functions ============================= //
/* ------------------------------------------------------------------------- *
 * A predicate for characters which are not line feeds.
 *
 * PARAMETERS
 * c                 A character
 *
 * RETURN
 * notLineFeed       Whether the character is not a line feed
 * ------------------------------------------------------------------------- */
static inline int isntLineFeed(int c){return c != '\n';}


/* ------------------------------------------------------------------------- *
 * Creates a `String` which correspond to the given file
 *
 * PARAMETERS
 * filepath          A readable file-like object containing some text
 *
 * RETURN
 * str               A `String` corresponding to the file
 * ------------------------------------------------------------------------- */
static String* loadFile(const char* filepath)
{
    FILE* fp = fopen(filepath , "r");
    String* str = createEmptyString();
    char buffer[BUFFER_SIZE];

    while(fgets(buffer, BUFFER_SIZE, fp))
    {
        if(!appendToString(str, buffer, strlen(buffer)))
        {
            fclose(fp);
            freeString(str, true);
            return NULL;
        }
    }
    fclose(fp);

    return str;
}


// ========================= IOHandler.h functions ========================== //
char* readFilterPlainText(const char* filepath, size_t nbrColumns,
                          size_t* length)
{
    String* content = loadFile(filepath);
    if(!content)
    {
        *length = 0;
        return NULL;
    }

    // Filter string and cast to lower case
    mapString(filterString(content, isalpha), tolower);

    // Compute padding
    size_t nbrRows = (size_t)ceil((float)(content->length)/nbrColumns);
    size_t nbrToPad = nbrRows*nbrColumns - content->length;
    char padding[nbrToPad];

    for(size_t i=0; i<nbrToPad; i++)
        padding[i] = 'a' + (rand() % 26);

    if(!appendToString(content, padding, nbrToPad))
    {
        *length = 0;
        freeString(content, true);
        return NULL;
    }

    return stringToCharArray(content, length);
}


char* readCipheredText(const char* filepath, size_t* length, size_t* nbrLines)
{
    String* content = loadFile(filepath);
    if(!content)
    {
        *length = 0;
        return NULL;
    }

    size_t fullLength = content->length;

    // Filter string
    filterString(content, isntLineFeed);
    *length = content->length;

    // The only characters which are removed are the line feeds.
    // There is one line feed per line
    *nbrLines = fullLength - content->length;

    return stringToCharArray(content, length);
}

int* readKey(const char* filepath, size_t* length)
{
    String* content = loadFile(filepath);
    if(!content)
    {
        *length = 0;
        return NULL;
    }

    return stringToIntArray(content, length);
}
