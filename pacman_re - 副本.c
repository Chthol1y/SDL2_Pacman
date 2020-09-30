//Final Version

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define OK 1
#define ERROR 0
#define N 15
#define UP 10
#define DOWN 20
#define LEFT 30
#define RIGHT 40

//坐标结构体
struct POS
{
	int x;
	int y;
};
//地图结构体，定义全局变量map
struct MAP
{
	int m[15][15];
	struct POS pac;
	struct POS monster[3];
}map;
//monster rect
struct monster_rect
{
	SDL_Rect rect;
	SDL_Rect old_rect;
}monster_rect[3];
//SDL整体初始化函数
int Init_Window(char s[], SDL_Window** window, SDL_Surface** surface, SDL_Renderer** rend, TTF_Font** font)
{
	//SDL初始化
	if (SDL_Init(SDL_INIT_VIDEO) < 0)	
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return ERROR;
	}
	else
	{
		//窗口初始化
		*window = SDL_CreateWindow(s, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return ERROR;
		}
		//窗口创建正常，进行Surface创建
		else
		{
			*surface = SDL_GetWindowSurface(*window);
		}
	}
	//渲染初始化
	*rend = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(*rend);
	//字体导入初始化
	if (TTF_Init() == -1)
	{
		printf("SDL_TTF init failed\n");
		return ERROR;
	}
	else
	{
		*font = TTF_OpenFont("image/AGENCYR.ttf", 48);
		return OK;
	}
}
//读取地图文件
void read_file()
{
	int i = 0, j = 0;
	int flag = 1;		//用于配合读取pacman与monster的坐标
	int val;
	int k = 0;
	FILE* fp;
	if ((fp = fopen("image/scene.txt", "r")) == NULL)
	{
		//后续版本修改成显示在Window界面
		printf("Map load failed");
	}
	else
	{
		while (fscanf(fp, "%d", &val) != EOF)
		{
			//读取pacman坐标
			if (flag == 1)
			{
				map.pac.x = val;
				flag++;
			}
			else if (flag == 2)
			{
				map.pac.y = val;
				flag++;
			}
			//读取三组monster坐标
			else if (flag > 2 && k < 3)
			{
				if (flag % 2 != 0)
				{
					map.monster[k].x = val;
					monster_rect[k].rect.x = val;
					monster_rect[k].old_rect.x = val;
					flag++;
				}
				else
				{
					map.monster[k].y = val;
					monster_rect[k].rect.y = val;
					monster_rect[k].old_rect.y = val;
					flag++;
					k++;
				}
			}
			//读取地图信息
			else
			{
				if (val != ' ')
				{
					map.m[i][j] = val;
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
	//控制台端输出读取到的地图信息供检验
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			printf("%d ", map.m[i][j]);
		printf("\n");
	}
	//控制台端输出monster坐标
	for (i = 0; i < 3; i++)
	{
		printf("%d %d", map.monster[i].x, map.monster[i].y);
		printf("\n");
	}
	//读取完毕
	printf("File reading complete\n");
}
//绘画地图
void draw_map(SDL_Renderer** rend, SDL_Window** window)
{
	int i = 0, j = 0;
	//素材载入与转换
	SDL_Surface* wall_img = IMG_Load("image/wall.png");
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* wall = SDL_CreateTextureFromSurface(*rend, wall_img);
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	//绘图坐标
	SDL_Rect rect;
	rect.w = 50;
	rect.h = 50;
	//
	for ( i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			rect.x = j * 50 + 25;
			rect.y = i * 50 + 25;
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
	SDL_RenderPresent(*rend);
}

void ttf_print(char s[], TTF_Font** font, SDL_Renderer** rend, SDL_Rect rect)
{
	SDL_Color color = { 255,255,255 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(*font, s, color);
	rect.h = text_surface->h;
	rect.w = text_surface->w;
	if (text_surface != NULL)
	{
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(*rend, text_surface);
		SDL_RenderCopy(*rend, text_texture, NULL, &rect);
	}
}

void draw_UI(SDL_Renderer** rend, SDL_Window** window, TTF_Font** font)
{
	//绘画地图
	draw_map(rend, window);
	char s[] = "Point";
	char a[] = "create";
	char b[] = "by";
	char c[] = "Chtholly";
	//文字显示
	SDL_Rect rect;
	rect.x = 840;
	rect.y = 175;
	ttf_print(s, font, rend, rect);
	rect.y = 275;
	ttf_print(a, font, rend, rect);
	rect.y += 60;
	ttf_print(b, font, rend, rect);
	rect.y += 60;
	ttf_print(c, font, rend, rect);
	SDL_RenderPresent(*rend);
}

void draw_Pac(SDL_Renderer** rend, SDL_Texture** face, SDL_Texture** bk, int dire)
{
	int wall_touch(int dire, SDL_Rect rect);
	SDL_Rect rect, old_rect;
	rect.x = map.pac.x;
	rect.y = map.pac.y;
	rect.w = 50;
	rect.h = 50;
	old_rect = rect;
	if (dire == UP)
	{
		if (wall_touch(dire, rect))
		{
			map.pac.y -= 50;
			rect.y -= 50;
		}
	}
	else if (dire == DOWN)
	{
		if (wall_touch(dire, rect))
		{
			map.pac.y += 50;
			rect.y += 50;
		}
	}
	else if (dire == LEFT)
	{
		if (wall_touch(dire, rect))
		{
			map.pac.x -= 50;
			rect.x -= 50;
		}
	}
	else if (dire == RIGHT)
	{
		if (wall_touch(dire, rect))
		{
			map.pac.x += 50;
			rect.x += 50;
		}
	}
	printf("A%d %d\n", rect.x, rect.y);
	printf("B%d %d\n", old_rect.x, old_rect.y);
	SDL_RenderCopy(*rend, *bk, NULL, &old_rect);
	SDL_RenderCopy(*rend, *face, NULL, &rect);
	SDL_RenderPresent(*rend);
}

int wall_touch(int dire,SDL_Rect rect)
{
	if (dire == UP && map.m[(rect.y - 50 - 25) / 50][(rect.x - 25) / 50] != 1)
	{
		return OK;
	}
	else if(dire == DOWN && map.m[(rect.y + 50 - 25) / 50][(rect.x - 25) / 50] != 1)
	{
		return OK;
	}
	else if (dire == LEFT && map.m[(rect.y - 25) / 50][(rect.x - 50 - 25) / 50] != 1)
	{
		return OK;
	}
	else if(dire == RIGHT && map.m[(rect.y - 25) / 50][(rect.x + 50 - 25) / 50] != 1)
	{
		return OK;
	}
	else
	{

		printf("wall\n");
		return ERROR;
	}
}

void Game(SDL_Renderer** rend)
{
	bool quit = false;
	int dire = RIGHT;
	SDL_Event event;
	//素材提取与转换
		//Pacman四方向图片
	SDL_Surface* face_up_img = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down_img = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left_img = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right_img = IMG_Load("image/pacmanRight.png");
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
		//Pacman背景
	SDL_Surface* bk_img = IMG_Load("image/point.png");
	SDL_Texture* bk = SDL_CreateTextureFromSurface(*rend, bk_img);
		//Monster三色图片
	SDL_Surface* monster_1_img = IMG_Load("image/red.png");
	SDL_Surface* monster_2_img = IMG_Load("image/yellow.png");
	SDL_Surface* monster_3_img = IMG_Load("image/blue.png");
	SDL_Texture* monster_1 = SDL_CreateTextureFromSurface(*rend, monster_1_img);
	SDL_Texture* monster_2 = SDL_CreateTextureFromSurface(*rend, monster_2_img);
	SDL_Texture* monster_3 = SDL_CreateTextureFromSurface(*rend, monster_3_img);
		//食物图片
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	//定义一个Pacman默认朝向
	SDL_Texture* currentFace = face_right;
	//pacman初始rect
	SDL_Rect rect;
	rect.x = map.pac.x;
	rect.y = map.pac.y;
	rect.w = 50;
	rect.h = 50;
	SDL_Delay(2000);
	while (quit == false)
	{
		SDL_WaitEventTimeout(&event,750);
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				currentFace = face_up;
				dire = UP;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				currentFace = face_down;
				dire = DOWN;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				currentFace = face_left;
				dire = LEFT;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				currentFace = face_right;
				dire = RIGHT;
			}
			draw_Pac(rend, &currentFace, &bk, dire);
		}
		else
		{
			draw_Pac(rend, &currentFace, &bk, dire);
		}
		
	}
}
int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* rend = NULL;
	TTF_Font* font;
	char s[] = "Pacman create by Chtholly 2019/12/17";
	//窗口结构初始化
	Init_Window(s, &window, &surface, &rend, &font);
	//文件读取
	read_file();
	draw_UI(&rend, &window, &font);
	//整体Pacman移动控制及monster移动
	Game(&rend);
	return 0;
}