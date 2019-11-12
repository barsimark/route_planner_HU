//
// Created by Mark on 10/12/2019.
//
//Graph management with Dijkstra algorithm

#include "managegraph.h"
#include "managefile.h"
#include <stdlib.h>
#include <float.h>
//#include "../debugmalloc.h"

typedef struct {
    double value;
    int vertex;
} table;
/*
 * @param graph with the distances in it
 * @param graphSize the size of the graph
 * @param startPoint of the Dijkstra algorithm
 * @param endPoint of the algorithm
 * @param dist is the overall shortest distance will be placed here
 *
 * @return the vertexes' id during the shortest path in reverse order
 * @return the overall shortest distance between the start and endpoints
 *
 * @date 2019.11.02.
 */
int* dijkstraAlgorithm(const Graph graph, int startPoint, int endPoint, double *dist){
    /*
     * The distance array is the table where there is the shortest distance from the starting point so far
     * and the previous vertex's id for the optimal way.
     * The content of this array is updated when there is a better option, a shorter path
     *
     * At first all the distances are unknown, therefore infinite. DBL_MAX signs the infinity.
     * And we don't know the previous vertex either, which is shown by the -1 id
     */
    table distance[graph.size];
    for (int i = 0; i < graph.size; ++i) {
        distance[i].value = DBL_MAX;
        distance[i].vertex = -1;
    }
    //Setting the distance from the starting point to 0, and the previous vertex to itself
    distance[startPoint].value = 0;
    distance[startPoint].vertex = startPoint;

    /*
     * Dividing the vertexes into two arrays:
     * - visited is which we have already visited, therefore we won't do it again
     * - unvisited is which we are vising at some point
     */
    // At first none of the vertexes is visited...
    int *pVisited = malloc(0);
    //...and all of them are unvisited
    int *pUnvisited = malloc(graph.size * sizeof(int));
    for (int i = 0; i < graph.size; ++i) {
        pUnvisited[i] = i;
    }

    //The size of the visited array is 0, and the size of the unvisited ones is the graph's size
    int size = graph.size;
    int sizeVisited = 0;
    //The algorithm is running while we have not visited all the vertexes
    while (size > 0){
        /*
         * We choose the vertex with the smallest distance from the starting point
         * At first this is the starting point itself
         */
        int current = pUnvisited[0];
        for (int i = 0; i < size; ++i) {
            if (distance[pUnvisited[i]].value < distance[current].value)
                current = pUnvisited[i];
        }
        /*
         * When we have that vertex, we look at the distances from here to all of its neighbours
         * If that distance is smaller than the known distance, which is in the distance array, we
         * update the array with the calculated distance, and the previous vertex with the current one.
         */
        for (int i = 0; i <= graph.size; ++i) {
            if (graph.values[current][i] != 0 && graph.values[current][i] != DBL_MAX){
                double weight = distance[current].value + graph.values[current][i];
                if (weight < distance[i].value){
                    distance[i].value = weight;
                    distance[i].vertex = current;
                }
            }
        }
        //When that's ready we put the current vertex from the unvisited array to the visited one
        sizeVisited++;
        pVisited = realloc(pVisited, sizeVisited * sizeof(int));
        pVisited[sizeVisited-1] = current;
        int *pNew = malloc((size-1) * sizeof(int));
        int pNewCount = 0;
        for (int i = 0; i < size; ++i) {
            if (pUnvisited[i] != current)
                pNew[pNewCount++] = pUnvisited[i];
        }
        free(pUnvisited);
        pUnvisited = pNew;
        size--;
        //And repeat this process for all of the vertexes
    }
    //The distance array is full of data, so we don't need the visited and unvisited arrays anymore
    free(pUnvisited);
    free(pVisited);

    /*
     * Then we only have to read the data out of the distance array. We search for the endpoint, then read out the
     * previous vertex from there. Go to that one and read out the next vertex, etc. We repeat this process, until
     * we find the starting point or a -1 id, which means that the endpoint is separated from the starting point,
     * therefore there is no path between the two.
     */
    int routeCount = 0;
    int *pRoute = malloc((routeCount+1) * sizeof(int));
    pRoute[routeCount++] = endPoint;
    int id = endPoint;
    while (id != startPoint){
        if (id == -1){
            free(pRoute);
            return NULL;
        }
        id = distance[id].vertex;
        pRoute = realloc(pRoute, (routeCount+1) * sizeof(int));
        pRoute[routeCount++] = id;
    }
    // -1 signs the end of the array
    pRoute = realloc(pRoute, (routeCount+1) * sizeof(int));
    pRoute[routeCount] = -1;

    *dist = distance[endPoint].value;
    return pRoute;
}

/*
 * @param size is the size of the graph
 * @return graph is the created graph with DBL_MAX or actual value at each position
 *
 * @date 2019.11.03.
 */
double** initGraph(int size){
    //Create empty 2D array
    double **graph = (double**) malloc(size * sizeof(double*));
    graph[0] = (double*) malloc(size * size * sizeof(double));
    for (int i = 1; i < size; ++i) {
        graph[i] = graph[0] + i * size;
    }
    //Fill it with infinity, DBL_MAX
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            graph[i][j] = DBL_MAX;
        }
    }
    /*
     * managefile.c function
     * Fills graph with actual value from szak.txt
     */
    graph = readGraph(graph);
    if (graph == NULL){
        free(graph[0]);
        free(graph);
        return NULL;
    }

    return graph;
}


