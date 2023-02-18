typedef XEvent MuiEvent;
typedef XButtonPressedEvent MuiButtonPressedEvent;
typedef XKeyPressedEvent MuiKeyPressedEvent;
typedef XKeyReleasedEvent MuiKeyReleasedEvent;
typedef XExposeEvent MuiExposedEvent;
typedef XftColor Clr;

/* enums */
enum {
	WMNormalHints,
	WMAtomCount
}; /* Window Manager atoms */
enum {ClrIdBackground, ClrIdForeground, ClrIdBorder, ClrIdCount}; /* colors indices for accessing different schemes' elements */

typedef struct {
	Display *dpy;
	int scr;
	unsigned int w, h;
	Window main_win;
	GC gc;
	Clr *clrmodes;
} Mui;

static Atom wmatom[WMAtomCount];

/* initialization and setup */
Mui *mui_init(const char *displayname);
void mui_setup(Mui *mui, const int xposition, const int yposition, const unsigned int windowwidth, const unsigned int windowheight, const long unsigned int backgroundcolor, const long unsigned int foregroundcolor, const char *appname);
void mui_show(Mui *mui);
void mui_createclr(Mui *mui, const char *clrname, Clr *clr);
Clr *mui_createclrmode(Mui *mui, char *clrnames[], size_t clrcount);

/* drawing */
void mui_rectangle(Mui *mui, int xposition, int yposition, unsigned int width, unsigned int height, int isfilled, int isinverted);


/* cleanup and finalization */
void mui_cleanup(Mui *mui);
void mui_fin(Mui *mui, int andcleanup);
