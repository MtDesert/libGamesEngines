#include"ColorCIE.h"

//CIE
void ColorCIERGB::fromRGB(const ColorRGB &color){
	const uint8 &r(color.red),&g(color.green),&b(color.blue);
	red=(r*1167-g*146-b*151)/1000;
	green=(r*144-g*753-b*159)/1000;
	blue=(-r-g*59-b*1128)/1000;
}
void ColorCIERGB::toRGB(ColorRGB &color)const{}

ColorCIEXYZ::ColorCIEXYZ(uint8 x,uint8 y,uint8 z,Type type):
	x(x),y(y),z(z),type(type){}
void ColorCIEXYZ::fromRGB(const ColorRGB &color){
	const uint8 &r(color.red),&g(color.green),&b(color.blue);
	uint16 xr=0,xg=0,xb=0,yr=0,yg=0,yb=0,zr=0,zg=0,zb=0;
	switch(type){
		case REC601:
			xr=607;xg=174;xb= 201;
			yr=299;yg=587;yb= 114;
			zr=000;zg=066;zb=1117;
			break;
		case REC709:
			xr=412;xg=358;xb=180;
			yr=213;yg=715;yb= 72;
			zr= 19;zg=119;zb=950;
			break;
		case ITU:
			xr=431;xg=342;xb=178;
			yr=222;yg=707;yb= 71;
			zr= 20;zg=130;zb=939;
			break;
	}
	x=(r*xr+g*xg+b*xb)/1000;
	y=(r*yr+g*yg+b*yb)/1000;
	z=(r*zr+g*zg+b*zb)/1000;
}
void ColorCIEXYZ::toRGB(ColorRGB &color)const{
	uint8 &r(color.red),&g(color.green),&b(color.blue);
	uint16 rx=0,ry=0,rz=0,gx=0,gy=0,gz=0,bx=0,by=0,bz=0;
	switch(type){
		case REC601:
			rx=1910;ry=-532;rz=-288;
			gx=-985;gy=1999;gz= -28;
			bx=  58;by=-118;bz= 898;
		break;
		case REC709:
			rx=3241;ry=-1537;rz=-499;
			gx=-969;gy= 1876;gz= -42;
			bx=  56;by= -204;bz=1057;
		break;
		case ITU:
			rx=3063;ry=-1393;rz=-476;
			gx=-969;gy= 1876;gz=  42;
			bx=  68;by= -229;bz=1069;
		break;
	}
	r=(x*rx+y*ry+z*rz)/1000;
	g=(x*gx+y*gy+z*gz)/1000;
	b=(x*bx+y*by+z*bz)/1000;
}