#ifndef SDL_H
#define SDL_H

//#include"SDL.h"
//#include"DataBlock.h"

class SDL
{
public:
	SDL();

	bool initial();
	void eventLoop();
protected:
	/*virtual void event_KeyDown(const SDL_KeyboardEvent&);
	virtual void event_KeyUp(const SDL_KeyboardEvent&);

	virtual void event_MouseMotion(const SDL_MouseMotionEvent&);
	virtual void event_MouseButtonDown(const SDL_MouseButtonEvent&);
	virtual void event_MouseButtonUp(const SDL_MouseButtonEvent&);

	virtual void event_JoyAxisMotion(const SDL_JoyAxisEvent&);
	virtual void event_JoyBallMotion(const SDL_JoyBallEvent&);
	virtual void event_JoyHatMotion(const SDL_JoyHatEvent&);
	virtual void event_JoyButtonDown(const SDL_JoyButtonEvent&);
	virtual void event_JoyButtonUp(const SDL_JoyButtonEvent&);

	virtual void event_VideoResize(const SDL_ResizeEvent&);
	virtual void event_VideoExpose(const SDL_ExposeEvent&);

	virtual void event_Active(const SDL_ActiveEvent&);
	virtual void event_Quit(const SDL_QuitEvent&);
	virtual void event_SysWM(const SDL_SysWMEvent&);
	virtual void event_User(const SDL_UserEvent&);

	virtual void goTicks();
private:
	//Video
	SDL_Surface *surface;
	//timer
	Uint32 ticks;
	Uint32 elapse;

	Uint32 interval;
	Uint32 fps;
	Uint32 frameElapse;

	//Audio
	SDL_AudioSpec audioSpec;
	DataBlock wavBlock;
	SDL_AudioSpec *wavAudioSpec;

	static void audioCallBack(void *self,Uint8 *stream,int len);
	void audioEvent(Uint8 *stream,int len);*/
};

#endif // SDL_H
