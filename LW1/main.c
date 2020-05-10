#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TITLE "0111101001100001011000110110100001111001011011110111010000100000011100000110110001111010"
#define DEFAULT_X 100
#define DEFAULT_Y 100

#define ERROR {printf("Error: %s\n", SDL_GetError());exit(1);}

int main()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)) ERROR

	int WIDTH = 1120,
		HEIGHT = 630;
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

		// отрисовка осей
		SDL_RenderDrawLine(ren, 20, HEIGHT / 2, WIDTH - 20, HEIGHT / 2);
		SDL_RenderDrawLine(ren, WIDTH / 2, 20, WIDTH / 2, HEIGHT - 20);

		// отрисовка графика
		int a = WIDTH / 12;
		for (float m = -HEIGHT / 2 + 20; m <= HEIGHT / 2 - 20; m += 0.1)
			SDL_RenderDrawPoint(ren,
								WIDTH / 2 + a * (m*m)/(a*a) / (1 + (m*m)/(a*a)),
								HEIGHT / 2 - a * (m*m*m)/(a*a*a) / (1 + (m*m)/(a*a))
							   );

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    system("clear");
}