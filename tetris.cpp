#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

/* interface */
void console();				// dashboard
void gotoxy(int x,int y);	// control position
void my_print();			// set interface
void my_randout();			// randomly show block and print in hint
void my_printblock();		// print 4*4 block

/* control */
void my_up();				// rotate
void my_down();				// move downward
void my_left();				// move to left-hand side
void my_right();			// move to right-hand side
void my_stop();				// freeze

/* situation */
void clear();				// clear entire row
void gameover();			// OuO

/* block */
int tmp[4][4];
int block[4][4];			// control block's mode
int hintblock[4][4];		// the next block's mode
int movingblock[4][4];		// showing the moving block
int scope[25][34]={};		// scope of manipulating 

/* block mode */
int block0[4][4]={{0,1,1,0},{0,0,1,0},{0,0,1,0},{0,0,0,0}};  // 7
int block1[4][4]={{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}};  // mirror 7
int block2[4][4]={{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}};  // z
int block3[4][4]={{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}};  // mirror z
int block4[4][4]={{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}};  // down T
int block5[4][4]={{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}};  // square
int block6[4][4]={{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}};  // ----

/* variable */
int point_x,point_y;		// block's left-up coordinate x,y
int scores=0;				// points you get in the game
bool checkdown=true;			// whether the block could moving down further

int main()
{
	unsigned seed;
	seed=(unsigned)time(NULL);
	srand(seed);
	
	char key;
	int time=0;
	
	console();
	my_print();
	my_randout();
	
	while(1)
	{
		if(checkdown)
		{
			checkdown=false;
			point_x=14;
			point_y=0;
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					movingblock[i][j]=block[i][j];
			
			my_printblock();
			my_randout();
		}
		
		if(_kbhit())		// detect whether there is any input
		{
			key=_getch();
			switch(key)
			{
				case 72:
					my_up();
					break;
				case 75:
					my_left();
					break;
				case 77:
					my_right();
					break;
				case 80:
					my_down();
					break;
				case 32:
					my_stop();
					break;
				case 27:		// esc
					exit(0);
			}
		}
		
		Sleep(15); 		// wait for 0.03 second
		
		if(0 == ++time % 15)
		{
			my_down();
		}
		
		if(checkdown)	// can't moving down further
		{
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					if(movingblock[i][j]==1)
						scope[point_x/2+j][point_y+i]=movingblock[i][j];
			
			my_printblock();
			clear();
		}
	}
	return 0;
}

void console()
{
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);	// HANDLE: pointer to void
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hOut,&bInfo);
	SetConsoleTitle("Tetris");
	COORD size={80,32};
	SetConsoleScreenBufferSize(hOut,size);
	SMALL_RECT rc={0,0,63,31};
	SetConsoleWindowInfo(hOut,true,&rc);
}

void my_print()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
	for(int i=1;i<30;i++)
		cout << "@                               @ |" << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ |---------------------------|" << endl;
	gotoxy(35,0);
	cout << "---------------------------|" << endl;
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	
	gotoxy(45,3);
	cout << "SCORE : " << scores;
	gotoxy(38,6);
	cout << "next : ";
	gotoxy(38,14);
	cout << "manipulate : ";
	gotoxy(42,16);
	cout << "space : stop";
	gotoxy(42,18);
	cout << "Esc : escape";
	gotoxy(47,28);
	cout << " 2018 / 4 / 7 ";
}

void gotoxy(int x,int y)
{
	HANDLE app;
	COORD pos;
	pos.X=x;
	pos.Y=y;
	app=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(app,pos);
}

void my_up()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			tmp[i][j]=movingblock[i][j];
			
			if(movingblock[i][j]==1)
			{
				gotoxy(point_x+j*2,point_y+i);
				cout << " ";
			}
		}
	}
	
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			movingblock[j][3-i]=tmp[i][j];
		
	for(int j=0;j<2;j++)
	{
		for(int i=0;i<4;i++)
		{
			if(movingblock[i][j]==1 && point_x<2)
				point_x+=2;
			else if(movingblock[i][3-j]==1 && point_x>24)
				point_x-=2;
		}
	}
	
	my_printblock();
}

void my_left()
{
	for(int i=0;i<3;i++)
		for(int j=0;j<4;j++)
			if(movingblock[j][i]==1 && (point_x+2*i-1<2 || scope[point_x/2+i-1][point_y+j]==1))
				return;
				
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		if(movingblock[i][j]==1)
		{
			gotoxy(point_x+j*2,point_y+i);
			cout << " ";
		}
	}
	
	point_x-=2;
	my_printblock();
}				

void my_right()
{
	for(int i=3;i>0;i--)
		for(int j=0;j<4;j++)
			if(movingblock[j][i]==1 && (point_x+2*i+2>30 || scope[point_x/2+i+1][point_y+j]==1)) 
				return;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		if(movingblock[i][j]==1)
		{
			gotoxy(point_x+j*2,point_y+i);
			cout << " ";
		}
	}
	
	point_x+=2;
	my_printblock();
}

void my_down()
{
	if(checkdown)
		return;
	
	gotoxy(point_x,point_y);
	
	for(int i=3;i>0;i--)
		for(int j=0;j<4;j++)
			if(movingblock[i][j]==1)
				if(scope[point_x/2+j][point_y+i+1]==1 || point_y+i+1>28)
				{
					checkdown=true;
					return;
				}
	
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(movingblock[i][j]==1)
			{
				gotoxy(point_x+j*2,point_y+i);
				cout << " ";
			}
	
	point_y++;
	my_printblock();
}

void my_stop()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
	
	gotoxy(12, 10);printf("       ");
	gotoxy(12, 11);printf(" @@@@  ");
	gotoxy(12, 12);printf(" @   @ ");
	gotoxy(12, 13);printf(" @   @ ");
	gotoxy(12, 14);printf(" @@@@  ");
	gotoxy(12, 15);printf(" @     ");
	gotoxy(12, 16);printf(" @     ");
	gotoxy(12, 17);printf(" @     ");
	gotoxy(12, 18);printf("       ");

	if (32 == _getch()) 
	{
		gotoxy(12, 10);printf("       ");
		gotoxy(12, 11);printf(" @@@@@ ");
		gotoxy(12, 12);printf("     @ ");
		gotoxy(12, 13);printf("     @ ");
		gotoxy(12, 14);printf(" @@@@@ ");
		gotoxy(12, 15);printf("     @ ");
		gotoxy(12, 16);printf("     @ ");
		gotoxy(12, 17);printf(" @@@@@ ");
		gotoxy(12, 18);printf("       ");
		
		Sleep(1000);
		
		gotoxy(12, 10);printf("       ");
		gotoxy(12, 11);printf(" @@@@@ ");
		gotoxy(12, 12);printf("     @ ");
		gotoxy(12, 13);printf("     @ ");
		gotoxy(12, 14);printf(" @@@@@ ");
		gotoxy(12, 15);printf(" @     ");
		gotoxy(12, 16);printf(" @     ");
		gotoxy(12, 17);printf(" @@@@@ ");
		gotoxy(12, 18);printf("       ");
		
		Sleep(1000);
		
		gotoxy(12, 10);printf("       ");
		gotoxy(12, 11);printf("   @   ");
		gotoxy(12, 12);printf("   @   ");
		gotoxy(12, 13);printf("   @   ");
		gotoxy(12, 14);printf("   @   ");
		gotoxy(12, 15);printf("   @   ");
		gotoxy(12, 16);printf("   @   ");
		gotoxy(12, 17);printf("   @   ");
		gotoxy(12, 18);printf("       ");
		
		Sleep(1000);
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		
		for(int i=0;i<9;i++)
		{
			gotoxy(10, 10 + i);
		
			for(int j=0;j<7;j++)
			{
				if (scope[5+j][10+i]==1)
					cout << "@";
				else
					cout << " ";
		
			}
		}
	}
}

void clear()
{
	int i,j,k,m=0;
	
	for(i=point_y+3;i>=point_y;i--)
	{
		for(j=1;j<16;j++)
			if(scope[j][i]==0)
				break;	
		
		if(j==16)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			m++;
			
			for(k=i;k>3;k--)
			{
				for(j=1;j<16;j++)
				{
					scope[j][k]=scope[j][k-1];
					gotoxy(j*2,k);
					if(scope[j][k]==1)
						cout << "@";
					else
						cout << " ";
				}
			}
			
			for(j=1;j<16;j++)
				scope[j][4]=0;
			i++;
		}
	}	
	
	switch(m)
	{
		case 1:
			scores+=100;
			break;
		case 2:
			scores+=230;
			break;
		case 3:
			scores+=438;
			break;
		case 4:
			scores+=854;
			break;
		default:;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	
	gotoxy(45,3);
	cout << "SCORE : " << scores;
	
	for(j=7;j<11;j++)
		if(scope[j][4]==1)
			gameover();
}

void my_randout()
{
	int i,j;
	switch(rand()%7)
	{
		case 0:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block0[i][j];
			break;
		case 1:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block1[i][j];
			break;
		case 2:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block2[i][j];
			break;
		case 3:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block3[i][j];
			break;
		case 4:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block4[i][j];
			break;
		case 5:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block5[i][j];
			break;
		case 6:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[i][j]=block6[i][j];
			break;
		default :;
	}
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			tmp[i][j]=block[i][j];
	
	switch(rand()%4)
	{
		case 0:
			break;
		case 1:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[j][3-i]=tmp[i][j];
			break;
		case 2:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[3-i][3-j]=tmp[i][j];
			break;
		case 3:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					block[3-j][i]=tmp[i][j];
			break;
		default :;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	
	for(i=0;i<4;i++)
	{
		gotoxy(44,i+8);
		for(j=0;j<4;j++)
		{
			if(block[i][j]==1)
				cout << "@";
			else
				cout << " ";
		}
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
	
void my_printblock()
{
	int i,j;
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(point_x/2+j>0)
			{
				if(scope[point_x/2+j][point_y+i]==1)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);		
					gotoxy(point_x+j*2,point_y+i);
					printf("@");
				}
				else if(movingblock[i][j]==1)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	
					gotoxy(point_x+j*2,point_y+i);
					printf("@");
				}
			}
		}
	}
}

void gameover()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	
	gotoxy(4, 4);printf("                      ");
	gotoxy(4, 5);printf(" @@@@ @    @  @  @@@@ ");
	gotoxy(4, 6);printf(" @    @@   @@ @  @    ");
	gotoxy(4, 7);printf(" @ @@ @ @  @ @ @ @@@@ ");
	gotoxy(4, 8);printf(" @  @ @@@@ @ @ @ @    ");
	gotoxy(4, 9);printf(" @@@@ @  @ @ @ @ @@@@ ");
	gotoxy(4, 10);printf("                      ");
	
	gotoxy(6, 10);printf("                      ");
	gotoxy(6, 11);printf(" @@@@ @  @ @@@@ @@@@  ");
	gotoxy(6, 12);printf(" @  @ @  @ @    @   @ ");
	gotoxy(6, 13);printf(" @  @ @  @ @@@@ @@@@  ");
	gotoxy(6, 14);printf(" @  @ @ @  @    @ @   ");
	gotoxy(6, 15);printf(" @@@@ @    @@@@ @  @  ");
	gotoxy(6, 16);printf("                      ");
	
	gotoxy(2, 19);printf("                             ");
	gotoxy(2, 20);printf("space : restart  Esc : escape");
	gotoxy(2, 21);printf("                             ");
	
	switch (_getch())
	{
		case 32: 
			system("cls");
			console();		
			my_print();		
			my_randout();
			memset(scope, 0, sizeof(int) * 25 * 34);
			scores=0;
			break;
		case 27: 
			exit(0);
			break;
		default:;
	}
}
