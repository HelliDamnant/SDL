#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159265
#define ERROR {printf("Error: %s\n", SDL_GetError());exit(1);}
#define OX(x) (WIDTH / 2 + x)
#define OY(y) (HEIGHT / 2 - y)

#define TITLE "0111101001100001011000110110100001111001011011110111010000100000011100000110110001111010"
#define DEFAULT_X 200
#define DEFAULT_Y 150
int WIDTH = 1120;
int HEIGHT = 630;

typedef struct {
	float x;
	float y;
	float z;
} POINT;

typedef struct {
	POINT p1;
	POINT p2;
	POINT p3;
	POINT p4;
	POINT p5;
	POINT p6;
	POINT p7;
	POINT p8;
} CUBE;

typedef struct {
	int distance;
	int corner_x;
	int corner_y;
	int corner_z;
	int shift_x;
	int shift_y;
	int shift_z;
} CONDITION;

float sq(float);
void draw_line(SDL_Renderer*, POINT, POINT, CONDITION);
float vector_length(POINT, POINT);
float _point(float, float, float);

int main()
{
	system("clear");

	printf("Do you want to enter the data yourself? (y/n)\n");
	char c = getc(stdin);
	CONDITION condition;
	if (c == 'y' || c == 'Y') {

	} else if (c != 'n' || c != 'N') {
		condition.distance = 1;
		condition.corner_x = 0;
		condition.corner_y = 50;
		condition.corner_z = 0;
		condition.shift_x = 0;
		condition.shift_y = 0;
		condition.shift_z = 0;
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
                    condition.distance += 1;
                } else if (e.key.keysym.sym == SDLK_MINUS) {
                    condition.distance -= 1;
				} else if (e.key.keysym.sym == SDLK_UP) {
					condition.shift_y += 10;
				} else if (e.key.keysym.sym == SDLK_DOWN) {
					condition.shift_y -= 10;
				} else if (e.key.keysym.sym == SDLK_LEFT) {
					condition.shift_x -= 10;
				} else if (e.key.keysym.sym == SDLK_RIGHT) {
					condition.shift_x += 10;
				}
			}

        SDL_SetRenderDrawColor(ren, 15, 15, 15, 255);
        SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);

		CUBE cube;
		cube.p1.x = -50;
		cube.p1.y = -50;
		cube.p1.z =   1;

		cube.p2.x = -50;
		cube.p2.y =  50;
		cube.p2.z =   1;

		cube.p3.x =  50;
		cube.p3.y =  50;
		cube.p3.z =   1;

		cube.p4.x =  50;
		cube.p4.y = -50;
		cube.p4.z =   1;

		cube.p5.x =  50;
		cube.p5.y = -50;
		cube.p5.z =  51;

		cube.p6.x =  50;
		cube.p6.y =  50;
		cube.p6.z =  51;

		cube.p7.x = -50;
		cube.p7.y =  50;
		cube.p7.z =  51;

		cube.p8.x = -50;
		cube.p8.y = -50;
		cube.p8.z =  51;

		draw_line(ren, cube.p1, cube.p2, condition);
		draw_line(ren, cube.p2, cube.p3, condition);
		draw_line(ren, cube.p3, cube.p4, condition);
		draw_line(ren, cube.p4, cube.p5, condition);
		draw_line(ren, cube.p5, cube.p6, condition);
		draw_line(ren, cube.p6, cube.p7, condition);
		draw_line(ren, cube.p7, cube.p8, condition);
		draw_line(ren, cube.p8, cube.p1, condition);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

	system("clear");
	return 0;
}



void draw_line(SDL_Renderer *ren, POINT p1, POINT p2, CONDITION cond)
{
	float len = vector_length(p1, p2);
	for (float t = 0; t <= len; t += 0.01)
	{
		float mu = t/len;
		POINT p;
		p.x = _point(mu, p1.x, p2.x) + cond.shift_x;
		p.y = _point(mu, p1.y, p2.y) + cond.shift_y;
		p.z = _point(mu, p1.z, p2.z) + cond.shift_z;
		SDL_RenderDrawPoint(ren, OX(cond.distance*p.x / p.z), OY(cond.distance*p.y / p.z));
	}
}

float vector_length(POINT p1, POINT p2) {return sqrt(sq(p2.x - p1.x) + sq(p2.y - p1.y) + sq(p2.z - p1.z));}
float _point(float mu, float t1, float t2) {return (1 - mu) * t1 + mu * t2;}
float sq(float a) {return a * a;}

// POINT get_point(POINT p, float condition.shift_z, int corner, int shift_x, int shift_y)
// {
// 	p.x = condition.shift_z * p.x;
// 	p.y = condition.shift_z * p.y;

// 	float xt = p.x,
// 		  yt = p.y,
// 		  cost = cos(corner * PI / 180),
// 		  sint = sin(corner * PI / 180);
// 	p.x = xt * cost + yt * sint;
// 	p.y = yt * cost - xt * sint;

// 	p.x += shift_x;
// 	p.y += shift_y;

// 	return p;
// }
