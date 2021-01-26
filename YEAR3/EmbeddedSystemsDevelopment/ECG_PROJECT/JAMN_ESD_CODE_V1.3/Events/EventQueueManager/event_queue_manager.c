/**
 * @file EventQueueManager.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the event queue manager for the MSP430FR5969
 */

#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>
#include <stdlib.h>

#ifndef EVENT_QUEUE_MANAGER_C_
#define EVENT_QUEUE_MANAGER_C_


/** @brief Creates an instance of an event queue manager
 *
 *  @param void
 *  @return struct Event_Queue_Manager pointer
 */
struct Event_Queue_Manager* event_queue_manager_create(void){

    struct Event_Queue_Manager* manager = malloc(sizeof(struct Event_Queue_Manager));

    manager->listeners = 0;
    manager->pending_event_count = 0;

    return manager;
}

/** @brief Registers an event queue manager with and event queue
 *
 *  @param struct Event_Queue_Manager pointer
 *  @param struct Event_Queue pointer
 *  @return void
 */
void event_queue_manager_register_listener(struct Event_Queue_Manager* manager, struct Event_Queue* queue){

    manager->listener_queues[manager->listeners] = queue;
    manager->listeners++;
}


/** @brief Dispatches on a supplied event queue manager
 *
 *  @param struct Event_Queue_Manager pointer
 *  @return void
 */
void event_queue_manager_dispatch(struct Event_Queue_Manager* manager){

    int i;
    int j;

    for(i = 0; i < manager->pending_event_count; i++){

        for(j = 0; j < manager->listeners; j++){

            event_queue_push(manager->listener_queues[j], manager->pending_events[i]);
        }
    }

    manager->pending_event_count = 0;
}


/** @brief Pushes and event to an event queue via its manager
 *
 *  @param struct Event_Queue_Manager pointer
 *  @param struct Event
 *  @return void
 */
void event_queue_manager_push_event(struct Event_Queue_Manager* manager, struct Event e){

    manager->pending_events[manager->pending_event_count] = e;
    manager->pending_event_count++;
}

#endif /* EVENT_QUEUE_MANAGER_C_ */

