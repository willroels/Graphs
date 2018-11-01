#ifndef GRAPHS_H_
#define GRAPHS_H_

#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define ADJ_LIST_ELEM(type)         \
struct {                            \
    unsigned int ind;               \
    struct type *g_next;            \
}

#define ADJ_LIST_HEAD(name, type)   \
struct name {                       \
    unsigned int g_size;            \
    struct type **g_listh;          \
}

/* Approximate function prototype for GRAPH_NUM_VERTICES: */
/* unsigned int GRAPH_SIZE(GRAPH *graph, GRAPH_ENTRY name) */
#define GRAPH_NUM_VERTICES(graph, field) ((graph)->field.g_size)

/* The type that graph first returns is the type of the adjacency list entries */
/* TYPE *GRAPH_FIRST(GRAPH *graph, unsigned int ind, GRAPH_ENTRY name) */
#define GRAPH_FIRST(graph, field, ind) ((graph)->field.g_listh[ind])

#define GRAPH_VALID_INDEX(graph, field, index) \
    ((index) < (graph)->field.g_size)

#define GRAPH_ADD_EDGE(graph, graph_field, index, edge, edge_field)            \
do {                                                                           \
    ADJ_LIST_NEXT(edge, edge_field) = GRAPH_FIRST(graph, graph_field, index);  \
    GRAPH_FIRST(graph, graph_field, index) = (edge);                           \
} while (0)

#define GRAPH_FOREACH_NODE(var, graph, field) \
for ((var) = 0; (var) < (graph)->field.g_size; (var)++)

/* Macros for handing the array of pointers to heads of adjacency lists */
#define ADJ_ARRAY_CALLOC(graph, field, type)\
do {                                                            \
    (graph)->field.g_listh =                                    \
        calloc((graph)->field.g_size, sizeof(struct type *));   \
} while (0)

#define ADJ_ARRAY_IS_NULL(graph, field)                         \
    (((graph)->field.g_listh) == NULL)

#define ADJ_ARRAY_FREE(graph, field)                            \
do {                                                            \
    free((void *) ((graph)->field.g_listh));                    \
} while (0) 

/* Macros for manipulating the list elements themselves */
#define ADJ_LIST_INVALIDATE(graph, field, index) \
    ((graph)->field.g_listh[index] = NULL)

#define ADJ_LIST_NEXT(elem, field) ((elem)->field.g_next)
#define ADJ_LIST_IND(elem, field)  ((elem)->field.ind)
#define ADJ_LIST_SET_SIZE(graph, field, size) ((graph)->field.g_size = size)

#define ADJ_LIST_FOREACH(var, head, field)\
for ((var) = (head); (var); (var) = ADJ_LIST_NEXT(var, field))

#define ADJ_LIST_LENGTH(temp, elem, field, ans)\
do {                                                                   \
   unsigned int length_;                                               \
   ADJ_LIST_FOREACH(temp, elem, field) length_++;                      \
   ans = length_;                                                      \
} while (0)

#define ADJ_LIST_FREE(temp, head, field)                        \
do {                                                            \
    (temp) = ADJ_LIST_NEXT(head, field);                        \
    free(head);                                                 \
    (head) = (temp);                                            \
} while (head)

/* -- Representing a graph as a list of edges -- */

#define EDGE_LIST(type)         \
struct {                        \
    unsigned int from, to;      \
    struct type *elist_next;    \
}

#define EDGE_LIST_HEAD(name, type)  \
struct name {                       \
    struct type *elist_head;        \
}

#define EDGE_LIST_FIRST(head)       ((head)->field.elist_head)
#define EDGE_LIST_NEXT(obj, field)  ((obj)->field.elist_next)
#define EDGE_LIST_PUSH(head, elm, field)\
do { \
    EDGE_LIST_NEXT(elm, field) = EDGE_LIST_FIRST(head);     \
    EDGE_LIST_FIRST(head) = (elm);                          \
} while (0)

/* -- Matrix representation -- */

/* Definitions for the size of */
#define SQRT_UINT_MAX (2 << ((CHAR_BIT * sizeof(unsigned int) / 2) - 1))
#define MATRIX_MAX_INDEX (SQRT_UINT_MAX - 1)
#define PRACTICAL_MAX_INDEX (SQRT_UINT_MAX / 4)

#define DENSE_GRAPH(name, type)         \
struct name {                           \
    unsigned int n_nodes;               \
    type         *dg_weights;           \
}

#define DENSE_GRAPH_MATRIX_PTR(graph, field)\
    ((graph)->field.dg_weights)

#define DENSE_GRAPH_NUM_NODES(graph, field)\
    ((graph)->field.n_nodes)

#define DENSE_GRAPH_ACCESS(graph, field, row, col)\
    (*(DENSE_GRAPH_MATRIX_PTR(graph, field)                           \
     + (row)                                                        \
     + ((col) * DENSE_GRAPH_NUM_NODES(graph, field))))

/* Allocate a dense graph, the index into which can be calculated by
 * an unsigned int, checking that malloc succeded is still necessary 
 * */
#define DENSE_GRAPH_MALLOC(graph, field, num_nodes, element_size)\
do { \
    assert((element_size) <= (UINT_MAX / ((num_nodes) * (num_nodes))));        \
    ((graph)->field.dg_weights) =                                              \
        malloc((num_nodes) * (num_nodes) * (element_size));                    \
    ((graph)->field.n_nodes) = num_nodes;                                      \
} while (0)

#define DENSE_GRAPH_NODE_FOREACH(graph, field, var1, var2)\
for ((var1) = 0; (var1) < DENSE_GRAPH_NUM_NODES(graph, field); (var1)++)\
for ((var2) = 0; (var2) < DENSE_GRAPH_NUM_NODES(graph, field); (var2)++)

#define DENSE_GRAPH_REALLOC(temp, graph, field, num_nodes, element_size)\
do {\
    assert((element_size) <= (UINT_MAX / ((num_nodes) * (num_nodes))));        \
    temp = realloc(DENSE_GRAPH_MATRIX_PTR(graph, field),                       \
                  (element_size) * (num_nodes) * (num_nodes));        \
    if (temp) {                                                                \
    /*  TODO: shift the elements of the matrix to make room for new elems*/    \
    } else {                                                                   \
        perror("Reallocation of the matrix failed.");                          \
    }                                                                          \
} while (0)

#define DENSE_GRAPH_FREE(graph, field)\
    free((graph)->field.dg_weights);


#endif
