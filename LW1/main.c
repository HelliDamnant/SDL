#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#define TITLE "0111101001100001011000110110100001111001011011110111010000100000011100000110110001111010"
#define DEFAULT_X 100
#define DEFAULT_Y 100

#define PI 3.14

#define ERROR {printf("Error: %s\n", SDL_GetError());exit(1);}

int main()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)) ERROR

	int WIDTH = 1120, HEIGHT = 630;
	SDL_Window *win = SDL_CreateWindow(TITLE, DEFAULT_Y, DEFAULT_X, WIDTH, HEIGHT,
									   SDL_WINDOW_SHOWN|
									   SDL_WINDOW_RESIZABLE
									  );
	if (!win) ERROR

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
										   SDL_RENDERER_ACCELERATED|
										   SDL_RENDERER_PRESENTVSYNC
										  );
	if (!ren) ERROR

	int quit = 0;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) quit = 1;

		SDL_SetRenderDrawColor(ren, 15, 15, 15, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);

		SDL_GetWindowSize(win, &WIDTH, &HEIGHT);

		SDL_RenderDrawLine(ren, 20, HEIGHT / 2, WIDTH - 20, HEIGHT / 2);
		SDL_RenderDrawLine(ren, WIDTH / 2, 20, WIDTH / 2, HEIGHT - 20);

		int a = WIDTH / 10,
			l = WIDTH / 20;
		for (float t = -PI; t <= PI; t += 0.01)
		{
			float mu = cos(t),
				  nu = sin(t);
			SDL_RenderDrawPoint(ren,
								WIDTH / 2 + (a * mu * mu + l * mu),
								HEIGHT / 2 - (a * mu * nu + l * nu)
			);
		}

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	system("clear");
}