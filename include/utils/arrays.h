typedef struct DynamicArray
{
    char **keys;
    void **items;
    char *dataType;
    int *itemSizes;
    int length;
    int capacity;
} DynamicArray;

void ArrayCreate(DynamicArray *da, char *dataType);
// TODO: Make me
void ArrayDelete(DynamicArray *da);
void ArrayAppend(DynamicArray *da, char* key, void *item, int itemSize);
// TODO: Make me
DynamicArray *ArrayGetItemsByKey(DynamicArray *da, char *key);
void ArrayRemoveByIndex(DynamicArray *da, int index);
// TODO: Make me
void ArrayRemoveByKey(DynamicArray *da, char *key);
// TODO: Make me
void ArrayRemoveByItem(DynamicArray *da, void *item);