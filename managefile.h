//
// Created by Mark on 10/12/2019.
//

#include <stdbool.h>
#include <SDL.h>

#ifndef MANAGEFILE_H
#define MANAGEFILE_H

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
    Sint16 *x, *y;
}Border;

bool readGraph(double **data, const int size);
bool readPosition(Position *position);
bool readBorder(Border *border, const int windowY);

#endif //MANAGEFILE_H


