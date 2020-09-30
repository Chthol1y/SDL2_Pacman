//version 0.3.4.b rewrite with SDL_Renderer
//created by chtholly 2019/12/17 09:24
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL_ttf.h>
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
struct monster_Rect
{
	SDL_Rect rect;
	SDL_Rect old_rect;
};
int init_Window(SDL_Window** gWindow, SDL_Surface** gScreenSurface, char s[],TTF_Font** font)		//窗口初始化
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
		}
		if (TTF_Init() == -1)
		{
			printf("SDL_TTF init failed\n");
			return ERROR;
		}
		else
		{
			*font = TTF_OpenFont("image/AGENCYR.ttf", 28);
			return OK;
		}
	}
}

void ttf_print(TTF_Font** font, SDL_Renderer** rend, char s[], SDL_Rect rect)
{
	SDL_Color color = { 255,0,0 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(*font, s, color);
	if (text_surface != NULL)
	{
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(*rend, text_surface);
		SDL_RenderCopy(*rend, text_texture, NULL, &rect);
	}
}

struct monster_Rect generate_monster(struct MAP map, SDL_Texture* monster, SDL_Texture* background, SDL_Renderer** rend, SDL_Rect rect_monster, SDL_Rect rect_pac)
{
	int dire;
	SDL_Rect rect = rect_monster;
	SDL_Rect old_rect = rect;
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	dire = rand() % 3;
	
	//pac在左
	if (rect_monster.x >= rect_pac.x)
	{
		//pac在左上
		if (rect_monster.y >= rect_pac.y)
		{
			//monster往上走
			if (dire >= 1)
			{
				if (map.m[(rect.y - 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y -= 30;
					old_rect.y = rect.y + 30;
				}
			}
			//monster往左走
			else
			{
				if (map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
				{
					rect.x -= 30;
					old_rect.x = rect.x + 30;
				}
			}
		}
		//pac在左
		else if (rect_monster.y == rect_pac.y)
		{
			//monster往左
			if (map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
			{
				rect.x -= 30;
				old_rect.x = rect.x + 30;
			}
		}
		//pac在左下
		else
		{
			//monster往下走
			if (dire >= 1)
			{
				if (map.m[(rect.y + 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y += 30;
					old_rect.y = rect.y - 30;
				}
			}
			//monster往左走
			else
			{
				if (map.m[(rect.y - 300) / 30][(rect.x - 30 - 30) / 30] != 1)
				{
					rect.x -= 30;
					old_rect.x = rect.x + 30;
				}
			}
		}
	}
	//pac在右
	else
	{
		//pac在右上
		if (rect_monster.y >= rect_pac.y)
		{
			//monster往上走
			if (dire >= 1)
			{
				if (map.m[(rect.y - 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y -= 30;
					old_rect.y = rect.y + 30;
				}
			}
			//monster往右走
			else
			{
				if (map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
				{
					rect.x += 30;
					old_rect.x = rect.x - 30;
				}
			}
		}
		//pac在右
		else if (rect_monster.y == rect_pac.y)
		{
			//monster往右
			if (map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
			{
				rect.x += 30;
				old_rect.x = rect.x - 30;
			}
		}
		//pac在右下
		else
		{
			//monster往右走
			if (dire >= 1)
			{
				if (map.m[(rect.y - 300) / 30][(rect.x + 30 - 30) / 30] != 1)
				{
					rect.x += 30;
					old_rect.x = rect.x - 30;
				}
			}
			//monster往下走
			else
			{
				if (map.m[(rect.y + 30 - 300) / 30][(rect.x - 30) / 30] != 1)
				{
					rect.y += 30;
					old_rect.y = rect.y - 30;
				}
			}
		}
	}
	if (map.m[(rect.y - 300) / 30][(rect.x - 30) / 30] == 0)
	{
		SDL_RenderCopy(*rend, food, NULL, &old_rect);
	}
	else
	{
		SDL_RenderCopy(*rend, background, NULL, &old_rect);
	}
	
	SDL_RenderCopy(*rend, monster, NULL, &rect);

	struct monster_Rect monster_Rect;
	monster_Rect.rect = rect;
	monster_Rect.old_rect = old_rect;

	return monster_Rect;
}

void move_control(SDL_Window** gWindow, SDL_Renderer** rend,TTF_Font** font)		//移动控制
{
	int point = 0;
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
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Surface* point_bk_img = IMG_Load("image/point.png");
	//Texture转换
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
	SDL_Texture* background = SDL_CreateTextureFromSurface(*rend, bk_img);
	SDL_Texture* monster_1 = SDL_CreateTextureFromSurface(*rend, monster_1_img);
	SDL_Texture* monster_2 = SDL_CreateTextureFromSurface(*rend, monster_2_img);
	SDL_Texture* monster_3 = SDL_CreateTextureFromSurface(*rend, monster_3_img);
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	SDL_Texture* point_bk = SDL_CreateTextureFromSurface(*rend, point_bk_img);
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
	struct monster_Rect monster_Rect_1;
	struct monster_Rect monster_Rect_2;
	struct monster_Rect monster_Rect_3;
	monster_Rect_1.rect.x = map.monster[0].x;
	monster_Rect_1.rect.y = map.monster[0].y;
	monster_Rect_1.rect.h = 30;
	monster_Rect_1.rect.w = 30;

	monster_Rect_2.rect.x = map.monster[1].x;
	monster_Rect_2.rect.y = map.monster[1].y;
	monster_Rect_2.rect.h = 30;
	monster_Rect_2.rect.w = 30;

	monster_Rect_3.rect.x = map.monster[2].x;
	monster_Rect_3.rect.y = map.monster[2].y;
	monster_Rect_3.rect.h = 30;
	monster_Rect_3.rect.w = 30;
	//整体界面初次渲染
	SDL_SetRenderDrawColor(*rend, 25, 25, 112, 255);		//使用RGBA填充背景色，透明度为255
	SDL_RenderClear(*rend);
	//地图创建
	draw_map(gWindow, rend, map);
	SDL_RenderPresent(*rend);
	SDL_Rect rect_P,rect_N,rect_point;
	rect_P.x = 50;
	rect_P.y = 50;
	rect_P.h = 50;
	rect_P.w = 100;

	rect_N.x = 300;
	rect_N.y = 50;
	rect_N.h = 50;
	rect_N.w = 150;

	rect_point.x = 60;
	rect_point.y = 100;
	rect_point.h = 50;
	rect_point.w = 50;
	ttf_print(font, rend, "Point", rect_P);
	ttf_print(font, rend, "Pac-man", rect_N);
	
	SDL_RenderCopy(*rend, currentFace, NULL, &rect);
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
		monster_Rect_1 = generate_monster(map, monster_1, background, rend, monster_Rect_1.rect, rect);
		monster_Rect_2 = generate_monster(map, monster_2, background, rend, monster_Rect_2.rect, rect);
		monster_Rect_3 = generate_monster(map, monster_3, background, rend, monster_Rect_3.rect, rect);
		if (map.m[(rect.y - 300) / 30][(rect.x - 30) / 30] == 0)
		{
			point++;
			map.m[(rect.y - 300) / 30][(rect.x - 30) / 30] = 2;
		}
		char s[5];
		itoa(point, s, 10);
		if (point >= 10)
		{
			rect_point.w = 100;
		}
		SDL_RenderCopy(*rend, point_bk, NULL, &rect_point);
		ttf_print(font, rend, s, rect_point);

		if ((monster_Rect_1.rect.x == rect.x && monster_Rect_1.rect.y == rect.y) || (monster_Rect_2.rect.x == rect.x && monster_Rect_2.rect.y == rect.y) || (monster_Rect_3.rect.x == rect.x && monster_Rect_3.rect.y == rect.y))
		{
			printf("END\n");
			SDL_RenderClear(*rend);
			SDL_Rect over;
			over.x = 130;
			over.y = 300;
			over.h = 50;
			over.w = 250;
			ttf_print(font, rend, "Game Over", over);

			SDL_RenderPresent(*rend);
			SDL_Delay(5000);
			quit = true;
		}

		printf("%d\n", point);
		SDL_Delay(420);
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
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
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
				SDL_RenderCopy(*rend, food, NULL, &rect);	
			}
		}
	}
}
int main(int argc, char* args[])
{

	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* rend;
	TTF_Font* font;
	char s[] = "Pac man test version v0.3.4.b 17/12";
	//窗口创建过程---Window
	init_Window(&gWindow, &gScreenSurface, s, &font);
	//渲染界面生成---Renderer
	rend = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	//操作控制（采用死循环）
	move_control(&gWindow, &rend, &font);

	SDL_DestroyRenderer(rend);
	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	return 0;
}