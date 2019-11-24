//
// Created by Mark on 11/18/2019
//
// SDL rendering functions

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "drawui.h"
#include "managefile.h"
#include "managegraph.h"
#include "../debugmalloc.h"

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
 *Write to the SDL window
 *@param renderer is the SDL renderer
 *@param text is the text to show on the screen
 *@param x and y are the coordinates of the text's first letter
 *@param size is the size of the letters
 *
 *@param date 2019.11.09.
*/
static bool printTextToSDL(SDL_Renderer *renderer, char *text, int x, int y, int size){
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", size);
    if (!font) {
        SDL_Log("Error with opening font! %s\n", TTF_GetError());
        return false;
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
    return true;
}

/*
 *Draws the border to the window
 *@param renderer SDL renderer
 *@param windowY is the window's height
 *@param border is the structure containing the size and the values
 *
 *@date 2019.11.09.
*/
static void drawMap(SDL_Renderer *renderer, int windowY, const Border border){
    filledPolygonRGBA(renderer, border.x, border.y, border.size, 255, 255, 255, 255);
    aapolygonRGBA(renderer, border.x, border.y, border.size, 0, 0, 0, 255);
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
static void drawGraph(SDL_Renderer *renderer, int windowY, const Position position, const Graph graph){
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
static void drawPoints(SDL_Renderer *renderer, int windowY, const Position position){
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
static bool drawButtons(SDL_Renderer *renderer){
    Sint16 ButtonX[] = {1200, 1450, 1450, 1200};
    Sint16 newButtonY[] = {100, 100, 150, 150};
    Sint16 planButtonY[] = {170, 170, 220, 220};

    if(printTextToSDL(renderer, "Tervezés", 1270, 175, 32) == false) //Planning
        return false;
    if(printTextToSDL(renderer, "Új útvonal", 1260, 105, 32) == false) //New route
        return false;

    aapolygonRGBA(renderer, ButtonX, newButtonY, 4, 0, 0, 0, 255);
    aapolygonRGBA(renderer, ButtonX, planButtonY, 4, 0, 0, 0, 255);

    filledPolygonRGBA(renderer, ButtonX, newButtonY, 4, 0, 0, 0, 30);
    filledPolygonRGBA(renderer, ButtonX, planButtonY, 4, 0, 0, 0, 30);
    return true;
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
bool drawUI(SDL_Renderer *renderer, int windowX, int windowY, const Position position, const Graph graph, const Border border){
    boxRGBA(renderer, 0, 0, windowX, windowY, 255, 255, 255, 255);
    boxRGBA(renderer, 0, 0, windowX, windowY, 0, 0, 0, 30);
    drawMap(renderer, windowY, border);
    drawGraph(renderer, windowY, position, graph);
    drawPoints(renderer, windowY, position);
    if(drawButtons(renderer) == false)
        return false;

    SDL_RenderPresent(renderer);
    return true;
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
static void colorChosenVertex(SDL_Renderer *renderer, int windowY, Location vertex, RGB color){
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
bool displayFirstRoute(SDL_Renderer *renderer, Location place){
    const int startx = 1000;
    const int starty = 250;
    const int size = 20;
    if(printTextToSDL(renderer, "Kezdőpont:", startx, starty, size) == false)
        return false;
    if(printTextToSDL(renderer, place.name, startx+100, starty, size) == false)
        return false;

    return true;
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
bool displayRoute(SDL_Renderer *renderer, int num, Location place, double distance, char *text){
    const int startx = 1000;
    const int starty = 250;
    const int size = 20;
    if(printTextToSDL(renderer, text, startx, starty + num*(size+10), size) == false)
        return false;
    if(printTextToSDL(renderer, place.name, startx+100, starty + num*(size+10), size) == false)
        return false;
    char distText[10];
    sprintf(distText, "%.2f", distance);
    if(printTextToSDL(renderer, distText, startx+350, starty + num*(size+10), size) == false)
        return false;
    if(printTextToSDL(renderer, "km", startx+420, starty + num*(size+10), size) == false)
        return false;

    return true;
}
