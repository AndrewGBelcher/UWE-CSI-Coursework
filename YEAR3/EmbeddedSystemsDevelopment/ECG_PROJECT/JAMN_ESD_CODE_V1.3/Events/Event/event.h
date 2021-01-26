/**
 * @file event.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the events used for the MSP430FR5969
 */

#ifndef EVENT_H_
#define EVENT_H_

struct Event {

    int event_id;
};

/** @brief Creates an instance of an event
 *
 *  @param int the event id
 *  @return struct Event
 */
struct Event event_create(int event_id);

#endif /* EVENT_H_ */
