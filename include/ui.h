struct DynamicArray;
struct Vector2;

#ifndef RAYLIB_H
    #define RAYLIB_H
    typedef struct Color {
        unsigned char r;        // Color red value
        unsigned char g;        // Color green value
        unsigned char b;        // Color blue value
        unsigned char a;        // Color alpha value
    } Color;
#endif

typedef struct ColorSwitch 
{
    struct Color colorOne;
    struct Color colorTwo;
} ColorSwitch;

typedef struct Button
{
    int state;
    void (*onClick)();
    float x, y, width, height;
    float radius;
    float borderWidth;
    ColorSwitch borderColors;
    ColorSwitch backgroundColors;
    char *page;
} Button;

// ./src/UI.c
void DrawUI(char *page);

// ./src/Layouts.c
void GenerateLayouts();

// ./src/Buttons.c
void DrawButtons(char *page);
void CheckButtonCollisions(struct Vector2 mousePosition);