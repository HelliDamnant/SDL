#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159265
#define ERROR {printf("Error: %s\n", SDL_GetError());exit(1);}
#define OX(x) (WIDTH / 2 + x)
#define OY(y) (HEIGHT / 2 - y)
#define SQ(a) (a*a)

#define TITLE "0111101001100001011000110110100001111001011011110111010000100000011100000110110001111010"
#define DEFAULT_X 200
#define DEFAULT_Y 150
int WIDTH = 1120;
int HEIGHT = 630;
int CAMERA = 3000;

typedef struct {
	float x;
	float y;
	float z;
} POINT3D;

POINT3D CENTER = {0, 0, 0};

typedef struct {
	float x;
	float y;
} POINT2D;

typedef struct {
	POINT3D p1;
	POINT3D p2;
	POINT3D p3;
	POINT3D p4;
	POINT3D p5;
	POINT3D p6;
	POINT3D p7;
	POINT3D p8;
} CUBE;

CUBE get_cube(float, float, float, int);
void draw_cube(SDL_Renderer*, CUBE);
void draw_line(SDL_Renderer*, POINT2D, POINT2D);
void draw_quadrangle(SDL_Renderer*, POINT2D, POINT2D, POINT2D, POINT2D);
void draw_dotted_line(SDL_Renderer*, POINT2D, POINT2D);
void cube_rotate_x(CUBE*, int);
void cube_rotate_y(CUBE*, int);
void cube_rotate_z(CUBE*, int);

int main()
{
	system("clear");

	printf("Do you want to enter the data yourself? (y/n)\n");
	char c = getc(stdin);
	if (c == 'y' || c == 'Y') {
	} else if (c != 'n' || c != 'N') {
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

	CUBE cube = get_cube(-200, 200, 200, 400);
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
                    cube_rotate_z(&cube, 10);
                } else if (e.key.keysym.sym == SDLK_MINUS) {
                    cube_rotate_z(&cube, -10);
				} else if (e.key.keysym.sym == SDLK_UP) {
                    cube_rotate_y(&cube, 10);
				} else if (e.key.keysym.sym == SDLK_DOWN) {
                    cube_rotate_y(&cube, -10);
				} else if (e.key.keysym.sym == SDLK_LEFT) {
                    cube_rotate_x(&cube, -10);
				} else if (e.key.keysym.sym == SDLK_RIGHT) {
                    cube_rotate_x(&cube, 10);
				} else if (e.key.keysym.sym == SDLK_PERIOD) {
					CAMERA += 10;
				} else if (e.key.keysym.sym == SDLK_COMMA) {
					CAMERA -= 10;
				}
			}

		SDL_GetWindowSize(main_window, &WIDTH, &HEIGHT);

		SDL_SetRenderDrawColor(ren, 15, 15, 15, 255);
        SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);
		draw_cube(ren, cube);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_Quit();

	system("clear");
	return 0;
}

POINT2D convert(POINT3D q)
{
	POINT2D p;
	p.x = (q.x * CAMERA) / (q.z + CAMERA);
	p.y = (q.y * CAMERA) / (q.z + CAMERA);
	return p;
}

float line_length(POINT3D p1, POINT3D p2)
{
	return sqrt(SQ(p2.x - p1.x) + SQ(p2.y - p1.y) + SQ(p2.z - p1.z));
}

void draw_cube(SDL_Renderer *ren, CUBE cube)
{
	POINT2D cbp[8];
	cbp[0] = convert(cube.p1);
	cbp[1] = convert(cube.p2);
	cbp[2] = convert(cube.p3);
	cbp[3] = convert(cube.p4);
	cbp[4] = convert(cube.p5);
	cbp[5] = convert(cube.p6);
	cbp[6] = convert(cube.p7);
	cbp[7] = convert(cube.p8);

	POINT3D cb[8];
	cb[0] = cube.p1;
	cb[1] = cube.p2;
	cb[2] = cube.p3;
	cb[3] = cube.p4;
	cb[4] = cube.p5;
	cb[5] = cube.p6;
	cb[6] = cube.p7;
	cb[7] = cube.p8;

	int max = 0;
	for (int i = 1; i < 8; i++) if (line_length(cb[max], CENTER) < line_length(cb[i], CENTER)) max = i;

	if ()

	switch (max)
	{
		case 0: {
			draw_quadrangle(ren, cbp[0], cbp[1], cbp[2], cbp[3]);
			draw_quadrangle(ren, cbp[0], cbp[4], cbp[7], cbp[3]);
			draw_quadrangle(ren, cbp[0], cbp[4], cbp[5], cbp[1]);
			draw_dotted_line(ren, cbp[6], cbp[2]);
			draw_dotted_line(ren, cbp[6], cbp[5]);
			draw_dotted_line(ren, cbp[6], cbp[7]);
			break;
		} case 1: {
			draw_quadrangle(ren, cbp[1], cbp[0], cbp[4], cbp[5]);
			draw_quadrangle(ren, cbp[1], cbp[0], cbp[3], cbp[2]);
			draw_quadrangle(ren, cbp[1], cbp[5], cbp[6], cbp[2]);
			draw_dotted_line(ren, cbp[7], cbp[3]);
			draw_dotted_line(ren, cbp[7], cbp[4]);
			draw_dotted_line(ren, cbp[7], cbp[6]);
			break;
		} case 2: {
			draw_quadrangle(ren, cbp[2], cbp[3], cbp[0], cbp[1]);
			draw_quadrangle(ren, cbp[2], cbp[1], cbp[5], cbp[6]);
			draw_quadrangle(ren, cbp[2], cbp[3], cbp[7], cbp[6]);
			draw_dotted_line(ren, cbp[4], cbp[0]);
			draw_dotted_line(ren, cbp[4], cbp[7]);
			draw_dotted_line(ren, cbp[4], cbp[5]);
			break;
		} case 3: {
			draw_quadrangle(ren, cbp[3], cbp[2], cbp[6], cbp[7]);
			draw_quadrangle(ren, cbp[3], cbp[0], cbp[1], cbp[2]);
			draw_quadrangle(ren, cbp[3], cbp[0], cbp[4], cbp[7]);
			draw_dotted_line(ren, cbp[5], cbp[1]);
			draw_dotted_line(ren, cbp[5], cbp[4]);
			draw_dotted_line(ren, cbp[5], cbp[6]);
			break;
		} case 4: {
			draw_quadrangle(ren, cbp[4], cbp[0], cbp[1], cbp[5]);
			draw_quadrangle(ren, cbp[4], cbp[5], cbp[6], cbp[7]);
			draw_quadrangle(ren, cbp[4], cbp[7], cbp[3], cbp[0]);
			draw_dotted_line(ren, cbp[2], cbp[1]);
			draw_dotted_line(ren, cbp[2], cbp[3]);
			draw_dotted_line(ren, cbp[2], cbp[6]);
			break;
		} case 5: {
			draw_quadrangle(ren, cbp[5], cbp[1], cbp[2], cbp[6]);
			draw_quadrangle(ren, cbp[5], cbp[4], cbp[7], cbp[6]);
			draw_quadrangle(ren, cbp[5], cbp[1], cbp[0], cbp[4]);
			draw_dotted_line(ren, cbp[3], cbp[0]);
			draw_dotted_line(ren, cbp[3], cbp[2]);
			draw_dotted_line(ren, cbp[3], cbp[7]);
			break;
		} case 6: {
			draw_quadrangle(ren, cbp[6], cbp[5], cbp[4], cbp[7]);
			draw_quadrangle(ren, cbp[6], cbp[7], cbp[3], cbp[2]);
			draw_quadrangle(ren, cbp[6], cbp[5], cbp[1], cbp[2]);
			draw_dotted_line(ren, cbp[0], cbp[1]);
			draw_dotted_line(ren, cbp[0], cbp[3]);
			draw_dotted_line(ren, cbp[0], cbp[4]);
			break;
		} case 7: {
			draw_quadrangle(ren, cbp[7], cbp[3], cbp[0], cbp[4]);
			draw_quadrangle(ren, cbp[7], cbp[6], cbp[5], cbp[4]);
			draw_quadrangle(ren, cbp[7], cbp[3], cbp[2], cbp[6]);
			draw_dotted_line(ren, cbp[1], cbp[0]);
			draw_dotted_line(ren, cbp[1], cbp[2]);
			draw_dotted_line(ren, cbp[1], cbp[5]);
			break;
		}
	}
}

void draw_quadrangle(SDL_Renderer *ren, POINT2D p1, POINT2D p2, POINT2D p3, POINT2D p4)
{
	SDL_RenderDrawLine(ren, OX(p1.x), OY(p1.y), OX(p2.x), OY(p2.y));
	SDL_RenderDrawLine(ren, OX(p2.x), OY(p2.y), OX(p3.x), OY(p3.y));
	SDL_RenderDrawLine(ren, OX(p3.x), OY(p3.y), OX(p4.x), OY(p4.y));
	SDL_RenderDrawLine(ren, OX(p4.x), OY(p4.y), OX(p1.x), OY(p1.y));
}

void draw_dotted_line(SDL_Renderer *ren, POINT2D p1, POINT2D p2)
{
	SDL_SetRenderDrawColor(ren, 69, 139, 69, 255);
	SDL_RenderDrawLine(ren, OX(p1.x), OY(p1.y), OX(p2.x), OY(p2.y));
	SDL_SetRenderDrawColor(ren, 255, 139, 244, 255);
}

CUBE get_cube(float x, float y, float z, int side)
{
	CUBE cb;
	cb.p1.x = cb.p5.x = cb.p4.x = cb.p8.x = x;
	cb.p2.x = cb.p6.x = cb.p3.x = cb.p7.x = x + side;
	cb.p1.y = cb.p5.y = cb.p2.y = cb.p6.y = y;
	cb.p3.y = cb.p7.y = cb.p4.y = cb.p8.y = y - side;
	cb.p1.z = cb.p2.z = cb.p3.z = cb.p4.z = z;
	cb.p5.z = cb.p6.z = cb.p7.z = cb.p8.z = z + side;
	return cb;
}

void cube_rotate_x(CUBE *cb, int t)
{
	float cost = cos(t*PI/180), sint = sin(t*PI/180),
		  yt = cb->p1.y, zt = cb->p1.z;
	cb->p1.y = yt*cost + zt*sint;
	cb->p1.z = zt*cost - yt*sint;

	yt = cb->p2.y; zt = cb->p2.z;
	cb->p2.y = yt*cost + zt*sint;
	cb->p2.z = zt*cost - yt*sint;

	yt = cb->p3.y; zt = cb->p3.z;
	cb->p3.y = yt*cost + zt*sint;
	cb->p3.z = zt*cost - yt*sint;

	yt = cb->p4.y; zt = cb->p4.z;
	cb->p4.y = yt*cost + zt*sint;
	cb->p4.z = zt*cost - yt*sint;

	yt = cb->p5.y; zt = cb->p5.z;
	cb->p5.y = yt*cost + zt*sint;
	cb->p5.z = zt*cost - yt*sint;

	yt = cb->p6.y; zt = cb->p6.z;
	cb->p6.y = yt*cost + zt*sint;
	cb->p6.z = zt*cost - yt*sint;

	yt = cb->p7.y; zt = cb->p7.z;
	cb->p7.y = yt*cost + zt*sint;
	cb->p7.z = zt*cost - yt*sint;

	yt = cb->p8.y; zt = cb->p8.z;
	cb->p8.y = yt*cost + zt*sint;
	cb->p8.z = zt*cost - yt*sint;
}

void cube_rotate_y(CUBE *cb, int t)
{
	float cost = cos(t*PI/180), sint = sin(t*PI/180),
		  xt = cb->p1.x, zt = cb->p1.z;
	cb->p1.x = xt*cost + zt*sint;
	cb->p1.z = zt*cost - xt*sint;

	xt = cb->p2.x; zt = cb->p2.z;
	cb->p2.x = xt*cost + zt*sint;
	cb->p2.z = zt*cost - xt*sint;

	xt = cb->p3.x; zt = cb->p3.z;
	cb->p3.x = xt*cost + zt*sint;
	cb->p3.z = zt*cost - xt*sint;

	xt = cb->p4.x; zt = cb->p4.z;
	cb->p4.x = xt*cost + zt*sint;
	cb->p4.z = zt*cost - xt*sint;

	xt = cb->p5.x; zt = cb->p5.z;
	cb->p5.x = xt*cost + zt*sint;
	cb->p5.z = zt*cost - xt*sint;

	xt = cb->p6.x; zt = cb->p6.z;
	cb->p6.x = xt*cost + zt*sint;
	cb->p6.z = zt*cost - xt*sint;

	xt = cb->p7.x; zt = cb->p7.z;
	cb->p7.x = xt*cost + zt*sint;
	cb->p7.z = zt*cost - xt*sint;

	xt = cb->p8.x; zt = cb->p8.z;
	cb->p8.x = xt*cost + zt*sint;
	cb->p8.z = zt*cost - xt*sint;
}

void cube_rotate_z(CUBE *cb, int t)
{
	float cost = cos(t*PI/180), sint = sin(t*PI/180),
		  xt = cb->p1.x, yt = cb->p1.y;
	cb->p1.x = xt*cost + yt*sint;
	cb->p1.y = yt*cost - xt*sint;

	xt = cb->p2.x; yt = cb->p2.y;
	cb->p2.x = xt*cost + yt*sint;
	cb->p2.y = yt*cost - xt*sint;

	xt = cb->p3.x; yt = cb->p3.y;
	cb->p3.x = xt*cost + yt*sint;
	cb->p3.y = yt*cost - xt*sint;

	xt = cb->p4.x; yt = cb->p4.y;
	cb->p4.x = xt*cost + yt*sint;
	cb->p4.y = yt*cost - xt*sint;

	xt = cb->p5.x; yt = cb->p5.y;
	cb->p5.x = xt*cost + yt*sint;
	cb->p5.y = yt*cost - xt*sint;

	xt = cb->p6.x; yt = cb->p6.y;
	cb->p6.x = xt*cost + yt*sint;
	cb->p6.y = yt*cost - xt*sint;

	xt = cb->p7.x; yt = cb->p7.y;
	cb->p7.x = xt*cost + yt*sint;
	cb->p7.y = yt*cost - xt*sint;

	xt = cb->p8.x; yt = cb->p8.y;
	cb->p8.x = xt*cost + yt*sint;
	cb->p8.y = yt*cost - xt*sint;
}
