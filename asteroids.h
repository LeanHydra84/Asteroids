#ifndef _ASTEROIDS_H_
#define _ASTEROIDS_H_

#include <allegro5/allegro_color.h>

#define true 1
#define false 0

#define PI 3.14159265358979323846

//typedef int bool;
typedef struct
{
    float x, y;
} vector2f;

typedef struct
{
    int x, y;
} vector2i;

float v2f_distance(vector2f left, vector2f right);
vector2f angle_to_cartesian(float angle);

vector2f create_v2f(float x, float y);
vector2i create_v2i(int x, int y);

vector2f negative_v2f(vector2f val);
vector2f add_v2f(vector2f left, vector2f right);
vector2f mult_v2f(vector2f left, float right);

vector2i add_v2i(vector2i left, vector2i right);
vector2i mult_v2i(vector2i left, int right);

vector2i v2ftoi(vector2f val);
vector2f v2itof(vector2i val);

vector2f rotate_around(vector2f point, vector2f origin, float angle);

typedef struct
{
    ALLEGRO_COLOR asteroid;
    ALLEGRO_COLOR background;
    ALLEGRO_COLOR ship;
    ALLEGRO_COLOR invincible_ship;
    ALLEGRO_COLOR shot;
} palette;

typedef struct
{
    int size;
    float maxradius;
    vector2f* array;
} asteroid_mesh;

typedef struct
{
    asteroid_mesh* meshes;
    int count;
} asteroid_mesh_collection;

typedef struct 
{
    palette* color_palette;
    vector2i screen_size;
    float asteroid_tick;
    float mean_asteroid_timer;
    float stdev_asteroid_timer;
    float tickspeed;
    asteroid_mesh_collection collection;
} gamestate_t;

int game_loop(gamestate_t* state);

#endif