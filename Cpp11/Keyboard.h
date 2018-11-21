#ifndef KEYBOARD_H
#define KEYBOARD_H

#include"typedef.h"

//这是所有的键盘按键,根据需求可能有更多的按键
#define KEYBOARD_ALLKEY(MACRO)\
	MACRO(Esc) MACRO(Backspace) MACRO(Tab) MACRO(Enter) MACRO(Space)\
	MACRO(F1) MACRO(F2)  MACRO(F3)  MACRO(F4)\
	MACRO(F5) MACRO(F6)  MACRO(F7)  MACRO(F8)\
	MACRO(F9) MACRO(F10) MACRO(F11) MACRO(F12)\
	MACRO(PrintScreen) MACRO(ScrollLock) MACRO(Pause)\
	MACRO(0) MACRO(1) MACRO(2) MACRO(3) MACRO(4) MACRO(5) MACRO(6) MACRO(7) MACRO(8) MACRO(9)\
	MACRO(QuoteLeft) MACRO(Minus) MACRO(Equal) MACRO(BackSLash)\
	MACRO(A) MACRO(B) MACRO(C) MACRO(D) MACRO(E) MACRO(F) MACRO(G)\
	MACRO(H) MACRO(I) MACRO(J) MACRO(K) MACRO(L) MACRO(M) MACRO(N)\
	MACRO(O) MACRO(P) MACRO(Q) MACRO(R) MACRO(S) MACRO(T)\
	MACRO(U) MACRO(V) MACRO(W) MACRO(X) MACRO(Y) MACRO(Z)\
	MACRO(BracketLeft) MACRO(BracketRight)\
	MACRO(Semicolon) MACRO(Apostrophe)\
	MACRO(Comma) MACRO(Period) MACRO(Slash)\
	MACRO(ShiftL) MACRO(ShiftR) MACRO(CtrlL) MACRO(CtrlR) MACRO(AltL) MACRO(AltR)\
	MACRO(Up) MACRO(Down) MACRO(Left) MACRO(Right)\
	MACRO(PageUp) MACRO(PageDown) MACRO(Home) MACRO(End) MACRO(Insert) MACRO(Delete)\

class Keyboard{
public:
	Keyboard();

	//键盘按键枚举
#define KEYBOARD_ENUM(key) Key_##key,
	enum KeyboardKey{
		KEYBOARD_ALLKEY(KEYBOARD_ENUM)
		Amount_KeyboardKey
	};
	KeyboardKey asciiKey[0x100];
};
#endif
