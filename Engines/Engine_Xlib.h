#ifndef ENGINE_XLIB_H
#define ENGINE_XLIB_H

#include"../Game.h"
#include<X11/Xlib.h>

class Engine_Xlib
{
public:
	Engine_Xlib();
	Game *game;

	void initial();
	Window getWindow()const;
	void eventLoop();
private:
	void event_MotionNotify(const XMotionEvent &event);
	void event_Key(const XKeyEvent &event,bool pressed);
	void event_Button(const XButtonEvent &event,bool pressed);
	void event_Expose(const XExposeEvent &event);
	void event_GraphicsExpose(const XGraphicsExposeEvent &event);
	void event_NoExpose(const XNoExposeEvent &event);

	Display *display;
	int x,y,width,height,borderWidth;
	Window window;
	GC gc;
	XGCValues xgcValues;

	bool runningEventLoop;
};
#endif // ENGINE_XLIB_H