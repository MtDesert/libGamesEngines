#include"Game.h"
#include<GL/glut.h>
#include<stdio.h>

Game *game=nullptr;
const Keyboard keyboard;

enum{
	TimerCPU,
	TimerGPU,
	Timer_Amount
};
int timerInterval[Timer_Amount]={1,16};//microsecond

void glutTimerFunction(int timerID){
	switch(timerID){
		case TimerCPU:
			game->addTimeSlice(1000*timerInterval[timerID]);
		break;
		case TimerGPU:
			game->render();
			glFlush();
		break;
	}
	glutTimerFunc(timerInterval[timerID],glutTimerFunction,timerID);
}
void glutIdleFunction(){}

//input-keyboard
void keyboardFunction(unsigned char key,bool pressed){
	auto k=keyboard.asciiKey[key];
	if(k<Keyboard::Amount_KeyboardKey){
		game->keyboardKey(k,pressed);
	}else{
		printf("keyboardFunction: unknown %d\n",key);
	}
}
void glutKeyboardFunction(unsigned char key,int x,int y){keyboardFunction(key,true);}
void glutKeyboardUpFunction(unsigned char key,int x,int y){keyboardFunction(key,false);}

//input-special key
#define CASE(num) case GLUT_KEY_F##num:k=Keyboard::Key_F##num;break;
void specialFunction(int key,bool pressed){
	Keyboard::KeyboardKey k=Keyboard::Amount_KeyboardKey;
	switch(key){
		case GLUT_KEY_UP:k=Keyboard::Key_Up;break;
		case GLUT_KEY_DOWN:k=Keyboard::Key_Down;break;
		case GLUT_KEY_LEFT:k=Keyboard::Key_Left;break;
		case GLUT_KEY_RIGHT:k=Keyboard::Key_Right;break;
		CASE(1)
		CASE(2)
		CASE(3)
		CASE(4)
		CASE(5)
		CASE(6)
		CASE(7)
		CASE(8)
		CASE(9)
		CASE(10)
		CASE(11)
		CASE(12)
		case GLUT_KEY_PAGE_UP:k=Keyboard::Key_PageUp;break;
		case GLUT_KEY_PAGE_DOWN:k=Keyboard::Key_PageDown;break;
		case GLUT_KEY_HOME:k=Keyboard::Key_Home;break;
		case GLUT_KEY_END:k=Keyboard::Key_End;break;
		case GLUT_KEY_INSERT:k=Keyboard::Key_Insert;break;
		case 0x70:k=Keyboard::Key_ShiftL;break;
		case 0x71:k=Keyboard::Key_ShiftR;break;
		case 0x72:k=Keyboard::Key_CtrlL;break;
		case 0x73:k=Keyboard::Key_CtrlR;break;
		case 0x74:k=Keyboard::Key_AltL;break;
		case 0x75:k=Keyboard::Key_AltR;break;
		default:printf("specialFunction: unknown key %.8X\n",key);//do nothing
	}
	game->keyboardKey(k,pressed);
}
#undef CASE
void glutSpecialFunction(int key,int x,int y){specialFunction(key,true);}
void glutSpecialUpFunction(int key,int x,int y){specialFunction(key,false);}

//input-mouse
void glutMouseFunction(int button,int state,int x,int y){
	switch(button){
		case GLUT_LEFT_BUTTON:game->mouseKey(Game::Mouse_LeftButton,state==GLUT_DOWN);break;
		case GLUT_MIDDLE_BUTTON:game->mouseKey(Game::Mouse_MiddleButton,state==GLUT_DOWN);break;
		case GLUT_RIGHT_BUTTON:game->mouseKey(Game::Mouse_RightButton,state==GLUT_DOWN);break;
		case 3:game->mouseWheel(1);break;//mouse wheel roll frontward
		case 4:game->mouseWheel(-1);break;//mouse wheel roll backward
		default:printf("unknown mouse button: %d\n",button);
	}
}
void glutMotionFunction(int x,int y){
	game->mouseMove(x,y);
}
void glutPassiveMotionFunction(int x,int y){
	game->mouseMove(x,y);
}
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
void glutButtonBoxFunction(int button,int state){printf("glutButtonBoxFunction\n");}
void glutDialsFunction(int dial, int value){printf("glutDialsFunction\n");}
void glutTabletMotionFunction(int x,int y){printf("glutTabletMotionFunction\n");}
void glutTabletButtonFunction(int button, int state,int x, int y){printf("glutTabletButtonFunction\n");}

void glutMenuStateFunction(int status){printf("glutMenuStateFunction\n");}
void glutMenuStatusFunction(int status,int x,int y){printf("glutMenuStatusFunction\n");}
void glutWindowStatusFunction(int status){printf("glutWindowStatusFunction\n");}

//event
void glutReshapeFunction(int width,int height){printf("glutReshapeFunction\n");}
void glutVisibilityFunction(int state){
	switch(state){
		case GLUT_VISIBLE:break;
		case GLUT_NOT_VISIBLE:break;
		default:;
	}
}
void glutDisplayFunction(){}
void glutOverlayDisplayFunction(){}
void glutEntryFunction(int state){
	switch(state){
		case GLUT_LEFT:break;
		case GLUT_ENTERED:break;
		default:;
	}
}

void printGlutGet(){
	printf("Window:\n");
	printf("X,Y==%d,%d W*H==%d*%d\n",glutGet(GLUT_WINDOW_X),glutGet(GLUT_WINDOW_Y),glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
	printf("Buffer Size: %d\n",glutGet(GLUT_WINDOW_BUFFER_SIZE));
	printf("Stencil Size: %d\n",glutGet(GLUT_WINDOW_STENCIL_SIZE));
	printf("Depth Size: %d\n",glutGet(GLUT_WINDOW_DEPTH_SIZE));
	printf("Size(R,G,B,A): %d,%d,%d,%d\n",glutGet(GLUT_WINDOW_RED_SIZE),glutGet(GLUT_WINDOW_GREEN_SIZE),glutGet(GLUT_WINDOW_BLUE_SIZE),glutGet(GLUT_WINDOW_ALPHA_SIZE));
	printf("AccumSize(R,G,B,A): %d,%d,%d,%d\n",glutGet(GLUT_WINDOW_ACCUM_RED_SIZE),glutGet(GLUT_WINDOW_ACCUM_GREEN_SIZE),glutGet(GLUT_WINDOW_ACCUM_BLUE_SIZE),glutGet(GLUT_WINDOW_ACCUM_ALPHA_SIZE));
	printf("DoubleBuffer: %d\n",glutGet(GLUT_WINDOW_DOUBLEBUFFER));
	printf("RGBA: %d\n",glutGet(GLUT_WINDOW_RGBA));
	printf("Parent: %d\n",glutGet(GLUT_WINDOW_PARENT));
	printf("NumChildren: %d\n",glutGet(GLUT_WINDOW_NUM_CHILDREN));
	printf("ColorMap Size: %d\n",glutGet(GLUT_WINDOW_COLORMAP_SIZE));
	printf("Num Sample: %d\n",glutGet(GLUT_WINDOW_NUM_SAMPLES));
	printf("Stereo: %d\n",glutGet(GLUT_WINDOW_STEREO));
	printf("Cursor: %d\n",glutGet(GLUT_WINDOW_CURSOR));
	printf("FormatID: %d\n",glutGet(GLUT_WINDOW_FORMAT_ID));

	printf("\nScreen(W*H)==%d*%d %dmm*%dmm\n",glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT),glutGet(GLUT_SCREEN_WIDTH_MM),glutGet(GLUT_SCREEN_HEIGHT_MM));
	printf("Menu Num Item: %d\n",glutGet(GLUT_MENU_NUM_ITEMS));
	printf("Display Mode Possible: %d\n",glutGet(GLUT_MENU_NUM_ITEMS));
	printf("InitWindow: X,Y==%d,%d W*H==%d*%d\n",glutGet(GLUT_INIT_WINDOW_X),glutGet(GLUT_INIT_WINDOW_Y),glutGet(GLUT_INIT_WINDOW_WIDTH),glutGet(GLUT_INIT_WINDOW_HEIGHT));
	printf("Init Display Mode: %d\n",glutGet(GLUT_INIT_DISPLAY_MODE));
	printf("Elapsed Time: %d\n",glutGet(GLUT_ELAPSED_TIME));
}
void printGlutDeviceGet(){
	printf("\nDevice:\n");
	printf("Has Keyboard: %d\n",glutDeviceGet(GLUT_HAS_KEYBOARD));
	printf("Has Mouse: %d\n",glutDeviceGet(GLUT_HAS_MOUSE));
	printf("Has SpaceBall: %d\n",glutDeviceGet(GLUT_HAS_SPACEBALL));
	printf("Has Dial and Button Box: %d\n",glutDeviceGet(GLUT_HAS_DIAL_AND_BUTTON_BOX));
	printf("Has Tablet: %d\n",glutDeviceGet(GLUT_HAS_TABLET));
	printf("Has Joystick: %d\n",glutDeviceGet(GLUT_HAS_JOYSTICK));
	printf("Owns Joystick: %d\n",glutDeviceGet(GLUT_OWNS_JOYSTICK));
	printf("Joystick Buttons: %d\n",glutDeviceGet(GLUT_JOYSTICK_BUTTONS));
	printf("Joystick Axes: %d\n",glutDeviceGet(GLUT_JOYSTICK_AXES));
	printf("Joystick Poll Rate: %d\n",glutDeviceGet(GLUT_JOYSTICK_POLL_RATE));
	printf("Num Mouse Buttons: %d\n",glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS));
	printf("Num Spaceball Buttons: %d\n",glutDeviceGet(GLUT_NUM_SPACEBALL_BUTTONS));
	printf("Num Button Box Buttons: %d\n",glutDeviceGet(GLUT_NUM_BUTTON_BOX_BUTTONS));
	printf("Num Dials: %d\n",glutDeviceGet(GLUT_NUM_DIALS));
	printf("Num Tablet Buttons: %d\n",glutDeviceGet(GLUT_NUM_TABLET_BUTTONS));
	printf("Device ignore key repeat: %d\n",glutDeviceGet(GLUT_DEVICE_IGNORE_KEY_REPEAT));
	printf("Device key repeat: %d\n",glutDeviceGet(GLUT_DEVICE_KEY_REPEAT));
}

#define GAMESGLUT_GLUTFUNC(name) glut##name##Func(glut##name##Function)

int main(int argc,char* argv[]){
	game=Game::newGame();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	int window=glutCreateWindow("GamesGLUT");
	//callback
	glutTimerFunc(timerInterval[TimerCPU],glutTimerFunction,TimerCPU);
	glutTimerFunc(timerInterval[TimerGPU],glutTimerFunction,TimerGPU);
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

	printGlutGet();
	printGlutDeviceGet();

	//main loop
	glutMainLoop();
	glutDestroyWindow(window);
	delete game;
	return 0;
}