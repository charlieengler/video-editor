#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../../include/ui/text.h"
#include "../../include/ui/buttons.h"
#include "../../include/utils/arrays.h"

#define OPENING_TAGS_INDEX 0
#define ATTRIBUTES_INDEX 1
#define TEXT_INDEX 2
#define CLOSING_TAGS_INDEX 3

/**
    Reads a layout file and returns the raw data
    @param filename: The name [string] of the layout file to read
    @returns The contents of the file [string]
*/
char* ReadLayoutFile(char* filename)
{
    FILE *layoutFile = fopen(filename, "r");
    if(layoutFile == NULL)
    {
        // TODO: Error handling
        return NULL;
    }

    fseek(layoutFile, 0L, SEEK_END);
    long length = ftell(layoutFile);
    rewind(layoutFile);

    char *contents = malloc(length + 1);
    if(!fread(contents, length, 1, layoutFile))
    {
        if(ferror(layoutFile))
        {
            // TODO: Error handling
        }
        else
        {
            // TODO: End of file handling
        }
    }

    // TODO: fread returns something lol
    fclose(layoutFile);

    return contents;
}

// TODO: Documentation
void GenerateKey(char *key, char *base, int index)
{
    char indexString[12];
    strcpy(key, base);
    sprintf(indexString, "%d", index);
    strcat(key, indexString);

    key[strlen(base) + strlen(indexString)] = 0;
}

// TODO: Documentation
void AddOpeningTag(DynamicArray *openingTags, char *currentTag, int currentTagIndex, int *commonIndex)
{
    char *allocatedTag = malloc(strlen(currentTag));
    strcpy(allocatedTag, currentTag);

    ArrayAppend(openingTags, "", allocatedTag, currentTagIndex+1);
    (*commonIndex)++;
}

// TODO: Documentation
void AddAttribute(DynamicArray *attributes, char *currentAttribute, char *currentTag, int currentAttributeIndex, int commonIndex)
{
    char *allocatedAttribute = malloc(strlen(currentAttribute));
    strcpy(allocatedAttribute, currentAttribute);

    char key[140];
    GenerateKey(key, currentTag, commonIndex); 

    ArrayAppend(attributes, key, allocatedAttribute, currentAttributeIndex+1);
}

// TODO: Documentation
void AddText(DynamicArray *text, char *currentTag, char *currentText, int currentTextIndex, int commonIndex)
{
    char *allocatedText = malloc(strlen(currentText));
    strcpy(allocatedText, currentText);

    char key[140];
    GenerateKey(key, currentTag, commonIndex-1);

    ArrayAppend(text, key, allocatedText, currentTextIndex+1);
}

// TODO: Documentation
void AddClosingTag(DynamicArray *closingTags, char *currentTag, int currentTagIndex)
{
    char *allocatedTag = malloc(strlen(currentTag));
    strcpy(allocatedTag, currentTag);

    ArrayAppend(closingTags, "", allocatedTag, currentTagIndex+1);
}

// TODO: Documentation
// TODO: Add comments within this function, it's a mess!
DynamicArray *FormLayout(char* rawLayout)
{
    DynamicArray *openingTags = malloc(sizeof(DynamicArray));
    ArrayCreate(openingTags, "char*");

    DynamicArray *attributes = malloc(sizeof(DynamicArray));
    ArrayCreate(attributes, "char*");

    DynamicArray *text = malloc(sizeof(DynamicArray));
    ArrayCreate(text, "char*");

    DynamicArray *closingTags = malloc(sizeof(DynamicArray));
    ArrayCreate(closingTags, "char*");

    int commonIndex = 0;
    int readingTag = 0;
    int readingAttribute = 0;
    int readingText = 0;
    int closingTag = 0;

    // TODO: Note somewhere that the tag length limit is 128 characters including the tag type
    int currentTagIndex = 0;
    char currentTag[128]; 

    // TODO: Note somewhere that the attribute length limit is 2048 characters including the attribute type
    int currentAttributeIndex = 0;
    char currentAttribute[2048]; 

    // TODO: Note somewhere that the innerHTML text limit is 65536 characters
    int currentTextIndex = 0;
    char currentText[65536]; 

    for(long unsigned int i = 0; rawLayout[i] != 0; i++)
    {
        // TODO: Attributes are included in closing tags and shouldn't be
        switch(rawLayout[i])
        {
            // TODO: The < character cannot be included in text in this configuration
            case '<':
                if(!readingTag)
                {
                    currentText[currentTextIndex] = 0;

                    if(readingText)
                    {
                        int allNewlines = 1;
                        for(int j = 0; j < currentTextIndex; j++)
                            if(currentText[j] != 10 && currentText[j] != 13 && currentText[j] != 32)
                                allNewlines = 0;

                        if(!allNewlines)
                            AddText(text, currentTag, currentText, currentTextIndex, commonIndex);

                        currentTextIndex = 0;
                    }

                    readingTag = 1;
                    readingAttribute = 0;
                    readingText = 0;
                    if(rawLayout[i+1] == '/')
                        closingTag = 1;

                    continue;
                }
                break;

            case '>':
                readingText = 1;

                if(readingTag)
                {
                    currentTag[currentTagIndex] = 0;
                    currentAttribute[currentAttributeIndex] = 0;

                    if(currentAttributeIndex > 0)
                        AddAttribute(attributes, currentAttribute, currentTag, currentAttributeIndex, commonIndex);

                    if(!closingTag)
                        AddOpeningTag(openingTags, currentTag, currentTagIndex, &commonIndex);
                    else
                        AddClosingTag(closingTags, currentTag, currentTagIndex);

                    readingTag = 0;
                    readingAttribute = 0;
                    closingTag = 0;
                    currentTagIndex = 0;
                    currentAttributeIndex = 0;
                    
                    continue;
                }

                break;

            // TODO: In this configuration, an attribute can't contain a space within the quotes
            case ' ':
                if(readingTag)
                {
                    currentTag[currentTagIndex] = 0;
                    currentAttribute[currentAttributeIndex] = 0;

                    if(currentAttributeIndex > 0)
                        AddAttribute(attributes, currentAttribute, currentTag, currentAttributeIndex, commonIndex);

                    readingAttribute = 1;
                    currentAttributeIndex = 0;
                    continue;
                }
                break;
        }

        if(readingTag && !readingAttribute && !readingText)
        {
            currentTag[currentTagIndex] = rawLayout[i];
            currentTagIndex++;
        }
        
        if(readingAttribute)
        {
            currentAttribute[currentAttributeIndex] = rawLayout[i];
            currentAttributeIndex++;
        }

        if(readingText)
        {
            currentText[currentTextIndex] = rawLayout[i];
            currentTextIndex++;
        }
    }

    DynamicArray *layout = malloc(sizeof(DynamicArray));
    ArrayCreate(layout, "DynamicArray*");

    // TODO: Remember to deallocate the openingTags, attributes, text, and closingTags pointers
    ArrayAppend(layout, "openingTags", openingTags, sizeof(DynamicArray*));
    ArrayAppend(layout, "attributes", attributes, sizeof(DynamicArray*));
    ArrayAppend(layout, "text", text, sizeof(DynamicArray*));
    ArrayAppend(layout, "closingTags", closingTags, sizeof(DynamicArray*));

    return layout;
}

/**
    Finds all of the layout files in the layout folder and returns a dynamic array of file names.
    @returns A list [DynamicArray] of file names [string]
*/
DynamicArray *FindLayoutFiles()
{
    DynamicArray *fileNames = malloc(sizeof(DynamicArray));
    ArrayCreate(fileNames, "char*");

    // TODO: Better file path
    struct dirent *de;
    char *filepath = "./src/ui/layouts/";
    int filepathLen = strlen(filepath);
    DIR *dr = opendir(filepath);

    if(dr == NULL)
    {
        // TODO: Error handling
    }

    while((de = readdir(dr)) != NULL)
    {
        char *occurrence;
        if((occurrence = strstr(de->d_name, ".html")) != NULL)
        {
            char *key = malloc((occurrence - de->d_name) + 1);
            int i;
            for(i = 0; i < (occurrence - de->d_name); i++)
            {
                key[i] = de->d_name[i];
            }
            key[i+1] = 0;
            
            int filenameLen = strlen(de->d_name);
            char *fullName = malloc(filepathLen + filenameLen + 1);

            memcpy(fullName, filepath, filepathLen);

            for(int i = filepathLen; i < filepathLen + filenameLen; i++)
            {
                fullName[i] = de->d_name[i - filepathLen];
            }

            fullName[filepathLen + filenameLen] = 0;

            ArrayAppend(fileNames, key, fullName, sizeof(de->d_name));
        }
    }

    closedir(dr);

    return fileNames;
}

/**
    Generates a formatted dynamic array of layouts readable by UI.
    @returns Nothing
*/
void GenerateLayouts()
{
    DynamicArray *layoutFiles = FindLayoutFiles();

    DynamicArray *layouts = malloc(sizeof(DynamicArray));
    ArrayCreate(layouts, "DynamicArray*");

    for(int i = 0; i < layoutFiles->length; i++)
    {
        // TODO: Remember to deallocate this at some point
        char* rawLayout = ReadLayoutFile((char*)layoutFiles->items[i]);
        
        DynamicArray *layout = FormLayout(rawLayout);

        ArrayAppend(layouts, layoutFiles->keys[i], layout, sizeof(DynamicArray));
    }

    for(int i = 0; i < layouts->length; i++)
    {
        DynamicArray *layout = (DynamicArray*)layouts->items[i];
        char *page = layouts->keys[i];

        DynamicArray *openingTags = layout->items[OPENING_TAGS_INDEX];
        DynamicArray *attributes = layout->items[ATTRIBUTES_INDEX];
        DynamicArray *text = layout->items[TEXT_INDEX];

        for(int j = 0; j < openingTags->length; j++)
        {
            if(!strcmp((char*)openingTags->items[j], "button"))
            {
                char key[18] = "button";
                char index[12];

                sprintf(index, "%d", j);
                strcat(key, index);

                DynamicArray *buttonAttributes = ArrayGetItemsByKey(attributes, key);
                DynamicArray *buttonText = ArrayGetItemsByKey(text, key);

                AddButton(buttonAttributes, buttonText->items[0], page);

                free(buttonAttributes);
                free(buttonText);
            }

            if(!strcmp((char*)openingTags->items[j], "text"))
            {
                char key[16] = "text";
                char index[12];

                sprintf(index, "%d", j);
                strcat(key, index);

                DynamicArray *textElem = ArrayGetItemsByKey(text, key);
                DynamicArray *textAttributes = ArrayGetItemsByKey(attributes, key);

                AddTextElem(textElem->items[0], textAttributes, page);

                free(textAttributes);
            }
        }
    }

    ArrayDelete(layoutFiles);
}