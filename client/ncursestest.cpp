
#include <ncurses.h>

int main()
{	

	int x, y;
	x = 10; y = 10;

	initscr();

	int height,width, start_y, start_x;
	height = 10;
	width = 20;
	start_y = start_x = 10;

	
	

	int c = getch();
	endwin();

	return 0;
}