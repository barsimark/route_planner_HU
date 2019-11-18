//
// Created by Mark on 11/18/2019
//

#include <stdbool.h>
#include "managefile.h"
#include "managegraph.h"
#include <SDL.h>

#ifndef DRAWUI
#define DRAWUI

void sdl_init(char const *name, int width, int heigth, SDL_Window **pwindow, SDL_Renderer **prenderer);
bool drawUI(SDL_Renderer *renderer, int windowX, int windowY, const Position position, const Graph graph, const Border border);
void connectTwoPoints(SDL_Renderer *renderer, int windowY, const Position position, int first, int second);
int* vertexesChosen(SDL_Renderer *renderer, int windowY, const Position position, int vertex, int *chosenpoints, int *size);
bool displayFirstRoute(SDL_Renderer *renderer, Location place);
bool displayRoute(SDL_Renderer *renderer, int num, Location place, double distance, char *text);


#endif // DRAWUI
