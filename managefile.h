//
// Created by Mark on 10/12/2019.
//

#include <stdbool.h>
#ifndef HAZI_TESZT_MANAGEFILE_H
#define HAZI_TESZT_MANAGEFILE_H

typedef struct {
    double x, y;
}Point;

typedef struct {
    int num;
    double x, y;
    char name[51];
}Location;

typedef struct{
    int size;
    Location *values;
}Position;

typedef struct{
    int size;
    Point *values;
}Border;

bool readGraph(double **data);
Location* readPosition(int *size);
Point* readBorder(int *sizeB);

#endif //HAZI_TESZT_MANAGEFILE_H


