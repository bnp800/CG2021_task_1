#include<iostream>
#include<conio.h>
#include<graphics.h>
#include<fstream>
#include<string>
#include<Windows.h>
#include<stdlib.h>

using namespace std;

const int pixelsize = 16*2;
const int mapsize_x = 20;
const int mapsize_y = 20;
char cmap[mapsize_x][mapsize_y];
char CMAP[5][5];
char currentMap,nextMap;
int Count = 0;
int room_x = 0, room_y = 0;
int sceneChanged = 0;
RECT logs = { 0, mapsize_y * pixelsize, mapsize_x * pixelsize - 1, (mapsize_y + 1) * pixelsize - 1 };
IMAGE map_elem[10];

class character
{
	public:
	character(char i,int px, int py,int h,int g,int a,int d,int ag)
	{
		id = i;x = px;y = py;hp = h;gp = g;atk = a;def = d;agi = ag;
	}
	character()
	{
		id = 'n'; x = -1; y = -1; gp = atk = def = agi = 0;
	}
	char id;
	int x;
	int y;
	int hp = 50;
	int gp;
	int atk;
	int def;
	int agi;
	int dead = 0;
	int direction = 1;
	int roll()
	{
		return (rand() % 20) + 1;
	}
};
character pc('p',0,0,100,0,5,5,5);
character enemy[5];
void defeat()
{
		/*DWORD* pBuffer = GetImageBuffer(NULL);
		for (int i = mapsize_x * pixelsize * mapsize_y * pixelsize - 1; i >= 0; i--)
			if (pBuffer[i] != 0)
				pBuffer[i] = RGB(GetRValue(pBuffer[i]) >> 1, GetGValue(pBuffer[i]) >> 1, GetBValue(pBuffer[i]) >> 1);
				*///»­Ãæ±ä°µ50%£»
		IMAGE defeatImg;
		loadimage(&defeatImg, ".\\recourses\\defeat.png",mapsize_x*pixelsize,mapsize_y*pixelsize, true);
		putimage(0,0, &defeatImg);
		drawtext("Press any key to continute.", &logs, DT_LEFT | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void enmeyDraw()
{
	for (int i = 0; i < 5; i++)
	{
		if ((enemy + i)->dead == 0)
		{
			if ((enemy + i)->direction == 1)
			{
				putimage((enemy + i)->x * pixelsize, (enemy + i)->y * pixelsize, pixelsize, pixelsize, map_elem + 4, Count * pixelsize, 0);
			}
			if ((enemy + i)->direction == 0)
			{
				putimage((enemy + i)->x * pixelsize, (enemy + i)->y * pixelsize, pixelsize, pixelsize, map_elem + 7, Count * pixelsize, 0);
			}
		}
	}
}
void draw(IMAGE map_elem[7])
{
	BeginBatchDraw();
	for (int j = 0; j < mapsize_x; j++)
		for (int i = 0; i < mapsize_y; i++)
		{
			if (cmap[i][j] == '*')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem, 0, 0);
			if (cmap[i][j] == '#')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 1, 0, 0);
			if (cmap[i][j] == '.')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 2, 0, 0);
			if (cmap[i][j] == '@')
			{
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 3, Count * pixelsize, 0);
				Count++;
				if (Count > 3)
					Count = 0;
			}
			if (cmap[i][j] == '+')
				enmeyDraw();
			if (cmap[i][j] == 'G')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 5, Count * pixelsize, 0);
			if (cmap[i][j] == 'H')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 6, Count * pixelsize, 0);
			if (cmap[i][j] == 'X')
				putimage(i * pixelsize, j * pixelsize, pixelsize, pixelsize, map_elem + 8, 0, 0);
		}
	fillrectangle(0, mapsize_y * pixelsize, (double(pc.hp)/100)*mapsize_x*pixelsize-1, (mapsize_y + 1)*pixelsize-1);
	setfillcolor(BLACK);
	solidrectangle((double(pc.hp) / 100) * mapsize_x * pixelsize, mapsize_y * pixelsize, mapsize_x * pixelsize - 1, (mapsize_y + 1) * pixelsize - 1);
	setfillcolor(GREEN);
	char log[24];
	sprintf_s(log, "HP:%d,GP:%d",pc.hp,pc.gp);
	drawtext(log, &logs, DT_LEFT | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	FlushBatchDraw();
}
int moveCheck(character* role,int x,int y)
{
	if (cmap[role->x + x][role->y + y] == '*')
		return -1;
	if (cmap[role->x + x][role->y + y] == '.')
		return 1;
	if (cmap[role->x + x][role->y + y] == 'X')
		return 2;
	if (cmap[role->x + x][role->y + y] == 'H')
		return 3;
	if (cmap[role->x + x][role->y + y] == 'G')
		return 4;
	return 0;
}
void Cmove(character* role,int x, int y)
{
	if (cmap[role->x + x][role->y + y] != 'X')
	{
		cmap[role->x][role->y] = '.';
		if (role->id == 'p')
			cmap[role->x + x][role->y + y] = '@';
		else
			cmap[role->x + x][role->y + y] = '+';
		role->x += x;
		role->y += y;
	}
}
void changeScene(int x, int y)
{
	//currentMap = CMAP[room_x][room_y];
	if (x == 0)
	{
		room_y = max(0, room_y - 1);
		cmap[pc.x][pc.y] = '.';
		pc.x = mapsize_y-2;
	}
	if (x == mapsize_x - 1)
	{
		room_y = min(4, room_y + 1);
		cmap[pc.x][pc.y] = '.';
		pc.x = 1;
		//currentMap = CMAP[room_x][room_y];
	}
	if (y == 0)
	{
		room_x = max(0, room_x - 1);
		cmap[pc.x][pc.y] = '.';
		pc.y = mapsize_x - 2;
		//currentMap = CMAP[room_x][room_y];
	}
	if (y == mapsize_y - 1)
	{
		room_x = min(4, room_x + 1);
		cmap[pc.x][pc.y] = '.';
		pc.y = 1;
		//currentMap = CMAP[room_x][room_y];
	}
	nextMap = CMAP[room_x][room_y];
	sceneChanged = 1;

}
void input()
{
	char key = _getch();
		switch (key)
		{
		case 'a':
		case 'A':
			loadimage(map_elem + 3, ".\\recourses\\character_r.png", 4 * pixelsize, pixelsize);
			if (moveCheck(&pc, -1, 0) > 0)
			{
				if (moveCheck(&pc, -1, 0) == 2)
					changeScene(pc.x-1, pc.y);
				else
				{
					if (moveCheck(&pc, -1, 0) == 3)
						pc.hp += 20;
					if (moveCheck(&pc, -1, 0) == 4)
						pc.gp += 50;
					Cmove(&pc, -1, 0);
				}
				
			}
			else if (moveCheck(&pc,-1, 0) == -1)
				pc.dead = 1;
			break;
		case 'd':
		case 'D':
			loadimage(map_elem + 3, ".\\recourses\\character.png", 4 * pixelsize, pixelsize);
			if (moveCheck(&pc, 1, 0) > 0)
			{
				if (moveCheck(&pc, 1, 0) == 2)
					changeScene(pc.x+1, pc.y);
				else
				{
					if (moveCheck(&pc, 1, 0) == 3)
						pc.hp += 20;
					if (moveCheck(&pc, 1, 0) == 4)
						pc.gp += 50;
					Cmove(&pc, +1, 0);
				}
			}
			else if (moveCheck(&pc,1, 0) == -1)
				pc.dead = 1;
			break;
		case 'w':
		case 'W':
			if (moveCheck(&pc, 0, -1) > 0)
			{
				if (moveCheck(&pc, 0, -1) == 2)
					changeScene(pc.x, pc.y - 1);
				else
				{
					if (moveCheck(&pc, 0, -1) == 3)
						pc.hp += 20;
					if (moveCheck(&pc, 0, -1) == 4)
						pc.gp += 50;
					Cmove(&pc, 0, -1);
				}
			}
			else if (moveCheck(&pc,0, -1) == -1)
				pc.dead = 1;
			break;
		case 's':
		case 'S':
			if (moveCheck(&pc, 0, 1) > 0)
			{
				if (moveCheck(&pc, 0, 1) == 2)
					changeScene(pc.x, pc.y + 1);
				else
				{
					if (moveCheck(&pc, 0, 1) == 3)
						pc.hp += 20;
					if (moveCheck(&pc, 0, 1) == 4)
						pc.gp += 50;
					Cmove(&pc, 0, 1);
				}
			}
			else if (moveCheck(&pc,0, 1) == -1)
				pc.dead = 1;
			break;
		case 32:
		//case 'e':
		//case 'E':
			/*char log[24];
			sprintf_s(log, "Roll 1d100 = %d", pc.roll());
			drawtext(log, &logs, DT_LEFT | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			*/
			break;
		case 'i':
		case 'I':
			break;
		}
	}
int distant(int x, int y)
{
	return(abs(x - pc.x) + abs(y - pc.y));
}
void enemyMove(character enemy[5])
{

	int r, nx = 0, ny = 0;
	for (int i = 0; i < 5; i++)
	{
		if (!(enemy + i)->dead)
		{
			r = distant((enemy + i)->x, (enemy + i)->y);
			//if ((moveCheck(enemy + i, 1, 0) == 1) && (distant((enemy + i)->x + 1, (enemy + i)->y) < r))
				if (distant((enemy + i)->x + 1, (enemy + i)->y) < r)
				{
					nx = 1;
					(enemy + i)->direction = 1;
				}
			//if ((moveCheck(enemy + i, -1, 0) == 1) && (distant((enemy + i)->x - 1, (enemy + i)->y) < r))
				if (distant((enemy + i)->x - 1, (enemy + i)->y) < r)
				{
					nx = -1;
					(enemy + i)->direction = 0;
				}
			//if ((moveCheck(enemy + i, 0, 1) == 1) && (distant((enemy + i)->x, (enemy + i)->y + 1) < r))
				if (distant((enemy + i)->x, (enemy + i)->y + 1) < r)
					ny = 1;
			//if ((moveCheck(enemy + i, 0, -1) == 1) && (distant((enemy + i)->x, (enemy + i)->y - 1) < r))
				if (distant((enemy + i)->x, (enemy + i)->y - 1) < r)
					ny = -1;
				if (distant((enemy + i)->x + nx, (enemy + i)->y) < distant((enemy + i)->x, (enemy + i)->y + ny) && distant((enemy + i)->x + nx, (enemy + i)->y) != 0)
				{
					if(moveCheck(enemy + i, nx, 0) == 1)
						Cmove(enemy + i, nx, 0);
				}
				else //if (distant((enemy + i)->x + nx, (enemy + i)->y) >= distant((enemy + i)->x, (enemy + i)->y + ny) && distant((enemy + i)->x + nx, (enemy + i)->y) != 0)
				{
					if (moveCheck(enemy + i, 0, ny) == 1)
						Cmove(enemy + i, 0, ny);
				}
		}
	}
}
void combat()
{
	for (int i = 0; i < 5; i++)
	{
		if ((distant((enemy + i)->x, (enemy + i)->y) == 1) && !(enemy+i)->dead)
		{
			int enemyroll = (enemy + i)->roll();
			int pcroll = pc.roll();
			if (((enemy + i)->roll() + (enemy + i)->atk) > pc.agi)
			{
				pc.hp -= max((enemy + i)->roll() + (enemy + i)->atk - pc.def,0);
				//clearrectangle(0, mapsize_y * pixelsize,  mapsize_x * pixelsize - 1, (mapsize_y + 1) * pixelsize - 1);
				if (pc.hp <= 0)
					pc.dead = 1;
			}
			if ((pc.roll() + pc.atk) > (enemy + i)->agi)
			{
				(enemy + i)->hp -= (pc.roll() + pc.atk - (enemy + i)->def);
				if ((enemy + i)->hp <= 0)
				{
					(enemy + i)->dead = 1;
					cmap[(enemy + i)->x][(enemy + i)->y] = '.';
					pc.gp += 100;
				}
			}
		}
	}
}
void loadMap(char type)
{
	fstream map;
	//int count = 0;
	string name;
	switch (type)
	{
		case 'A':
			name = "map_a.txt";
			break;
		case 'B':
			name = "map_b.txt";
			break;
		case 'C':
			name = "map_c.txt";
			break;
		case 'D':
			name = "map_d.txt";
			break;
		default:
			break;
	}
	map.open(name);
	for (int i = 0; i < mapsize_x; i++)
		for (int j = 0; j < mapsize_y; j++)
		{
			map >> cmap[i][j];
		}
	for (int i = 0; i < 5; i++)
	{
		cmap[(enemy + i)->x][(enemy + i)->y] = '.';
		(enemy + i)->x = rand() % (mapsize_y-2) + 1;
		(enemy + i)->y = rand() % (mapsize_x-2) + 1;
		cmap[(enemy + i)->x][(enemy + i)->y] = '+';
	}
	cmap[pc.x][pc.y] = '@';
	map.close();
}
void init()
{
	srand((unsigned)time(NULL));
	initgraph(mapsize_x * pixelsize, (mapsize_y + 1) * pixelsize);
	setfillcolor(GREEN);
	setbkcolor(BLACK);
	loadimage(map_elem, ".\\recourses\\hole.png", pixelsize, pixelsize);
	loadimage(map_elem + 1, ".\\recourses\\wall.png", pixelsize, pixelsize);
	loadimage(map_elem + 2, ".\\recourses\\floor.png", pixelsize, pixelsize);
	loadimage(map_elem + 3,".\\recourses\\character.png", 4*pixelsize, pixelsize);
	loadimage(map_elem + 4, ".\\recourses\\skeleton.png", 4*pixelsize, pixelsize);
	loadimage(map_elem + 5, ".\\recourses\\coin.png", 4 * pixelsize, pixelsize);
	loadimage(map_elem + 6, ".\\recourses\\red_potion.png", 4*pixelsize, pixelsize);
	loadimage(map_elem + 7, ".\\recourses\\skeleton_r.png", 4 * pixelsize, pixelsize);
	loadimage(map_elem + 8, ".\\recourses\\door.png", pixelsize, pixelsize);
	fstream MAP;
	MAP.open("map.txt");
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			MAP >> CMAP[i][j];
	MAP.close();
	currentMap = CMAP[0][0];
	nextMap = currentMap;
	pc.x = rand() % (mapsize_y-2) + 1;
	pc.y = rand() % (mapsize_x-2) + 1;
	loadMap(currentMap);
}
int main()
{

	init();
	while (1)
	{
		if (sceneChanged)
		{
			sceneChanged = 0;
			currentMap = nextMap;
			loadMap(nextMap);
		}
		draw(map_elem);
		if (_kbhit())
		{
			input();
			enemyMove(enemy);
			combat();
		}
		if (pc.dead)
		{
			defeat();
			break;
		}
		Sleep(100);
	}
	EndBatchDraw();
	system("Pause");
	closegraph();
	return 0;
}