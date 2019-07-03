#include"Keyboard.h"
#include<stdio.h>

#define KEYBOARD_ASCII(a,b,name)\
asciiKey[(int)a]=asciiKey[(int)b]=Key_##name;

Keyboard::Keyboard(){
	for(int i=0;i<256;++i)asciiKey[i]=Amount_KeyboardKey;
    KEYBOARD_ASCII('0',')',0)
	KEYBOARD_ASCII('1','!',0)
	KEYBOARD_ASCII('2','@',0)
	KEYBOARD_ASCII('3','#',0)
	KEYBOARD_ASCII('4','$',0)
	KEYBOARD_ASCII('5','%',0)
	KEYBOARD_ASCII('6','^',0)
	KEYBOARD_ASCII('7','&',0)
	KEYBOARD_ASCII('8','*',0)
	KEYBOARD_ASCII('9','(',0)

	KEYBOARD_ASCII('A','a',A)
	KEYBOARD_ASCII('B','b',B)
	KEYBOARD_ASCII('C','c',C)
	KEYBOARD_ASCII('D','d',D)
	KEYBOARD_ASCII('E','e',E)
	KEYBOARD_ASCII('F','f',F)
	KEYBOARD_ASCII('G','g',G)
	KEYBOARD_ASCII('H','h',H)
	KEYBOARD_ASCII('I','i',I)
	KEYBOARD_ASCII('J','j',J)
	KEYBOARD_ASCII('K','k',K)
	KEYBOARD_ASCII('L','l',L)
	KEYBOARD_ASCII('M','m',M)
	KEYBOARD_ASCII('N','n',N)
	KEYBOARD_ASCII('O','o',O)
	KEYBOARD_ASCII('P','p',P)
	KEYBOARD_ASCII('Q','q',Q)
	KEYBOARD_ASCII('R','r',R)
	KEYBOARD_ASCII('S','s',S)
	KEYBOARD_ASCII('T','t',T)
	KEYBOARD_ASCII('U','u',U)
	KEYBOARD_ASCII('V','v',V)
	KEYBOARD_ASCII('W','w',W)
	KEYBOARD_ASCII('X','x',X)
	KEYBOARD_ASCII('Y','y',Y)
	KEYBOARD_ASCII('Z','z',Z)

	KEYBOARD_ASCII('`','~',QuoteLeft)
	KEYBOARD_ASCII('-','_',Minus)
	KEYBOARD_ASCII('=','+',Equal)
	KEYBOARD_ASCII('\\','|',BackSlash)
	KEYBOARD_ASCII('[','{',BracketLeft)
	KEYBOARD_ASCII(']','}',BracketRight)
	KEYBOARD_ASCII(';',':',Semicolon)
	KEYBOARD_ASCII('\'','\"',Apostrophe)
	KEYBOARD_ASCII(',','<',Comma)
	KEYBOARD_ASCII('.','>',Period)
	KEYBOARD_ASCII('/','?',Slash)

	asciiKey[(int)' ']=Key_Space;
	asciiKey[0x08]=Key_Backspace;
	asciiKey[0x09]=Key_Tab;
	asciiKey[0x0D]=Key_Enter;
	asciiKey[0x1B]=Key_Esc;
	asciiKey[0x7F]=Key_Delete;
}
