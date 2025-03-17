#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int x, y, vx, vy, w, h, b, cb, W, H, fps = 60;
clock_t ticks, lastticks;
float dt;
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

int main(int argc, char* argv[]) {
	srand(time(NULL));
	
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nodelay(stdscr, true);

	W = COLS - 2;
	H = LINES - 2;
	w = W / 4;
	h = w / 4;

	int opt;
	while ((opt = getopt(argc, argv, "w:h:f:")) != -1) {
		switch (opt) {
			case 'w':
				w = atoi(optarg);
				break;
			case 'h':
				h = atoi(optarg);
				break;
			case 'f':
				fps = atoi(optarg);
				break;
		}
	}
	
	x = rand() % (W - w);
	y = rand() % (H - h);
	vx = vy = 1;

	while (1) {
		lastticks = ticks;
		ticks = clock();
		dt = (float)ticks / CLOCKS_PER_SEC;

		if ((int)(dt * 100000) % (int)(1 / (float)fps * 100000) == 0 || skip) {
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
			mvprintw(0, W / 2 - (int)(log10f((float)b)), "%d", b);
			mvprintw(0, W / 2 + 2, "%d", cb);
			rectangle(y, x, y + h, x + w);

			char ch = getch();
			if (ch == 'q') break;
			if (ch == '\\') skip = !skip;

			refresh();
		}
	}

	clear();
	refresh();

	return 0;
}
