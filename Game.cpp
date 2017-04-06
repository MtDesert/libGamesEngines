#include"Game.h"
#include<sys/time.h>
#include<iostream>
using namespace std;

Game::Game(){}

void Game::keyboard_Up(bool pressed){cout<<"Up"<<endl;}
void Game::keyboard_Down(bool pressed){cout<<"Down"<<endl;}
void Game::keyboard_Left(bool pressed){cout<<"Left"<<endl;}
void Game::keyboard_Right(bool pressed){cout<<"Right"<<endl;}

void Game::mouse_Move(int x, int y)
{
	cout<<x<<","<<y<<endl;
}
void Game::mouse_LeftButton(int x,int y,bool){}
void Game::mouse_MiddleButton(int x,int y,bool){}
void Game::mouse_RightButton(int x,int y,bool){}

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
