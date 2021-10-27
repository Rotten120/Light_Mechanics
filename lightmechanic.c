#include "raylib.h"
#include "light.h"

#define SETFPS 60

const int screenWidth = 1000;
const int screenHeight = 1000;

int main()
{
    InitWindow(screenWidth, screenHeight, "TEST");
    SetTargetFPS(SETFPS);

    struct Light light;
    Light_Setup(&light, screenHeight / 10, screenWidth / 10, 10, 10);
    Light_add_Source(&light, (struct Source){4, 3, 3, RED});
    Light_add_Source(&light, (struct Source){4, 6, 3, WHITE});
    Light_remove_Source(&light, (struct Source){4, 6, 3, WHITE});

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            Light_Draw(&light, 0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}