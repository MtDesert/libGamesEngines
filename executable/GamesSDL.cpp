#include"SDL/SDL.h"
#include<stdio.h>

Uint32 frameTimerCallback(Uint32 interval,void *param){
	return interval;
}

void checkError(bool b){
	if(!b){
		printf("%s\n",SDL_GetError());
		exit(-1);
	}
}

int main(int argc,char* argv[]){
	atexit(SDL_Quit);
	checkError(SDL_Init(SDL_INIT_EVERYTHING)==0);
	auto surface=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	checkError(surface!=nullptr);
	auto timerID=SDL_AddTimer(16,frameTimerCallback,nullptr);
	checkError(timerID!=nullptr);

	//event loop
	SDL_Event event;
	while(SDL_WaitEvent(&event)){
		switch(event.type){
			case SDL_ACTIVEEVENT:break;
			case SDL_KEYDOWN:break;
			case SDL_KEYUP:break;
			case SDL_MOUSEMOTION:break;
			case SDL_MOUSEBUTTONDOWN:break;
			case SDL_MOUSEBUTTONUP:break;
			case SDL_JOYAXISMOTION:break;
			case SDL_JOYBALLMOTION:break;
			case SDL_JOYHATMOTION:break;
			case SDL_JOYBUTTONDOWN:break;
			case SDL_JOYBUTTONUP:break;
			case SDL_QUIT:
				exit(0);
			break;
			case SDL_SYSWMEVENT:break;
			case SDL_VIDEORESIZE:break;
			case SDL_VIDEOEXPOSE:break;
			default:printf("unknown event type:%d\n",event.type);
		}
	}
	//exit
	SDL_RemoveTimer(timerID);
	SDL_Quit();
	return 0;
}