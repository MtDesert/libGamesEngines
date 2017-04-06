#include"Engine_Xlib.h"
#include<X11/keysym.h>
#include<sys/time.h>

#include<iostream>
using namespace std;

int ii=0;

Engine_Xlib::Engine_Xlib()
{
	x=0,y=0,width=640,height=480,borderWidth=0;
}
void Engine_Xlib::initial()
{
	display=XOpenDisplay(nullptr);
	window=XCreateSimpleWindow(display,DefaultRootWindow(display),
							   x,y,width,height,borderWidth,0xFFFFFFFF,0xFF000000);
	gc=XCreateGC(display,window,0,&xgcValues);
	XMapWindow(display,window);
	XSelectInput(display,window,0x01FFFFFF);
	XSetForeground(display,gc,0xFFFF0000);
	game->startTicks();
}
Window Engine_Xlib::getWindow()const{return window;}
void Engine_Xlib::eventLoop()
{
	runningEventLoop=true;
	//event loop
	int x=0;
	XEvent xEvent;
	while(runningEventLoop)
	{
		if(XPending(display)<=0)
		{
			game->goTicks();
			if(game->needRedraw)
			{
				game->needRedraw=false;
				XClearWindow(display,window);
				++ii;
				XDrawLine(display,window,gc,0,0,ii,ii);
				XFlush(display);
			}
			continue;
		}
		XNextEvent(display,&xEvent);
		switch(xEvent.type)
		{
			case KeyPress:
				event_Key(xEvent.xkey,true);
			break;
			case KeyRelease:
				event_Key(xEvent.xkey,false);
			break;
			case ButtonPress:
				event_Button(xEvent.xbutton,true);
			break;
			case ButtonRelease:
				event_Button(xEvent.xbutton,false);
			break;
			case MotionNotify:
				event_MotionNotify(xEvent.xmotion);
			break;
			case EnterNotify:
			break;
			case LeaveNotify:
			break;
			case FocusIn:
			break;
			case FocusOut:
			break;
			case Expose:
				event_Expose(xEvent.xexpose);
			break;
			case GraphicsExpose:
				event_GraphicsExpose(xEvent.xgraphicsexpose);
			break;
			case NoExpose:
				event_NoExpose(xEvent.xnoexpose);
			break;
		}
	}
}
void Engine_Xlib::event_MotionNotify(const XMotionEvent &event)
{
}
void Engine_Xlib::event_Key(const XKeyEvent &event, bool pressed)
{
	KeySym keySym=XKeycodeToKeysym(display,event.keycode,0);
	switch(keySym){
		case XK_Up:game->keyboard_Up(pressed);break;
		case XK_Down:game->keyboard_Down(pressed);break;
		case XK_Left:game->keyboard_Left(pressed);break;
		case XK_Right:game->keyboard_Right(pressed);break;
		case XK_Escape:
			XFreeGC(display,gc);
			XCloseDisplay(display);
			runningEventLoop=false;
		break;
	}
}
void Engine_Xlib::event_Button(const XButtonEvent &event, bool pressed)
{
	switch(event.button){
		case Button1:game->mouse_LeftButton(event.x,event.y,pressed);break;
		case Button2:game->mouse_MiddleButton(event.x,event.y,pressed);break;
		case Button3:game->mouse_RightButton(event.x,event.y,pressed);break;
	}
}
void Engine_Xlib::event_Expose(const XExposeEvent &event)
{
	//cout<<"Expose"<<endl;
}
void Engine_Xlib::event_GraphicsExpose(const XGraphicsExposeEvent &event)
{
	cout<<"GraphicsExpose"<<endl;
}
void Engine_Xlib::event_NoExpose(const XNoExposeEvent &event)
{
	cout<<"NoExpose"<<endl;
}