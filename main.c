#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int x, y, vx, vy, w, h, b, cb, W, H;
bool skip;

void rectangle(int y1, int x1, int y2, int x2) {
	mvhline(y1, x1, 0, x2-x1);
	mvhline(y2, x1, 0, x2-x1);
	mvvline(y1, x1, 0, y2-y1);
	mvvline(y1, x2, 0, y2-y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
}

int main() {
	srand(time(NULL));
	
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	halfdelay(1);

	W = COLS - 2;
	H = LINES - 2;
	w = W / 4;
	h = w / 4;
	x = rand() % (W - w);
	y = rand() % (H - h);
	vx = vy = 1;

	while (1) {
		bool nc = y == 1;
		bool ec = x + w == W;
		bool sc = y + h == H;
		bool wc = x == 1;

		if (nc || ec || sc || wc) b++;

		if ((nc && ec) || (ec && sc)
		 || (sc && wc) || (wc && nc))
			cb++;

		if (nc) vy =  1;
		if (ec) vx = -1;
		if (sc) vy = -1;
		if (wc) vx =  1;

		x += vx;
		y += vy;

		clear();
		box(stdscr, 0, 0);
		mvprintw(0, 0, "%d %d", b, cb);
		rectangle(y, x, y + h, x + w);

		if (!skip && getch() == ' ')
			skip = true;
		else
			refresh();
	}
}
