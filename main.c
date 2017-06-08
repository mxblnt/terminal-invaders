#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sleep_ms(int milliseconds) 
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

char rand_char(int rseed) {
	switch (rseed) {
		case 0: return '*';
		case 1: return '.'; 
		case 2: return 'o'; 
		case 3: return '+';
		default: return ' '; 
	}
}

void print_screen(int row, int col, char **screen)
{
	for (int i=0;i<row;i++)
		for (int j=0;j<col;j++)
			mvaddch(i,j,screen[i][j]);
} 

void print_ship(int x, int y, int tick)
{
	mvprintw(x,y-1,"/\\");
	mvprintw(x+1,y-2,"|  |");
	mvprintw(x+2,y-3,"/|/\\|\\");
	mvprintw(x+3,y-4,"/_||||_\\");
	if (tick<=30)
		mvprintw(x+4,y-3,"W    W");
	else	
		mvprintw(x+4,y-3,"V    V");
}

char** move_screen(int row, int col, char **screen)
{
	int i,j,rseed;
	for(i=row-2;i>=0;i--)
		for (j=0;j<col;j++)
			screen[i+1][j]=screen[i][j];

	for (j=0;j<col;j++) {
		rseed=rand() % 100;
		screen[0][j]=rand_char(rseed);
	}

	return screen;
}

int main()
{
	int x,y,i,j,row,col,key,rseed,tick;
	
	noecho();
	cbreak();
	initscr();
	curs_set(0);
	nodelay(stdscr,TRUE);
	getmaxyx(stdscr,row,col);
	keypad(stdscr, TRUE);
	srand(time(NULL));

	char **screen = (char **)malloc(row * sizeof(char*));
	for(i = 0; i < row; i++) 
		screen[i] = (char *)malloc(col * sizeof(char));
	
	x=30;
	y=row/2+30;

	for (i=0;i<row;i++)
		for (j=0;j<col;j++) {
			rseed=rand() % 100;
			screen[i][j]=rand_char(rseed);
		}

	while(1) {
		print_screen(row,col,screen);
		print_ship(x,y,tick);
		key=getch();
		switch (key) {
			case KEY_LEFT: y--; break;
			case KEY_RIGHT: y++; break;
			case KEY_UP: x--; break;
			case KEY_DOWN: x++; break;
		}
		refresh();
		sleep_ms(10);
		tick++;
		if (tick%20==0)
			screen=move_screen(row,col,screen);
		else if (tick > 60)
			tick=0;
	}
	endwin();				
	return 0;
}
