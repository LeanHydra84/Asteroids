#include <stdio.h>
#include <allegro5/allegro_color.h>
#include "asteroids.h"

int main()
{
    palette pal;
    pal.asteroid = al_map_rgb(255, 255, 255);
    pal.background = al_map_rgb(0, 0, 0);
    pal.ship = al_map_rgb(200, 200, 200);
    pal.invincible_ship = al_map_rgb(100, 100, 100);
    pal.shot = al_map_rgb(255, 255, 255);

    gamestate_t state;
    state.tickspeed = 1.0f / 60.0f;
    state.screen_size = create_v2i(900, 800);
    state.color_palette = &pal;
    
    return game_loop(&state);
}