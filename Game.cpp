#include"Game.h"
#include<sys/time.h>
#include<iostream>
using namespace std;

Game::Game(){}

void Game::keyboard_Up(bool pressed){cout<<"Up"<<endl;}
void Game::keyboard_Down(bool pressed){cout<<"Down"<<endl;}
void Game::keyboard_Left(bool pressed){cout<<"Left"<<endl;}
void Game::keyboard_Right(bool pressed){cout<<"Right"<<endl;}
void Game::keyboard_bigLetter(unsigned char ch){}
void Game::keyboard_smallLetter(unsigned char ch){}

void Game::mouse_Move(int x, int y)
{
	cout<<x<<","<<y<<endl;
}
void Game::mouse_LeftButton(int x,int y,bool){}
void Game::mouse_MiddleButton(int x,int y,bool){}
void Game::mouse_RightButton(int x,int y,bool){}

//joystick
void Game::joystick_Up(bool pressed){cout<<"Joystick:Up"<<endl;}
void Game::joystick_Down(bool pressed){cout<<"Joystick:Down"<<endl;}
void Game::joystick_Left(bool pressed){cout<<"Joystick:Left"<<endl;}
void Game::joystick_Right(bool pressed){cout<<"Joystick:Right"<<endl;}
void Game::joystick_Select(bool pressed){cout<<"Joystick:Select"<<endl;}
void Game::joystick_Start(bool pressed){cout<<"Joystick:Start"<<endl;}
void Game::joystick_A(bool pressed){cout<<"Joystick:A"<<endl;}
void Game::joystick_B(bool pressed){cout<<"Joystick:B"<<endl;}
void Game::joystick_C(bool pressed){cout<<"Joystick:C"<<endl;}
void Game::joystick_X(bool pressed){cout<<"Joystick:X"<<endl;}
void Game::joystick_Y(bool pressed){cout<<"Joystick:Y"<<endl;}
void Game::joystick_Z(bool pressed){cout<<"Joystick:Z"<<endl;}
void Game::joystick_L1(bool pressed){cout<<"Joystick:L1"<<endl;}
void Game::joystick_L2(bool pressed){cout<<"Joystick:L2"<<endl;}
void Game::joystick_R1(bool pressed){cout<<"Joystick:R1"<<endl;}
void Game::joystick_R2(bool pressed){cout<<"Joystick:R2"<<endl;}

void Game::startTicks()
{
	usecRemain=0;
	interval=16666;
	gettimeofday(&oldTimeVal,nullptr);
}
void Game::goTicks()
{
	gettimeofday(&newTimeVal,nullptr);
	uint32 ticks=(newTimeVal.tv_sec-oldTimeVal.tv_sec)*1000000+newTimeVal.tv_usec-oldTimeVal.tv_usec;
	oldTimeVal=newTimeVal;
	usecRemain+=ticks;
	//
	if(usecRemain>=interval)
	{
		needRedraw=true;
		usecRemain-=interval;
	}
}
