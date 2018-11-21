#include"Keyboard.h"
#include<stdio.h>

Keyboard::Keyboard(){
	for(int i=0;i<256;++i)asciiKey[i]=Amount_KeyboardKey;
	asciiKey['0']=asciiKey[')']=Key_0;
	asciiKey['1']=asciiKey['!']=Key_1;
	asciiKey['2']=asciiKey['@']=Key_2;
	asciiKey['3']=asciiKey['#']=Key_3;
	asciiKey['4']=asciiKey['$']=Key_4;
	asciiKey['5']=asciiKey['%']=Key_5;
	asciiKey['6']=asciiKey['^']=Key_6;
	asciiKey['7']=asciiKey['&']=Key_7;
	asciiKey['8']=asciiKey['*']=Key_8;
	asciiKey['9']=asciiKey['(']=Key_9;

	asciiKey['A']=asciiKey['a']=Key_A;
	asciiKey['B']=asciiKey['b']=Key_B;
	asciiKey['C']=asciiKey['c']=Key_C;
	asciiKey['D']=asciiKey['d']=Key_D;
	asciiKey['E']=asciiKey['e']=Key_E;
	asciiKey['F']=asciiKey['f']=Key_F;
	asciiKey['G']=asciiKey['g']=Key_G;
	asciiKey['H']=asciiKey['h']=Key_H;
	asciiKey['I']=asciiKey['i']=Key_I;
	asciiKey['J']=asciiKey['j']=Key_J;
	asciiKey['K']=asciiKey['k']=Key_K;
	asciiKey['L']=asciiKey['l']=Key_L;
	asciiKey['M']=asciiKey['m']=Key_M;
	asciiKey['N']=asciiKey['n']=Key_N;
	asciiKey['O']=asciiKey['o']=Key_O;
	asciiKey['P']=asciiKey['p']=Key_P;
	asciiKey['Q']=asciiKey['q']=Key_Q;
	asciiKey['R']=asciiKey['r']=Key_R;
	asciiKey['S']=asciiKey['s']=Key_S;
	asciiKey['T']=asciiKey['t']=Key_T;
	asciiKey['U']=asciiKey['u']=Key_U;
	asciiKey['V']=asciiKey['v']=Key_V;
	asciiKey['W']=asciiKey['w']=Key_W;
	asciiKey['X']=asciiKey['x']=Key_X;
	asciiKey['Y']=asciiKey['y']=Key_Y;
	asciiKey['Z']=asciiKey['z']=Key_Z;

	asciiKey['`']=asciiKey['~']=Key_QuoteLeft;
	asciiKey['-']=asciiKey['_']=Key_Minus;
	asciiKey['=']=asciiKey['+']=Key_Equal;
	asciiKey['\\']=asciiKey['|']=Key_BackSLash;
	asciiKey['[']=asciiKey['{']=Key_BracketLeft;
	asciiKey[']']=asciiKey['}']=Key_BracketRight;
	asciiKey[';']=asciiKey[':']=Key_Semicolon;
	asciiKey['\'']=asciiKey['\"']=Key_Apostrophe;
	asciiKey[',']=asciiKey['<']=Key_Comma;
	asciiKey['.']=asciiKey['>']=Key_Period;
	asciiKey['/']=asciiKey['?']=Key_Slash;

	asciiKey[' ']=Key_Space;
	asciiKey[0x08]=Key_Backspace;
	asciiKey[0x09]=Key_Tab;
	asciiKey[0x0D]=Key_Enter;
	asciiKey[0x1B]=Key_Esc;
	asciiKey[0x7F]=Key_Delete;
}
