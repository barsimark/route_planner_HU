#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "managefile.h"
#include "managegraph.h"
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
//#include "../debugmalloc.h"

//RGB color code
typedef struct{
    int red, green, blue;
}RGB;

//initialize SDL
void sdl_init(char const *name, int width, int heigth, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Error with starting SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, heigth, 0);
    if (window == NULL) {
        SDL_Log("Error with creating window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Error with creating renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

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
 *Write to the SDL window
 *@param renderer is the SDL renderer
 *@param text is the text to show on the screen
 *@param x and y are the coordinates of the text's first letter
 *@param size is the size of the letters
 *
 *@param date 2019.11.09.
*/
void printTextToSDL(SDL_Renderer *renderer, char *text, int x, int y, int size){
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", size);
    if (!font) {
        SDL_Log("Error with opening font! %s\n", TTF_GetError());
        exit(2);
    }
    SDL_Surface *ptext;
    SDL_Texture *ptext_t;
    SDL_Color black = {0, 0, 0};

    ptext = TTF_RenderUTF8_Blended(font, text, black);
    ptext_t = SDL_CreateTextureFromSurface(renderer, ptext);

    SDL_Rect where = {0, 0, 0, 0};
    where.x = x;
    where.y = y;
    where.w = ptext->w;
    where.h = ptext->h;
    SDL_RenderCopy(renderer, ptext_t, NULL, &where);
    SDL_FreeSurface(ptext);
    SDL_DestroyTexture(ptext_t);

    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);
}

/*
 *Draws the border to the window
 *@param renderer SDL renderer
 *@param windowY is the window's height
 *@param border is the structure containing the size and the values
 *
 *@date 2019.11.09.
*/
void drawMap(SDL_Renderer *renderer, int windowY, const Border border){
    Sint16 *listX = (Sint16*) malloc(border.size * sizeof(Sint16));
    Sint16 *listY = (Sint16*) malloc(border.size * sizeof(Sint16));

    for (int i=0; i<border.size; ++i){
        double x = border.values[i].x;
        double y = windowY - (border.values[i].y);
        listX[i] = (Sint16) x;
        listY[i] = (Sint16) y;
    }

    filledPolygonRGBA(renderer, listX, listY, border.size, 255, 255, 255, 255);
    aapolygonRGBA(renderer, listX, listY, border.size, 0, 0, 0, 255);

    free(listX);
    free(listY);
}

/*
 *Draws the graph to the window
 *@param renderer SDL renderer
 *@param windowY is the height of the window
 *@param position the position structure with size and location list in it
 *@param graph the graph structure with the 2D array with the connections and the graph's size
 *
 *@date 2019.11.09.
*/
void drawGraph(SDL_Renderer *renderer, int windowY, const Position position, Graph graph){
    double x1, x2, y1, y2;
    for (int i=0; i<graph.size; ++i){
        for (int j=0; j<graph.size; ++j){
            if (graph.values[i][j] != DBL_MAX){
                x1 = position.values[i].x;
                y1 = windowY - (position.values[i].y);
                x2 = position.values[j].x;
                y2 = windowY - (position.values[j].y);
                lineRGBA(renderer, (int) x1, (int) y1, (int) x2, (int) y2, 0, 0, 0, 255);
            }
        }
    }
}

/*
 *Draws the vertexes to the window
 *@param renderer SDL renderer
 *@param windowY is the window's height
 *@param position vertex array
 *
 *@date 2019.11.09.
*/
void drawPoints(SDL_Renderer *renderer, int windowY, const Position position){
    for (int i = 0; i<position.size; ++i){
        double x = position.values[i].x;
        double y = windowY - (position.values[i].y);
        filledCircleRGBA(renderer, (int) x, (int) y, 3, 0, 0, 0, 255);
    }
    SDL_RenderPresent(renderer);
}

/*
 *Draws the two buttons on the left of the UI
 *@param renderer SDL renderer
 *
 *@date 2019.11.09.
*/
void drawButtons(SDL_Renderer *renderer){
    Sint16 ButtonX[] = {1200, 1450, 1450, 1200};
    Sint16 newButtonY[] = {100, 100, 150, 150};
    Sint16 planButtonY[] = {170, 170, 220, 220};

    printTextToSDL(renderer, "Tervezés", 1270, 175, 32); //Planing
    printTextToSDL(renderer, "Új útvonal", 1260, 105, 32); //New route

    aapolygonRGBA(renderer, ButtonX, newButtonY, 4, 0, 0, 0, 255);
    aapolygonRGBA(renderer, ButtonX, planButtonY, 4, 0, 0, 0, 255);

    filledPolygonRGBA(renderer, ButtonX, newButtonY, 4, 0, 0, 0, 30);
    filledPolygonRGBA(renderer, ButtonX, planButtonY, 4, 0, 0, 0, 30);
}

/*
 *Draws the entire UI when program starts and when new route button is pushed
 * @param renderer SDL renderer
 * @param windowY is the window's height
 * @param position vertex array
 * @param graph
 * @param border border points
 *
 * @date 2019.11.11.
*/
void drawUI(SDL_Renderer *renderer, int windowX, int windowY, const Position position, const Graph graph, const Border border){
    boxRGBA(renderer, 0, 0, windowX, windowY, 255, 255, 255, 255);
    boxRGBA(renderer, 0, 0, windowX, windowY, 0, 0, 0, 30);
    drawMap(renderer, windowY, border);
    drawGraph(renderer, windowY, position, graph);
    drawPoints(renderer, windowY, position);
    drawButtons(renderer);

    SDL_RenderPresent(renderer);
}

/*
 *Connects two adjacent vertexes
 *In use when drawing the optimal route on map
 *@param renderer SDL renderer
 *@param windowY is height of the window
 *@param position is vertex array
 *@param first and second are the points
 *
 *@date 2019.11.09.
*/
void connectTwoPoints(SDL_Renderer *renderer, int windowY, const Position position, int first, int second){
    double x1 = position.values[first].x;
    double y1 = windowY - position.values[first].y;
    double x2 = position.values[second].x;
    double y2 = windowY - position.values[second].y;
    thickLineRGBA(renderer, x1, y1, x2, y2, 3, 255, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

/*
 *Draws the graph to the window
 *@param renderer SDL renderer
 *@param windowY
 *@param vertex is the chosen vertex of the graph
 *@param color is either black or red will be the color of the vertex
 *
 *@date 2019.11.09.
*/
void colorChosenVertex(SDL_Renderer *renderer, int windowY, Location vertex, RGB color){
    filledCircleRGBA(renderer, vertex.x, windowY-vertex.y, 3, color.red, color.green, color.blue, 255);
    SDL_RenderPresent(renderer);
}

/* When a vertex is chosen it decides to add than to the list or remove from it
 * It also colors the vertex with red if adding and black if removing
 * @param renderer SDL renderer
 * @param windowY is the window's height
 * @param position is the vertex array
 * @param vertex is the chosen vertex by the user
 * @param chosenpoints is the array with the chosen vertexes in it
 * @param size is the size of the chosenpoints array
 *
 * @date 2019.11.09.
*/
int* vertexesChosen(SDL_Renderer *renderer, int windowY, const Position position, int vertex, int *chosenpoints, int *size){
    bool isInList = false;
    for (int i=0; i<*size; ++i){
        if (vertex == chosenpoints[i]){
            isInList = true;
            break;
        }
    }
    RGB color;
    if (isInList){
        int *newlist = (int*) malloc((*size-1) * sizeof(int));
        int idx = 0;
        for (int i=0; i<(*size); ++i){
            if (chosenpoints[i] != vertex)
                newlist[idx++] = chosenpoints[i];
        }
        *size -= 1;
        free(chosenpoints);
        chosenpoints = newlist;
        color = (RGB) {0, 0, 0};
    } else{
        chosenpoints = (int*) realloc(chosenpoints, (*size+1) * sizeof(int));
        chosenpoints[*size] = vertex;
        *size += 1;
        color = (RGB) {255, 0, 0};
    }
    colorChosenVertex(renderer, windowY, position.values[vertex], color);

    return chosenpoints;
}

/* Displays the starting point
 * @param renderer SDL renderer
 * @param place is the starting vertex
 *
 * @date 2019.11.12.
*/
void displayFirstRoute(SDL_Renderer *renderer, Location place){
    const int startx = 1000;
    const int starty = 250;
    const int size = 20;
    printTextToSDL(renderer, "Kezdőpont:", startx, starty, size);
    printTextToSDL(renderer, place.name, startx+100, starty, size);
}

/* Displays the route information
 * @param renderer SDL renderer
 * @param num
 * @param place is the current vertex
 * @param distance is the distance between the previous and this vertex
 * @param text is whether a middle point or the endpoint
 *
 * @date 2019.11.12.
*/
void displayRoute(SDL_Renderer *renderer, int num, Location place, double distance, char *text){
    const int startx = 1000;
    const int starty = 250;
    const int size = 20;
    printTextToSDL(renderer, text, startx, starty + num*(size+10), size);
    printTextToSDL(renderer, place.name, startx+100, starty + num*(size+10), size);
    char distText[10];
    sprintf(distText, "%.2f", distance);
    printTextToSDL(renderer, distText, startx+320, starty + num*(size+10), size);
    printTextToSDL(renderer, "km", startx+390, starty + num*(size+10), size);
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

    drawUI(renderer, windowX, windowY, position, graph, border);

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
        }
        if (click){
            if (x >= 1200 && x <= 1450){
                if (y >= 100 && y <= 150){
                    //New route button is pushed
                    drawUI(renderer, windowX, windowY, position, graph, border);
                    SDL_RenderPresent(renderer);
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
                                if (i == 1)
                                    displayFirstRoute(renderer, position.values[route[j-1]]);
                                if (i == numberOfChosen-1)
                                    displayRoute(renderer, i, position.values[route[0]], distanceSum, "Végpont:"); //Endpoint
                                else
                                    displayRoute(renderer, i, position.values[route[0]], distanceSum, "Köztes pont:"); //Middle point
                                free(route);
                            }
                        }
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

    free(border.values);
    free(position.values);
    free(graph.values[0]);
    free(graph.values);

    SDL_Quit();

    return 0; // 0 => no error; else => error
}
