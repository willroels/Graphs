#ifndef GRAPHS_H_
#define GRAPHS_H_

#include <stdio.h>
#include <stdlib.h>

#define ALIST_ELEM(type)            \
struct {                            \
    unsigned int ind;               \
    struct type *next;              \
}

#define GRAPH(name, type)           \
struct name {                       \
    unsigned int graph_n_vertices;  \
    unsigned int graph_n_edges;     \
           type **graph_list_heads; \
}

#define GRAPH_NUM_VERTICES(graph)   \
    ((graph)->graph_n_vertices)

#define GRAPH_NUM_EDGES(graph)      \
    ((graph)->graph_n_edges)

#define GRAPH_ADJ_ARRAY(graph)      \
    ((graph)->graph_list_heads)

#define GRAPH_EMPTY(graph)          \
    (GRAPH_ADJ_ARRAY(graph) == NULL)

#define GRAPH_INIT()         {0}

#define GRAPH_ALLOC(graph, type, nvertices)                         \
do {                                                                \
    GRAPH_ADJ_ARRAY(graph) = calloc(nvertices, sizeof(type *));     \
    if (!GRAPH_EMPTY(graph))                                        \
        GRAPH_NUM_VERTICES(graph) = nvertices;                      \
    else                                                            \
        perror("Could not allocate the adjacency list pointers");   \
} while (0)

#define ALIST_FIRST(graph, index)       \
    ((graph)->graph_list_heads[index])

#define ALIST_NEXT(elem, field)         \
    ((elem)->field.next)

#define ALIST_INDEX(edge, field)        \
    ((edge)->field.ind)

#define ALIST_ADD_EDGE(graph, index, edge, field)               \
do {                                                            \
    ALIST_NEXT(edge, field) = ALIST_FIRST(graph, index);        \
    ALIST_FIRST(graph, index) = edge;                           \
} while (0)

#define GRAPH_FOREACH_VERTEX(graph, itervar)                         \
for (itervar = 0; (itervar) < GRAPH_NUM_VERTICES(graph); itervar++)

#define ALIST_FOREACH(temp, head, field)                             \
for ((temp) = (head); (temp); (temp) = ALIST_NEXT(temp, field))

#define GRAPH_CLEANUP(graph, temp, field)                   \
do {                                                        \
    unsigned int graph_cleanup_var;                         \
    GRAPH_FOREACH_VERTEX(graph_cleanup_var, graph){         \
        temp = ALIST_FIRST(graph, graph_cleanup_var);       \
        if (temp){                                          \
            do {                                            \
                void *t = (void *) ALIST_NEXT(temp, field); \
                free(temp);                                 \
                temp = t;                                   \
            } while (temp);                                 \
        }                                                   \
        ALIST_FIRST(graph, graph_cleanup_var) = NULL;       \
    }                                                       \
    GRAPH_NUM_VERTICES(graph) = 0;                          \
    if (GRAPH_ADJ_ARRAY(graph))                             \
        free(GRAPH_ADJ_ARRAY(graph));                       \
    GRAPH_ADJ_ARRAY(graph) = NULL;                          \
} while (0)

/* -- Edge finding macro and edge removing macro -- */
#define GRAPH_FIND_EDGE(graph, vtex1, vtex2, edge, field)   \
do {                                                        \
    ALIST_FOREACH(edge, ALIST_FIRST(graph, vtex1), field)   \
        if (ALIST_INDEX(edge, field) == (vtex2))            \
            break;                                          \
} while (0)

#define GRAPH_FIND_BEFORE(graph, vtex1, vtex2, edge, field) \
do {                                                        \
    ALIST_FOREACH(edge, ALIST_FIRST(graph, vtex1), field)   \
        if (ALIST_NEXT(edge, field) &&                      \
            ALIST_INDEX(ALIST_NEXT(edge, field), field))    \
                break;                                      \
} while(0)                                                    

#define GRAPH_REMOVE(graph, vtex1, vtex2, dbl_indirect, field)      \
do {                                                                \
    for (dbl_indirect = &ALIST_FIRST(graph, vtex1);                 \
        *(dbl_indirect);                                            \
        dbl_indirect = &(ALIST_NEXT(*(dbl_indirect), field))){      \
        if (ALIST_INDEX(*(dbl_indirect), field) == vtex2) {         \
            void *t_ = *dbl_indirect;                               \
            *dbl_indirect = ALIST_NEXT(*(dbl_indirect), field);     \
            free(t_);                                               \
            break;                                                  \
        }                                                           \
    }                                                               \
    GRAPH_NUM_EDGES(graph)--;                                       \
} while (0)

/* -- Graph property macros -- */

#define GRAPH_DENSITY(graph)                                            \
((double) 2 * GRAPH_NUM_EDGES(graph))                                   \
        /(GRAPH_NUM_VERTICES(graph) * (GRAPH_NUM_VERTICES(graph) - 1))

/* -- Kruskal MST algorithm stuff -- */

// This struct can form part of an element of an edge array
struct ft_pair {
    unsigned from, to;
};

#define FT_FROM(ft, field) ((ft)->field.from)
#define FT_TO(ft, field) ((ft)->field.to)

#define UFIND_INIT(ptr, nvertices)                              \
for (unsigned index_ = 0; index_ < (nvertices); index_++)       \
    ptr[index_] = index_

#define FIND_PARENT(ptr, vertex, root)                              \
do {                                                                \
	for (root = vertex; (root) != ptr[root]; root = ptr[root]);     \
} while (0)

#define IS_CYCLE(ft_pairs, n_edges, size_elem, roots, n_vertices,accepted, is_cyle)     \
do {                                                                           \
    is_cycle = 0;                                                              \
    struct ft_pair *ftptr = ft_pairs;                                          \
    UFIND_INIT(roots, n_vertices);                                             \
    for (unsigned i = 0; i < (accepted); i++){                                 \
        unsigned a_, b_, root_a, root_b;                                       \
        a_ = (ftptr)->from;                                                    \
        b_ = (ftptr)->to;                                                      \
        FIND_PARENT(roots, a_, root_a);                                        \
        FIND_PARENT(roots, b_, root_b);                                        \
        if (root_a == root_b) {                                                \
            is_cycle = 1;                                                      \
            break;                                                             \
        }                                                                      \
        roots[root_a] = root_b;                                                \
        ftptr = (struct ft_pair *)(((char *)ftptr) + (size_elem));             \
    }                                                                          \
} while (0)

/* -- Conversion -- */
/* Picks the unique elements from a graph and copies it into an array of edges
 * requires a copy function that looks like:
 * void copy_func(EdgeArray *a, Edge *b), and copies the weight from b to a
 */
#define EARRAY_FROM_ALIST(graph, edge, field, edge_array, ft_field, copy_func) \
do {                                                                           \
    unsigned vtex_no_ = 0, edge_no_ = 0;                                       \
    GRAPH_FOREACH_VERTEX(vtex_no_, graph) {                                    \
        ALIST_FOREACH(edge, ALIST_FIRST(graph, vtex_no_), field) {             \
            if (ALIST_INDEX(edge, field) > vtex_no_) {                         \
                copy_func((edge_array) + edge_no_, edge);                      \
                FT_FROM((edge_array) + edge_no_, ft_field) = vtex_no_;         \
                FT_TO((edge_array) + edge_no_, ft_field) =                     \
                                                ALIST_INDEX(edge, field);      \
                edge_no_++;                                                    \
            }                                                                  \
        }                                                                      \
    }                                                                          \
} while (0)

#endif
