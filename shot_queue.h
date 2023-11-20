#ifndef _SHOT_QUEUE_H_
#define _SHOT_QUEUE_H_

#include "asteroids.h"

typedef struct
{
    vector2f position;
    vector2f velocity;
} shot_t;

typedef struct __shotnode_
{
    struct __shotnode_* next;
    shot_t value;
} shot_node_t;

typedef struct
{
    shot_node_t* head;
} shot_queue_t;

typedef struct
{
    shot_node_t* cur;
    bool pre;
} shot_iterator;


shot_queue_t create_shot_queue();
void push_shot_to_queue(shot_queue_t* queue, vector2f origin, vector2f direction);

shot_iterator create_shot_iterator(shot_queue_t queue);
shot_t* shot_iterator_cur(shot_iterator* iter);
bool shot_iterator_increment(shot_iterator* iter);

#endif