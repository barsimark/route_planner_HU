//
// Created by Mark on 10/12/2019.
//

#ifndef HAZI_TESZT_MANAGEFILE_H
#define HAZI_TESZT_MANAGEFILE_H

typedef struct {
    double x, y;
}point;

typedef struct {
    int num;
    double x, y;
    char name[51];
}location;

typedef struct{
    int size;
    location *values;
}Position;

typedef struct{
    int size;
    point *values;
}Border;

double **readGraph(double **data);
location* readPosition(int *size);
point* readBorder(int *sizeB);

#endif //HAZI_TESZT_MANAGEFILE_H


