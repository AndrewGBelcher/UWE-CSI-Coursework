/**
 * @file event_queue_manager.h
 *
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the event queue manager for the MSP430FR5969
 */

#ifndef EVENT_QUEUE_MANAGER_H_
#define EVENT_QUEUE_MANAGER_H_

#include <Events/EventQueue/event_queue.h>

#define EVENT_QUEUE_LISTENER_SIZE 4

struct Event_Queue_Manager {

    struct Event pending_events[EVENT_QUEUE_MAX_SIZE];
    struct Event_Queue* listener_queues[EVENT_QUEUE_LISTENER_SIZE];
    int listeners;
    int pending_event_count;
};


/** @brief Creates an instance of an event queue manager
 *
 *  @param void
 *  @return struct Event_Queue_Manager pointer
 */
struct Event_Queue_Manager* event_queue_manager_create(void);


/** @brief Registers an event queue manager with and event queue
 *
 *  @param struct Event_Queue_Manager pointer
 *  @param struct Event_Queue pointer
 *  @return void
 */
void event_queue_manager_register_listener(struct Event_Queue_Manager*, struct Event_Queue*);

/** @brief Dispatches on a supplied event queue manager
 *
 *  @param struct Event_Queue_Manager pointer
 *  @return void
 */
void event_queue_manager_dispatch(struct Event_Queue_Manager*);


/** @brief Pushes and event to an event queue via its manager
 *
 *  @param struct Event_Queue_Manager pointer
 *  @param struct Event
 *  @return void
 */
void event_queue_manager_push_event(struct Event_Queue_Manager*, struct Event);

#endif /* EVENT_QUEUE_MANAGER_H_ */
