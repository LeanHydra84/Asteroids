#include "asteroids_pvt.h"
#include "ship.h"

ship_t create_ship(gamestate_t* game)
{
    ship_t ship;
    ship.lives = 4;
    ship.position = create_v2f(game->screen_size.x / 2.0f, game->screen_size.y / 2.0f);
    ship.rotation = 0.25f;
    ship.velocity = create_v2f(0, 0);
    ship.state = game;
    ship.speed = 50.0f;
    ship.speeddampen = 0.9f;
    ship.hit_flag = false;
    ship.invincibility_time = -1.0f;
    return ship;
}