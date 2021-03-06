//version 0.3 rewrite with SDL_Renderer
//created by chtholly 2019/12/9 20:08

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800
#define OK 1
#define ERROR 0

struct Point
{
	int x;
	int y;
};
struct MAP
{
	int m[25][25];
	struct Point point;
};

int init_Window(SDL_Window** gWindow, SDL_Surface** gScreenSurface, char s[])		//窗口初始化
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

void move_control(SDL_Window** gWindow, bool quit, SDL_Renderer** rend)		//移动控制
{
	SDL_Event event;
	
	//图像提取
	SDL_Surface* face_up_img = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down_img = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left_img = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right_img = IMG_Load("image/pacmanRight.png");
	//Texture转换
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
	//初始pacman图像
	SDL_Texture* currentFace = face_left;
	//坐标
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = 30;
	rect.w = 30;
	//整体界面初次渲染
	//SDL_SetRenderDrawColor(*rend, 25, 25, 112, 255);		//使用RGBA填充背景色，透明度为255
	//SDL_RenderClear(*rend);
	//SDL_RenderCopy(*rend, currentFace, NULL, &rect);
	//SDL_Delay(5);
	//SDL_RenderPresent(*rend);
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
		SDL_Delay(10);
		SDL_RenderPresent(*rend);
	}
}

struct MAP read_file ()	//文件读取
{
	int N = 25;
	int i = 0, j = 0;
	int x;
	int flag = 1;
	int pr, pc, mr, mc;	//（pr,pc)为pacman初始位置，（mr,mc）为怪物初始位置 （!现阶段暂只设置一个怪物）
	struct MAP map;
	FILE* fp;
	if ((fp = fopen("image/scene.txt","r")) == NULL)
	{
		//后续版本修改成显示在Window界面
		printf("Map load failed");
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
					map.m[i][j] = x;
					j++;
					if (j == N)
					{
						i++;
						j = 0;
					}
				}
			}
		}
	}
	fclose(fp);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			printf("%d ", map.m[i][j]);
		printf("\n");
	}
	//文件读取完毕
	return map;
}

int draw_map(SDL_Window** gWindow, SDL_Renderer** rend, struct MAP map)
{
	int N = 25;
	SDL_Rect rect;
	int i = 0, j = 0;
	rect.w = 18;
	rect.h = 18;
	//背景
	SDL_SetRenderDrawColor(*rend, 25, 25, 112, 255);		//使用RGBA填充背景色，透明度为255
	SDL_RenderClear(*rend);
	SDL_RenderPresent(*rend);

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			rect.x = i * 18 + 25;
			rect.y = j * 18 + 300;
			if (map.m[i][j] == 1)
			{
				//SDL_SetRenderDrawColor(*rend, 100, 149, 237, 255);
				SDL_RenderFillRect(&rend, &rect);
				//SDL_SetRenderDrawColor(*rend, 245, 245, 245, 255);
				//SDL_RenderDrawRect(&rend, &rect);

				//SDL_RenderClear(*rend);
				
				SDL_RenderPresent(*rend);
			}
		}
	}
}
int main(int argc, char* args[])
{
	struct MAP map;
	bool quit = false;
	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* rend;
	char s[] = "Pac man test version v0.3 9/12";

	//窗口创建过程---Window
	init_Window(&gWindow, &gScreenSurface, s);
	//渲染界面生成---Renderer
	rend = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);

	//surface基础界面生成(已经没软用)
	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x6A, 0x5A, 0xCD));
	//SDL_UpdateWindowSurface(gWindow);
	map = read_file();
	draw_map(&gWindow, &rend, map);

	
	//操作控制（采用死循环）
	move_control(&gWindow, quit, &rend);


	SDL_Delay(4000);
	SDL_DestroyRenderer(rend);
	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	
	


	return 0;
}
