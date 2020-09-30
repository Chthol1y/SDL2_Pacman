//version 0.2 rewrite with SDL_Renderer
//created by chtholly 2019/12/8 00:50

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800
#define OK 1
#define ERROR 0

int init_Window(SDL_Window** gWindow, SDL_Surface** gScreenSurface, char s[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return ERROR;
	}
	else
	{
		//��������
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

void move_control(SDL_Window** gWindow, bool quit, SDL_Renderer** rend)
{
	SDL_Event event;
	
	//ͼ����ȡ
	SDL_Surface* face_up_img = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down_img = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left_img = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right_img = IMG_Load("image/pacmanRight.png");
	//Textureת��
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
	//��ʼpacmanͼ��
	SDL_Texture* currentFace = face_left;
	//����
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = 30;
	rect.w = 30;
	//������������Ⱦ
	SDL_SetRenderDrawColor(*rend, 25, 25, 112, 1);		//ʹ��RGBA��䱳��ɫ��͸����Ϊ1
	SDL_RenderClear(*rend);
	SDL_RenderCopy(*rend, currentFace, NULL, &rect);
	SDL_Delay(5);
	SDL_RenderPresent(*rend);
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
				SDL_RenderClear(*rend);
				SDL_RenderCopy(*rend, currentFace, NULL, &rect);
			}
		}
		SDL_Delay(5);
		SDL_RenderPresent(*rend);
	}
}

int read_file()
{
	int x;
	int flag = 1;
	int pr, pc, mr, mc;	//��pr,pc)Ϊpacman��ʼλ�ã���mr,mc��Ϊ�����ʼλ�� ��!�ֽ׶���ֻ����һ�����
	int map[42][42];
	FILE* fp;
	if ((fp = fopen("image/scene.txt","rt")) == NULL)
	{
		//�����汾�޸ĳ���ʾ��Window����
		printf("Map load failed");
		return ERROR;
	}
	else
	{
		while (fscanf(fp, "%d", &x) != EOF)
		{
			if (flag == 1)
			{
				pr = x;
				flag++;
			}
			else if (flag == 2)
			{
				pc = x;
				flag++;
			}
			else if (flag == 3)
			{
				mr = x;
				flag++;
			}
			else if (flag == 4)
			{
				mc = x;
				flag++;
			}
			else
			{
				if (x != " ")
				{

				}
			}
		}
	}

}

int main(int argc, char* args[])
{
	bool quit = false;
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* rend;
	char s[] = "Pac man test version v0.2 7/12";

	//���ڴ�������---Window
	init_Window(&gWindow, &gScreenSurface, s);
	//��Ⱦ��������---Renderer
	rend = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);

	//surface������������(�Ѿ�û����)
	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x6A, 0x5A, 0xCD));
	//SDL_UpdateWindowSurface(gWindow);

	//�������ƣ�������ѭ����
	move_control(&gWindow, quit, &rend);


	SDL_Delay(4000);

	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	
	


	return 0;
}
