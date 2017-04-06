#ifndef ENGINE_GLES_H
#define ENGINE_GLES_H

#include<GLES/gl.h>

class Engine_GLES
{
public:
	Engine_GLES();

	bool initial();
	void redraw();

	GLuint buffer;
	GLuint texture;

	const GLubyte* glVendor;
	const GLubyte* glRenderer;
	const GLubyte* glVersion;
	const GLubyte* glExtension;
};
#endif // ENGINE_GLES_H
