#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/raylib.h"
#include "../../include/ui.h"
#include "../../include/ui/buttons.h"

#include "../../include/utils/arrays.h"

// TODO: Notify the user that the max number of buttons per layout is 1024
#define MAXBUTTONS 1024
int numButtons = 0;
Button buttons[MAXBUTTONS];

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
void DrawAllButtons(char *page)
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

// TODO: Remove me, this is temporary
void onClick()
{
    printf("I was clicked\n");
}

// TODO: Documentation
// TODO: Default placement, scaling, etc. when attributes aren't specified
// TODO: Maybe add smart default values for attributes that are formed based on what looks right
void AddButton(DynamicArray *attributes, char *text, char *page)
{
    // TODO: Make onClick so it isn't hard coded
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
    newButton.text = text;

    for(int i = 0; i < attributes->length; i++)
    {
        char *item = attributes->items[i];
        char attribute[ATTRIBLEN];
        strcpy(attribute, item);

        ParseAttribute(attribute);

        if(item[0] == 'x')
            newButton.x = atof(attribute);

        if(item[0] == 'y')
            newButton.y = atof(attribute);

        if(strstr(item, "width") != NULL)
            newButton.width = atof(attribute);

        if(strstr(item, "height") != NULL)
            newButton.height = atof(attribute);

        if(strstr(item, "radius") != NULL)
            newButton.radius = atof(attribute) / 100.0f;

        if(strstr(item, "borderWidth") != NULL)
            newButton.borderWidth = atof(attribute);

        if(strstr(item, "bgColorOne") != NULL)
            newButton.backgroundColors.colorOne = RGBAToColor(attribute);

        if(strstr(item, "bgColorTwo") != NULL)
            newButton.backgroundColors.colorTwo = RGBAToColor(attribute);

        if(strstr(item, "borderColorOne") != NULL)
            newButton.borderColors.colorOne = RGBAToColor(attribute);

        if(strstr(item, "borderColorsTwo") != NULL)
            newButton.borderColors.colorTwo = RGBAToColor(attribute);
    }


    buttons[numButtons] = newButton;
    numButtons = numButtons < MAXBUTTONS ? numButtons + 1 : 0;
}