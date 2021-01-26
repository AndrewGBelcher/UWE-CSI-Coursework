/**
 * @file event_queue.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the event queue for the MSP430FR5969
 */

#include <Events/Event/event.h>

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_


#define EVENT_QUEUE_MAX_SIZE 10

struct Event_Queue{

    struct Event events[EVENT_QUEUE_MAX_SIZE];
    int back;
};

/** @brief Creates and instance of an event queue
 *
 *  @param void
 *  @return struct Event_Queue pointer
 */
struct Event_Queue* event_queue_create(void);


/** @brief Pushes an event to the selected event queue
 *
 *  @param struct Event_Queue pointer
 *  @param struct Event
 *  @return void
 */
void event_queue_push(struct Event_Queue* queue, struct Event event);


/** @brief Pops and event from the selected event queue
 *
 *  @param struct Event_Queue pointer
 *  @return struct Event
 */
struct Event event_queue_get(struct Event_Queue* queue);


/** @brief Validates if the event queue selected is empty
 *
 *  @param struct Event_Queue pointer
 *  @return int, false/true
 */
int event_queue_empty(struct Event_Queue* queue);

#endif /* EVENT_QUEUE_H_ */
