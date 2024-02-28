#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>

#include "../../include/utils/arrays.h"

// TODO: Global arrays variable for cleanup after the program executes

/**
    Creates a dynamic array on the heap.
    @param da: A pointer [DynamicArray*] to a blank dynamic array
    @param dataType: A string representation of the data type being stored
    @returns Nothing
*/
void ArrayCreate(DynamicArray *da, char *dataType)
{
    char *allocatedDataType = malloc(strlen(dataType));
    strcpy(allocatedDataType, dataType);

    da->capacity = 256;
    da->length = 0;
    da->keys = malloc(sizeof(char*) * da->capacity);
    da->items = malloc(sizeof(void*) * da->capacity);
    da->itemSizes = (int*)malloc(sizeof(int) * da->capacity);
    da->dataType = allocatedDataType;
}

void ArrayDelete(DynamicArray *da)
{
    // TODO: Make me
}

/**
    Resizes a dynamic array when necessary.
    @param da: A pointer [DynamicArray*] to an allocated dynamic array
    @param capacity: The new size [int] that the dynamic array should be
    @returns Nothing
*/
void ArrayResize(DynamicArray *da, int capacity)
{
    void **items = realloc(da->items, sizeof(void*) * capacity);
    if(items)
    {
        da->items = items;
        da->capacity = capacity;
    }
    // TODO: Error checking

    void *keys = realloc(da->keys, sizeof(int) * capacity);
    if(keys)
    {
        da->keys = keys;
    }
    // TODO: Error checking

    void *dataSizes = realloc(da->itemSizes, sizeof(int) * capacity);
    if(dataSizes)
    {
        da->itemSizes = dataSizes;
    }
    // TODO: Error checking
}

/**
    Appends an item to a dynamic array. When appending, you must malloc the item. The key and
    itemSize should not be malloc'd.
    @param da: A pointer [DynamicArray*] to an allocated dynamic array
    @param key: A key [string] for the item being appended
    @param item: A pointer [void*] to an item to append
    @param itemSize: The size [int] of the item being appended
    @returns Nothing
*/
void ArrayAppend(DynamicArray *da, char* key, void *item, int itemSize)
{
    char *allocatedKey = malloc(strlen(key) + 1);

    memcpy(allocatedKey, key, strlen(key));
    allocatedKey[strlen(key)] = 0;

    if(da->length >= da->capacity)
        ArrayResize(da, da->capacity*2);

    da->keys[da->length] = allocatedKey;
    da->itemSizes[da->length] = itemSize;
    da->items[da->length++] = item;
    
    // TODO: Maybe type checking to make sure the item is the same type as the others in the DA
}

/**
    Gets all items from a dynamic array with a given key.
    @param da: A pointer [DynamicArray*] to an allocated dynamic array
    @param key: A key [char*] associated with an item
    @returns A pointer [DynamicArray*] to the array of items
*/
DynamicArray *ArrayGetItemsByKey(DynamicArray *da, char *key)
{
    DynamicArray *items = malloc(sizeof(DynamicArray));
    ArrayCreate(items, "void*");

    for(int i = 0; i < da->length; i++)
    {
        if(strcmp(key, da->keys[i]) == 0)
            ArrayAppend(items, "", da->items[i], sizeof(da->items[i]));
    }

    // TODO: Better return if nothing is found
    return items;
}

/**
    Removes an item from a dynamic array by its index.
    @param da: A pointer [DynamicArray*] to an allocated dynamic array
    @param index: An index [int] to an item to remove
    @returns Nothing
*/
void ArrayRemoveByIndex(DynamicArray *da, int index)
{
    // TODO: Better error checking
    if(index >= da->length || index < 0)
        return;

    da->length--;

    for(int i = index; i < da->length; i++)
    {
        da->keys[i] = da->keys[i+1];
        da->items[i] = da->items[i+1];
        da->itemSizes[i] = da->itemSizes[i+1];
    }
}

/**
    Removes an item from a dynamic array by item data.
    @param da: A pointer [DynamicArray*] to an allocated dynamic array
    @param item: An item [void*] to remove
    @returns Nothing
*/
void ArrayRemoveByItem(DynamicArray *da, void *item)
{
    for(int i = 0; i < da->length; i++)
    {
        if(memcmp(da->items[i], item, da->itemSizes[i]) == 0)
        {
            ArrayRemoveByIndex(da, i);
            break;
        }
    }

    // TODO: Add a message if nothing is found
}