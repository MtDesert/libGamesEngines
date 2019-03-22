#include"GameObject.h"
#include<stdio.h>

GameObject::GameObject():timeSlice(0),minTimeSlice(16000),maxTimeSlice(100000){}
GameObject::~GameObject(){clearSubObjects();}

#define FOR_SUB_OBJECTS(code)\
for(auto obj:subObjects){\
	if(obj){code;}\
}

void GameObject::clearSubObjects(){
	//FOR_SUB_OBJECTS(delete obj;)
	subObjects.clear();
}

void GameObject::reset(){
	FOR_SUB_OBJECTS(obj->reset())
}
void GameObject::joystickKey(JoystickKey key,bool pressed){
	FOR_SUB_OBJECTS(obj->joystickKey(key,pressed))
}
void GameObject::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	FOR_SUB_OBJECTS(obj->keyboardKey(key,pressed))
}
void GameObject::mouseKey(MouseKey key, bool pressed){
	FOR_SUB_OBJECTS(obj->mouseKey(key,pressed))
}
void GameObject::mouseMove(int x,int y){
	FOR_SUB_OBJECTS(obj->mouseMove(x,y))
}
void GameObject::mouseWheel(int angle){
	FOR_SUB_OBJECTS(obj->mouseWheel(angle))
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
	FOR_SUB_OBJECTS(obj->addTimeSlice(usec))
}
void GameObject::render()const{
	FOR_SUB_OBJECTS(obj->render());
}

void GameObject::consumeTimeSlice(){}