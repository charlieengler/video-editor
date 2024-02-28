#include <string.h>
#include <stdlib.h>

#include "../../include/raylib.h"
#include "../../include/ui.h"
#include "../../include/utils/arrays.h"

Vector2 mousePosition = { 0.0f, 0.0f };

// TODO: Documentation
void DrawUI(char *page)
{
    mousePosition = GetMousePosition();

    CheckButtonCollisions(mousePosition);

    // TODO: Possible z-axis layering issues with the order of these functions
    DrawAllButtons(page);
    DrawAllText(page);
}

// TODO: Documentation
void ParseAttribute(char *attribute)
{
    // TODO: Handle different units (i.e. pixels, percentages, etc.)
    // TODO: Use units to influence the final value inside this function
    char value[1024], units[1024];

    int readingValue = 0;
    int valueIndex = 0;
    int unitsIndex = 0;
    for(unsigned long i = 0; i < strlen(attribute); i++)
    {
        if(attribute[i] == '"' && readingValue)
            readingValue = 0;

        if(readingValue)
        {
            if((attribute[i] >= 48 && attribute[i] <= 57) || attribute[i] == ',' || attribute[i] == '.')
            {
                value[valueIndex] = attribute[i];
                valueIndex++;
            }
            else
            {
                units[unitsIndex] = attribute[i];
                unitsIndex++;
            }
        }

        if(attribute[i] == '"' && !readingValue)
            readingValue = 1;
    }

    value[valueIndex] = 0;
    units[unitsIndex] = 0;

    strcpy(attribute, value);
}

// TODO: Documentation
Color RGBAToColor(char *rgba)
{
    // TODO: Error handling if the rgba value is improperly formatted
    Color newColor;
    int currentEntry = 0;
    int valueIndex = 0;
    char currentValue[1024];
    for(unsigned long i = 0; i < strlen(rgba); i++)
    {
        if(rgba[i] != ',')
        {
            currentValue[valueIndex] = rgba[i];
            valueIndex++;
        }
        else
        {
            currentValue[valueIndex] = 0;
            valueIndex = 0;

            // TODO: Error checking
            int value = atoi(currentValue);

            switch(currentEntry)
            {
                // r
                case 0:
                    newColor.r = value;
                    break;
                
                // g
                case 1:
                    newColor.g = value;
                    break;
                
                // b
                case 2:
                    newColor.b = value;
                    break;
            }

            currentEntry++;
        }
    }

    currentValue[valueIndex] = 0;
    valueIndex = 0;

    // TODO: Error checking
    newColor.a = atoi(currentValue);

    return newColor;
}