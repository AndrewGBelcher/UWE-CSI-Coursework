/**
 * @file graph.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the ECG graph module for the MSP430FR5969
 */

#include <Graph/graph.h>

#ifndef GRAPH_C_
#define GRAPH_C_


/** @brief creates and initialises an instance of a graph
 *
 *  @param void
 *  @return struct Graph
 */
struct Graph graph_create(void){

    int i;
    struct Graph graph;

    graph.max_readings = 9;
    graph.bounds_upper = 0;
    graph.bounds_lower = 0;
    graph.index_drawn = 0;
    graph.index_written = 0;
    graph.reset_pending = 0;


    for(i = 0; i < 96; i++){

        graph.readings[i] = GRAPH_READING_EMPTY;
    }

    return graph;
}

/** @brief Clears all readings in the graph
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @return void
 */
void graph_clear(struct Graph* graph){

    int i;

    for(i = 0; i < 96; i++){

        graph->readings[i] = GRAPH_READING_EMPTY;
    }

    graph->reset_pending = 1;
    graph->index_written = 0;
    graph->readings[graph->index_written] = 0;
    graph->index_written = 1;
    graph->index_drawn = 0;
}


/** @brief sets the floor and ceiling of the graph within the display bounds
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @param int, the floor value
 *  @param int, the ceiling value
 *  @return void
 */
void graph_set_bounds(struct Graph* graph, int min, int max){

    graph->bounds_lower = min;
    graph->bounds_upper = max;
}


/** @brief Shifts each reading in the graph to the left
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @return void
 */
void graph_shift_readings(struct Graph* graph){

        int i;

        for(i = 0; i < graph->max_readings-1; i++){

            graph->readings[i] = graph->readings[i+1];
        }

        graph->readings[graph->max_readings] = 0;
}


/** @brief Adds a reading to the graph
 *
 *  @param struct Graph*, pointer to a graph structure
 *  @param unsigned int, the reading to add
 *  @return void
 */
void graph_add_reading(struct Graph* graph, unsigned int value){

    float maximum_divider = graph->bounds_upper - graph->bounds_lower;
    float percent = (value-graph->bounds_lower)/maximum_divider;
    int i;

    if(percent > 1){

        percent = 1;
    }

    else if(percent < 0){

        percent = 0;
    }

    graph->readings[graph->index_written] = (1-percent)*40;
    graph->index_written++;

    if(graph->index_written > (graph->max_readings) + 2){

        for(i = 0; i < graph->max_readings + 1; i++){

            graph->readings[i] = GRAPH_READING_EMPTY;
        }

        graph->reset_pending = 1;
        graph->index_written = 0;
        graph->readings[graph->index_written] = (1-percent)*40;
        graph->index_written = 1;
        graph->index_drawn = 0;
    }
}


#endif /* GRAPH_C_ */
