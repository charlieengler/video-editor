#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/raylib.h"
#include "../../include/ui.h"
#include "../../include/ui/text.h"

#include "../../include/utils/arrays.h"

// Notify the user that the max number of text elements per layout is 4096
#define MAXTEXTELEMS 4096
int numTextElems = 0;
Text textElems[MAXTEXTELEMS];

// TODO: Documentation
// TODO: This function is also very similar to the DrawButton function
void DrawTextElem(int index)
{
    Text textElem = textElems[index];

    // TODO: Use DrawTextEx for custom font
    DrawText(textElem.text, textElem.x, textElem.y, textElem.size, textElem.color);
}

// TODO: Documentation
void DrawAllText(char *page)
{
    for(int i = 0; i < numTextElems; i++)
        if(!strcmp(page, textElems[i].page))
            DrawTextElem(i);
}

// TODO: Documentation
// TODO: Getting attributes is common between text and buttons, maybe combine somehow?
// TODO: Default placement, scaling, etc. when attributes aren't specified
// TODO: Maybe add smart default values for attributes that are formed based on what looks right
void AddTextElem(char *text, DynamicArray *attributes, char *page)
{
    Text newText;
    newText.x = 0;
    newText.y = 0;
    newText.size = 15;
    newText.color = BLACK;
    newText.page = page;
    newText.text = text;

    for(int i = 0; i < attributes->length; i++)
    {
        char *item = attributes->items[i];
        char attribute[ATTRIBLEN];
        strcpy(attribute, item);

        ParseAttribute(attribute);

        if(item[0] == 'x')
            newText.x = atof(attribute);

        if(item[0] == 'y')
            newText.y = atof(attribute);

        if(strstr(item, "size") != NULL)
            newText.size = atof(attribute);

        if(strstr(item, "color") != NULL)
            newText.color = RGBAToColor(attribute);
    }

    textElems[numTextElems] = newText;
    numTextElems = numTextElems < MAXTEXTELEMS ? numTextElems + 1 : 0;
}