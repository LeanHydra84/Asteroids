#include "asteroids.h"
#include "asteroids_pvt.h"
#include "ship.h"
#include "shot_queue.h"

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define I_FRAME_TIME 2.0f

static void key_array(bool* keys, int key_type, bool set_value)
{
    int offset = 0;
    switch(key_type)
    {
        case ALLEGRO_KEY_LEFT:
            offset = HK_LEFTARROW;
            break;
        case ALLEGRO_KEY_RIGHT:
            offset = HK_RIGHTARROW;
            break;
        case ALLEGRO_KEY_DOWN:
            offset = HK_DOWNARROW;
            break;
        case ALLEGRO_KEY_UP:
            offset = HK_UPARROW;
            break;
        default:
            return;
    }
    keys[offset] = set_value;
}

static bool any_key_array(bool* keys)
{
    for(int i = 0; i < N_HOLD_KEYS; i++)
        if(keys[i]) return true;
    return false;
}

// radius = ASTEROID_SIZEMULT * asteroid_size

static vector2f wrapPosition(vector2f pos, vector2i size, float objRadius)
{
    const float radius = ASTEROID_SIZEMULT * objRadius;
    if (pos.x + radius < 0) pos.x = size.x;
    else if (pos.x - radius >= size.x) pos.x = 0;

    if (pos.y + radius < 0) pos.y = size.y;
    else if (pos.y - radius >= size.y) pos.y = 0;
    return pos;
}

static bool out_of_bounds(vector2f shot, vector2i bounds)
{
    if(shot.x > bounds.x || shot.x < 0) return true;
    if(shot.y > bounds.y || shot.y < 0) return true;
    return false;
}

void shot_tick_render(shot_queue_t* queue, gamestate_t* state)
{
    shot_iterator iter = create_shot_iterator(*queue);

    shot_node_t* last = NULL;
    shot_node_t* markdelete = NULL;

    while(shot_iterator_increment(&iter))
    {
        if(markdelete != NULL)
        {
            free(markdelete);
            puts("deleting node");
            markdelete = NULL;
        }

        shot_t* shot = shot_iterator_cur(&iter);
        shot->position = add_v2f(shot->position, mult_v2f(shot->velocity, state->tickspeed));

        // remove shot
        if(out_of_bounds(shot->position, state->screen_size))
        {
            markdelete = iter.cur;
            if(last == NULL)
            {
                queue->head = iter.cur->next;
            }
            else
            {
                last->next = iter.cur->next;
            }
        }

        draw_shot(shot, state->color_palette->shot);

        last = iter.cur;
    }

    if(markdelete != NULL)
    {
        free(markdelete);
    }

}

void asteroid_tick_hit_check_render(asteroid_queue* queue, shot_queue_t* shots, ship_t* ship)
{
    asteroid_iterator iter = create_asteroid_iterator(queue);

    ast_node* mark_split = NULL;
    ast_node* lasteroid = NULL;

    while(ast_iterator_increment(&iter))
    {
        if(mark_split != NULL)
        {
            split_asteroid(queue, &mark_split->asteroid);
            free(mark_split);
            mark_split = NULL;
        }

        asteroid_t* ast = ast_iterator_current(&iter);
        // position tick
        ast->location = add_v2f(ast->location, mult_v2f(ast->velocity, ast->game->tickspeed));
        ast->location = wrapPosition(ast->location, ast->game->screen_size, ast->asteroid_size);

        ast->rotation += ast->rot_velocity;

        // ship hit checking
        if(ship->invincibility_time <= 0 &&
            v2f_distance(ship->position, ast->location) < ast->asteroid_size * ASTEROID_SIZEMULT)
        {
            if(is_point_in_asteroid(ast, ship->position))
                ship->hit_flag = true;
        }

        // shot hit checking
        shot_iterator shot_rator = create_shot_iterator(*shots);

        shot_node_t* last_shot = NULL;
        shot_node_t* markdelete = NULL;  

        while(shot_iterator_increment(&shot_rator))
        {
            if(markdelete != NULL)
            {
                free(markdelete);
                markdelete = NULL;
            }

            shot_t* bullet = shot_iterator_cur(&shot_rator);
            float distance = v2f_distance(bullet->position, ast->location);

            if(distance < ast->mesh.maxradius * ast->asteroid_size * ASTEROID_SIZEMULT)
            {
                //puts("Passed first hit check");
                if(is_point_in_asteroid(ast, bullet->position))
                {
                    // hit
                    if (last_shot == NULL)
                        shots->head = shot_rator.cur->next;
                    else
                        last_shot->next = shot_rator.cur->next;

                    if (lasteroid == NULL)
                        queue->head = iter.cur->next;
                    else 
                        lasteroid->next = iter.cur->next;

                    mark_split = iter.cur;
                    markdelete = shot_rator.cur;
                }

            }

            last_shot = shot_rator.cur;
        }

        if(markdelete != NULL)
        {
            free(markdelete);
        }

        draw_asteroid(ast);
        lasteroid = iter.cur;
    }

    if(mark_split != NULL)
    {
        split_asteroid(queue, &mark_split->asteroid);
        free(mark_split);
    }

}

int game_loop(gamestate_t* state)
{
    srand(time(NULL));

    if(!al_init())
        return -1;

    if(!al_init_primitives_addon())
        return -2;

    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(state->screen_size.x, state->screen_size.y);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(state->tickspeed);

    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    ALLEGRO_EVENT e;

    bool redraw = true;
    bool keys[N_HOLD_KEYS] = {0};

    asteroid_queue asteroids = create_asteroid_queue();
    shot_queue_t shots = create_shot_queue();
    ship_t ship = create_ship(state);
    ship.invincibility_time = I_FRAME_TIME;

    state->collection = n_random_meshes(4, 15, 3);

    for (int i = 0; i < 4; i++)
        push_new_asteroid(&asteroids, 3, state, state->collection);

    while(true)
    {
        al_wait_for_event(queue, &e);

        if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        else if(e.type == ALLEGRO_EVENT_TIMER)
        {
            // tick event
            redraw = true;
            if(ship.invincibility_time >= 0)
                ship.invincibility_time -= state->tickspeed;
        }
        else if(e.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
            else if(e.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                vector2f vel = mult_v2f(angle_to_cartesian(ship.rotation), 500);
                push_shot_to_queue(&shots, ship.position, vel);
            }
            else key_array(keys, e.keyboard.keycode, true);
        }
        else if(e.type == ALLEGRO_EVENT_KEY_UP)
        {
            key_array(keys, e.keyboard.keycode, false);
        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(state->color_palette->background);

            if(any_key_array(keys))
            {
                if(keys[HK_LEFTARROW])
                    ship.rotation -= 0.08f;
                else if(keys[HK_RIGHTARROW])
                    ship.rotation += 0.08f;

                if (keys[HK_UPARROW])
                {
                    vector2f addvel = mult_v2f(angle_to_cartesian(ship.rotation), ship.speed);
                    ship.velocity = add_v2f(ship.velocity, addvel);
                }
            }

            ship.position = add_v2f(ship.position, mult_v2f(ship.velocity, state->tickspeed));
            ship.position = wrapPosition(ship.position, state->screen_size, 0);
            ship.velocity = mult_v2f(ship.velocity, ship.speeddampen);

            shot_tick_render(&shots, state);
            asteroid_tick_hit_check_render(&asteroids, &shots, &ship);

            if(ship.hit_flag == true)
            {
                ship.hit_flag = false;
                ship.lives--;

                if(ship.lives <= 0)
                    break;

                vector2f middle = mult_v2f(v2itof(state->screen_size), 0.5f);
                ship.position = middle;
                ship.velocity = create_v2f(0, 0);
                ship.invincibility_time = I_FRAME_TIME;

            }
            else draw_ship(&ship);

            if(asteroids.head == NULL)
            {
                for (int i = 0; i < 4; i++)
                    push_new_asteroid(&asteroids, 3, state, state->collection);
                ship.invincibility_time = I_FRAME_TIME;
            }

            al_flip_display();
            redraw = false;

        }

    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_uninstall_keyboard();

    destroy_meshes(state->collection);

    return 0;

}