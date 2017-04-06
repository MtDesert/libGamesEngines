#include "Engine_GLUT.h"
#include<GL/glut.h>

Engine_GLUT::Engine_GLUT(){}

/*void timerFunc(int i)
{
	cout<<"timerFunc()"<<endl;
	cout.flush();
	glutPostRedisplay();
	glutTimerFunc(100,&timerFunc,0);
}
void idleFunc()
{
	//cout<<"idleFunc()"<<endl;
	//cout.flush();
}

void keyboardFunc(unsigned char key,int x,int y)
{
	cout<<"keyboardFunc("<<key<<","<<x<<","<<y<<")"<<endl;
	cout.flush();
}
void specialFunc(int key,int x,int y)
{
	cout<<"specialFunc("<<key<<","<<x<<","<<y<<")"<<endl;
	cout.flush();
}
void reshapeFunc(int width,int height)
{
	cout<<"reshapeFunc("<<width<<","<<height<<")"<<endl;
	cout.flush();
	glutSwapBuffers();
	glFlush();
}
void visibilityFunc(int state)
{
	cout<<"visibilityFunc("<<state<<")"<<endl;
	cout.flush();
}
void displayFunc()
{
	cout<<"displayFunc()"<<endl;
	cout.flush();
}
void mouseFunc(int button,int state,int x,int y)
{
	cout<<"mouseFunc("<<button<<","<<state<<","<<x<<","<<y<<")"<<endl;
	cout.flush();
}

int main(int argc,char *argv[])
{
	//EGL egl;
	//egl.initial(xlib.getWindow());
	//xlib.eventLoop();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(400,400);
	glutCreateWindow("Hello World");

	//global call back function
	glutTimerFunc(100,&timerFunc,0);
	//glutIdleFunc(&idleFunc);

	//window specific call back function
	glutKeyboardFunc(&keyboardFunc);
	glutSpecialFunc(&specialFunc);
	glutReshapeFunc(&reshapeFunc);
	glutVisibilityFunc(&visibilityFunc);
	glutDisplayFunc(&displayFunc);
	glutMouseFunc(&mouseFunc);
	//glutMotionFunc( void (* callback)( int, int ) );
	//glutPassiveMotionFunc( void (* callback)( int, int ) );
	//glutEntryFunc( void (* callback)( int ) );

	//event loop
	glutMainLoop();
	return 0;
}×/

void GLUT::initial(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(500,500);
	glutInitWindowSize(400,400);
	glutCreateWindow("Hello World");
	//call back function
	//event loop
	glutMainLoop();
}*/