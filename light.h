#include "raylib.h"

/*  READ ME
    This header file is used to create light effects in a 2d grid space.
    Before using the functions, make sure to declare "struct Light" first in your program.
    The default maximum size for grid table is 100 x 100.
    This can be change by modifying the values of definition below.
    This goes same for the maximum size of light sources it can hold.
    Be sure to have "raylib.h" installed in your device for this to use.
*/

#define GRID_WIDTH_MAX_SIZE 100
#define GRID_HEIGHT_MAX_SIZE 100
#define LIGHT_SOURCE_MAX_SIZE 100

struct Data                         //struct data for every grid of light
{
    Color color;                    //color of that grid
    int brightness;                 //this affects the brightness of the grid
};

struct Source                       //struct for light sources
{
    int x;                          //x position of light source
    int y;                          //y position of light source
    int range;                      //how large the light will be
    Color color;                    //color of the light
};

struct Light                        //main struct for light
{
    struct Data Grid[GRID_HEIGHT_MAX_SIZE][GRID_WIDTH_MAX_SIZE];    //hold the plane where all the functions go
    struct Source source[LIGHT_SOURCE_MAX_SIZE];                    //holds all light source data
    int grid_height;                                                //the number of grids in y_axis
    int grid_width;                                                 //the number of grids in x_axis
    int sourceSize;                                                 //size of source
    int pixel_grid_height;                                          //height of each grid
    int pixel_grid_width;                                           //width of each grid
};

void Light_Setup(struct Light*, int, int, int, int);                                    //constructs data in Light
void Light_add_Source(struct Light*, struct Source);                                    //creates a light source in Light
void Light_Source_Produce(int, int, int, int, struct Light*, struct Source, bool);      //this creates/removes the glow from the light source
void Light_Draw(struct Light*, int, int);                                               //draws the Grid relative to x and y positions
void Light_remove_Source(struct Light*, struct Source);                                 //removes a light source in Light
bool Light_is_equal(struct Light, struct Source, int);                                  //special function for comparing light array and a source

bool Light_is_equal(struct Light light, struct Source source, int index)
{
    return (light.source[index].x == source.x && light.source[index].y == source.y && light.source[index].range == source.range &&
            light.source[index].color.r == source.color.r && light.source[index].color.g == source.color.g &&
            light.source[index].color.b == source.color.g);
}

void Light_Setup(struct Light* light_ptr, int pixel_h, int pixel_w, int h, int w)
{
    struct Light light = *light_ptr;        //using pointers to set the value of light

    light.pixel_grid_height = pixel_h;      //designate a value for light.pixel_grid_height
    light.pixel_grid_width = pixel_w;       //designate a value for light.pixel_grid_width
    light.grid_width = w;                   //designate a value for light.grid_width
    light.grid_height = h;                  //designate a value for light.grid_height
    light.sourceSize = 0;                   //sets the value to 0 since there are no light source yet

    struct Data Blank = {BLACK, 255};       //this will be the value of all grid on initialization

    for(int height = 0; height < light.grid_height; height++) //assign the value of Blank to every grid
        for(int width = 0; width < light.grid_width; width++)
            light.Grid[height][width] = Blank;

    *light_ptr = light;                     //copying the value of light back to the called struct
}

void Light_add_Source(struct Light *light_ptr, struct Source source)
{
    struct Light light = *light_ptr;            //using pointers to set the value of light

    light.source[light.sourceSize] = source;    //adds the paramter source to the array
    light.sourceSize++;                         //increase the number of sources in the array

    Light_Source_Produce(source.range, source.y, source.x, 255, &light, source, true);

    *light_ptr = light;                         //copying the value of light back to the called struct
}

void Light_Source_Produce(int range, int y_axis, int x_axis, int brightness, struct Light* light_ptr, struct Source source, bool isProduce)
{
    if(range <= 0) return;
    int Ydirection = (y_axis > source.y)? 1 : -1;
    int Xdirection = (x_axis > source.x)? 1 : -1;

    if(range == source.range)   
    {
        Light_Source_Produce(range - 1, y_axis - 1, x_axis, brightness - (255 / source.range), light_ptr, source, isProduce);
        Light_Source_Produce(range - 1, y_axis + 1, x_axis, brightness - (255 / source.range), light_ptr, source, isProduce);
        Light_Source_Produce(range - 1, y_axis, x_axis - 1, brightness - (255 / source.range), light_ptr, source, isProduce);
        Light_Source_Produce(range - 1, y_axis, x_axis + 1, brightness - (255 / source.range), light_ptr, source, isProduce);
    }
    else if(x_axis == source.x)
    {
        Light_Source_Produce(range - 1, y_axis + Ydirection, x_axis, brightness - (255 / source.range), light_ptr, source, isProduce);
        Light_Source_Produce(range - 1, y_axis, x_axis - 1, brightness - (255 / source.range), light_ptr, (struct Source){source.x, y_axis, source.range, source.color}, isProduce);
        Light_Source_Produce(range - 1, y_axis, x_axis + 1, brightness - (255 / source.range), light_ptr, (struct Source){source.x, y_axis, source.range, source.color}, isProduce);
    }
    else if(y_axis == source.y)
    {
        Light_Source_Produce(range - 1, y_axis, x_axis + Xdirection, brightness - (255 / source.range), light_ptr, source, isProduce);
    }

    struct Light light = *light_ptr;

    if(y_axis <= light.grid_height || x_axis <= light.grid_width)
    {
        if(isProduce)
        {
            if(light.Grid[y_axis][x_axis].color.r != BLACK.r ||
               light.Grid[y_axis][x_axis].color.b != BLACK.b ||
               light.Grid[y_axis][x_axis].color.g != BLACK.g   )
            {
                source.color = (Color){(light.Grid[y_axis][x_axis].color.r + source.color.r) / 2,
                                       (light.Grid[y_axis][x_axis].color.b + source.color.b) / 2,
                                       (light.Grid[y_axis][x_axis].color.g + source.color.g) / 2};
                brightness = (light.Grid[y_axis][x_axis].brightness + brightness) / 2;
            }
        }
        else
        {
            if(light.Grid[y_axis][x_axis].color.r != source.color.r ||
               light.Grid[y_axis][x_axis].color.g != source.color.g ||
               light.Grid[y_axis][x_axis].color.b != source.color.b   )
            {
                source.color = (Color){(2 * light.Grid[y_axis][x_axis].color.r) - source.color.r, 
                                       (2 * light.Grid[y_axis][x_axis].color.b) - source.color.b,
                                       (2 * light.Grid[y_axis][x_axis].color.g) - source.color.g};
                brightness = (2 * light.Grid[y_axis][x_axis].brightness) - brightness;
            }
            else
            {
                source.color = BLACK;
                brightness = 255;
            }
        }

        light.Grid[y_axis][x_axis].color = source.color;
        light.Grid[y_axis][x_axis].brightness = brightness;
    }

    *light_ptr = light;

    return;
}

void Light_Draw(struct Light* light_ptr, int pos_X, int pos_Y)
{
    struct Light light = *light_ptr;

    for(int h = 0; h < light.grid_height; h++)
        for(int w = 0; w < light.grid_width; w++)
            DrawRectangle(pos_X + (w * light.pixel_grid_width), pos_Y + (h * light.pixel_grid_height), light.pixel_grid_width, light.pixel_grid_height,
                         (Color){light.Grid[h][w].color.r, light.Grid[h][w].color.g, light.Grid[h][w].color.b, light.Grid[h][w].brightness});
}

void Light_remove_Source(struct Light *light_ptr, struct Source source) 
{
    struct Light light = *light_ptr;

    for(int i = 0; i < light.sourceSize; i++)
    {
        if(Light_is_equal(light, source, i))
        {
            for(int n = i; n < light.sourceSize - 1; n++)
                light.source[n] = light.source[n + 1];
            light.sourceSize--;
            Light_Source_Produce(source.range, source.y, source.x, 255, &light, source, false);
            break;
        }
    }

    *light_ptr = light;
}