
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphs.h"

// Typedefs for a graph where the weights are integers
typedef struct edge {
    int weight;
    ALIST_ELEM(edge) pointer;
} Edge;

// An element of an edge array, it contains the weight of the edge
// as well as a pair of unsigned integers denoting the related vertices
typedef struct {
    int weight;
    struct ft_pair ft;
} EdgeArr;

// Type Graph represents a graph where the edges are type Edge
typedef GRAPH(int_graph, Edge) Graph;

int add_edge(Graph *g, unsigned from, unsigned to, int weight);

int main(void) {
    // Initialize the graph and allocate space for the adjacency list pointers
    Graph g = GRAPH_INIT();
    GRAPH_ALLOC(&g, Edge, 9);

    if (GRAPH_EMPTY(&g)) {
        perror("Allocating adjacency list pointers failed.");
        return EXIT_FAILURE;
    }

    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 7, 8);
    add_edge(&g, 1, 2, 8);
    add_edge(&g, 1, 7, 11);
    add_edge(&g, 2, 3, 7);
    add_edge(&g, 2, 5, 4);
    add_edge(&g, 2, 8, 2);
    add_edge(&g, 3, 4, 9);
    add_edge(&g, 3, 5, 14);
    add_edge(&g, 4, 5, 10);
    add_edge(&g, 5, 6, 2);
    add_edge(&g, 6, 7, 1);
    add_edge(&g, 6, 8, 6);
    add_edge(&g, 7, 8, 7);

    // Intermediate feedback on the number of edges added
    printf("Edges added: %i\n", GRAPH_NUM_EDGES(&g));

    unsigned i = 0;
    Edge *ptr = NULL;
    GRAPH_FOREACH_VERTEX(i, &g) {
        printf("%i: ", i);
        ALIST_FOREACH(ptr, ALIST_FIRST(&g, i), pointer)
            printf("(%i, %i) -> ", ALIST_INDEX(ptr, pointer), ptr->weight);
        printf("\\0\n");
    }

    printf("Density of the graph: %f\n", GRAPH_DENSITY(&g));
    printf("Now removing edges 0 and 7\n");
    Edge **m;
    GRAPH_REMOVE(&g, 0,1, m, pointer);
    GRAPH_REMOVE(&g, 1,0, m, pointer);

    GRAPH_FOREACH_VERTEX(i, &g) {
        printf("%i: ", i);
        ALIST_FOREACH(ptr, ALIST_FIRST(&g, i), pointer)
            printf("(%i, %i) -> ", ALIST_INDEX(ptr, pointer), ptr->weight);
        printf("\\0\n");
    }
    printf("Density of the graph: %f\n", GRAPH_DENSITY(&g));
    // Cleanup
    Edge *temp;
    GRAPH_CLEANUP(&g, temp, pointer);
}

int add_edge(Graph *g, unsigned from, unsigned to, int weight) {
    // Allocate edges to be added to the graph, if unsuccessful
    // reutrn 0, indicating that no edges were added
    Edge *edge1 = malloc(sizeof(Edge));
    if (!edge1) {
        return 0;
    }

    Edge *edge2 = malloc(sizeof(Edge));
    if (!edge2) {
        free(edge1);
        return 0;
    }

    // Copy the given weight parameter into both datastructures
    edge1->weight = weight;
    edge2->weight = weight;

    // Keep a record of to which node the adjacency refers
    ALIST_INDEX(edge1, pointer) = from;
    ALIST_INDEX(edge2, pointer) = to;

    // Push the edges onto the proper adjacency lists
    // The edge with ALIST_INDEX 'from' should be
    // added to the 'to' adjacency list and vice versa
    ALIST_ADD_EDGE(g, to, edge1, pointer);
    ALIST_ADD_EDGE(g, from, edge2, pointer);

    // Increment the number of edges in the graph and return
    // Not sure I like the way this looks
    GRAPH_NUM_EDGES(g)++;
    return 1;
}
