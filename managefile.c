//
// Created by Mark on 10/12/2019.
//
// Reading files: szak.txt, csp.txt, hatar.txt

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "managefile.h"
#include "../debugmalloc.h"

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
static void dataManipulation(double *x, double *y){
    const int xRed = 418900; //Reduce x coord.
    const int yRed = 36200; //Reduce y coord.
    const double scale = 0.00211; //Squeeze

    *x -= xRed;
    *y -= yRed;
    *x *= scale;
    *y *= scale;
}

/* Read from csp.txt file, and put it into the position structure
 * @param position is the position array which is built here
 * @return whether the file reading is successful or not
 *
 * @date 2019.11.01.
 */
bool readPosition(Position *position){
    FILE *fp;
    fp = fopen("csp.txt", "r");

    //Error handling
    if (fp == NULL){
        printf("Error with opening the file");
        return false;
    }

    int maxlen = 0;
    position->values = (Location*) malloc(0 * sizeof(Location));
    Location temp;

    while (fscanf(fp, "%d %s %lf %lf", &temp.num, temp.name, &temp.x, &temp.y) == 4){
        dataManipulation(&temp.x, &temp.y);
        if (temp.num > maxlen){
                maxlen = temp.num;
            position->values = (Location*) realloc(position->values, maxlen * sizeof(Location));
        }
        position->values[temp.num-1] = temp;
    }
    position->size = maxlen;

    fclose(fp);
    return true;
}

/* Read from hatar.txt file, and put it into the border structure
 * The x and y coordinates are separated into two Sint16 arrays for the drawing function
 * @param border is the border structure where the data is stored
 * @param windowY is the height of the window
 * @return logic variable which is false if the hatar.txt doesn't exist
 *
 * @date 2019.11.01.
 */
bool readBorder(Border *border, const int windowY){
    FILE *fp;
    fp = fopen("hatar.txt", "r");

    //Error handling
    if (fp == NULL){
        printf("Error with opening the file");
        return false;
    }

    int num = 0;
    border->x = (Sint16*) malloc(0 * sizeof(Sint16));
    border->y = (Sint16*) malloc(0 * sizeof(Sint16));
    double tempX, tempY;

    while (fscanf(fp, "%lf %lf", &tempX, &tempY) == 2){
        dataManipulation(&tempX, &tempY);
        num++;
        border->x = (Sint16*) realloc(border->x, num * sizeof(Sint16));
        border->y = (Sint16*) realloc(border->y, num * sizeof(Sint16));
        border->x[num-1] = (Sint16) tempX;
        border->y[num-1] = (Sint16) (windowY - tempY);
    }
    border->size = num;

    fclose(fp);
    return true;
}

