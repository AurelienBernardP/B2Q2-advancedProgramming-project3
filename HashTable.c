#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "HashTable.h"

const double MAX_LOAD_FACTOR = 0.7;

/*-------------------------------------------------------------------
*  STRUCTURE Cell :
*  This structure represents a cell of the hash table (more precisely,
*  a cell of the array representing the hash table)
*
*  FIELDS :
*     - key : a key value
*     - value : a value associated to key
--------------------------------------------------------------------*/
typedef struct
{
    void* value;
    void* key;
} Cell;


/*-------------------------------------------------------------------
*  STRUCTURE HashTable :
*  This structure represents a hashtable.
*
*  FIELDS :
*     - compareFunction : The comparison function returns an integer less than,
*       equal to, or greater than zero if $a$ is found, respectively, to be
*       less than, to match, or be greater than $b$.
*     - encodingFunction : The encoding function returns a positive integer
*       value for key $key$
*     - capacity : the total capacity of the hashtable
*     - nbElem : the current number of elements
*     - hashArray : the array of the hashtable
--------------------------------------------------------------------*/
struct HashTable_t
{
    int (*compareFunction)(const void* a, const void* b);
    size_t (*encodingFunction)(const void* key);
    size_t capacity;
    size_t nbElem;
    Cell* hashArray;
};



/*-------------------------------------------------------------------
*  FUNCTION hash :
*  Quadratic probing hash function.
*
*  INPUT :
*     - rawIndex : raw index
*     - nbCellCovered : a value associated to $key$
*     - capacity : the capacity of the hash table
*  OUTPUT :
*     - an index value between 0 and (capacity - 1)
--------------------------------------------------------------------*/
static inline size_t hash(size_t rawIndex, size_t nbCellCovered, size_t capacity)
{
    return (rawIndex +  nbCellCovered * (nbCellCovered + 1)/2) % capacity;
}

/*-------------------------------------------------------------------
*  FUNCTION searchElement :
*  This function search in the hash table for the element matching
*  with the given key. It returns a pointer to the Cell
*  associated with the given key if a match is found. Otherwise, it
*  returns a NULL pointer.
*
*  If $key$ or $hashTable$ are a NULL pointer the function returns a NULL
*  pointer.
*
*  INPUT :
*     - hashTable : a pointer to the HashTable element
*     - key : a key value to search for in the hashTable
*  OUTPUT :
*     - pointer to a Cell element of which the key match with $key$
*     - NULL if ERROR or if no matching
--------------------------------------------------------------------*/
static Cell* searchElement(const HashTable* hashTable, const void* key)
{
    if(hashTable == NULL || key == NULL)
        return NULL;

    size_t nbCellCovered = 0, hashedKey = 0;

    Cell* hashArray = hashTable->hashArray;

    hashedKey = hash(hashTable->encodingFunction(key), nbCellCovered, hashTable->capacity);

    while((hashArray[hashedKey].key != NULL) && (nbCellCovered < hashTable->capacity))
    {
        // Check whether the key is matching with the key of the current cell
        if(!hashTable->compareFunction(key, hashArray[hashedKey].key))
        {
            return &(hashArray[hashedKey]);
        }

        nbCellCovered++;
        // Hash the key
        hashedKey = hash(hashTable->encodingFunction(key), nbCellCovered, hashTable->capacity);
    }
    // Key not found
    return NULL;
}

/*-------------------------------------------------------------------
*  FUNCTION getLoadFactor :
*  This function returns the load factor of a hash table. The pointer
*  to the hash table must be valid and the hash table must be initialized.
*
*  INPUT :
*     - hashTable : a valid pointer to the HashTable
*  OUTPUT :
*     - load factor of the given table
*     - -1 if an error occurs
--------------------------------------------------------------------*/
static inline double getLoadFactor(const HashTable* hashTable)
{
    if(hashTable && hashTable->capacity != 0)
        return ((double) hashTable->nbElem)/hashTable->capacity;
    else
        return -1;
}

/*-------------------------------------------------------------------
*  FUNCTION rehash :
*  This function rehashes in-place the key-value pairs of the given HashTable
*  in a twice wider array.
*
*  INPUT :
*     - hashTable : a pointer to the HashTable element
*  OUTPUT :
*     - true if the reshashing went fine
*     - false otherwise (e.g. memory problem)
--------------------------------------------------------------------*/
static bool rehash(HashTable* hashTable)
{
    if(!hashTable)
        return false;

    Cell* oldHashArray = hashTable->hashArray;
    Cell* newHashArray = NULL;

    size_t oldCapacity = hashTable->capacity,
           newCapacity = 2 * oldCapacity,
           oldNbElem = hashTable->nbElem;

    // creating the new hash array
    newHashArray = (Cell*) calloc(newCapacity, sizeof(Cell));

    // allocation has failed
    if(newHashArray == NULL)
        return false;


    // resetting the hash table structure's fields
    hashTable->capacity = newCapacity;
    hashTable->nbElem = 0;
    hashTable->hashArray = newHashArray;

    // copying data from old to new hash table
    for(size_t i = 0; i < oldCapacity; i++)
    {
        // Skip empty cells
        if(!oldHashArray[i].key)
            continue;

        if(!insertElement(hashTable, oldHashArray[i].key, oldHashArray[i].value))
        {
            // Copy has failed, restoring old hash table and freeing new HashArray;
            hashTable->capacity = oldCapacity;
            hashTable->nbElem = oldNbElem;
            hashTable->hashArray = oldHashArray;
            free(newHashArray);
            return false;
        }

    }

    // Copy has succeed, freeing old hash array
    free(oldHashArray);
    return true;

}

/*-------------------------------------------------------------------
*  FUNCTION getUpperPowerOfTwo :
*  This function returns the upper power of two of the non-zero positive
*  integer $v$.
*
*  INPUT :
*     - v : a non-zero positive integer
*  OUTPUT :
*     - the upper power of two of $v$
--------------------------------------------------------------------*/
static inline size_t getUpperPowerOfTwo(size_t v)
{
    return (v == 0)? 1 : 1 << ((size_t)log2(v) + 1);
}



HashTable* createHashTable(size_t capacity,
                           size_t (*encodingFunction)(const void* key),
                           int (*compareFunction)(const void* a, const void* b))
{
    // Error cases
    if(capacity <= 0 || !encodingFunction || !compareFunction)
        return NULL;

    HashTable* myHashTable = (HashTable*) malloc(sizeof(HashTable));

    if(!myHashTable)
        return NULL;

    // Initialization of the HashTable
    capacity = getUpperPowerOfTwo(capacity);
    myHashTable->compareFunction = compareFunction;
    myHashTable->encodingFunction = encodingFunction;
    myHashTable->capacity = capacity;
    myHashTable->nbElem = 0;

    // Allocate memory for the hashTable's array
    myHashTable->hashArray = (Cell*) calloc(capacity, sizeof(Cell));

    if(!myHashTable->hashArray)
    {
        free(myHashTable);
        return NULL;
    }

    return myHashTable;
}

void freeHashTable(HashTable* hashTable, bool freeContent)
{
    if(!hashTable)
        return;

    if(freeContent)
    {
        Cell* cell;
        for(size_t i=0; i < hashTable->capacity; i++)
        {
            cell = hashTable->hashArray + i;
            if(cell)
            {
                free(cell->key);
                free(cell->value);
            }
        }

    }
    free(hashTable->hashArray);
    free(hashTable);
}

void* insertElement(HashTable* hashTable, void* key, void* value)
{
    // Error cases
    if(hashTable == NULL || hashTable->hashArray == NULL || key == NULL)
        return NULL;

    // ReHash ?
    double loadFactor = getLoadFactor(hashTable);
    if(loadFactor > MAX_LOAD_FACTOR)
    {
        if(!rehash(hashTable))
            return NULL;
        loadFactor = getLoadFactor(hashTable);
    }



    size_t nbCellCovered = 0,
           rawIndex = hashTable->encodingFunction(key),
           index = 0;

    Cell* hashArray = hashTable->hashArray;

    do{
        index = hash(rawIndex, nbCellCovered, hashTable->capacity);

        // Check whether the key is already in the hashtable
        if(hashArray[index].key != NULL)
        {
            if(!hashTable->compareFunction(key, hashArray[index].key))
            {
                // Changing value whether the key is found
                void* valueToReturn = hashArray[index].value;
                hashArray[index].value = value;
                return valueToReturn;
            }
        } else {
            // Spot found, insert
            hashArray[index].key = key;
            hashArray[index].value = value;
            hashTable->nbElem++;
            return key;
        }

    }while(++nbCellCovered < hashTable->capacity);

    // hashTable's array is full
    return NULL;
}



size_t getNumElements(const HashTable* hashTable)
{
    if(!hashTable)
        return 0;

    return hashTable->nbElem;
}

bool hasKey(const HashTable* hashTable, const void* key)
{
    if(!hashTable || !hashTable->hashArray || !key)
        return false;

    Cell* searchResult = searchElement(hashTable, key);

    return (searchResult == NULL)? false : true;
}


void* getValue(const HashTable* hashTable, const void* key)
{
    if(!hashTable || !hashTable->hashArray || !key)
        return NULL;

    Cell* searchResult = searchElement(hashTable, key);
    if(!searchResult)
        return NULL;
    return searchResult->value;

}
