#include <GLFW/glfw3.h>
#include<stdio.h>
#include<string.h>
#include<string>
using namespace std;
#include"Game.h"

Game game;

static int monitorsCount=0;
static GLFWmonitor** monitors=NULL;
static GLFWmonitor* primaryMonitor=NULL;

static int videoModesCount;
static const GLFWvidmode* videoModes=NULL;
static const GLFWvidmode* currentMode=NULL;

void printMonitor(int id,GLFWmonitor &monitor){
	int xpos,ypos,width,height;
	auto monitorName=glfwGetMonitorName(&monitor);
	glfwGetMonitorPos(&monitor,&xpos,&ypos);
	glfwGetMonitorPhysicalSize(&monitor,&width,&height);
	printf("Monitor[%d]: %s pos=%d,%d width*height=%d*%d",id,monitorName,xpos,ypos,width,height);
	if(&monitor==primaryMonitor){
		printf(" (Primary)");
	}
	printf("\n");
	
	//video mode of monitor
	videoModes=glfwGetVideoModes(&monitor,&videoModesCount);
	printf("Video Mode: %d\n",videoModesCount);
	currentMode=glfwGetVideoMode(&monitor);
	for(int j=0;j<videoModesCount;++j){
		auto mode=&videoModes[j];
		printf("mode[%d]: width*height=%d*%d bit(r,g,b)=%d,%d,%d refresh rate=%2d",
			j,mode->width,mode->height,
			mode->redBits,mode->greenBits,mode->blueBits,mode->refreshRate);
		if(memcmp(currentMode,mode,sizeof(GLFWvidmode))==0){
			printf(" (Current)");
		}
		printf("\n");
	}
}

string string_ClientAPI(int api){
	switch(api){
		case GLFW_OPENGL_API:return "OpenGL";
		case GLFW_OPENGL_ES_API:return "OpenGL ES";
		default:return "Unknown";
	}
}
string string_Robustness(int robustness){
	switch(robustness){
		case GLFW_NO_ROBUSTNESS:return "No robustness";
		case GLFW_NO_RESET_NOTIFICATION:return "No reset notification";
		case GLFW_LOSE_CONTEXT_ON_RESET:return "Lose context on reset";
		default:return "Unknown";
	}
}
string string_Profile(int profile){
	switch(profile){
		case GLFW_OPENGL_ANY_PROFILE:return "Any profile";
		case GLFW_OPENGL_CORE_PROFILE:return "Core profile";
		case GLFW_OPENGL_COMPAT_PROFILE:return "Compat profile";
		default:return "Unknown";
	}
}
string string_InputMethod_Cursor(int cursor){
	switch(cursor){
		case GLFW_CURSOR_NORMAL:return "Normal";
		case GLFW_CURSOR_HIDDEN:return "Hidden";
		case GLFW_CURSOR_DISABLED:return "Disabled";
		default:return "Unknown";
	}
}

void printWindowAttrib(GLFWwindow &window){
	printf("Focused: %d\n",glfwGetWindowAttrib(&window,GLFW_FOCUSED));
	printf("Iconified: %d\n",glfwGetWindowAttrib(&window,GLFW_ICONIFIED));
	printf("Resizable: %d\n",glfwGetWindowAttrib(&window,GLFW_RESIZABLE));
	printf("Visible: %d\n",glfwGetWindowAttrib(&window,GLFW_VISIBLE));
	printf("Decorated: %d\n",glfwGetWindowAttrib(&window,GLFW_DECORATED));
	
	printf("Clinet API: %s\n",string_ClientAPI(glfwGetWindowAttrib(&window,GLFW_CLIENT_API)).data());
	printf("Context Version Major: %d\n",glfwGetWindowAttrib(&window,GLFW_CONTEXT_VERSION_MAJOR));
	printf("Context Version Minor: %d\n",glfwGetWindowAttrib(&window,GLFW_CONTEXT_VERSION_MINOR));
	printf("Context Revision: %d\n",glfwGetWindowAttrib(&window,GLFW_CONTEXT_REVISION));
	printf("Context Robustness: %s\n",string_Robustness(glfwGetWindowAttrib(&window,GLFW_CONTEXT_ROBUSTNESS)).data());
	printf("OpenGL Forward Compat: %d\n",glfwGetWindowAttrib(&window,GLFW_OPENGL_FORWARD_COMPAT));
	printf("OpenGL Debug Context: %d\n",glfwGetWindowAttrib(&window,GLFW_OPENGL_DEBUG_CONTEXT));
	printf("OpenGL Profile: %s\n",string_Profile(glfwGetWindowAttrib(&window,GLFW_OPENGL_PROFILE)).data());
}

//callback functions
void errorFun(int error,const char *description){
	printf("error: %d, %s\n",error,description);
}
void monitorFun(GLFWmonitor *monitor,int event){
	if(event==GLFW_CONNECTED){
		printf("Monitor connected\n");
	}
	else if(event==GLFW_DISCONNECTED){
		printf("Monitor disconnected\n");
	}
	else{
		printf("Monitor connect???\n");
	}
}

void windowPosFun(GLFWwindow *window,int xpos,int ypos){printf("pos %d,%d\n",xpos,ypos);}
void windowSizeFun(GLFWwindow *window,int width,int height){printf("size %d,%d\n",width,height);}
void windowCloseFun(GLFWwindow *window){printf("windowCloseFun()\n");}
void windowRefreshFun(GLFWwindow *window){printf("windowRefreshFun()\n");}
void windowFocusFun(GLFWwindow *window,int focused){printf("windowFocusFun()\n");}
void windowIconifyFun(GLFWwindow *window,int iconified){printf("windowIconifyFun()\n");}
void frameBufferSizeFun(GLFWwindow *window,int width,int height){printf("frameBufferSizeFun()\n");}

void keyCallback(int key,bool pressed){
	switch(key){
		/*case GLFW_KEY_UP:game.keyboard_Up(pressed);break;
		case GLFW_KEY_DOWN:game.keyboard_Down(pressed);break;
		case GLFW_KEY_LEFT:game.keyboard_Left(pressed);break;
		case GLFW_KEY_RIGHT:game.keyboard_Right(pressed);break;*/
	}
}
void glfwKeyCallback(GLFWwindow *window,int key,int scancode,int action,int mods){
	//printf("key==%d scancode==%d action==%d mods==%d\n",key,scancode,action,mods);
	switch(action){
		case GLFW_PRESS:keyCallback(key,true);break;
		case GLFW_RELEASE:keyCallback(key,false);break;
		default:;
	}
	switch(mods){
		case GLFW_MOD_ALT:case GLFW_MOD_CONTROL:case GLFW_MOD_SHIFT:case GLFW_MOD_SUPER:break;
		default:;
	}
}

int main(int argc,char* argv[])
{
	int major,minor,rev;
	glfwGetVersion(&major,&minor,&rev);
	printf("Version: %d.%d.%d\n",major,minor,rev);
	printf("Version string: %s\n",glfwGetVersionString());
	if (!glfwInit())return -1;
	//callback
	glfwSetErrorCallback(errorFun);
	glfwSetMonitorCallback(monitorFun);

	//monitor
	monitors=glfwGetMonitors(&monitorsCount);
	printf("Monitors count: %d\n",monitorsCount);
	primaryMonitor=glfwGetPrimaryMonitor();
	for(int i=0;i<monitorsCount;++i){
		printMonitor(i,*monitors[i]);
	}
	
	glfwDefaultWindowHints();
	
	/* Create a windowed mode window and its OpenGL context */
	auto window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
	if(window){
		int xpos,ypos,width,height;
		glfwGetWindowPos(window,&xpos,&ypos);
		glfwGetWindowSize(window,&width,&height);
		printf("Window pos=%d,%d size=%d,%d\n",xpos,ypos,width,height);
		glfwGetFramebufferSize(window,&width,&height);
		printf("Frame buffer size=%d,%d\n",width,height);
		//monitor
		auto monitor=glfwGetWindowMonitor(window);
		if(monitor){
			int id=0;
			for(;id<monitorsCount;++id){
				if(monitor==monitors[id])break;
			}
			printMonitor(id,*monitor);
		}
		printWindowAttrib(*window);
		//window callback
		glfwSetWindowPosCallback(window,windowPosFun);
		glfwSetWindowSizeCallback(window,windowSizeFun);
		glfwSetWindowCloseCallback(window,windowCloseFun);
		glfwSetWindowRefreshCallback(window,windowRefreshFun);
		glfwSetWindowFocusCallback(window,windowFocusFun);
		glfwSetWindowIconifyCallback(window,windowIconifyFun);
		glfwSetFramebufferSizeCallback(window,frameBufferSizeFun);
		//input callback
		glfwSetKeyCallback(window,glfwKeyCallback);
		
		printf("%s\n",string_InputMethod_Cursor(glfwGetInputMode(window,GLFW_CURSOR)).data());
		printf("%.8X\n",glfwGetInputMode(window,GLFW_STICKY_KEYS));
		printf("%.8X\n",glfwGetInputMode(window,GLFW_STICKY_MOUSE_BUTTONS));
		
		//loop
		glfwMakeContextCurrent(window);
		while (!glfwWindowShouldClose(window))// Loop until the user closes the window
		{
			/* Draw a triangle */
			glBegin(GL_TRIANGLES);
			
			glColor3f(1.0, 0.0, 0.0);    // Red
			glVertex3f(0.0, 1.0, 0.0);
			
			glColor3f(0.0, 1.0, 0.0);    // Green
			glVertex3f(-1.0, -1.0, 0.0);
			
			glColor3f(0.0, 0.0, 1.0);    // Blue
			glVertex3f(1.0, -1.0, 0.0);
			
			glEnd();
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			
			/* Poll for and process events */
			glfwPollEvents();
		}
		glfwDestroyWindow(window);
	}
	
	glfwTerminate();
	return 0;
}