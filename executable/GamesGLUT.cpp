#include <GL/glut.h>
#include<stdio.h>
#include"Game.h"

Game game;

void glutTimerFunction(unsigned int time,void (*callback)(int),int value){}
void glutIdleFunction(){}

//input-keyboard
void keyboardFunction(unsigned char key,bool pressed){
	if('A'<=key&&key<='Z')game.keyboard_bigLetter(key);
	else if('a'<=key&&key<='z')game.keyboard_smallLetter(key);
}
void glutKeyboardFunction(unsigned char key,int x,int y){keyboardFunction(key,true);}
void glutKeyboardUpFunction(unsigned char key,int x,int y){keyboardFunction(key,false);}

//input-special key
void specialFunction(int key,bool pressed){
	switch(key){
		case GLUT_KEY_UP:game.keyboard_Up(pressed);break;
		case GLUT_KEY_DOWN:game.keyboard_Down(pressed);break;
		case GLUT_KEY_LEFT:game.keyboard_Left(pressed);break;
		case GLUT_KEY_RIGHT:game.keyboard_Right(pressed);break;
		default:;//do nothing
	}
}
void glutSpecialFunction(int key,int x,int y){specialFunction(key,true);}
void glutSpecialUpFunction(int key,int x,int y){specialFunction(key,false);}

//input-mouse
void glutMouseFunction(int button,int state,int x,int y){
	switch(button){
		case GLUT_LEFT_BUTTON:break;
		case GLUT_MIDDLE_BUTTON:break;
		case GLUT_RIGHT_BUTTON:break;
		default:;
	}
	switch(state){
		case GLUT_UP:break;
		case GLUT_DOWN:break;
		default:;
	}
}
void glutMotionFunction(int x,int y){}
void glutPassiveMotionFunction(int x,int y){}
//input-joystick
void glutJoystickFunction(unsigned int buttonMask,int x, int y, int z){
	if(buttonMask&GLUT_JOYSTICK_BUTTON_A){}
	if(buttonMask&GLUT_JOYSTICK_BUTTON_B){}
	if(buttonMask&GLUT_JOYSTICK_BUTTON_C){}
	if(buttonMask&GLUT_JOYSTICK_BUTTON_D){}
}
//input-spaceball
void glutSpaceballMotionFunction(int x,int y,int z){}
void glutSpaceballRotateFunction(int x,int y,int z){}
void glutSpaceballButtonFunction(int button,int state){}
//input-others
void glutButtonBoxFunction(int button,int state){}
void glutDialsFunction(int dial, int value){}
void glutTabletMotionFunction(int x,int y){}
void glutTabletButtonFunction(int button, int state,int x, int y){}

void glutMenuStateFunction(int status){}
void glutMenuStatusFunction(int status,int x,int y){}
void glutWindowStatusFunction(int status){}

//event
void glutReshapeFunction(int width,int height){}
void glutVisibilityFunction(int state){
	switch(state){
		case GLUT_VISIBLE:break;
		case GLUT_NOT_VISIBLE:break;
		default:;
	}
}
void glutDisplayFunction()
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f (0.25, 0.25, 0.0);
	glVertex3f (0.75, 0.25, 0.0);
	glVertex3f (0.75, 0.75, 0.0);
	glVertex3f (0.25, 0.75, 0.0);
	glEnd();
	glFlush();
}
void glutOverlayDisplayFunction(){}
void glutEntryFunction(int state){
	switch(state){
		case GLUT_LEFT:break;
		case GLUT_ENTERED:break;
		default:;
	}
}

#define GAMESGLUT_GLUTFUNC(name) glut##name##Func(glut##name##Function)

int main(int argc,char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	int window=glutCreateWindow("GamesGLUT");
	//callback
	GAMESGLUT_GLUTFUNC(Idle);
	//callback-input
	GAMESGLUT_GLUTFUNC(Keyboard);
	GAMESGLUT_GLUTFUNC(KeyboardUp);
	GAMESGLUT_GLUTFUNC(Special);
	GAMESGLUT_GLUTFUNC(SpecialUp);
	GAMESGLUT_GLUTFUNC(Mouse);
	GAMESGLUT_GLUTFUNC(Motion);
	GAMESGLUT_GLUTFUNC(PassiveMotion);
	glutJoystickFunc(glutJoystickFunction,1);
	//callback-spaceball
	GAMESGLUT_GLUTFUNC(SpaceballMotion);
	GAMESGLUT_GLUTFUNC(SpaceballRotate);
	GAMESGLUT_GLUTFUNC(SpaceballButton);
	//callback-others
	GAMESGLUT_GLUTFUNC(ButtonBox);
	GAMESGLUT_GLUTFUNC(Dials);
	GAMESGLUT_GLUTFUNC(TabletMotion);
	GAMESGLUT_GLUTFUNC(TabletButton);
	
	GAMESGLUT_GLUTFUNC(MenuState);
	GAMESGLUT_GLUTFUNC(MenuStatus);
	GAMESGLUT_GLUTFUNC(WindowStatus);
	//callback-output
	GAMESGLUT_GLUTFUNC(Reshape);
	GAMESGLUT_GLUTFUNC(Visibility);
	GAMESGLUT_GLUTFUNC(Display);
	GAMESGLUT_GLUTFUNC(OverlayDisplay);
	GAMESGLUT_GLUTFUNC(Entry);
	//main loop
	glutMainLoop();
	glutDestroyWindow(window);
	return 0;
}