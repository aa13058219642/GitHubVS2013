#include "MineSweeper.h"
#include<stdlib.h>

int abs(int x){
	if (x < 0)
		return -x;
	return x;
}


MineSweeper::MineSweeper()
{
	setGameDifficulty(GameDifficulty::EASY);
	gamedata.gamestat = GameStat::NOT_READY;

}



bool MineSweeper::setGameDifficulty(GameDifficulty difficulty, int width, int height, int mineCount)
{
	switch (difficulty)
	{
	default:
	case GameDifficulty::EASY:
		gamedata.MapWidth = 9;
		gamedata.MapHeight = 9;
		gamedata.MapMineCount = 10;
		return true;
	case GameDifficulty::MIDDLE:
		gamedata.MapWidth = 16;
		gamedata.MapHeight = 16;
		gamedata.MapMineCount = 40;
		return true;
	case GameDifficulty::HARD:
		gamedata.MapWidth = 30;
		gamedata.MapHeight = 16;
		gamedata.MapMineCount = 99;
		return true;
	case GameDifficulty::CUSTOM:
		if (width >= MinWidth && width <= MaxWidth &&
			height >= MinHeight && height <= MaxHeight &&
			mineCount >= MinMineCount && mineCount <= (width*height*0.85))
		{
			gamedata.MapWidth = width;
			gamedata.MapHeight = height;
			gamedata.MapMineCount = mineCount;
			return true;
		}
		return false;
	}
}

void MineSweeper::GameStart()
{
	for (int i = 0; i < gamedata.MapHeight; i++)
	{
		for (int j = 0; j < gamedata.MapWidth; j++)
		{
			gamedata.Map[i][j] = 0;							//清空地图
			gamedata.MapStat[i][j] = GameMapStat::UNKNOW;	//重置格子状态
		}
	}
	gamedata.gamestat = GameStat::READY;
}

void MineSweeper::InitMap(int mx, int my){
	int i = gamedata.MapMineCount;
	int rx, ry = 0;



	//布雷
	while (i > 0)
	{
		do{
			rx = rand() % gamedata.MapWidth;
			ry = rand() % gamedata.MapHeight;

			if (gamedata.Map[ry][rx] == -1)continue;					//当前格有雷
			else if (abs(rx - mx) <= 1 && abs(ry - my) <= 1)continue;	//第一次点击，周围一圈不能有雷
			else break;
		} while (true);
		gamedata.Map[ry][rx] = -1;
		i--;
	}

	//计算数字
	for (int i = 0; i < gamedata.MapHeight; i++)
	{
		for (int j = 0; j < gamedata.MapWidth; j++)
		{
			if (gamedata.Map[i][j] == 0)
				gamedata.Map[i][j] = getMineNumber(j, i);
			
		}
	}


	gamedata.gamestat = GameStat::NORMAL;
}

int MineSweeper::getMine(int x, int y)
{
	if (x < 0 || y < 0 || x >= gamedata.MapWidth || y >= gamedata.MapHeight)
		return 0;
	if (gamedata.Map[y][x] < 0)
	{
		return -gamedata.Map[y][x];
	}
	else
	{
		return 0;
	}

}

int MineSweeper::getMineNumber(int x, int y){
	return getMine(x - 1, y - 1) + getMine(x, y - 1) + getMine(x + 1, y - 1) +
		getMine(x - 1, y) + getMine(x + 1, y) +
		getMine(x - 1, y + 1) + getMine(x, y + 1) + getMine(x + 1, y + 1);

}

void MineSweeper::ClickMap(int mx, int my)
{
	//点到已经翻开的格子，或错误的格子，直接退出
	if (gamedata.MapStat[my][mx] == GameMapStat::EMPTY || mx < 0 || my < 0 || mx >= gamedata.MapWidth || my >= gamedata.MapHeight)
		return;

	//如果游戏没有准备好，则先布雷
	if (gamedata.gamestat == GameStat::READY)
	{
		InitMap(mx, my);
	}

	if (gamedata.MapStat[my][mx] < 0)
	{	//点到插旗的格子
		return;
	}else if (gamedata.Map[my][mx] < 0)
	{	//点到雷
		gamedata.gamestat=GameStat::LOST;
		GameLost(mx, my);
	}
	else if (gamedata.Map[my][mx]>0)
	{	//点到数字格，直接翻开
		gamedata.MapStat[my][mx] = GameMapStat::EMPTY;
	}
	else
	{	//点到空白格,翻开自己及周围8个格
		gamedata.MapStat[my][mx] = GameMapStat::EMPTY;
		ClickMap(mx - 1, my - 1); 
		ClickMap(mx, my - 1); 
		ClickMap(mx + 1, my - 1);
		ClickMap(mx - 1, my); 
		ClickMap(mx + 1, my);
		ClickMap(mx - 1, my + 1); 
		ClickMap(mx, my + 1); 
		ClickMap(mx + 1, my + 1);
	}



}

int MineSweeper::getFlag(int x, int y)
{
	if (x < 0 || y < 0 || x >= gamedata.MapWidth || y >= gamedata.MapHeight)
		return 0;
	else if (gamedata.MapStat[y][x] < 0)
	{
		return -gamedata.MapStat[y][x];
	}
	else
		return 0;
}

int MineSweeper::getFlagNumber(int x, int y)
{
	return getFlag(x - 1, y - 1) + getFlag(x, y - 1) + getFlag(x + 1, y - 1) +
		getFlag(x - 1, y) + getFlag(x + 1, y) +
		getFlag(x - 1, y + 1) + getFlag(x, y + 1) + getFlag(x + 1, y + 1);
}

void MineSweeper::DoubleClick(int mx, int my)
{
	if (mx < 0 || my < 0 || mx >= gamedata.MapWidth || my >= gamedata.MapHeight)
		return;
	
	if (gamedata.MapStat[my][mx] < 0)
	{	//点到插旗的格子
		return;
	}
	else 
	{	
		//点到未翻开的格子,先翻开自己
		if (gamedata.MapStat[my][mx] == GameMapStat::UNKNOW)
		{
			ClickMap(mx, my);
		}


		//点开的格子是数字
		if (gamedata.Map[my][mx] > 0)
		{	
			//周围的旗数大于等于格子提示数
			int aroundMineCount = getFlagNumber(mx, my);
			if (aroundMineCount >= gamedata.Map[my][mx])
			{
				//翻开周围8格
				ClickMap(mx - 1, my - 1); ClickMap(mx, my - 1); ClickMap(mx + 1, my - 1);
				ClickMap(mx - 1, my); ClickMap(mx + 1, my);
				ClickMap(mx - 1, my + 1); ClickMap(mx, my + 1); ClickMap(mx + 1, my + 1);
			}
		}
	}
}

void MineSweeper::RightClick(int mx, int my)
{
	if (mx < 0 || my < 0 || mx >= gamedata.MapWidth || my >= gamedata.MapHeight)
		return;

	if (gamedata.MapStat[my][mx] == 0)
	{	//已经翻开的格子
		return ;
	}
	else
	{
		if (gamedata.MapStat[my][mx] == GameMapStat::UNKNOW)
			gamedata.MapStat[my][mx] = GameMapStat::Falg1;
		else 
			gamedata.MapStat[my][mx] = GameMapStat::UNKNOW;
	}

}

double MineSweeper::getProgress()
{
	//计算已经翻开的格子
	int open = 0;
	for (int i = 0; i < gamedata.MapHeight; i++)
	{
		for (int j = 0; j < gamedata.MapWidth; j++)
		{
			if (gamedata.MapStat[i][j] == GameMapStat::EMPTY)
				open++;
		}
	}
	//能被翻开的格子
	double count = gamedata.MapHeight*gamedata.MapWidth - gamedata.MapMineCount;

	return open / count;
}




bool MineSweeper::IsWin(){
	if (gamedata.gamestat != GameStat::NORMAL)
	{
		return false;
	}

	for (int i = 0; i < gamedata.MapHeight; i++)
	{
		for (int j = 0; j < gamedata.MapWidth; j++)
		{
			if (gamedata.Map[i][j] >= 0 )
			{	//只要有一个没有雷的格没翻开，就未胜利
				if(gamedata.MapStat[i][j] != GameMapStat::EMPTY)
					return false;
			}
		}
	}
	return true;
}

void MineSweeper::GameWin(int x, int y)
{
	gamedata.gamestat = GameStat::WIN;
}

void MineSweeper::GameLost(int mx, int my)
{
	for (int i = 0; i < gamedata.MapHeight; i++)
	{
		for (int j = 0; j < gamedata.MapWidth; j++)
		{
			if (gamedata.MapStat[i][j] < GameMapStat::EMPTY)
			{
				//判断插旗是否正确
				if (gamedata.Map[i][j] == gamedata.MapStat[i][j])
				{
					gamedata.MapStat[i][j] = GameMapStat::FLAG_RIGHT;

				}
				else
				{
					gamedata.MapStat[i][j] = GameMapStat::FLAG_ERROR;
				}

			}
			else
			{
				gamedata.MapStat[i][j] = GameMapStat::EMPTY;
			}
		}
	}

	if (mx >= 0 && my >= 0 && mx < gamedata.MapWidth && my < gamedata.MapHeight)
	{
		gamedata.MapStat[my][mx] = GameMapStat::BOOM;
	}


	gamedata.gamestat = GameStat::LOST;
}

