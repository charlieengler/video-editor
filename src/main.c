#include "../include/raylib.h"
#include "../include/ui.h"
#include "../include/utils/arrays.h"

int main(void)
{
    // TODO: There are a ton of memory leaks in this entire program, find them

    GenerateLayouts();
    InitWindow(1920, 1080, "Video Editor");

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