#include"SDL2/SDL.h"
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
	auto surface=SDL_CreateWindow("GamesSDL2",0,0,640,480,SDL_WINDOW_RESIZABLE);
	checkError(surface!=nullptr);
	auto timerID=SDL_AddTimer(16,frameTimerCallback,nullptr);
	checkError(timerID!=0);

	//event loop
	SDL_Event event;
	while(SDL_WaitEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				exit(0);
			break;
			case SDL_APP_TERMINATING:break;
			case SDL_APP_LOWMEMORY:break;
			case SDL_APP_WILLENTERBACKGROUND:break;
			case SDL_APP_DIDENTERBACKGROUND:break;
			case SDL_APP_WILLENTERFOREGROUND:break;
			case SDL_APP_DIDENTERFOREGROUND:break;
			/* Window events */
			case SDL_WINDOWEVENT:break;
			case SDL_SYSWMEVENT:break;
			/* Keyboard events */
			case SDL_KEYDOWN:break;
			case SDL_KEYUP:break;
			case SDL_TEXTEDITING:break;
			case SDL_TEXTINPUT:break;
			case SDL_KEYMAPCHANGED:break;
			/* Mouse events */
			case SDL_MOUSEMOTION:break;
			case SDL_MOUSEBUTTONDOWN:break;
			case SDL_MOUSEBUTTONUP:break;
			case SDL_MOUSEWHEEL:break;
			/* Joystick events */
			case SDL_JOYAXISMOTION:break;
			case SDL_JOYBALLMOTION:break;
			case SDL_JOYHATMOTION:break;
			case SDL_JOYBUTTONDOWN:break;
			case SDL_JOYBUTTONUP:break;
			case SDL_JOYDEVICEADDED:break;
			case SDL_JOYDEVICEREMOVED:break;
			/* Game controller events */
			case SDL_CONTROLLERAXISMOTION:break;
			case SDL_CONTROLLERBUTTONDOWN:break;
			case SDL_CONTROLLERBUTTONUP:break;
			case SDL_CONTROLLERDEVICEADDED:break;
			case SDL_CONTROLLERDEVICEREMOVED:break;
			case SDL_CONTROLLERDEVICEREMAPPED:break;
			/* Touch events */
			case SDL_FINGERDOWN:break;
			case SDL_FINGERUP:break;
			case SDL_FINGERMOTION:break;
			/* Gesture events */
			case SDL_DOLLARGESTURE:break;
			case SDL_DOLLARRECORD:break;
			case SDL_MULTIGESTURE:break;
			/* Clipboard events */
			case SDL_CLIPBOARDUPDATE:break;
			/* Drag and drop events */
			case SDL_DROPFILE:break;
			case SDL_DROPTEXT:break;
			case SDL_DROPBEGIN:break;
			case SDL_DROPCOMPLETE:break;
			/* Audio hotplug events */
			case SDL_AUDIODEVICEADDED:break;
			case SDL_AUDIODEVICEREMOVED:break;
			/* Render events */
			case SDL_RENDER_TARGETS_RESET:break;
			case SDL_RENDER_DEVICE_RESET:break;
			default:printf("unknown event type:%d\n",event.type);
		}
	}
	//exit
	SDL_RemoveTimer(timerID);
	SDL_Quit();
	return 0;
}