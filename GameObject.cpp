#include"GameObject.h"
#include<stdio.h>

GameObject::GameObject():timeSlice(0),minTimeSlice(1000),maxTimeSlice(100000){}
GameObject::~GameObject(){clearSubObjects();}

void GameObject::clearSubObjects(){
	for(auto obj:subObjects){
		if(obj)delete obj;
	}
	subObjects.clear();
}

void GameObject::joystickKey(JoystickKey key,bool pressed){
	for(auto obj:subObjects){
		if(obj)obj->joystickKey(key,pressed);
	}
}
void GameObject::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	for(auto obj:subObjects){
		if(obj)obj->keyboardKey(key,pressed);
	}
}
void GameObject::mouseKey(MouseKey key, bool pressed){
	for(auto obj:subObjects){
		if(obj)obj->mouseKey(key,pressed);
	}
}
void GameObject::mouseMove(int x,int y){
	for(auto obj:subObjects){
		if(obj)obj->mouseMove(x,y);
	}
}
void GameObject::mouseWheel(int angle){
	for(auto obj:subObjects){
		if(obj)obj->mouseWheel(angle);
	}
}
void GameObject::addTimeSlice(uint usec){
	timeSlice += usec;
	if(timeSlice>maxTimeSlice){//限制最大时间片
		timeSlice=maxTimeSlice;
	}
	if(timeSlice >= minTimeSlice){//试图消耗时间片
		timeSlice -= minTimeSlice;
		consumeTimeSlice();
	}
	//向下传递
	for(auto *obj:subObjects){
		if(obj)obj->addTimeSlice(usec);
	}
}
void GameObject::render()const{
	for(auto obj:subObjects){
		if(obj)obj->render();
	}
}

void GameObject::consumeTimeSlice(){}