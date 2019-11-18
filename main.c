#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "managefile.h"
#include "managegraph.h"
#include "drawui.h"
#include "../debugmalloc.h"


/*
 * Decide which vertex is the closest to the user's click
 * @param position is the vertexes' coordinates
 * @param windowY is the height of the window
 * @param x and y are the coordinates of the click
 * @return vertex is the closest one within the given range (eps)
 *
 * @date 2019.11.03.
 */
int vertexFromCoordinates(const Position position, int windowY, int x, int y) {
    const int eps = 5;
    int vertex = -1;
    int minDist = INT_MAX;
    for (int i = 0; i < position.size; ++i) {
        int dist = (int) sqrt(pow(position.values[i].x - x, 2) + pow((windowY - position.values[i].y) - y, 2));
        if (dist <= eps) {
            if (dist < minDist) {
                vertex = i;
                minDist = dist;
            }
        }
    }
    if (minDist == INT_MAX)
        return -1;
    return vertex;
}

/*
 * This function frees the graph, position and border
 *
*/
void freeAll(Graph graph, Position position, Border border){
    free(graph.values[0]);
    free(graph.values);
    free(position.values);
    free(border.values);
}

/*
 * Handle fatal errors: unable to draw UI, unable to write text to SDL, unable to open font ...
 * In case of these, the function frees the used memory, closes the SDL window and exits the program with error code 6.
*/
void fatalError(Graph graph, Position position, Border border){
    freeAll(graph, position, border);
    SDL_Quit();
    exit(6);
}


int main(int argc, char *argv[]) {
    const int windowY = 700;
    const int windowX = 1500;

    //load data
    Position position;
    Border border;
    Graph graph;
    position.size = 0;
    border.size = 0;
    position.values = readPosition(&position.size);
    if (position.values == NULL){
        exit(3);
    }
    graph.size = position.size;
    graph.values = initGraph(graph.size);
    if (graph.values == NULL){
        free(position.values);
        exit(4);
    }
    border.values = readBorder(&border.size);
    if (border.values == NULL){
        free(position.values);
        free(graph.values[0]);
        free(graph.values);
        exit(5);
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Utvonaltervezo", windowX, windowY, &window, &renderer); //Route planner
    TTF_Init();

    if(drawUI(renderer, windowX, windowY, position, graph, border) == false)
        fatalError(graph, position, border);

    int numberOfChosen = 0;
    int *chosenpoints = (int*) malloc(numberOfChosen * sizeof(int));

    bool quit = false;
    bool click = false;
    bool canEdit = true;
    int x, y;
    while (!quit){
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type){
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT){
                click = true;
                x = event.button.x;
                y = event.button.y;
            }
            break;
        }
        if (click){
            if (x >= 1200 && x <= 1450){
                if (y >= 100 && y <= 150){
                    //New route button is pushed
                    if(drawUI(renderer, windowX, windowY, position, graph, border) == false)
                        fatalError(graph, position, border);
                    SDL_RenderPresent(renderer);
                    if (numberOfChosen > 0)
                        free(chosenpoints);
                    numberOfChosen = 0;
                    chosenpoints = (int*) malloc(numberOfChosen * sizeof(int));
                    canEdit = true;
                }
                else if (y >= 170 && y <= 220 && canEdit){
                    //Planing button is pushed
                    if (numberOfChosen >= 2){
                        double distanceCurrent;
                        double distanceSum = 0;
                        int *route = NULL;
                        for (int i=1; i<numberOfChosen; ++i){
                            route = dijkstraAlgorithm(graph, chosenpoints[i-1], chosenpoints[i], &distanceCurrent);
                            if (route != NULL){
                                distanceSum += distanceCurrent;
                                int j;
                                for (j=1; route[j] != -1; j++)
                                    connectTwoPoints(renderer, windowY, position, route[j-1], route[j]);
                                if (i == 1){
                                    if(displayFirstRoute(renderer, position.values[route[j-1]]) == false){
                                        free(route);
                                        free(chosenpoints);
                                        fatalError(graph, position, border);
                                    }
                                }
                                if (i == numberOfChosen-1){
                                    if(displayRoute(renderer, i, position.values[route[0]], distanceSum, "Végpont:") == false){ //Endpoint
                                        free(route);
                                        free(chosenpoints);
                                        fatalError(graph, position, border);
                                    }
                                }
                                else{
                                    if(displayRoute(renderer, i, position.values[route[0]], distanceSum, "Köztes pont:") == false){ //Middle point
                                        free(route);
                                        free(chosenpoints);
                                        fatalError(graph, position, border);
                                    }
                                }
                                free(route);
                            }
                        }
                        numberOfChosen = 0;
                        free(chosenpoints);
                        canEdit = false;
                    }
                }
            } else if (canEdit) {
                int vertex = vertexFromCoordinates(position, windowY, x, y);
                if (vertex != -1)
                    chosenpoints = vertexesChosen(renderer, windowY, position, vertex, chosenpoints, &numberOfChosen);
            }
            click = false;
        }
    }

    freeAll(graph, position, border);

    SDL_Quit();
    return 0; // 0 => no error; else => error
}
