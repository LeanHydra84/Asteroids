#include <stdio.h>
#include "asteroids_pvt.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

int orientation(vector2f p, vector2f q, vector2f r)
{
    float val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);
    
    if(fabs(val) < 0.00001f) return 0;
    return (val > 0) ? 1 : 2;
}

bool intersects(vector2f p1, vector2f q1, vector2f p2, vector2f q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    //printf("Orientations: %d, %d, %d, %d\n", o1, o2, o3, o4);

    if(o1 != o2 && o3 != o4)
        return true;

    // ignoring special cases for now

    return false;
}

bool is_point_in_asteroid(asteroid_t* ast, vector2f pos)
{
    const int n_verts = ast->mesh.size;
    const float maxdiameter = 2 * ast->mesh.maxradius * ast->asteroid_size;
    vector2f* verts = ast->mesh.array;

    vector2f left = pos;
    left.x -= maxdiameter * ASTEROID_SIZEMULT;

    int count = 0;

    for(int i = 0; i < n_verts; i++)
    {
        int l2 = i + 1;
        if(l2 >= ast->mesh.size) l2 = 0;

        vector2f pos1 = add_v2f(mult_v2f(rotate_around(verts[i], ast->location, ast->rotation), ASTEROID_SIZEMULT * ast->asteroid_size), ast->location);
        vector2f pos2 = add_v2f(mult_v2f(rotate_around(verts[l2], ast->location, ast->rotation), ASTEROID_SIZEMULT * ast->asteroid_size), ast->location);
              
        bool inter = intersects(pos1, pos2, left, pos);

        if(inter)
            count++;
    }

    //printf("Line count: %d\n", count);
    return count % 2 != 0;
}