// ЭТО НЕ ПРОСТО ЦИССОИДА, А ЦИССОИДА ДИОКЛЕСА!!!

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TITLE "SHITSHITSHIT"
#define DEFAULT_X 100
#define DEFAULT_Y 100
int WIDTH = 1120;
int HEIGHT = 630;


#define PI 3.14159265
#define DIAMETER 40

#define INIT \
    if (SDL_Init(SDL_INIT_EVERYTHING)) ERROR\
    SDL_Window *win = SDL_CreateWindow(TITLE, DEFAULT_Y, DEFAULT_X, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);\
    if (!win) ERROR\

#define QUIT \
	SDL_DestroyRenderer(ren);\
    SDL_DestroyWindow(win);\
	SDL_Quit();\
	system("clear");\

#define ERROR \
{\
	printf("Error: %s\n", SDL_GetError());\
    exit(1);\
}

void draw_point(SDL_Renderer*, int, int, int);
void draw_coordinate_axes(SDL_Renderer*);
float radian(int);
float mod(float);
float angular_coef(int);

int main()
{
    INIT

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ERROR

    SDL_Event e;
    int quit = 0,
        corner = 0,
        scale = 1;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                } else if (e.key.keysym.sym == SDLK_EQUALS) {
                    scale += 1;
                } else if (e.key.keysym.sym == SDLK_MINUS) {
                    if (scale > 1) scale -= 1;
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    if (corner == 359)
                        corner = 0;
                    else
                        corner += 1;
                } else if (e.key.keysym.sym == SDLK_RIGHT) {
                    if (corner == 0)
                        corner = 359;
                    else
                        corner -= 1;
                }
            }

        SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 180, 180, 180, 255);

        draw_coordinate_axes(ren);

        float x = 0, y = x * angular_coef(corner);
        do {
			if (corner > 90 && corner < 270)
				SDL_RenderDrawPoint(ren, WIDTH / 2 - x, HEIGHT / 2 - y);
            else
				SDL_RenderDrawPoint(ren, WIDTH / 2 + x, HEIGHT / 2 - y);
            x += 0.01;
            y = x * angular_coef(corner);
        } while (x < WIDTH / 2 - 20 && y < HEIGHT / 2 - 20);

        // int a = DIAMETER * scale;
        // float x = 0,
        //       y = sqrt(scale * (x*x*x) / (a - x)) / scale;

        // do {
        //     SDL_RenderDrawPoint(ren,
        //                         WIDTH / 2 + x),
        //                         HEIGHT / 2 + y);
        //     SDL_RenderDrawPoint(ren,
        //                         WIDTH / 2 + x,
        //                         HEIGHT / 2 - y);
        //     x += 0.001;
        //     y = sqrt(scale * (x*x*x) / (a - x)) / scale;
        // } while (x < WIDTH / 2 && y < HEIGHT / 2);

        SDL_RenderPresent(ren);
    }

    QUIT
}

void draw_point(SDL_Renderer *ren, int x, int y, int sc)
{
}

void draw_coordinate_axes(SDL_Renderer *ren)
{
    SDL_RenderDrawLine(ren, 20, HEIGHT / 2, WIDTH - 20, HEIGHT / 2);
    SDL_RenderDrawLine(ren, WIDTH / 2, 20, WIDTH / 2, HEIGHT - 20);
}

float radian (int corner)
{
    return corner * PI / 180;
}

float mod (float num)
{
    if (num < 0)
        return -num;
    else
        return num;
}

float angular_coef(int corner)
{
    if (corner == 90 || corner == 270)
        return 0;
    else
        return tan(radian(corner));
}
