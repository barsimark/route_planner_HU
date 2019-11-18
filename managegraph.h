//
// Created by Mark on 10/12/2019.
//

#ifndef MANAGEGRAPH_H
#define MANAGEGRAPH_H

typedef struct{
    int size;
    double **values;
}Graph;

int* dijkstraAlgorithm(Graph graph, int startPoint, int endPoint, double *dist);
double** initGraph(int size);

#endif //MANAGEGRAPH_H


