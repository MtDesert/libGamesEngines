#include "Engine_GLES.h"
#include<GLES/gl.h>

Engine_GLES::Engine_GLES():texture(0),buffer(0){}

bool Engine_GLES::initial()
{
	//string name
	glVendor=glGetString(GL_VENDOR);
	glRenderer=glGetString(GL_RENDERER);
	glVersion=glGetString(GL_VERSION);
	glExtension=glGetString(GL_EXTENSIONS);
	// Initialize GL state.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glBlendFunc(GL_ONE,GL_SRC_COLOR);
	glShadeModel(GL_SMOOTH);

	glGenTextures(1,&texture);
	//if(!glIsTexture(texture))return false;

	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	unsigned char spriteData[]={
		0,0,0,255,
		0,0,255,255,
		0,255,0,255,
		0,255,255,255,
		255,0,0,255,
		255,0,255,255,
		255,255,0,255,
		255,255,255,255,
		0,0,0,255,
		0,0,255,255,
		0,255,0,255,
		0,255,255,255,
		255,0,0,255,
		255,0,255,255,
		255,255,0,255,
		255,255,255,255
	};
	GLsizei width=4;
	GLsizei height=4;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);

	return true;
}
void Engine_GLES::redraw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//glColor4f(0,0,1,1);

	//glBindTexture(GL_TEXTURE_2D,texture);
	GLfloat vertex[]={0,0 , 1,0 , 1,1 , 0,1};
	glVertexPointer(2,GL_FLOAT,0,vertex);

	GLfloat texCoord[]={0,0 , 1,0 , 1,1 , 0,1};
	glTexCoordPointer(2,GL_FLOAT,0,texCoord);

	glDrawArrays(GL_TRIANGLE_FAN,0,4);

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}