#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>

#define OK 1
#define ERROR 0

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



int init_Window(SDL_Window** gWindow, SDL_Surface** gScreenSurface, char s[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return ERROR;
	}
	else
	{
		//创建窗口
		*gWindow = SDL_CreateWindow(s, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return ERROR;
		}
		else
		{
			*gScreenSurface = SDL_GetWindowSurface(*gWindow);
			return OK;
		}
	}
}

void move_control(SDL_Window** gWindow, SDL_Surface** gScreenSurface, bool quit)
{
	SDL_Event event;
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	//图像提取
	SDL_Surface* face_up = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right = IMG_Load("image/pacmanRight.png");
	//当前pacman图像
	SDL_Surface* currentFace = face_left;
	SDL_BlitSurface(currentFace, NULL, *gScreenSurface, NULL);
	while (quit == false)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_UP)
				{
					currentFace = face_up;
					rect.y -= 10;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					currentFace = face_down;
					rect.y += 10;
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					currentFace = face_left;
					rect.x -= 10;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					currentFace = face_right;
					rect.x += 10;
				}
				SDL_FillRect(*gScreenSurface, NULL, SDL_MapRGB((*gScreenSurface)->format, 0x6A, 0x5A, 0xCD));
				SDL_BlitSurface(currentFace, NULL, *gScreenSurface, &rect);
			}
		}
		SDL_UpdateWindowSurface(*gWindow);
	}
}

int read_file()
{

}

int main(int argc, char* args[])
{
	bool quit = false;
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	char s[] = "Pac man test version v0.1 7/12";

	//窗口创建过程
	init_Window(&gWindow, &gScreenSurface, s);
	SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x6A, 0x5A, 0xCD));
	SDL_UpdateWindowSurface(gWindow);

	//操作控制（是用死循环）
	move_control(&gWindow, &gScreenSurface, quit);


	SDL_Delay(4000);

	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	
	


	return 0;
}
