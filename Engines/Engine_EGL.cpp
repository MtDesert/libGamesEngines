#include "Engine_EGL.h"

Engine_EGL::Engine_EGL(){}

bool Engine_EGL::initial(EGLNativeWindowType type)
{
	eglDisplay=eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(eglDisplay==EGL_NO_DISPLAY||eglGetError()!=EGL_SUCCESS)return false;

	initializeOK=eglInitialize(eglDisplay,&versionMajor,&versionMinor);
	if(!initializeOK)return false;

	getConfigsOK=eglGetConfigs(eglDisplay,nullptr,0,&configNum);
	if(!getConfigsOK)return false;

	getConfigsOK=eglGetConfigs(eglDisplay,&eglConfig,configNum,&configNum);
	if(!getConfigsOK)return false;

	chooseConfigOK=eglChooseConfig(eglDisplay,nullptr,&eglConfig,configNum,&configChoose);
	if(!chooseConfigOK)return false;

	//eglGetConfigAttrib(eglDisplay,eglConfig,EGL_NATIVE_VISUAL_ID,&eglNativeVisualID);

	eglSurface=eglCreateWindowSurface(eglDisplay,eglConfig,type,nullptr);
	if(eglSurface==EGL_NO_SURFACE)return false;

	eglContext=eglCreateContext(eglDisplay,eglConfig,EGL_NO_CONTEXT,nullptr);
	if(eglContext==EGL_NO_CONTEXT)return false;

	makeCurrentOK=eglMakeCurrent(eglDisplay,eglSurface,eglSurface,eglContext);
	if(!makeCurrentOK)return false;

	bindApiOK=eglBindAPI(EGL_OPENGL_ES_API);
	if(!bindApiOK)return false;

	eglQuerySurface(eglDisplay,eglSurface,EGL_WIDTH,&width);
	eglQuerySurface(eglDisplay,eglSurface,EGL_HEIGHT,&height);

	return true;
}
bool Engine_EGL::terminate()
{
	if (eglDisplay != EGL_NO_DISPLAY) {
		eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (eglContext != EGL_NO_CONTEXT) {
			eglDestroyContext(eglDisplay,eglContext);
		}
		if (eglSurface != EGL_NO_SURFACE) {
			eglDestroySurface(eglDisplay,eglSurface);
		}
		eglTerminate(eglDisplay);
	}
	eglDisplay = EGL_NO_DISPLAY;
	eglContext = EGL_NO_CONTEXT;
	eglSurface = EGL_NO_SURFACE;
}
void Engine_EGL::swap()
{
	eglSwapBuffers(eglDisplay,eglSurface);
}
