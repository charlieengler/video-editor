#include "../include/raylib.h"
#include "../include/ui.h"
#include "../include/utils/arrays.h"

int main(void)
{
    GenerateLayouts();

    InitWindow(800, 450, "Video Editor");

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawUI("home");
        EndDrawing();
    }

    CloseWindow();

    return 0;
}