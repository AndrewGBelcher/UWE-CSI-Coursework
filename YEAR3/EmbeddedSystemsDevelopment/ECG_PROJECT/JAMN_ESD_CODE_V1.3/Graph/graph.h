/**
 * @file graph.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the ECG graph module for the MSP430FR5969
 */


#ifndef GRAPH_H_
#define GRAPH_H_

#define GRAPH_READING_EMPTY -1

struct Graph {
    char bounds_upper;
    char bounds_lower;
    char max_readings;
    unsigned int readings[96];

    char index_written;
    char index_drawn;
    char reset_pending;
};

extern struct Graph graph;

/** @brief creates and initialises an instance of a graph
 *
 *  @param void
 *  @return struct Graph
 */
struct Graph graph_create();


/** @brief Shifts each reading in the graph to the left
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @return void
 */
void graph_shift_readings(struct Graph* graph);


/** @brief Clears all readings in the graph
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @return void
 */
void graph_clear(struct Graph* graph);


/** @brief Adds a reading to the graph
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @param unsigned int, the reading to add
 *  @return void
 */
void graph_add_reading(struct Graph* graph, unsigned int value);


/** @brief sets the floor and ceiling of the graph within the display bounds
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @param int, the floor value
 *  @param int, the ceiling value
 *  @return void
 */
void graph_set_bounds(struct Graph* graph, int min, int max);




#endif /* GRAPH_H_ */




