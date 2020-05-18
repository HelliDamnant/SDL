#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159265
#define ERROR {printf("Error: %s\n", SDL_GetError());exit(1);}

#define TITLE "0111101001100001011000110110100001111001011011110111010000100000011100000110110001111010"
#define DEFAULT_X 200
#define DEFAULT_Y 150
int WIDTH = 1120;
int HEIGHT = 630;

typedef struct {
	float x;
	float y;
	float e;
} point;

typedef struct {
	float x;
	float y;
	float e;
} vector;

typedef struct {
	float matrix[3][3];
} matrix_3x3;

float rad(int);
void draw_coordinate_axes(SDL_Renderer*);
float sq(float);
point get_point(point, float, int, int, int);
void draw_graph(SDL_Renderer*, float, int, int, int);

int main()
{
	system("clear");

	int corner = 0,
		shift_x = 0,
		shift_y = 0;
	float scale = 1;

	printf("Do you want to enter the data yourself? (y/n)\n");
	char c = getc(stdin);
	if (c == 'y' || c == 'Y') {
		printf("Angle of rotation: ");
		scanf("%d", &corner);
		if (corner < 0 || corner > 360) ERROR
		if (corner == 360) corner = 0;

		printf("Shift by x: ");
		scanf("%d", &shift_x);

		printf("Shift by y: ");
		scanf("%d", &shift_y);

		printf("Scale: ");
		scanf("%f", &scale);
		if (scale < 0.2 || scale > 4) ERROR
	} else if (c != 'n' && c != 'N') {
		printf("Incorrect input!\n");
		exit(1);
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) ERROR

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
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                } else if (e.key.keysym.sym == SDLK_EQUALS) {
                    if (scale < 1)
						scale += 0.1;
					else if (scale < 5)
						scale += 1;
                } else if (e.key.keysym.sym == SDLK_MINUS) {
					if (scale > 1)
						scale -= 1;
					else if (scale > 0.1)
						scale -= 0.1;
				} else if (e.key.keysym.sym == SDLK_PERIOD) {
                    if (corner == 359)
                        corner = 0;
                    else
                        corner += 1;
				} else if (e.key.keysym.sym == SDLK_COMMA) {
                    if (corner == 0)
                        corner = 359;
                    else
                        corner -= 1;
				} else if (e.key.keysym.sym == SDLK_UP) {
					shift_y += 10;
				} else if (e.key.keysym.sym == SDLK_DOWN) {
					shift_y -= 10;
				} else if (e.key.keysym.sym == SDLK_LEFT) {
					shift_x -= 10;
				} else if (e.key.keysym.sym == SDLK_RIGHT) {
					shift_x += 10;
				}
            }

        SDL_SetRenderDrawColor(ren, 15, 15, 15, 255);
        SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);

		SDL_GetWindowSize(win, &WIDTH, &HEIGHT);

        draw_coordinate_axes(ren);

		draw_graph(ren, scale, corner, shift_x, shift_y);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

	system("clear");
	return 0;
}

void draw_graph(SDL_Renderer *ren, float scale, int corner, int shift_x, int shift_y)
{
	int a = WIDTH / 10, l = WIDTH / 20;
	for (float t = -PI; t <= PI; t += 0.01)
	{
		float mu = cos(t), nu = sin(t);
		point p;

		p.x = a * mu * mu + l * mu;
		p.y = a * mu * nu + l * nu;

		p = get_point(p, scale, corner, shift_x, shift_y);

		SDL_RenderDrawPoint(ren, WIDTH / 2 + p.x, HEIGHT / 2 - p.y);
	}
}

void draw_coordinate_axes(SDL_Renderer *ren)
{
    SDL_RenderDrawLine(ren, 20, HEIGHT / 2, WIDTH - 20, HEIGHT / 2);
    SDL_RenderDrawLine(ren, WIDTH / 2, 20, WIDTH / 2, HEIGHT - 20);
}

float sq(float a) {return a * a;}

point get_point(point p, float scale, int corner, int shift_x, int shift_y)
{
	p.x = scale * p.x;
	p.y = scale * p.y;

	float xt = p.x,
		  yt = p.y,
		  cost = cos(corner * PI / 180),
		  sint = sin(corner * PI / 180);
	p.x = xt * cost + yt * sint;
	p.y = yt * cost - xt * sint;

	p.x += shift_x;
	p.y += shift_y;

	return p;
}

matrix_3x3 translation_matrix(float T1, float T2)
{
	matrix_3x3 m;

	m.matrix[0][1] = m.matrix[1][0] = m.matrix[2][0] = m.matrix[2][1] = 0;
	m.matrix[0][0] = m.matrix[1][1] = m.matrix[2][2] = 1;
	m.matrix[0][2] = -T1;
	m.matrix[1][2] = -T2;

	return m;
}

matrix_3x3 dalatation_matrix(float S1, float S2)
{
	matrix_3x3 m;

	m.matrix[0][1] = m.matrix[0][2] = m.matrix[1][2] = m.matrix[1][0] = m.matrix[2][0] = m.matrix[2][1] = 0;
	m.matrix[2][2] = 1;
	m.matrix[0][0] = S1;
	m.matrix[1][1] = S2;

	return m;
}

matrix_3x3 rotation_matrix(int theta)
{
	theta = rad(theta);
	matrix_3x3 m;

	m.matrix[0][2] = m.matrix[1][2] = m.matrix[2][0] = m.matrix[2][1] = 0;
	m.matrix[2][2] = 1;
	m.matrix[0][0] = m.matrix[1][1] = cos(theta);
	m.matrix[1][0] = -(m.matrix[0][1] = sin(theta));

	return m;
}

float rad(int corn)
{
	return corn * 180 / PI;
}
