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

// Utility functions to add edges to the graph and to construct 
// a minimum spanning tree
int add_edge(Graph *g, unsigned from, unsigned to, int weight);
EdgeArr *Kruskal_MST(Graph *g);

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

    // Perform and report the result of the minimum spanning tree algorithm
    EdgeArr *ans = Kruskal_MST(&g);
    for (unsigned i = 0; i < GRAPH_NUM_VERTICES(&g) - 1; i++) {
        printf("%i to %i: %i\n", FT_FROM(ans + i, ft), FT_TO(ans + i, ft), ans[i].weight);
    }

    // Cleanup
    free(ans);
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

// Edge comparison function for qsort which is used in the MST algorithm
int edge_cmp(const void *a, const void *b) {
    return
    (((const EdgeArr *)a)->weight > ((const EdgeArr *)b)->weight) -
    (((const EdgeArr *)a)->weight < ((const EdgeArr *)b)->weight);
}

EdgeArr *Kruskal_MST(Graph *g) {
    // Allocate the space necessary to store the array of edges (type EdgeArr)
    EdgeArr *edges = calloc(GRAPH_NUM_EDGES(g), sizeof(EdgeArr));
    if (!edges) {
        perror("Could not allocate space for the edge array");
        return NULL;
    }

    // This array is necessary for the union-find algorithm to determine
    // if a propsed set of edges contains a cycle
    unsigned *roots = calloc(GRAPH_NUM_VERTICES(g), sizeof(unsigned));
    if (!roots) {
        perror("Could not allocate space for the roots array");
        free(edges);
        return NULL;
    }

    // Copy the unique elements of the graph into the edge array
    unsigned i = 0, edge_no = 0;
    GRAPH_FOREACH_VERTEX(i, g) {
        Edge *ptr;
        ALIST_FOREACH(ptr, ALIST_FIRST(g, i), pointer) {
            if (ALIST_INDEX(ptr, pointer) > i) {
                edges[edge_no].weight = ptr->weight;
                edges[edge_no].ft.from = i;
                edges[edge_no].ft.to = ALIST_INDEX(ptr, pointer);
                edge_no++;
            }
        }
    }

    // Sort the edge array using the comparison function defined above
    qsort(edges, GRAPH_NUM_EDGES(g), sizeof(EdgeArr), edge_cmp);

    // Perform the meat of the algorithm
    unsigned accepted;
    for (i = 0, accepted = 0; i < edge_no && accepted < GRAPH_NUM_VERTICES(g) - 1; i++) {
        // Move the proposed edge to the end of the edge array
        memcpy(edges + accepted, edges + i, sizeof(EdgeArr));

        // Check if this set of edges results in a cycle
        unsigned is_cycle;
        /* Macro arguments:
         * 1. array of from-to pairs
         * 2. num. of edges in graph
         * 3. size of the edge array struct
         * 4. array for the roots of edges
         * 5. number of vertices in the graph
         * 6. number accepted so far
         * 7. is_cycle variable
         */
        IS_CYCLE(&(edges[0].ft), edge_no, sizeof(EdgeArr),
                    roots, GRAPH_NUM_VERTICES(g),  accepted + 1, is_cycle);

        // If including the edge does not result in a cycle, add it
        if (!is_cycle) accepted++;
    }

    // Cleanup and return
    free(roots);
    return edges;
}
