#include <stdlib.h>
#include "shot_queue.h"
#include "asteroids_pvt.h"

void push_shot_to_queue(shot_queue_t* queue, vector2f origin, vector2f direction)
{
    shot_node_t* node = malloc(sizeof(shot_node_t));
    node->value.position = origin;
    node->value.velocity = direction;

    node->next = queue->head;
    queue->head = node;
}

shot_queue_t create_shot_queue()
{
    shot_queue_t rv;
    rv.head = NULL;
    return rv;
}

shot_iterator create_shot_iterator(shot_queue_t queue)
{
    shot_iterator iter;
    iter.cur = queue.head;
    iter.pre = false;
    return iter;
}

shot_t* shot_iterator_cur(shot_iterator* iter)
{
    if(iter == NULL) return NULL;
    return &iter->cur->value;
}

bool shot_iterator_increment(shot_iterator* iter)
{
    if(iter->cur == NULL) return false;
    if(iter->pre != false)
        iter->cur = iter->cur->next;
    else iter->pre = true;

    if(iter->cur == NULL) return false;
    return true;
}