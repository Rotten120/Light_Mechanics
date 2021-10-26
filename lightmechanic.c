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
    Light_Setup(&light, 20, 20);
    Light_add_Source(&light, (struct Source){10, 10, 5, RED});
    Light_add_Source(&light, (struct Source){15, 10, 4, BLUE});

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            Light_Draw(screenWidth, screenHeight, &light);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}