#ifndef _SHIP_H_
#define _SHIP_H_

#include "asteroids.h"

typedef struct
{
    gamestate_t* state;
    vector2f position;
    vector2f velocity;
    float rotation;
    int lives;
    float speed;
    float speeddampen;
    bool hit_flag;
    float invincibility_time;
} ship_t;

ship_t create_ship(gamestate_t* state);

#endif