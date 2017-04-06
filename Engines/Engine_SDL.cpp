#include"Engine_SDL.h"
//#include<SDL/SDL.h>
//#include<SDL/SDL_sound.h>

#include<iostream>
using namespace std;

/*SDL::SDL()
{
	elapse=0;
	frameElapse=0;
	fps=60;
	interval=16;
}

bool SDL::initial()
{
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
	{
		cout<<SDL_GetError()<<endl;
		return false;
	}
	surface=SDL_SetVideoMode(640,480,32,
							 SDL_HWSURFACE|
							 SDL_ASYNCBLIT|
							 SDL_ANYFORMAT|
							 SDL_HWPALETTE|
							 SDL_DOUBLEBUF|
							 SDL_RESIZABLE);
	if(!surface)
	{
		cout<<SDL_GetError()<<endl;
		return false;
	}
	return true;
}
void SDL::eventLoop()
{
	SDL_Event event;
	while(true)
	{
		if(SDL_PollEvent(&event))
		{
			//deal with event
			switch(event.type)
			{
				//keyboard
				case SDL_KEYDOWN:event_KeyDown(event.key);break;
				case SDL_KEYUP:event_KeyUp(event.key);break;
				//mouse
				case SDL_MOUSEMOTION:event_MouseMotion(event.motion);break;
				case SDL_MOUSEBUTTONDOWN:event_MouseButtonDown(event.button);break;
				case SDL_MOUSEBUTTONUP:event_MouseButtonUp(event.button);break;
				//joystick
				case SDL_JOYAXISMOTION:event_JoyAxisMotion(event.jaxis);break;
				case SDL_JOYBALLMOTION:event_JoyBallMotion(event.jball);break;
				case SDL_JOYHATMOTION:event_JoyHatMotion(event.jhat);break;
				case SDL_JOYBUTTONDOWN:event_JoyButtonDown(event.jbutton);break;
				case SDL_JOYBUTTONUP:event_JoyButtonUp(event.jbutton);break;
				//video
				case SDL_VIDEORESIZE:event_VideoResize(event.resize);break;
				case SDL_VIDEOEXPOSE:event_VideoExpose(event.expose);break;
					//other
				case SDL_NOEVENT:break;
				case SDL_ACTIVEEVENT:event_Active(event.active);break;
				case SDL_QUIT:event_Quit(event.quit);break;
				case SDL_SYSWMEVENT:event_SysWM(event.syswm);break;
				case SDL_USEREVENT:event_User(event.user);break;
			}
		}
		else
		{
			SDL_Delay(1);
		}
		if(event.type==SDL_QUIT)break;
		goTicks();
	}
}

void SDL::event_KeyDown(const SDL_KeyboardEvent&){cout<<"KeyDown"<<endl;}
void SDL::event_KeyUp(const SDL_KeyboardEvent&){cout<<"KeyUp"<<endl;}

void SDL::event_MouseMotion(const SDL_MouseMotionEvent&){/*cout<<"MouseMotion"<<endl;}
void SDL::event_MouseButtonDown(const SDL_MouseButtonEvent&){cout<<"MouseButtonDown"<<endl;}
void SDL::event_MouseButtonUp(const SDL_MouseButtonEvent&){cout<<"MouseButtonUp"<<endl;}

void SDL::event_JoyAxisMotion(const SDL_JoyAxisEvent&){cout<<"JoyAxis"<<endl;}
void SDL::event_JoyBallMotion(const SDL_JoyBallEvent&){cout<<"JoyBall"<<endl;}
void SDL::event_JoyHatMotion(const SDL_JoyHatEvent&){cout<<"JoyHat"<<endl;}
void SDL::event_JoyButtonDown(const SDL_JoyButtonEvent&){cout<<"JoyButtonDown"<<endl;}
void SDL::event_JoyButtonUp(const SDL_JoyButtonEvent&){cout<<"JoyButtonUp"<<endl;}

void SDL::event_VideoResize(const SDL_ResizeEvent&){cout<<"Resize"<<endl;}
void SDL::event_VideoExpose(const SDL_ExposeEvent& event)
{
	//cout<<"Expose"<<endl;
	SDL_Flip(surface);
}

void SDL::event_Active(const SDL_ActiveEvent&){cout<<"Active"<<endl;}
void SDL::event_Quit(const SDL_QuitEvent&)
{
	cout<<"Quit"<<endl;
	SDL_Event event;
	while(SDL_PollEvent(&event));//clean all event
	SDL_FreeSurface(surface);
	SDL_Quit();
}
void SDL::event_SysWM(const SDL_SysWMEvent&){cout<<"SysWM"<<endl;}
void SDL::event_User(const SDL_UserEvent&){cout<<"User"<<endl;}

void SDL::goTicks()
{
	ticks=SDL_GetTicks();
	Uint32 remain=ticks-elapse;
	//redraw by interval
	/*if(remain>=interval)
	{
		//send event
		SDL_Event event;
		event.type=SDL_VIDEOEXPOSE;
		SDL_PushEvent(&event);
		//elapse
		elapse+=interval;
	}
	//redraw by fps
	if(remain*fps>=(frameElapse+1)*1000)
	{
		//send event
		SDL_Event event;
		event.type=SDL_VIDEOEXPOSE;
		SDL_PushEvent(&event);
		//elapse
		++frameElapse;
		cout<<frameElapse<<"/"<<fps<<" "<<remain<<"/"<<1000<<endl;
		if(frameElapse>=fps)
		{
			frameElapse-=fps;
			elapse+=1000;
		}
	}
}

void SDL::audioCallBack(void *param,Uint8 *stream,int len)
{
	SDL *self=reinterpret_cast<SDL*>(param);
	self->audioEvent(stream,len);
}
void SDL::audioEvent(Uint8 *stream,int len)
{
	int remain=wavBlock.dataLength-wavBlock.dataOffset;
	int length=remain<len?remain:len;
	//copy
	cout<<wavBlock.dataOffset*100/wavBlock.dataLength<<"%"<<endl;
	memcpy(stream,&wavBlock.dataPointer[wavBlock.dataOffset],length);

	wavBlock.dataOffset+=length;
	if(wavBlock.dataOffset>=wavBlock.dataLength)wavBlock.dataOffset=0;
}*/