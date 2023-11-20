#ifndef _ASTEROIDS_PVT_H_
#define _ASTEROIDS_PVT_H_

#include "asteroids.h"
#include "shot_queue.h"
#include "ship.h"

#define N_HOLD_KEYS 4

#define HK_LEFTARROW 0
#define HK_RIGHTARROW 1
#define HK_UPARROW 2
#define HK_DOWNARROW 3

#define ASTEROID_SIZEMULT 20


typedef struct
{
    gamestate_t* game;
    vector2f location;
    vector2f velocity;
    float rotation;
    float rot_velocity;
    int asteroid_size;
    asteroid_mesh mesh;
} asteroid_t;

asteroid_mesh_collection n_random_meshes(int n, int avg_segments, int deviation);
asteroid_mesh random_mesh(int segments);
void destroy_meshes(asteroid_mesh_collection collection);

void draw_shot(shot_t* shot, ALLEGRO_COLOR color);
void draw_asteroid(asteroid_t* asteroid);
void draw_ship(ship_t* ship);

struct asteroid_node
{
    asteroid_t asteroid;
    struct asteroid_node* next;
};

typedef struct asteroid_node ast_node;

typedef struct 
{
    ast_node* head;
} asteroid_queue;

typedef struct
{
    ast_node* cur;
    bool pre;
} asteroid_iterator;

double rand01();

asteroid_iterator create_asteroid_iterator(asteroid_queue* queue);
bool ast_iterator_increment(asteroid_iterator* iter);
asteroid_t* ast_iterator_current(asteroid_iterator* iter);

asteroid_queue create_asteroid_queue();
void destroy_asteroid_queue(asteroid_queue* queue);
void push_new_asteroid(asteroid_queue* asteroids, int size, gamestate_t* state, asteroid_mesh_collection meshes);
void split_asteroid(asteroid_queue* asteroids, asteroid_t* to_split);

bool is_point_in_asteroid(asteroid_t* ast, vector2f pos);

#endif