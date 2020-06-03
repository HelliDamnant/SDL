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
} POINT;

typedef struct {
	POINT p1;
	POINT p2;
	POINT p3;
	POINT p4;
} QUADRANGLE;

typedef struct {
	int a;
	int b;
} ELIPSE;

typedef struct {
	int x;
	int y;
} SHIFT_CS;

float rad(int);
float sq(float);
void draw_primitives(SDL_Renderer*, QUADRANGLE, ELIPSE, SHIFT_CS, SHIFT_CS, SDL_Rect);

int main()
{
	system("clear");

	printf("Do you want to enter the data yourself? (y/n)\n");
	char c = getc(stdin);
	SDL_Rect viewing_window;
	if (c == 'y' || c == 'Y') {

	} else if (c == 'n' || c == 'N') {
		viewing_window.x = WIDTH / 2 - 300;
		viewing_window.y = HEIGHT / 2 - 200;
		viewing_window.w = 600;
		viewing_window.h = 400;
	} else {
		printf("Incorrect input!\n");
		exit(1);
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) ERROR

	SDL_Window *main_window = SDL_CreateWindow(TITLE, DEFAULT_Y, DEFAULT_X, WIDTH, HEIGHT,
									   SDL_WINDOW_SHOWN|
									   SDL_WINDOW_RESIZABLE
									  );
	if (!main_window) ERROR

    SDL_Renderer *ren = SDL_CreateRenderer(main_window, -1,
										   SDL_RENDERER_ACCELERATED|
										   SDL_RENDERER_PRESENTVSYNC
										  );
    if (!ren) ERROR

	QUADRANGLE primitive_1;
	primitive_1.p1.x = -280;
	primitive_1.p1.y =  100;
	primitive_1.p2.x = -180;
	primitive_1.p2.y =  100;
	primitive_1.p3.x = -180;
	primitive_1.p3.y = -100;
	primitive_1.p4.x = -280;
	primitive_1.p4.y =    0;

	ELIPSE primitive_2;
	primitive_2.a = 50;
	primitive_2.b = 100;

	SHIFT_CS shift_1, shift_2;
	shift_1.x = 0;
	shift_1.y = 0;
	shift_2.x = 230;
	shift_2.y = 0;

	int quit = 0;
	int mot1 = 0, mot2 = 0;
    while (!quit)
    {
		SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.motion.x - WIDTH / 2,
					y = e.motion.y - HEIGHT / 2;
                if (sq((x - shift_2.x)/primitive_2.a) + sq((y - shift_2.y)/primitive_2.b) <= 1)
					mot1 = 1;
				else if (x>primitive_1.p1.x + shift_1.x &&
						 y<primitive_1.p1.y - shift_1.y &&
						 x<primitive_1.p3.x + shift_1.x &&
						 y>primitive_1.p3.y - shift_1.y)
					mot2 = 1;
            } else if (e.type == SDL_MOUSEBUTTONUP) {
				mot1 = mot2 = 0;
			}

			if (mot1) {
				shift_2.x = e.motion.x - WIDTH / 2;
				shift_2.y = -(e.motion.y - HEIGHT / 2);
			} else if (mot2) {
				shift_1.x = e.motion.x - WIDTH / 2 + 230;
				shift_1.y = -(e.motion.y - HEIGHT / 2);
			}
		}

        SDL_SetRenderDrawColor(ren, 15, 15, 15, 255);
        SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);
		SDL_RenderDrawRect(ren, &viewing_window);


		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		draw_primitives(ren, primitive_1, primitive_2, shift_1, shift_2, viewing_window);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

	system("clear");
	return 0;
}

float vector_length(POINT p1, POINT p2)
{
	return sqrt(sq(p2.x - p1.x) + sq(p2.y - p1.y));
}

float point_line(float mu, float t1, float t2)
{
	return (1 - mu) * t1 + mu * t2;
}

int intersec(POINT p, ELIPSE e, SHIFT_CS es)
{
	if (sq((p.x - es.x)/e.a) + sq((p.y - es.y)/e.b) <= 1)
		return 1;
	else
		return 0;
}

int visibility(POINT p, SDL_Rect w)
{
	int wx = WIDTH / 2 - w.x, wy = HEIGHT / 2 - w.y;
	if (p.x < wx && p.y < wy && p.x > -wx && p.y > -wy)
		return 1;
	else
		return 0;
}

void draw_line(SDL_Renderer *ren, POINT p1, POINT p2, SHIFT_CS shift, ELIPSE e, SHIFT_CS es, SDL_Rect w)
{
	float len = vector_length(p1, p2);
	if (p1.x > p2.x) {
		POINT tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	POINT l = p1;
	int f = 0;
	for (float t = 0; t <= len; t += 0.01)
	{
		float mu = t/len;
		POINT p;

		p.x = point_line(mu, p1.x, p2.x) + shift.x;
		p.y = point_line(mu, p1.y, p2.y) + shift.y;

		if (visibility(p, w)) {
			if (intersec(p, e, es)) {
				if (f < 1000) SDL_RenderDrawPoint(ren, WIDTH / 2 + p.x, HEIGHT / 2 - p.y);
				if (f > 3000) f = 0;
				f++;
			} else {
				SDL_RenderDrawPoint(ren, WIDTH / 2 + p.x, HEIGHT / 2 - p.y);
			}
		}

		if (l.x != p2.x) l.x += 0.01;
		if (l.y != p2.y) l.y += 0.01;
	}
}

void draw_primitives(SDL_Renderer* ren, QUADRANGLE q, ELIPSE e, SHIFT_CS qs, SHIFT_CS es, SDL_Rect w)
{
	draw_line(ren, q.p1, q.p2, qs, e, es, w);
	draw_line(ren, q.p2, q.p3, qs, e, es, w);
	draw_line(ren, q.p3, q.p4, qs, e, es, w);
	draw_line(ren, q.p4, q.p1, qs, e, es, w);

	for (float t = -PI; t < PI; t += 0.01)
	{
		POINT p;
		p.x = e.a * cos(t) + es.x;
		p.y = e.b * sin(t) + es.y;
		if(visibility(p, w))SDL_RenderDrawPoint(ren, WIDTH / 2 + p.x, HEIGHT / 2 - p.y);
	}
}

POINT get_point(POINT p, float scale, int corner, int shift_x, int shift_y)
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

float sq(float a) {return a * a;}
