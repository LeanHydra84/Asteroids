#include "asteroids_pvt.h"
#include "ship.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>

#include <stdio.h>

double rand01()
{
    return (double)rand() / (double)RAND_MAX;
}

void draw_asteroid(asteroid_t* asteroid)
{
    const vector2f pos = asteroid->location;
    vector2f* mesh = asteroid->mesh.array;
    palette* pal = asteroid->game->color_palette;

    for(int i = 0; i < asteroid->mesh.size; i++)
    {
        int l2 = i + 1;
        if(l2 >= asteroid->mesh.size) l2 = 0;

        vector2f pos1 = add_v2f(mult_v2f(rotate_around(mesh[i], pos, asteroid->rotation), ASTEROID_SIZEMULT * asteroid->asteroid_size), pos);
        vector2f pos2 = add_v2f(mult_v2f(rotate_around(mesh[l2], pos, asteroid->rotation), ASTEROID_SIZEMULT * asteroid->asteroid_size), pos);
                
        al_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, pal->asteroid, 2.0f);
    }

    // draw bounding box
    //al_draw_circle(pos.x, pos.y, asteroid->mesh.maxradius * asteroid->asteroid_size * ASTEROID_SIZEMULT, al_map_rgb(0, 200, 0), 2);
}

static const vector2f ship_render[] = 
{
    {.x = 0, .y = 0.75f },
    {.x = 0.5f, .y = -0.5f },
    {.x = 0, .y = -0.25f },
    {.x = -0.5f, .y = -0.5f },
};

void draw_ship(ship_t* ship)
{
    ALLEGRO_COLOR color = (ship->invincibility_time <= 0) ? ship->state->color_palette->ship : ship->state->color_palette->invincible_ship;
    for(int i = 0; i < 4; i++)
    {
        int l2 = i + 1;
        if(l2 >= 4) l2 = 0;

        vector2f pos1 = add_v2f(mult_v2f(rotate_around(ship_render[i], ship->position, ship->rotation), 25), ship->position);
        vector2f pos2 = add_v2f(mult_v2f(rotate_around(ship_render[l2], ship->position, ship->rotation), 25), ship->position);
        al_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, color, 2.0f);
    }
    al_draw_filled_circle(ship->position.x, ship->position.y, 2.0f, color);

}

void draw_shot(shot_t* shot, ALLEGRO_COLOR color)
{
    al_draw_filled_circle(shot->position.x, shot->position.y, 4.0f, color);
}

asteroid_mesh_collection n_random_meshes(int n, int avg_segments, int deviation)
{
    asteroid_mesh_collection meshes;
    meshes.count = n;
    meshes.meshes = malloc(sizeof(asteroid_mesh) * n);

    for(int i = 0; i < n; i++)
    {
        int sidecount = avg_segments + (rand() % (deviation * 2)) - deviation;
        //printf("Side count: %d\n", sidecount);
        meshes.meshes[i] = random_mesh(sidecount);
    }
    return meshes;
}

void destroy_meshes(asteroid_mesh_collection collection)
{
    for(int i = 0; i < collection.count; i++)
    {
        free(collection.meshes[i].array);
    }
    free(collection.meshes);
}

asteroid_mesh random_mesh(int n)
{
    asteroid_mesh mesh;
    mesh.size = n;

    const float seg_angle = (PI * 2) / n;
    float cur_angle = 0;

    vector2f* segments = malloc(sizeof(vector2f) * n);
    mesh.maxradius = 0;

    for(int i = 0; i < n; i++)
    {
        float radius = rand01() + 1;
        if(radius > mesh.maxradius)
            mesh.maxradius = radius;

        segments[i].x = radius * cos(cur_angle);
        segments[i].y = radius * sin(cur_angle);

        cur_angle += seg_angle;
    }

    mesh.array = segments;
    return mesh;

}