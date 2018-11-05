#include <stdio.h>
#include <stdlib.h>

#include "graphs.h"

/* The weight of an edge in this graph
 * is represented by an integer 
 */
typedef struct edge {
    int weight;
    ALIST_ELEM(edge) pointer;
} Edge;

/* typedef the graph type for ease of use */
typedef GRAPH(mygraph, Edge) Graph;

int main(void){
    Graph g = GRAPH_INIT();

    // Allocate space for 5 pointers to adjacency lists
    GRAPH_ALLOC(&g, Edge, 5);   

    if (GRAPH_EMPTY(&g))
        return 1;

    // Show that we have an empty graph
    printf("The graph has %i vertices and %i edges\n",
        GRAPH_NUM_VERTICES(&g), GRAPH_NUM_EDGES(&g));

    // Allocate a new edge
    Edge *n = calloc(1, sizeof(Edge)), *temp = NULL;
    if (n){
        // This edge has a weight of 5
        n->weight = 5;
        ALIST_NEXT(n, pointer) = NULL;

        // This edge refers to an adjacency to vertex 1
        ALIST_INDEX(n, pointer) = 1;

        // Actually add the edge to the 0th adjacency list
        ALIST_ADD_EDGE(&g, 0, n, pointer);

        // Increment the number of edges, since one has been added
        GRAPH_NUM_EDGES(&g)++;

        // Use temp to look into the adjacency list of the first vertex
        temp = ALIST_FIRST(&g, 0);
        printf("%i to %i: %i\n",
            0, ALIST_INDEX(temp, pointer), temp->weight);
    }


    // Free the memory that was allocated,
    // this will automatically deallocate edges
    GRAPH_CLEANUP(&g, temp, pointer);

    return 0;
}

