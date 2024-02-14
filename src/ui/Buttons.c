#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/raylib.h"
#include "../../include/ui.h"
#include "../../include/ui/buttons.h"

#include "../../include/utils/arrays.h"

// TODO: Notify the user that the max number of buttons per layout is 1024
int numButtons = 0;
Button buttons[1024];

/**
    Draws buttons with borders and rounded edges to the screen.
    Each button has a default color set and selected color set for both the border and background.

    @param bounds: The x [float], y [float], width [float], and height [float] attributes of the button
    @param radius: The border radius [float, 0-1] (rounding of the corners) as a percentage
    @param borderWidth: The width of the border [float] in pixels
    @param borderColors: Two colors [ColorSwitch] that define the default and selected colors of the border
    @param backgroundColors: Two colors [ColorSwitch] that define the default and selected colors of the background
    @returns Nothing
*/
// TODO: Add button text
void DrawButton(Rectangle bounds, float radius, float borderWidth, ColorSwitch borderColors, ColorSwitch backgroundColors)
{
    Rectangle backgroundRect = { bounds.x, bounds.y, bounds.width, bounds.height };
    Color defaultBackgroundColor = backgroundColors.colorOne;
    DrawRectangleRounded(backgroundRect, radius, 10.0f, defaultBackgroundColor);

    Rectangle borderRect = { bounds.x + (borderWidth/2), bounds.y + (borderWidth/2), bounds.width - borderWidth, bounds.height - borderWidth };
    Color defaultBorderColor = borderColors.colorOne;
    DrawRectangleRounded(borderRect, radius, 10.0f, defaultBorderColor);
}

/**
    Draws all buttons to the screen.
    @returns Nothing
*/
void DrawButtons(char *page)
{
    for(int i = 0; i < numButtons; i++)
    {
        Button button = buttons[i];

        if(button.isDrawn)
            continue;

        if(button.isUpdated)
            continue;

        if(strcmp(page, button.page))
            continue;

        Rectangle buttonBounds = { button.x, button.y, button.width, button.height };

        DrawButton(buttonBounds, button.radius, button.borderWidth, button.borderColors, button.backgroundColors);

        button.isDrawn = 1;
        button.isUpdated = 0;
    }
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

                // a
                case 3:
                    newColor.a = value;
                    break;
            }

            currentEntry++;
        }
    }

    return newColor;
}

// TODO: Documentation
// TODO: Default placement, scaling, etc. when attributes aren't specified
// TODO: Maybe add smart default values for attributes that are formed based on what looks right
void AddButton(DynamicArray *attributes, char *page)
{
    Button newButton;
    newButton.isDrawn = 0;
    newButton.isUpdated = 0;
    newButton.x = 0;
    newButton.y = 0;
    newButton.width = 100;
    newButton.height = 100;
    newButton.radius = 10;
    newButton.borderWidth = 5;
    newButton.backgroundColors.colorOne = BLACK;
    newButton.backgroundColors.colorTwo = BLACK;
    newButton.borderColors.colorOne = RED;
    newButton.borderColors.colorTwo = RED;
    newButton.page = page;

    for(int i = 0; i < attributes->length; i++)
    {
        char *item = attributes->items[i];
        char value[1024];
        strcpy(value, item);

        ParseAttribute(value);

        if(item[0] == 'x')
            newButton.x = atof(value);

        if(item[0] == 'y')
            newButton.y = atof(value);

        if(strstr(item, "width"))
            newButton.width = atof(value);

        if(strstr(item, "height") != NULL)
            newButton.height = atof(value);

        if(strstr(item, "radius") != NULL)
            newButton.radius = atof(value) / 100.0f;

        if(strstr(item, "borderWidth") != NULL)
            newButton.borderWidth = atof(value);

        if(strstr(item, "bgColorOne") != NULL)
            newButton.backgroundColors.colorOne = RGBAToColor(value);

        if(strstr(item, "bgColorTwo") != NULL)
            newButton.backgroundColors.colorTwo = RGBAToColor(value);

        if(strstr(item, "borderColorOne") != NULL)
            newButton.borderColors.colorOne = RGBAToColor(value);

        if(strstr(item, "borderColorsTwo") != NULL)
            newButton.borderColors.colorTwo = RGBAToColor(value);
    }

    buttons[numButtons] = newButton;
    numButtons = numButtons < 1023 ? numButtons + 1 : 0;
}