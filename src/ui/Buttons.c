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

// TODO: Documentation
Color ClickColor(Color input)
{
    Color output;

    output.r = input.r;
    output.g = input.g;
    output.b = input.b;
    
    output.a = input.a / 2;

    return output;
}

/**
    Draws buttons with borders and rounded edges to the screen.
    Each button has a default color set and selected color set for both the border and background.

    @param index: The index [int] of the button
    @returns Nothing
*/
// TODO: Add button text
void DrawButton(int index)
{
    Button button = buttons[index];

    Color borderColor = button.borderColors.colorOne;
    Color backgroundColor = button.backgroundColors.colorOne;

    switch(button.state)
    {
        case 1:
            borderColor = button.borderColors.colorTwo;
            backgroundColor = button.backgroundColors.colorTwo;
            break;

        case 2:
            borderColor = ClickColor(button.borderColors.colorOne);
            backgroundColor = ClickColor(button.backgroundColors.colorOne);
            break;

        default:
            borderColor = button.borderColors.colorOne;
            backgroundColor = button.backgroundColors.colorOne;
            break;
    }

    Rectangle backgroundRect = { button.x, button.y, button.width, button.height };
    Color defaultBackgroundColor = backgroundColor;
    DrawRectangleRounded(backgroundRect, button.radius, 10.0f, defaultBackgroundColor);

    Rectangle borderRect = { button.x + (button.borderWidth/2), button.y + (button.borderWidth/2), button.width - button.borderWidth, button.height - button.borderWidth };
    Color defaultBorderColor = borderColor;
    DrawRectangleRounded(borderRect, button.radius, 10.0f, defaultBorderColor);
}

/**
    Draws all buttons to the screen.
    @returns Nothing
*/
void DrawButtons(char *page)
{
    for(int i = 0; i < numButtons; i++)
        if(!strcmp(page, buttons[i].page))
            DrawButton(i);
}

// TODO: Documentation
void CheckButtonCollisions(Vector2 mousePosition)
{
    for(int i = 0; i < numButtons; i++)
    {
        Rectangle bounds = { buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height };
        if(CheckCollisionPointRec(mousePosition, bounds))
        {
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                buttons[i].state = 2;
                buttons[i].onClick();
            }
            else
            {
                buttons[i].state = 1;
            }
        }
        else
        {
            buttons[i].state = 0;
        }
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

// TODO: Remove me, this is temporary
void onClick()
{
    printf("I was clicked\n");
}

// TODO: Documentation
// TODO: Default placement, scaling, etc. when attributes aren't specified
// TODO: Maybe add smart default values for attributes that are formed based on what looks right
void AddButton(DynamicArray *attributes, char *page)
{
    void (*onClickRef)() = &onClick;

    Button newButton;
    newButton.state = 0;
    newButton.onClick = onClickRef;
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