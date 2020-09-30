//Final Version

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define OK 1
#define ERROR 0
#define N 15
#define UP 10
#define DOWN 20
#define LEFT 30
#define RIGHT 40

//ȫ�ֱ���point���ڱ�ʾ����
int point = 0;
//��ͼ�ṹ�壬����ȫ�ֱ���map
struct MAP
{
	int m[15][15];
	SDL_Rect pac;
	SDL_Rect monster[3];
}map;
//SDL�����ʼ������
int Init_Window(char s[], SDL_Window** window, SDL_Surface** surface, SDL_Renderer** rend, TTF_Font** font)
{
	//SDL��ʼ��
	if (SDL_Init(SDL_INIT_VIDEO) < 0)	
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return ERROR;
	}
	else
	{
		//���ڳ�ʼ��
		*window = SDL_CreateWindow(s, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return ERROR;
		}
		//���ڴ�������������Surface����
		else
		{
			*surface = SDL_GetWindowSurface(*window);
		}
	}
	//��Ⱦ��ʼ��
	*rend = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(*rend);
	//���嵼���ʼ��
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
//��ȡ��ͼ�ļ�
void read_file()
{
	int i = 0, j = 0;
	int flag = 1;		//������϶�ȡpacman��monster������
	int val;
	int k = 0;
	FILE* fp;
	if ((fp = fopen("image/scene.txt", "r")) == NULL)
	{
		//�����汾�޸ĳ���ʾ��Window����
		printf("Map load failed");
	}
	else
	{
		while (fscanf(fp, "%d", &val) != EOF)
		{
			//��ȡpacman����
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
			//��ȡ����monster����
			else if (flag > 2 && k < 3)
			{
				if (flag % 2 != 0)
				{
					map.monster[k].x = val;
					flag++;
				}
				else
				{
					map.monster[k].y = val;
					flag++;
					k++;
				}
			}
			//��ȡ��ͼ��Ϣ
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
	//����̨�������ȡ���ĵ�ͼ��Ϣ������
	k = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%d ", map.m[i][j]);
			if (map.m[i][j] == 0)
				k++;
		}
		printf("\n");
	}
	printf("\nTotal 0:%d\n", k);
	//����̨�����monster����
	for (i = 0; i < 3; i++)
	{
		printf("%d %d %d", i, map.monster[i].x, map.monster[i].y);
		printf("\n");
	}
	//��ȡ���
	printf("File reading complete\n");
}
//���Ƶ�ͼ
void draw_map(SDL_Renderer** rend, SDL_Window** window)
{
	int i = 0, j = 0;
	//�ز�������ת��
	SDL_Surface* wall_img = IMG_Load("image/wall.png");
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* wall = SDL_CreateTextureFromSurface(*rend, wall_img);
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	//��ͼ����
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
//������ʾ
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
//�����ʼ������ͼ+�̶����֣�
void draw_UI(SDL_Renderer** rend, SDL_Window** window, TTF_Font** font)
{
	//�滭��ͼ
	draw_map(rend, window);
	char s[] = "Point";
	char a[] = "create";
	char b[] = "by";
	char c[] = "Chtholly";
	//������ʾ
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
//����Pacman
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
	if (map.m[(old_rect.y - 25) / 50][(old_rect.x - 25) / 50] == 0)
	{
		map.m[(old_rect.y - 25) / 50][(old_rect.x - 25) / 50] = 2;
		point++;
	}
	
	printf("A%d %d\n", rect.x, rect.y);
	printf("B%d %d\n", old_rect.x, old_rect.y);
	printf("Point:%d\n", point);
	SDL_RenderCopy(*rend, *bk, NULL, &old_rect);
	SDL_RenderCopy(*rend, *face, NULL, &rect);
}
//����Monster
void draw_Monster(SDL_Renderer** rend, SDL_Texture** monster, SDL_Texture** bk,int i)
{
	int wall_touch(int dire, SDL_Rect rect);
	int dire = rand() % 3;	//����0-4�������,ʹ��ʱ�����Ϊ0-1��1-2��2-3��3-4�Ķ�
	SDL_Rect old = map.monster[i];
	old.w = 50;
	old.h = 50;
	map.monster[i].w = 50;
	map.monster[i].h = 50;
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	//pac������
	if (map.monster[i].x >= map.pac.x && map.monster[i].y > map.pac.y)
	{
		if (dire < 1 && wall_touch(UP, map.monster[i]))
		{
			map.monster[i].y -= 50;
		}
		else if (dire >= 1 && wall_touch(LEFT, map.monster[i]))
		{
			map.monster[i].x -= 50;
		}
		else if (dire >= 1 && wall_touch(UP, map.monster[i]))
		{
			map.monster[i].y -= 50;
		}
		else if (dire < 1 && wall_touch(LEFT, map.monster[i]))
		{
			map.monster[i].x -= 50;
		}
		else
		{
			if (wall_touch(RIGHT, map.monster[i]))
			{
				map.monster[i].x += 50;
			}
			else
			{
				map.monster[i].y += 50;
			}
		}
	}
	//pac������
	else if (map.monster[i].x > map.pac.x&& map.monster[i].y <= map.pac.y)
	{
		if (dire < 1 && wall_touch(DOWN, map.monster[i]))
		{
			map.monster[i].y += 50;
		}
		else if (dire >= 1 && wall_touch(LEFT, map.monster[i]))
		{
			map.monster[i].x -= 50;
		}
		else if (dire >= 1 && wall_touch(DOWN, map.monster[i]))
		{
			map.monster[i].y += 50;
		}
		else if (dire < 1 && wall_touch(LEFT, map.monster[i]))
		{
			map.monster[i].x -= 50;
		}
		else
		{
			if (wall_touch(RIGHT, map.monster[i]))
			{
				map.monster[i].x += 50;
			}
			else
			{
				map.monster[i].y -= 50;
			}
		}
	}
	//pac������
	else if (map.monster[i].x <= map.pac.x && map.monster[i].y > map.pac.y)
	{
		if (dire < 1 && wall_touch(UP, map.monster[i]))
		{
			map.monster[i].y -= 50;
		}
		else if (dire >= 1 && wall_touch(RIGHT, map.monster[i]))
		{
			map.monster[i].x += 50;
		}
		else if (dire >= 1 && wall_touch(UP, map.monster[i]))
		{
			map.monster[i].y -= 50;
		}
		else if (dire < 1 && wall_touch(RIGHT, map.monster[i]))
		{
			map.monster[i].x += 50;
		}
		else
		{
			if (wall_touch(LEFT, map.monster[i]))
			{
				map.monster[i].x -= 50;
			}
			else
			{
				map.monster[i].y += 50;
			}
		}
	}
	//pac������
	else if(map.monster[i].x < map.pac.x && map.monster[i].y <= map.pac.y)
	{
		if (dire < 1 && wall_touch(DOWN, map.monster[i]))
		{
			map.monster[i].y += 50;
		}
		else if (dire >= 1 && wall_touch(RIGHT, map.monster[i]))
		{
			map.monster[i].x += 50;
		}
		else if (dire >= 1 && wall_touch(DOWN, map.monster[i]))
		{
			map.monster[i].y += 50;
		}
		else if (dire < 1 && wall_touch(RIGHT, map.monster[i]))
		{
			map.monster[i].x += 50;
		}
		else
		{
			if (wall_touch(LEFT, map.monster[i]))
			{
				map.monster[i].x -= 50;
			}
			else
			{
				map.monster[i].y -= 50;
			}
		}
	}
	//pac�����Ϸ�
	else if (map.monster[i].x == map.pac.x && map.monster[i].y > map.pac.y)
	{
		if (wall_touch(UP, map.monster[i]))
		{
			map.monster[i].y -= 50;
		}
	}
	//pac�����·�
	else if (map.monster[i].x == map.pac.x && map.monster[i].y < map.pac.y)
	{
		if (wall_touch(DOWN, map.monster[i]))
		{
			map.monster[i].y += 50;
		}
	}
	//pac������
	else if (map.monster[i].x > map.pac.x && map.monster[i].y == map.pac.y)
	{
		if (wall_touch(LEFT, map.monster[i]))
		{
			map.monster[i].x -= 50;
		}
	}
	//pac�����ҷ�
	else if (map.monster[i].x < map.pac.x&& map.monster[i].y == map.pac.y)
	{
		if (wall_touch(RIGHT, map.monster[i]))
		{
			map.monster[i].x += 50;
		}
	}
	//�жϵ�ǰ�ƶ�λ���Ƿ���ʳ��
	if (map.m[(old.y - 25) / 50][(old.x - 25) / 50] == 0)
	{
		SDL_RenderCopy(*rend, food, NULL, &old);
	}
	else
	{
		SDL_RenderCopy(*rend, *bk, NULL, &old);
	}
	printf("monster:%d %d\n", map.monster[i].x, map.monster[i].y);
	SDL_RenderCopy(*rend, *monster, NULL, &map.monster[i]);
	//SDL_RenderPresent(*rend);
}
//���Ʒ�������
void draw_point(SDL_Renderer** rend, TTF_Font** font)
{
	char s[4];
	sprintf(s, "%d", point);
	SDL_Rect rect,ttf_rect;
	SDL_Surface* ttf_bk_img = IMG_Load("image/ttf_bk.png");
	SDL_Texture* ttf_bk = SDL_CreateTextureFromSurface(*rend, ttf_bk_img);
	rect.x = 840;
	rect.y = 225;
	ttf_rect.x = 840;
	ttf_rect.y = 225;
	ttf_rect.w = ttf_bk_img->w;
	ttf_rect.h = ttf_bk_img->h;
	
	SDL_RenderCopy(*rend, ttf_bk, NULL, &ttf_rect);
	ttf_print(s, font, rend, rect);
}
//��ײ���
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
//������Ϸ��Pacman��Monster��ײ��
void End_Game(SDL_Renderer** rend)
{
	int i;
	if (point == 107)
	{
		SDL_Surface* win_img = IMG_Load("image/win.png");
		SDL_Texture* win = SDL_CreateTextureFromSurface(*rend, win_img);
		SDL_Rect rect;
		rect.x = 300;
		rect.y = 100;
		rect.w = win_img->w;
		rect.h = win_img->h;
		SDL_RenderCopy(*rend, win, NULL, &rect);
		SDL_RenderPresent(*rend);
		SDL_Delay(5000);
		exit(0);
	}
	for (i = 0; i < 3; i++)
	{
		if (map.pac.x == map.monster[i].x && map.pac.y == map.monster[i].y)
		{
			printf("END\n");
			SDL_Surface* end_img = IMG_Load("image/game_over.png");
			SDL_Texture* end = SDL_CreateTextureFromSurface(*rend, end_img);
			SDL_Rect rect;
			rect.x = 300;
			rect.y = 100;
			rect.w = end_img->w;
			rect.h = end_img->h;
			SDL_RenderCopy(*rend, end, NULL, &rect);
			SDL_RenderPresent(*rend);
			SDL_Delay(5000);
			exit(0);
		}
	}
	
}
//��Ϸ���̿���
void Game(SDL_Renderer** rend, TTF_Font** font)
{
	bool quit = false;
	int dire = RIGHT;
	SDL_Event event;
	//�ز���ȡ��ת��
		//Pacman�ķ���ͼƬ
	SDL_Surface* face_up_img = IMG_Load("image/pacmanUp.png");
	SDL_Surface* face_down_img = IMG_Load("image/pacmanDown.png");
	SDL_Surface* face_left_img = IMG_Load("image/pacmanLeft.png");
	SDL_Surface* face_right_img = IMG_Load("image/pacmanRight.png");
	SDL_Texture* face_up = SDL_CreateTextureFromSurface(*rend, face_up_img);
	SDL_Texture* face_down = SDL_CreateTextureFromSurface(*rend, face_down_img);
	SDL_Texture* face_left = SDL_CreateTextureFromSurface(*rend, face_left_img);
	SDL_Texture* face_right = SDL_CreateTextureFromSurface(*rend, face_right_img);
		//Pacman����
	SDL_Surface* bk_img = IMG_Load("image/point.png");
	SDL_Texture* bk = SDL_CreateTextureFromSurface(*rend, bk_img);
		//Monster��ɫͼƬ
	SDL_Surface* monster_1_img = IMG_Load("image/red.png");
	SDL_Surface* monster_2_img = IMG_Load("image/yellow.png");
	SDL_Surface* monster_3_img = IMG_Load("image/blue.png");
	SDL_Texture* monster_1 = SDL_CreateTextureFromSurface(*rend, monster_1_img);
	SDL_Texture* monster_2 = SDL_CreateTextureFromSurface(*rend, monster_2_img);
	SDL_Texture* monster_3 = SDL_CreateTextureFromSurface(*rend, monster_3_img);
		//ʳ��ͼƬ
	SDL_Surface* food_img = IMG_Load("image/food.png");
	SDL_Texture* food = SDL_CreateTextureFromSurface(*rend, food_img);
	//����һ��PacmanĬ�ϳ���
	SDL_Texture* currentFace = face_right;
	//pacman��ʼrect
	SDL_Rect rect;
	rect.x = map.pac.x;
	rect.y = map.pac.y;
	rect.w = 50;
	rect.h = 50;
	//SDL_Delay(2000);
	while (quit == false)
	{
		while (SDL_WaitEventTimeout(&event, 750))
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
				//draw_Pac(rend, &currentFace, &bk, dire);
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				continue;
			}
		}
		
		draw_Pac(rend, &currentFace, &bk, dire);
		draw_Monster(rend, &monster_1, &bk, 0);
		draw_Monster(rend, &monster_2, &bk, 1);
		draw_Monster(rend, &monster_3, &bk, 2);
		draw_point(rend, font);

		SDL_RenderPresent(*rend);
		End_Game(rend);
	}
}
int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* rend = NULL;
	TTF_Font* font;
	//BGM����
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* BGM = Mix_LoadMUS("image/BGM.wav");
	Mix_PlayMusic(BGM, -1);
	char s[] = "Pacman create by Chtholly 2019/12/17";
	//���ڽṹ��ʼ��
	Init_Window(s, &window, &surface, &rend, &font);
	//�ļ���ȡ
	read_file();
	draw_UI(&rend, &window, &font);
	//����Pacman�ƶ����Ƽ�monster�ƶ�
	Game(&rend,&font);

	SDL_DestroyRenderer(rend);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	return 0;
}