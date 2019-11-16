//
// Created by Mark on 10/12/2019.
//
// Reading files: szak.txt, csp.txt, hatar.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "managefile.h"
//#include "../debugmalloc.h"

/*
 * The content of the three files:
 * csp.txt:
 * The list of Hungarian places with real geological coordinates, around 250 of them.
 * szak.txt:
 * The connection between two points is shown in this one with the actual distance between them.
 * hatar.txt:
 * The coordinates of the Hungarian border line
 */

typedef struct {
    int num, first, second;
    double weight;
}GraphData;

/* Read from szak.txt file, and fill the graph
 * @param graph is the graph filled with DBL_MAX
 * @return modified graph When there is a connection between two points, the value is the distance between them.
 *
 * graph[a][b] = graph[b][a]
 *
 * @date 2019.11.01.
 */
bool readGraph(double **graph){
    FILE *fp;
    fp = fopen("szak.txt", "r");

    //Error handling
    if (fp == NULL){
        printf("Error with opening the graph");
        return false;
    }
    GraphData temp;
    while (fscanf(fp, "%d %d %d %lf", &temp.num, &temp.first, &temp.second, &temp.weight) == 4){
        graph[temp.first - 1][temp.second - 1] = temp.weight;
        graph[temp.second - 1][temp.first - 1] = temp.weight;
    }

    fclose(fp);
    return true;
}

/*
 * Converts the GPS coordinates to pixels in order to match a 1400x700 window (actual value: ~1100 x ~700)
 * @param x and y one point's coordinates
 *
 * @date 2019.11.12.
*/
void dataManipulation(double *x, double *y){
    const int xRed = 418900; //Reduce x coord.
    const int yRed = 36200; //Reduce y coord.
    const double scale = 0.00211; //Squeeze

    *x -= xRed;
    *y -= yRed;
    *x *= scale;
    *y *= scale;
}

/* Read from csp.txt file, and put it into a location array
 * @param size is the number of vertexes in the graph, first it's 0 and the function increases it
 * @return position is the location array, with the points x and y coordinates in it
 *
 * @date 2019.11.01.
 */
Location* readPosition(int *size){
    FILE *fp;
    fp = fopen("csp.txt", "r");

    //Error handling
    if (fp == NULL){
        printf("Error with opening the file");
        return NULL;
    }
    int len = 0;
    Location* position;
    position = (Location*) malloc(len * sizeof(Location));
    Location temp;

    while (fscanf(fp, "%d %s %lf %lf", &temp.num, temp.name, &temp.x, &temp.y) == 4){
        dataManipulation(&temp.x, &temp.y);
        if (temp.num > len){
                len = temp.num;
            position = (Location*) realloc(position, (len+1) * sizeof(Location));
        }
        position[temp.num-1] = temp;
        *size += 1;
    }

    fclose(fp);
    return position;
}

/* Read from hatar.txt file, and put it into a point array
 * @param sizeB is the size of this file. The function sets this value
 * @return borderline is the point array with the border's points
 *
 * @date 2019.11.01.
 */
Point* readBorder(int *sizeB){
    FILE *fp;
    fp = fopen("hatar.txt", "r");

    //Error handling
    if (fp == NULL){
        printf("Error with opening the file");
        return NULL;
    }
    Point *borderline;
    borderline = (Point*) malloc(*sizeB * sizeof(Point));
    Point temp;

    while (fscanf(fp, "%lf %lf", &temp.x, &temp.y) == 2){
        dataManipulation(&temp.x, &temp.y);
        borderline = (Point*) realloc(borderline, (*sizeB + 1) * sizeof(Point));
        borderline[*sizeB].x = temp.x;
        borderline[*sizeB].y = temp.y;
        *sizeB += 1;
    }
    fclose(fp);

    return borderline;
}

