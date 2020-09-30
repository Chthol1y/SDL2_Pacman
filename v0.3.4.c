//version 0.3.1 rewrite with SDL_Renderer
//created by chtholly 2019/12/10 02:45
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#define SCREEN_WIDTH 510
#define SCREEN_HEIGHT 800
#define OK 1
#define ERROR 0
#define N 15

struct Point
{
	int x;
	int y;
};
struct MAP
{
	int m[15][15];
	struct Point pac;
	struct Point monster[3];
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

SDL_Rect generate_monster(struct MAP map, SDL_Texture* monster, SDL_Texture* background, SDL_Renderer** rend, SDL_Rect rect_m, int i)
{
	int dire;
	SDL_Rect rect = rect_m;
	SDL_Rect old_rect = rect;
	dire = rand() % 5;
	//rect.x = map.monster[i].x;
	//rect.y = map.monster[i].y;
	//rect.h = 30;
	//rect.w = 30;
	//old_rect = rect;
	//定义0-1为上，1-2为下，2-3为左，3-4为右
	if (dire <= 1 && map.m[(rect.y - 30 - 300) / 30][(rect.x - 30) / 30] != 1)
	{
		rect.y -= 30;
		old_rect.y = rect.y + 30;
	}
	else if (dire <= 2 && map.m[(rect.y + 30 - 300) / 30][(rect.x - 30) / 30] != 1)
	{
		rect.y += 30;
		old_rect.y = rect.y - 30;
	}
	else if (dire <= 3 && map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
	{
		rect.x -= 30;
		old_rect.x = rect.x + 30;
	}
	else if (dire <= 4 && map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
	{
		rect.x += 30;
		old_rect.x = rect.x - 30;
	}
	SDL_RenderCopy(*rend, background, NULL, &old_rect);
	SDL_RenderCopy(*rend, monster, NULL, &rect);
	return rect;
}

void move_control(SDL_Window** gWindow, SDL_Renderer** rend)		//移动控制
{
	bool quit = false;
	void draw_map(SDL_Window * *gWindow, SDL_Renderer * *rend, struct MAP map);
	struct MAP read_file();
	SDL_Event event;
	//图像提取
	SDL_Surface* face_up_img = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down_img = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left_img = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right_img = IMG_Load("image/pacmanRight.png");
	SDL_Surface* bk_img = IMG_Load("image/background.png");
	SDL_Surface* monster_1_img = IMG_Load("image/red.png");
	SDL_Surface* monster_2_img = IMG_Load("image/yellow.png");
	SDL_Surface* monster_3_img = IMG_Load("image/blue.png");

	//Texture转换
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
	SDL_Texture* background = SDL_CreateTextureFromSurface(*rend, bk_img);
	SDL_Texture* monster_1 = SDL_CreateTextureFromSurface(*rend, monster_1_img);
	SDL_Texture* monster_2 = SDL_CreateTextureFromSurface(*rend, monster_2_img);
	SDL_Texture* monster_3 = SDL_CreateTextureFromSurface(*rend, monster_3_img);
	//初始pacman图像
	SDL_Texture* currentFace = face_right;
	//地图读取
	struct MAP map;
	map = read_file();
	//Pacman坐标
	SDL_Rect rect;
	rect.x = map.pac.x;
	rect.y = map.pac.y;
	rect.h = 30;
	rect.w = 30;
	SDL_Rect old_rect = rect;
	//Monster1/2/3 坐标
	SDL_Rect rect_m1;
	SDL_Rect rect_m2;
	SDL_Rect rect_m3;
	rect_m1.x = map.monster[1].x;
	rect_m1.y = map.monster[1].y;
	rect_m1.h = 30;
	rect_m1.w = 30;
	//整体界面初次渲染
	SDL_SetRenderDrawColor(*rend, 25, 25, 112, 255);		//使用RGBA填充背景色，透明度为255
	SDL_RenderClear(*rend);
	//地图创建
	draw_map(gWindow, rend, map);
	SDL_RenderCopy(*rend, currentFace, NULL, &rect);
	//SDL_Delay(5);
	//SDL_RenderPresent(*rend);
	while (quit == false)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			old_rect = rect;
			if (event.key.keysym.sym == SDLK_UP)
			{
				currentFace = face_up;
				if (map.m[(rect.y - 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y -= 30;
					old_rect.y = rect.y + 30;
				}
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				currentFace = face_down;
				if (map.m[(rect.y + 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y += 30;
					old_rect.y = rect.y - 30;
				}
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				currentFace = face_left;
				if (map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
				{
					rect.x -= 30;
					old_rect.x = rect.x + 30;
				}
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				currentFace = face_right;
				if (map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
				{
					rect.x += 30;
					old_rect.x = rect.x - 30;
				}
			}
			SDL_RenderCopy(*rend, background, NULL, &old_rect);
			SDL_RenderCopy(*rend, currentFace, NULL, &rect);
		}
		else
		{
			if (currentFace == face_up)
			{
				if (map.m[(rect.y - 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y -= 30;
					old_rect.y = rect.y + 30;
				}
			}
			else if (currentFace == face_down)
			{
				if (map.m[(rect.y + 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y += 30;
					old_rect.y = rect.y - 30;
				}
			}
			else if (currentFace == face_left)
			{
				if (map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
				{
					rect.x -= 30;
					old_rect.x = rect.x + 30;
				}
			}
			else if (currentFace == face_right)
			{
				if (map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
				{
					rect.x += 30;
					old_rect.x = rect.x - 30;
				}
			}
			SDL_RenderCopy(*rend, background, NULL, &old_rect);
			SDL_RenderCopy(*rend, currentFace, NULL, &rect);
		}
		printf("A%d %d\n", rect.x, rect.y);
		printf("B%d %d\n", old_rect.x, old_rect.y);
		//进行怪物-Pacman位置重叠检测
		rect_m1 = generate_monster(map, monster_1, background, rend, rect_m1, 1);
		SDL_Delay(330);
		SDL_RenderPresent(*rend);		//渲染当前surface，显示出所有画面
	}
}

struct MAP read_file()	//文件读取
{

	int i = 0, j = 0;
	int k = 0;
	int x;
	int flag = 1;
	struct MAP map;
	FILE* fp;
	if ((fp = fopen("image/scene.txt", "r")) == NULL)
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
				map.pac.x = x;
				flag++;
			}
			else if (flag == 2)
			{
				map.pac.y = x;
				flag++;
			}
			else if (flag > 2 && k < 3)
			{
				if (flag % 2 != 0)
				{
					map.monster[k].x = x;
					flag++;
				}
				else
				{
					map.monster[k].y = x;
					flag++;
					k++;
				}
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
		fclose(fp);
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			printf("%d ", map.m[i][j]);
		printf("\n");
	}
	for (i = 0; i < 3; i++)
	{
		printf("%d %d", map.monster[i].x, map.monster[i].y);
		printf("\n");
	}
	//文件读取完毕
	return map;
}

void draw_map(SDL_Window** gWindow, SDL_Renderer** rend, struct MAP map)
{
	SDL_Rect rect;
	int i = 0, j = 0;
	rect.w = 30;
	rect.h = 30;
	//背景
	SDL_SetRenderDrawColor(*rend, 25, 25, 112, 255);		//使用RGBA填充背景色，透明度为255
	SDL_RenderClear(*rend);
	SDL_RenderPresent(*rend);
	SDL_Surface* wall_img = IMG_Load("image/wall.png");
	SDL_Texture* wall = SDL_CreateTextureFromSurface(*rend, wall_img);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			rect.x = j * 30 + 30;
			rect.y = i * 30 + 300;
			if (map.m[i][j] == 1)
			{
				SDL_RenderCopy(*rend, wall, NULL, &rect);
			}
			else if (map.m[i][j] == 0)
			{
				SDL_SetRenderDrawColor(*rend, 255, 255, 255, 255);
				SDL_RenderFillRect(*rend, &rect);
				SDL_RenderDrawRect(*rend, &rect);
			}
		}
	}
}
int main(int argc, char* args[])
{

	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* rend;
	char s[] = "Pac man test version v0.3. 12/12";
	//窗口创建过程---Window
	init_Window(&gWindow, &gScreenSurface, s);
	//渲染界面生成---Renderer
	rend = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	//surface基础界面生成(已经没软用)
	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x6A, 0x5A, 0xCD));
	//SDL_UpdateWindowSurface(gWindow);
	//操作控制（采用死循环）
	move_control(&gWindow, &rend);



	SDL_DestroyRenderer(rend);
	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();




	return 0;
}