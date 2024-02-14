#include "../../include/raylib.h"
#include "../../include/ui.h"
#include "../../include/utils/arrays.h"

Vector2 mousePosition = { 0.0f, 0.0f };

// TODO: Documentation
void DrawUI(char *page)
{
    mousePosition = GetMousePosition();

    DrawButtons(page);
}