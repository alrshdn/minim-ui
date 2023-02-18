
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "minim-ui.h"


Mui *
mui_init(const char *displayname)
{
	Mui *mui = calloc(1, sizeof(Mui));

	if (!(mui->dpy = XOpenDisplay(displayname))) {
		printf("Mui: Error: Could not initialize connection to X server for opening display.\n");
		return NULL;
	}
	
	return mui;	
}


void
mui_setup(Mui *mui,
		const int xposition, const int yposition,
		const unsigned int windowwidth, const unsigned int windowheight,
		const long unsigned int backgroundcolor, const long unsigned int foregroundcolor,
		const char *appname)
{
	XSetWindowAttributes wa;
	XGCValues gcv;
	unsigned long wvm, gcvm;

	/* screen setup */
	mui->scr = XDefaultScreen(mui->dpy);

	/* main window and main window's attributes setup */
	mui->w = windowwidth;
	mui->h = windowheight;
	wa.background_pixel = backgroundcolor;
	wa.event_mask = KeyPressMask|KeyReleaseMask
		|ButtonPressMask
		|ExposureMask;
	wvm = CWBackPixel|CWEventMask;
	mui->main_win = XCreateWindow(mui->dpy,
			DefaultRootWindow(mui->dpy),
			xposition, yposition,
			windowwidth, windowheight,
			0,					/* border width */
			DefaultDepth(mui->dpy, mui->scr),
			InputOutput,
			DefaultVisual(mui->dpy, mui->scr),
			wvm,
			&wa);

	XStoreName(mui->dpy, mui->main_win, appname);

	/* graphical context and state setup */
	gcv.line_width = 44;
	gcv.line_style = LineSolid;
	gcv.cap_style = CapButt;
	gcv.join_style = JoinMiter;
	gcv.background = backgroundcolor;
	gcv.foreground = foregroundcolor;
	gcv.fill_style = FillSolid;
	gcvm = GCLineWidth|GCLineStyle
		|GCCapStyle|GCJoinStyle
		|GCForeground|GCBackground
		|GCFillStyle;
		
	mui->gc = XCreateGC(mui->dpy, mui->main_win, gcvm, &gcv);

	/* atoms setup */
	wmatom[WMNormalHints] = XInternAtom(mui->dpy, "WM_NORMAL_HINTS", False);
}

void
mui_show(Mui *mui)
{
	if (XMapWindow(mui->dpy, mui->main_win))
		XSync(mui->dpy, False);
}

void
mui_rectangle(Mui *mui,
		const int xposition, const int yposition,
		const unsigned int width, const unsigned int height,
		const int isfilled, const int isinverted)
{
	/*
	if (!mui || !mui->scheme)
		return;
	*/
	XSetForeground(mui->dpy, mui->gc, WhitePixel(mui->dpy, mui->scr));
	if (isfilled)
		XFillRectangle(mui->dpy, mui->main_win, mui->gc, xposition, yposition, width, height);
	else
		XDrawRectangle(mui->dpy, mui->main_win, mui->gc, xposition, yposition, width - 1, height - 1);
}

void
mui_createclr(Mui *mui,
		const char *clrname,
		Clr *clr)
{
	XftColorAllocName(mui->dpy, DefaultVisual(mui->dpy, mui->scr),
			DefaultColormap(mui->dpy, mui->scr),
			clrname, clr);
}

Clr *
mui_createclrmode(Mui *mui,
		char *clrnames[],
		size_t clrcount)
{
	size_t i;
	Clr *clr;

	for (i = 0; i < clrcount; i++)
		mui_createclr(mui, clrnames[i], clr);

	return clr;
}

void
mui_cleanup(Mui *mui)
{	
	mui->scr = 0;
	mui->w = 0;
	mui->h = 0;

	XUnmapWindow(mui->dpy, mui->main_win);
	XDestroyWindow(mui->dpy, mui->main_win);

	XFreeGC(mui->dpy, mui->gc);
	
	free(mui->clrmodes);
}

void
mui_fin(Mui *mui, int andcleanup)
{
	if (andcleanup)
		mui_cleanup(mui);

	XCloseDisplay(mui->dpy);
	free(mui);
}
