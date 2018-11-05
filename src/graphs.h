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

#define GRAPH_NUM_EDGES(graph)   \
    ((graph)->graph_n_edges)

#define GRAPH_ADJ_ARRAY(graph)  \
    ((graph)->graph_list_heads)

#define GRAPH_EMPTY(graph)  \
    (GRAPH_ADJ_ARRAY(graph) == NULL)

#define GRAPH_INIT()         {0}

#define GRAPH_ALLOC(graph, type, nvertices)   \
do {                                                                \
    GRAPH_ADJ_ARRAY(graph) = calloc(nvertices, sizeof(type *));       \
    if (!GRAPH_EMPTY(graph))                                        \
        GRAPH_NUM_VERTICES(graph) = nvertices;                      \
    else                                                            \
        perror("Could not allocate the adjacency list pointers");   \
} while (0)

#define GRAPH_FOREACH_VERTEX(itervar, graph)        \
for (itervar = 0; (itervar) < GRAPH_NUM_VERTICES(graph); itervar++)

#define ALIST_FIRST(graph, index) \
    ((graph)->graph_list_heads[index])

#define ALIST_NEXT(elem, field) \
    ((elem)->field.next)

#define ALIST_INDEX(edge, field)        \
    ((edge)->field.ind)

#define ALIST_ADD_EDGE(graph, index, edge, field)     \
do {        \
    ALIST_NEXT(edge, field) = ALIST_FIRST(graph, index);        \
    ALIST_FIRST(graph, index) = edge;                           \
} while (0)

#define ALIST_FOREACH(temp, head, field)            \
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

/* -- Union find algorithm stuff -- */

struct ft_pair {
    unsigned from, to;
};

#define FT_FROM(ft, field) ((ft)->field.from)
#define FT_TO(ft, field) ((ft)->field.to)

#define UFIND_INIT(ptr, nvertices)          \
for (unsigned index_ = 0; index_ < (nvertices); index_++)       \
    ptr[index_] = index_

#define FIND_PARENT(ptr, vertex, root)                               \
do {                                                                 \
	for (root = vertex; (root) != ptr[root]; root = ptr[root]);     \
} while (0)

#define IS_CYCLE(ft_pairs, n_edges, size_elem, roots, n_vertices,accepted, is_cyle)     \
do {                                                                           \
    is_cycle = 0;                                                              \
    struct ft_pair *ftptr = ft_pairs;                                          \
    UFIND_INIT(roots, n_vertices);                                             \
    for (unsigned i = 0; i < (accepted); i++){                                  \
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

#endif
