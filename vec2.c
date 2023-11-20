#include "asteroids.h"
#include <math.h>

vector2f add_v2f(vector2f left, vector2f right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

vector2f mult_v2f(vector2f left, float right)
{
    left.x *= right;
    left.y *= right;
    return left;
}

vector2i add_v2i(vector2i left, vector2i right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

vector2i mult_v2i(vector2i left, int right)
{
    left.x *= right;
    left.y *= right;
    return left;
}

vector2i v2ftoi(vector2f op)
{
    vector2i rv;
    rv.x = (int)op.x;
    rv.y = (int)op.y;
    return rv;
}

vector2f v2itof(vector2i op)
{
    vector2f rv;
    rv.x = op.x;
    rv.y = op.y;
    return rv;
}

vector2f negative_v2f(vector2f vec)
{
    vec.x = -vec.x;
    vec.y = -vec.y;
    return vec;
}

vector2f rotate_around(vector2f point, vector2f origin, float angle)
{
    //point = add_v2f(point, negative_v2f(origin)); // make relative

    vector2f rv;
    rv.x = point.x * cos(angle) - point.y * sin(angle);
    rv.y = point.x * sin(angle) + point.y * cos(angle);
    return rv;
    //return add_v2f(point, origin);
}

vector2f create_v2f(float x, float y)
{
    vector2f v2;
    v2.x = x;
    v2.y = y;
    return v2;
}

vector2i create_v2i(int x, int y)
{
    vector2i v2;
    v2.x = x;
    v2.y = y;
    return v2;
}

vector2f angle_to_cartesian(float angle)
{
    // x = r * cos(a)
    // y = r * sin(a)

    angle += PI / 2.0f; // hooooly shit

    vector2f vec;
    vec.x = cos(angle);
    vec.y = sin(angle);
    return vec;
}

static float square(float n) { return n * n; }

float v2f_distance(vector2f left, vector2f right)
{
    float sqrmagnitude = square(left.x - right.x) + square(left.y - right.y);
    return sqrt(sqrmagnitude);
}