#pragma once

#define MinMineCount 10
#define MinWidth 6
#define MinHeight 6
#define MaxWidth 50
#define MaxHeight 50

enum GameDifficulty
{
	CUSTOM=0,
	EASY,
	MIDDLE,
	HARD
};



enum  GameStat
{
	NORMAL=0,
	NOT_READY,
	READY,
	WIN,
	LOST
};

enum GameMapStat
{
	Falg1 = -1,
	EMPTY = 0,
	UNKNOW,
	BOOM,
	FLAG_RIGHT,
	FLAG_ERROR
};





struct GameData
{
	GameDifficulty gameDifficulty;
	int MapWidth, MapHeight;
	int Map[MaxHeight][MaxWidth];					// <0 雷; =0 无; >0 提示数字
	GameMapStat MapStat[MaxHeight][MaxWidth];


	int MapMineCount;
	GameStat gamestat;


};






class MineSweeper
{
private:
	int getMine(int x, int y);
	int getMineNumber(int x, int y);
	int getFlag(int x, int y);
	int getFlagNumber(int x, int y);

public:
	GameData gamedata;

	MineSweeper();
	bool setGameDifficulty(GameDifficulty difficulty, int width = 0, int height = 0, int mineCount = 0);
	void GameStart();
	void InitMap(int mx ,int my);

	void ClickMap(int mx, int my);
	void DoubleClick(int mx, int my);
	void RightClick(int mx, int my);

	void GameLost(int mx, int my);
	void GameWin(int mx, int my);
	bool IsWin();
	double getProgress();
};

