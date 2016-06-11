#include<iostream>
#include<stdio.h>
#include"MineSweeper.h"

using namespace std;

void Draw(MineSweeper ms)
{
	system("cls");

	switch (ms.gamedata.gamestat)
	{
	case GameStat::WIN:
	case GameStat::LOST:
	case GameStat::READY:
	case GameStat::NORMAL:
		cout << "     1 2 3 4 5 6 7 8 910 1 2 3 4 5 6 7 8 920 1 2 3 4 5 6 7 830" << endl;
		cout << "    □□□□□□□□□□□□□□□□□□□□□□□□□□□□□" << endl;
		for (int i = 0; i < ms.gamedata.MapHeight; i++)
		{
			printf("%2d□", i+1);
			for (int j = 0; j < ms.gamedata.MapWidth; j++)
			{
				//○●□■☆★△▲×√
				switch (ms.gamedata.MapStat[i][j])
				{
				case GameMapStat::UNKNOW:
					printf("■");
					break;
				case GameMapStat::EMPTY:
					if (ms.gamedata.Map[i][j]<0)
					{
						printf("●");
					}
					else if (ms.gamedata.Map[i][j] > 0)
					{
						printf("%2d", ms.gamedata.Map[i][j]);
					}
					else
					{
						printf("  ");
					}
					break;
				case GameMapStat::Falg1:
					printf("☆");
					break;
				case GameMapStat::BOOM:
					printf("▲");
					break;
				case GameMapStat::FLAG_RIGHT:
					printf("√");
					break;
				case GameMapStat::FLAG_ERROR:
					printf("×");
					break;
				default:
					break;
				}
			}
			printf("□\n");
		}
		cout << "    □□□□□□□□□□□□□□□□□□□□□□□□□□□□□" << endl;

		//游戏状态判断
		if (ms.gamedata.gamestat == GameStat::WIN)
			cout << "赢了！" << endl<<"输入1回到标题"<<endl;
		else if (ms.gamedata.gamestat == GameStat::LOST)
			cout << "输了！" << endl << "输入1回到标题" << endl;


		break;
	case GameStat::NOT_READY:
		cout << "选择难度：" << endl << "1.EASY    2.MIDDLE    3.HARD" << endl;
		break;

	default:
		break;
	}


}

int getCMD()
{
	int cmd;
	while (true)
	{
		cin >> cmd;
		if (!cin)     //条件可改写为cin.fail()
		{
			cout << "输入有误！请重新输入" << endl;
			cin.clear();
			cin.sync();      //清空流
		}
		else
		{
			//cout << cmd;
			return cmd;
			//break;
		}

	}


}





int main(int argc, char *argv[])
{
	MineSweeper ms;
	int cmd,x,y;

	while (true)
	{
		Draw(ms);


		switch (ms.gamedata.gamestat)
		{
		case GameStat::WIN:
		case GameStat::LOST:
			cmd = getCMD();
			ms.gamedata.gamestat = GameStat::NOT_READY;
			break;
		case GameStat::READY:
		case GameStat::NORMAL:
			cout << "输入操作：1.单击    2.双击    3.插旗:";
			cmd = getCMD();

			cout << "横坐标:";
			x = getCMD();

			cout << "纵坐标:";
			y = getCMD();

			switch (cmd)
			{
			case 1:
				ms.ClickMap(x - 1, y - 1);
				break;
			case 2:
				ms.DoubleClick(x - 1, y - 1);
				break;
			case 3:
				ms.RightClick(x - 1, y - 1);
				break;
			default:
				break;
			}

			if (ms.IsWin())
			{
				ms.GameWin(x - 1, y - 1);
			}

			break;
		case GameStat::NOT_READY:
			cmd=getCMD();
			switch (cmd)
			{
			case GameDifficulty::EASY:
				ms.setGameDifficulty(GameDifficulty::EASY);
				ms.GameStart();
				break; 
			case GameDifficulty::MIDDLE:
				ms.setGameDifficulty(GameDifficulty::MIDDLE);
				ms.GameStart();
				break;
			case GameDifficulty::HARD:
				ms.setGameDifficulty(GameDifficulty::HARD);
				ms.GameStart();
				break;
			default:
				break;
			}
			
		default:
			break;

		}

	}





	return 0;
}

