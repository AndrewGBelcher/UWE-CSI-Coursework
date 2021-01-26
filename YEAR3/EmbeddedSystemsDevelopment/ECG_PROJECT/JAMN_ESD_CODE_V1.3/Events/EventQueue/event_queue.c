/**
 * @file event_queue.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the event queue for the MSP430FR5969
 */

#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <stdlib.h>

#ifndef EVENT_QUEUE_C_
#define EVENT_QUEUE_C_


/** @brief Creates and instance of an event queue
 *
 *  @param void
 *  @return struct Event_Queue pointer
 */
struct Event_Queue* event_queue_create(void){

    struct Event_Queue* queue = malloc(sizeof(struct Event_Queue));

    queue->back = 0;

    return queue;
}


/** @brief Pushes an event to the selected event queue
 *
 *  @param struct Event_Queue pointer
 *  @param struct Event
 *  @return void
 */
void event_queue_push(struct Event_Queue* queue, struct Event event){

    queue->events[queue->back] = event;
    queue->back++;
}


/** @brief Pops and event from the selected event queue
 *
 *  @param struct Event_Queue pointer
 *  @return struct Event
 */
struct Event event_queue_get(struct Event_Queue* queue){

    int i;
    struct Event event = queue->events[0];

    for(i = 1; i < queue->back; i++){

        queue->events[i-1] = queue->events[i];
    }

    queue->back--;

    return event;
}


/** @brief Validates if the event queue selected is empty
 *
 *  @param struct Event_Queue pointer
 *  @return int, false/true
 */
int event_queue_empty(struct Event_Queue* queue){

    return (queue->back == 0);
}

#endif /* EVENT_QUEUE_C_ */
