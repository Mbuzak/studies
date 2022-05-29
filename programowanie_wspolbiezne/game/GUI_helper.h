#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define BLACK 0
#define RED 1
#define BLUE 2

Display *dis;
int screen;
Window win;
GC gc;
unsigned int black, white, red, blue;

XEvent event;
KeySym key;
char text[255];
char *line = new char[2];


struct vertice_pos {
	char letter;
	int x;
	int y;
};

struct click_pos {
	int x;
	int y;
};


vertice_pos *vp = new vertice_pos[6];
click_pos cp;


unsigned int RGB(int r, int g, int b) {
	return b + (g << 8) + (r << 16);
}


void init_vertices() {
	vp[0].letter = 'A'; vp[0].x = 50; vp[0].y = 200;
	vp[1].letter = 'B'; vp[1].x = 200; vp[1].y = 100;
	vp[2].letter = 'C'; vp[2].x = 350; vp[2].y = 200;
	vp[3].letter = 'D'; vp[3].x = 350; vp[3].y = 300;
	vp[4].letter = 'E'; vp[4].x = 200; vp[4].y = 400;
	vp[5].letter = 'F'; vp[5].x = 50; vp[5].y = 300;
}

void init(int color) {
	cp.x = 100;
	cp.y = 100;
	
	dis = XOpenDisplay((char *) 0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);
	red = RGB(250, 150, 150);
	blue = RGB(150, 150, 250);
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 450, 600, 5, black, white);
	
	if ( color == RED ) XSetStandardProperties(dis, win, "GRACZ CZERWONY", "Hi", None, NULL, 0, NULL);
	else XSetStandardProperties(dis, win, "GRACZ NIEBIESKI", "Hi", None, NULL, 0, NULL);
	
	XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
	gc = XCreateGC(dis, win, 0, 0);
	XSetBackground(dis, gc, white);
	XSetForeground(dis, gc, black);
	XSetLineAttributes(dis, gc, 5, LineSolid, CapNotLast, JoinMiter);
	
	XClearWindow(dis, win);
	
	
	XMapRaised(dis, win);	
}

void close() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	exit(0);
}

void draw() {
	XClearWindow(dis, win);
}

void draw_vertices() {
	for ( int i = 0 ; i < 6 ; i++ ) {
		XSetLineAttributes(dis, gc, 10, LineSolid, CapNotLast, JoinMiter);
		XDrawArc(dis, win, gc, vp[i].x, vp[i].y, 10, 10, 0, 360*64);
		XSetLineAttributes(dis, gc, 5, LineSolid, CapNotLast, JoinMiter);
		char *text = new char[1];
		text[0] = vp[i].letter;
		XDrawString(dis, win, gc, vp[i].x - 5, vp[i].y - 5, text, 1);
		delete text;
	}
	
}

void draw_edges(edge *edges) {
	for ( int i = 0 ; i < 15 ; i++ ) {
		if ( edges[i].color == BLUE ) {
			XSetLineAttributes(dis, gc, 10, LineOnOffDash, CapNotLast, JoinMiter);
			XSetForeground(dis, gc, blue);
		}
		else if ( edges[i].color == RED ) {
			XSetLineAttributes(dis, gc, 10, LineSolid, CapNotLast, JoinMiter);
			XSetForeground(dis, gc, red);
		} 
		else continue;
		
		int x1, y1, x2, y2;
		for ( int j = 0 ; j < 6 ; j++ ) {
			if ( edges[i].v1 == vp[j].letter ) {
				x1 = vp[j].x; y1 = vp[j].y;
			}
			else if ( edges[i].v2 == vp[j].letter ) {
				x2 = vp[j].x; y2 = vp[j].y;
			}
		}
		
		XDrawLine(dis, win, gc, x1, y1, x2, y2);
	}
	XSetForeground(dis, gc, black);
}

void show_game(edge *edges) {
	draw_vertices();
	draw_edges(edges);
	XNextEvent(dis, &event);
}

char clicked_letter(int x, int y) {
	for ( int i = 0 ; i < 6 ; i++ ) {
		if ( x >= vp[i].x - 15 and x <= vp[i].x + 15 and 
			 y >= vp[i].y - 15 and y <= vp[i].y + 15) {
			return vp[i].letter;
		}
	}
	return ' ';
}

void reset_line(char *line) {
	line[0] = ' ';
	line[1] = ' ';
}

