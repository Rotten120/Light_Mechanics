#include "raylib.h"

struct Data
{
    Color color;
    int brightness;
};

struct Source
{
    int x;
    int y;
    int range;
    Color color;
};

struct Light
{
    struct Data Grid[100][100];
    struct Source source[100]; 
    int grid_height, grid_width, sourceSize;
};

void Light_Setup(struct Light*, int, int);
void Light_add_Source(struct Light*, struct Source);
void Light_Source_Produce(int, int, int, int, struct Light*, struct Source);
void Light_Draw(struct Light*, int, int);

void Light_Setup(struct Light* light_ptr, int h, int w)
{
    struct Light light = *light_ptr;

    light.grid_width = w;
    light.grid_height = h;
    light.sourceSize = 0;

    struct Data Blank = {BLACK, 255};

    for(int height = 0; height < light.grid_height; height++)
        for(int width = 0; width < light.grid_width; width++)
            light.Grid[height][width] = Blank;

    *light_ptr = light;
}

void Light_add_Source(struct Light *light_ptr, struct Source source)
{
    struct Light light = *light_ptr;

    light.source[light.sourceSize] = source;
    light.sourceSize++;

    Light_Source_Produce(source.range, source.y, source.x, 255, &light, source);

    *light_ptr = light;
}

void Light_Source_Produce(int range, int y_axis, int x_axis, int brightness, struct Light* light_ptr, struct Source source)
{
    if(range <= 0) return;
    int Ydirection = (y_axis > source.y)? 1 : -1;
    int Xdirection = (x_axis > source.x)? 1 : -1;

    if(range == source.range)
    {
        Light_Source_Produce(range - 1, y_axis - 1, x_axis, brightness - (255 / source.range), light_ptr, source);
        Light_Source_Produce(range - 1, y_axis + 1, x_axis, brightness - (255 / source.range), light_ptr, source);
        Light_Source_Produce(range - 1, y_axis, x_axis - 1, brightness - (255 / source.range), light_ptr, source);
        Light_Source_Produce(range - 1, y_axis, x_axis + 1, brightness - (255 / source.range), light_ptr, source);
    }
    else if(x_axis == source.x)
    {
        Light_Source_Produce(range - 1, y_axis + Ydirection, x_axis, brightness - (255 / source.range), light_ptr, source);
        Light_Source_Produce(range - 1, y_axis, x_axis - 1, brightness - (255 / source.range), light_ptr, (struct Source){source.x, y_axis, source.range, source.color});
        Light_Source_Produce(range - 1, y_axis, x_axis + 1, brightness - (255 / source.range), light_ptr, (struct Source){source.x, y_axis, source.range, source.color});
    }
    else if(y_axis == source.y)
    {
        Light_Source_Produce(range - 1, y_axis, x_axis + Xdirection, brightness - (255 / source.range), light_ptr, source);
    }

    struct Light light = *light_ptr;

    if(y_axis <= light.grid_height || x_axis <= light.grid_width)
    {
        if(light.Grid[y_axis][x_axis].color.r != BLACK.r ||
           light.Grid[y_axis][x_axis].color.b != BLACK.b ||
           light.Grid[y_axis][x_axis].color.g != BLACK.g   )
        {
            source.color = (Color){(light.Grid[y_axis][x_axis].color.r + source.color.r) / 2, (light.Grid[y_axis][x_axis].color.b + source.color.b) / 2, (light.Grid[y_axis][x_axis].color.g + source.color.g) / 2};
            brightness = (light.Grid[y_axis][x_axis].brightness + brightness) / 2;
        }
        light.Grid[y_axis][x_axis].color = source.color;
        light.Grid[y_axis][x_axis].brightness = brightness;
    }

    *light_ptr = light;

    return;
}

void Light_Draw(struct Light* light_ptr, int screenWidth, int screenHeight)
{
    struct Light light = *light_ptr;
    double Pixel_Grid_Height = screenHeight / light.grid_height;
    double Pixel_Grid_Width = screenWidth / light.grid_width;

    for(int h = 0; h < light.grid_height; h++)
        for(int w = 0; w < light.grid_width; w++)
            DrawRectangle(w * Pixel_Grid_Width, h * Pixel_Grid_Height, Pixel_Grid_Width, Pixel_Grid_Height,
                         (Color){light.Grid[h][w].color.r, light.Grid[h][w].color.g, light.Grid[h][w].color.b, light.Grid[h][w].brightness});
}