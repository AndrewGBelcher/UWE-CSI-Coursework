/**
 * @file event.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the events used for the MSP430FR5969
 */

#include <Events/Event/event.h>

#ifndef EVENT_C_
#define EVENT_C_


/** @brief Creates an instance of an event
 *
 *  @param int the event id
 *  @return struct Event
 */
struct Event event_create(int event_id){

    struct Event event;

    event.event_id = event_id;

    return event;
}

#endif /* EVENT_C_ */

