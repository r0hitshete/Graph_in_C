#if !defined(STRING_GRAPH_DATASTRUCTURE_H)
#define STRING_GRAPH_DATASTRUCTURE_H


/**
  * two structs that define node and String_graph
 **/

/** defined with a name array, a weight, and a pointer to the next element **/
typedef struct node{
  char *name;
  int weight;
  struct node *next;
} node;

/**
:: * defined with a number of vertices and a double pointer which points to an array of nodes
**/
typedef struct String_graph{
  int num_vert;
  node** list;
} String_graph;

#endif
