#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"typedef.h"
#include"List.h"

//这是所有的手柄按键,根据需求可能有更多的按键
#define JOYSTICK_ALLKEY(MACRO)\
	MACRO(Up)\
	MACRO(Down)\
	MACRO(Left)\
	MACRO(Right)\
	MACRO(Select)\
	MACRO(Start)\
	MACRO(A)\
	MACRO(B)\
	MACRO(C)\
	MACRO(X)\
	MACRO(Y)\
	MACRO(Z)\
	MACRO(L1)\
	MACRO(L2)\
	MACRO(R1)\
	MACRO(R2)\

//这是所有的键盘按键,根据需求可能有更多的按键
#define KEYBOARD_ALLKEY(MACRO)\
	MACRO(Esc)\
	MACRO(F1)\
	MACRO(F2)\
	MACRO(F3)\
	MACRO(F4)\
	MACRO(F5)\
	MACRO(F6)\
	MACRO(F7)\
	MACRO(F8)\
	MACRO(F9)\
	MACRO(F10)\
	MACRO(F11)\
	MACRO(F12)\
	MACRO(PrintScreen)\
	MACRO(ScrollLock)\
	MACRO(Pause)\
	MACRO(QuoteLeft)\
	MACRO(0) MACRO(1) MACRO(2) MACRO(3) MACRO(4) MACRO(5) MACRO(6) MACRO(7) MACRO(8) MACRO(9)\
	MACRO(Minus) MACRO(Equal) MACRO(BacksLash) MACRO(Backspace)\
	MACRO(A) MACRO(B) MACRO(C) MACRO(D) MACRO(E) MACRO(F) MACRO(G)\
	MACRO(H) MACRO(I) MACRO(J) MACRO(K) MACRO(L) MACRO(M) MACRO(N)\
	MACRO(O) MACRO(P) MACRO(Q) MACRO(R) MACRO(S) MACRO(T)\
	MACRO(U) MACRO(V) MACRO(W) MACRO(X) MACRO(Y) MACRO(Z)\
	MACRO(Up)\
	MACRO(Down)\
	MACRO(Left)\
	MACRO(Right)\
	MACRO(PageUp)\
	MACRO(PageDown)\
	MACRO(Home)\
	MACRO(End)\
	MACRO(Insert)\
	MACRO(Delete)\

#define MOUSE_ALLKEY(MACRO)\
	MACRO(LeftButton)\
	MACRO(MiddleButton)\
	MACRO(RightButton)\

/** @brief GameObject 游戏中的物体
 * 游戏中的场景、图层、精灵等都可以看作物体,当然也可以不继承此类
 * 此类的作用主要是输入事件的传递,比如键盘事件,鼠标事件,手柄事件等
 * 此类还可以基于时间片来进行响应(类似于timer机制),时钟其实也是一种输入
 */
class GameObject{
public:
	GameObject();
	virtual ~GameObject();

	//手柄按键枚举
#define JOYSTICK_ENUM(key) Joystick_##key,
	enum JoystickKey{
		JOYSTICK_ALLKEY(JOYSTICK_ENUM)
		Amount_JoystickKey
	};

	//键盘按键枚举
#define KEYBOARD_ENUM(key) Keyboard_##key,
	enum KeyboardKey{
		KEYBOARD_ALLKEY(KEYBOARD_ENUM)
		Amount_KeyboardKey
	};
	
	//鼠标按键枚举
#define MOUSE_ENUM(key) Mouse_##key,
	enum MouseKey{
		MOUSE_ALLKEY(MOUSE_ENUM)
		Amount_MouseKey
	};
	
	//保存子类的容器,主要用于事件传递
	List<GameObject*> subObjects;
	void clearSubObjects();

	//传递手柄按键事件,如需要响应事件,请在子类重写
	virtual void joystickKey(JoystickKey key,bool pressed);
	virtual void keyboardKey(KeyboardKey key,bool pressed);
	virtual void mouseKey(MouseKey key,bool pressed);
	//鼠标有移动事件和滚轮事件
	virtual void mouseMove(int x,int y);//鼠标移动
	virtual void mouseWheel(int angle);//单位为角度
	
	//时间片,一些基于时间的运动(如匀速移动)
	void addTimeSlice(uint usec);//以毫秒为单位增加时间片,并且试图消耗时间片
protected:
	virtual void consumeTimeSlice();//消耗时间片
	uint timeSlice;//目前累积的时间片
	uint minTimeSlice;//timeSlice超过此值时可调用consumeTimeSlice()
	uint maxTimeSlice;//timeSlice超过此值则会被限制为此值
};

#endif