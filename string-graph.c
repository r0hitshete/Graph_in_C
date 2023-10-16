#include "string-graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string-graph-datastructure.h"


/**
 *
 * This file initializes a weighted String graph with the definition found in 
 * "string-graph-datastructure.h". It has methods to add a vertex to the graph,
 * get the number of verticies in the graph, get a list of all the verticies,
 * add edges between the graph, get the weight of an edge, and get the number 
 * neigboring vertices of a node
 *
**/

/**
 * New additions in Proj08 are methods that remove edges, vertices, removing
 * the whole graph, and more.
**/


/**
 * This initializes the parameter graph to defult values
**/
void graph_init(String_graph *const graph) {
  String_graph *temp;

  if (graph) {
    temp = malloc(sizeof(String_graph));
    *graph = *temp;
    graph -> num_vert = 0;
    graph -> list = NULL;
    free(temp);
  }
}


/**
 * This adds a vertex to graph. If the vertex doesn't exists, then it adds it,
 * if it does exist, it doesn't add anything
**/
short add_vertex_to_graph(String_graph *const graph, const char new_vertex[]) {
  node *vertex;
  char *name_ptr;
  
  if (!graph || !new_vertex || isa_vertex(graph, new_vertex)) 
    return 0;


  vertex = malloc(sizeof(node));
  name_ptr = malloc(((int)strlen(new_vertex)*sizeof(char))+1);
  strcpy(name_ptr, new_vertex);
  
  vertex->name = name_ptr;
  vertex->weight = -1;
  vertex->next = NULL;
  (graph->num_vert)++;
  
  /**
   * Adds the node to the list of all nodes in the graph struct.
   * Reallocates the memory if the list already contains information.  
  **/
  if (graph -> num_vert > 1) {
    graph->list = realloc((graph->list),(graph->num_vert)*sizeof(node));
    *((graph->list) + (graph->num_vert)-1) = vertex;
  }
  else {
    graph->list = malloc(sizeof(node));
    *(graph->list) = vertex;
  }
  return 1;
}

/** 
 * This returns the number of verticies there are
**/
short vertex_count(const String_graph *const graph) {
  if (!graph) 
    return 0;
  return graph->num_vert;
}

/**
 * This checks if a given sting is already asscioated with a vertex
**/
short isa_vertex(const String_graph *const graph, const char name[]) {
  int i;

  if (!graph || !name) 
    return 0;
  /** Parses through the array until it finds a match **/
  for (i = 0; i < graph->num_vert; i++) {
    if (strcmp(((*((graph->list)+i))->name), name) == 0) {
      return 1;
    }
  }
  return 0;
}

/**
 * Returns a list of all the vertices
**/
char **get_vertex_list(const String_graph *const graph) {
  int i;
  int j;
  node *temp;
  char **arr;
  char *name_ptr;

  if (!graph)
    return 0;

  /** allocates enough space for the array **/
   arr = malloc(((graph->num_vert)+1)*sizeof(char*));

  /** sorts graph->list based of name using bubble sort **/
  for (i = 0; i < (graph->num_vert)-1; i++) {
    for (j = 0; j < (graph->num_vert)-i-1; j++) {
      if (strcmp((*((graph->list)+j))->name,
                 (*((graph->list)+j+1))->name) > 0) {
        temp = (*((graph->list)+j));
        (*((graph->list)+j)) = (*((graph->list)+j+1));
        (*((graph->list)+j+1)) = temp;
      }
    }
  }

  /** adds all names in graph->list to arr **/
  for (i = 0; i < (graph->num_vert) ; i++) {
    name_ptr = 
      malloc((((int)strlen((*((graph->list)+i))->name))*sizeof(char))+1);
    strcpy(name_ptr, (*((graph->list)+i))->name);
    *(arr+i) = name_ptr;
  }
  /** sets last array index to NULL **/
  *(arr+(graph->num_vert)) = NULL;
  return arr;  
}

/**
 * Creates an edge between two vertices. If the verticies don't exist,
 * it creates them, if the already have an edge, it updates the weight
**/
short create_graph_edge(String_graph *const graph, const char source[], 
                        const char dest[], int weight) {
  int i = 0;
  node *d;
  node *curr;
  char *name_ptr;
  

  if (!graph || !source || !dest || weight < 0) 
    return 0;
  
  /** makes node that has the information from destination and weight **/
  d = malloc(sizeof(node));
  name_ptr = malloc(((int)strlen(dest)*sizeof(char))+1);
  strcpy(name_ptr, dest);
  d->name = name_ptr;
  d->weight = weight;
  d->next = NULL;

  /** 
   * adds vertex to the graph, if it already exixts, it wont add it,
   * the check is already in add_vertex_to_graph
  **/
  add_vertex_to_graph(graph, source);
  add_vertex_to_graph(graph, dest);

  /** finds the source node in the list of nodes **/
  while (strcmp((*((graph->list)+i))->name,source) !=0) 
    i++;
  
  /** moves it over 1, needed for if a node points back to itself **/
  curr = *((graph->list)+i);
  curr = curr->next;

  /** finds if their is already an edge between the nodes **/
  while (curr && curr ->next && strcmp(curr->name, dest) != 0) 
    curr = curr->next;
  
  /** Updates weight if node already exists **/
  if (curr && strcmp(curr->name,dest) == 0) 
    curr->weight = weight;
  /** if it doesn't exist then addes the node to be an edge **/
  else if ((*((graph->list)+i))->next == NULL)
    (*((graph->list)+i))->next = d;
  else {
    d->next = (*((graph->list)+i))->next;
    (*((graph->list)+i))->next = d;
  }
  return 1;
}

/**
 * Returns the weight of an edge if it exists otherwise returns -1
**/
short get_graph_edge_weight(const String_graph *const graph, 
                            const char source[], const char dest[]) {
  int i = 0;
  node *curr;

  if (!graph || !source || !dest || !(isa_vertex(graph,source))
    || !(isa_vertex(graph, dest)))
    return -1;
  /** finds the source node **/
  while (strcmp((*((graph->list)+i))->name,source) !=0) 
    i++;

  /** 
   * sets curr to be the node and shifts over to the next 
   * one (nodes that point to themselves) 
  **/
  curr = *((graph->list)+i);
  curr = curr->next;

  /** finds if destination isi an edge **/
  while (curr && curr->next && strcmp(curr->name, dest) != 0) 
    curr = curr->next;
  
  /** returns weight if edge is found, else returns -1 if nothing is found**/
  if (curr && strcmp(curr->name , dest) == 0) 
    return curr->weight;
  
  return -1;
}
/**
 * returns the number of neighbors a node has
**/
short neighbor_count(const String_graph *const graph, const char vertex[]) {
  int count = 0;
  int i = 0;
  node *curr;
  if (!graph || !vertex || !(isa_vertex(graph, vertex))) {
    return -1;
  }
  /** finds source node **/
  while (strcmp((*((graph->list)+i))->name,vertex) !=0) 
    i++;
  curr = *((graph->list)+i);
  /** counts the number of nodes in the linked list **/
  while (curr->next)  {
    curr = curr->next;
    count++;
  }
  return count;
}


/* frees all the names that are in the names array and then frees the array */
void free_vertex_name_list(char **const names) {
  int i = 0;

  if (names) {
    /* last index is NULL as we set in our other methods*/
    while (*(names+i) != NULL) {
      free(*(names+i));
      i++;
    }
    free(names);
  }
}

/* returns a dynamically allocated array that contains neigbors of vertex */
char **get_neighbor_names(const String_graph *const graph, 
                          const char vertex[]) {
  
  int i = 0;
  int j;
  int k;
  char **list;
  node *curr;
  char *name_ptr;
  char *name_ptr_t;
  
  if (!graph || !vertex || !isa_vertex(graph, vertex)) 
    return NULL;

  /* finds vertex */
  while (strcmp((*((graph->list)+i))->name,vertex) != 0) 
    i++;

  list = malloc(((neighbor_count(graph, vertex)+1)*sizeof(char*)));
  curr = (*((graph->list)+i));
  i = 0;

  /* adds all neighhbors in linked list to array */
  while (curr->next) {
    curr = curr->next;
    name_ptr = malloc((((int)strlen(curr->name))*sizeof(char))+1);
    strcpy(name_ptr, curr->name);
    *(list+i) = name_ptr;
    i++;
  }
  *(list+i) = NULL;  
  
  /* sorts array with bubble sort */
  for (k = 0; k < i - 1; k++) {
    for (j = 0; j < i - k - 1; j++) {
      if (strcmp(*(list+j),*(list+j+1)) > 0) {
        name_ptr_t = *(list+j);
        *(list+j) = *(list+j+1);
        *(list+j+1) = name_ptr_t;
        
      }
    }
  }
  return list;
  
  return NULL;
}

/* remove the edge between source and dest if it allocates it */
int remove_graph_edge(String_graph *const graph, const char source[], 
                      const char dest[]) {
  int i = 0;
  node *curr;
  node *prev;

  if (!graph || !source || !dest || !(isa_vertex(graph, source)) 
    || !(isa_vertex(graph, dest)) 
    || get_graph_edge_weight(graph, source, dest) == -1) 
    return 0;

  /* finds source */
  while (strcmp((*((graph->list)+i))->name,source) != 0) 
    i++;


  curr = *((graph->list)+i);
  prev = curr;
  curr = curr->next;
  
  /* finds dest */
  while (strcmp(curr->name, dest) != 0) {
    prev = curr;
    curr = curr->next;
  }


  /* removes dest from the linked list and frees the memmory */
  if (curr->next) 
    prev->next = curr->next;
  else
    prev->next = NULL;
  free(curr->name);
  free(curr);
  return 1;
}

/* removes vertex from a the graph, then removes any edges the go to it */
int remove_vertex_from_graph(String_graph *const graph, const char vertex[]) {
  int i = 0;
  int j;
  node *curr;
  node *temp;

  if (!graph || !vertex || !(isa_vertex(graph, vertex)))
    return 0;

  /* finds vertex */
  while (strcmp((*((graph->list)+i))->name, vertex) != 0) 
    i++;

  curr = *((graph->list)+i);

  /* frees vertex memory */
  while (curr->next) {
    temp = curr;
    curr = curr->next;
    free(temp->name);
    free(temp);
  }

  /* frees it from array */
  free((*((graph->list)+i))->name);
  free(*((graph->list)+i));

  /* shifts every element in arrary after vertex back one */
  for (j = i; j < graph->num_vert - 1; j++) 
    *((graph->list)+j) = *((graph->list)+j+1);
  
  /* reallocates the array memory */
  graph->list = realloc(graph->list, ((graph->num_vert)-1) * sizeof(node));
  (graph->num_vert)--;

  /* removes the vertex node as an edge */
  for (i = 0; i < graph->num_vert; i++) 
    remove_graph_edge(graph, (*((graph->list)+i))->name, vertex);

  return 1;
}

/* deletes the entire graph, freeing all the memory */
void graph_delete(String_graph *const graph) {
  
  int i;
  node *curr;
  node *temp;

  if (graph) {

    /* frees all elements and the linked list associated with them */
    for (i = 0; i < (graph->num_vert); i++) {
      curr = (*((graph->list)+i));
      while (curr) {
        temp = curr;
        curr = curr->next;
        free(temp->name);
        free(temp);
      }
    }
    /* frees the list */
    free(graph->list);
  }
}
