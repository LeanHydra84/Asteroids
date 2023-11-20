#include "asteroids_pvt.h"

asteroid_queue create_asteroid_queue()
{
    asteroid_queue q;
    q.head = NULL;
    return q;
};


void destroy_asteroid_queue(asteroid_queue* ast)
{
    ast_node* node = ast->head;
    while(node != NULL)
    {
        ast_node* temp = node->next;
        free(node);
        node = temp;
    }
}

#define MAXVEL 100

static void push_new_asteroid_withpos(asteroid_queue* asteroids, int size, gamestate_t* game, vector2f posi, asteroid_mesh_collection collection)
{
    ast_node* newnode = malloc(sizeof(ast_node));
    asteroid_t* ast = &newnode->asteroid;

    ast->asteroid_size = size;
    ast->rotation = rand01() * (2 * PI);
    ast->rot_velocity = (rand01() - 0.5f) * 0.01f;
    ast->game = game;

    ast->location = posi;
    ast->mesh = collection.meshes[rand() % collection.count];

    vector2f velo;
    velo.x = (float)(rand() % MAXVEL) + 1;
    velo.y = (float)(rand() % MAXVEL) + 1;
    ast->velocity = velo;

    newnode->next = asteroids->head;
    asteroids->head = newnode;
}

void push_new_asteroid(asteroid_queue* asteroids, int size, gamestate_t* game, asteroid_mesh_collection collection)
{
    vector2f posi;
    posi.x = abs(rand()) % game->screen_size.x;
    posi.y = abs(rand()) % game->screen_size.y;

    push_new_asteroid_withpos(asteroids, size, game, posi, collection);
}

void split_asteroid(asteroid_queue* asteroids, asteroid_t* to_split)
{
    if (to_split->asteroid_size <= 1) return;

    int n;
    if(rand() % 2 == 0) n = 2;
    else n = 3;

    for(int i = 0; i < n; i++)
    {
        push_new_asteroid_withpos(asteroids, to_split->asteroid_size - 1, to_split->game, to_split->location, to_split->game->collection);
    }
}

asteroid_iterator create_asteroid_iterator(asteroid_queue* queue)
{
    asteroid_iterator rator;
    rator.cur = queue->head;
    rator.pre = false;
    return rator;
}

bool ast_iterator_increment(asteroid_iterator* iter)
{
    if(iter->cur == NULL) return false;
    if(iter->pre != false)
        iter->cur = iter->cur->next;
    else iter->pre = true;

    if(iter->cur == NULL) return false;
    return true;
}

asteroid_t* ast_iterator_current(asteroid_iterator* iter)
{
    if(iter->cur == NULL) return NULL;
    return &iter->cur->asteroid;
}