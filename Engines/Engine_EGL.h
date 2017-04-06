#ifndef EGL_H
#define EGL_H

#include<EGL/egl.h>

class Engine_EGL
{
public:
	Engine_EGL();

	bool initial(EGLNativeWindowType);
	bool terminate();
	void swap();
//private:
	EGLDisplay eglDisplay;

	EGLint versionMajor;
	EGLint versionMinor;
	EGLBoolean initializeOK;

	EGLConfig eglConfig;
	EGLint configNum;
	EGLBoolean getConfigsOK;

	EGLint configChoose;
	EGLBoolean chooseConfigOK;

	EGLint eglNativeVisualID;

	EGLSurface eglSurface;
	EGLContext eglContext;
	EGLBoolean makeCurrentOK;

	EGLBoolean bindApiOK;

	int width,height;
};
#endif // EGL_H
