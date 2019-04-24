#include"GameString.h"
#include"Charset.h"
#include"extern.h"

Charset GameString::charset;
FontTextureCache GameString::fontTextureCache;
//渲染用的变量
static uint8 u8;//存放单字节的字符
static uint16 u16;//存放双字节的字符
static size_t w;//渲染时候计算字符长度用的变量
static Point2D<float> p;//渲染纹理用的点
//静态缓冲区,用于字符转换
static const size_t bufferSize=256;
static char stringBuffer[bufferSize];
static char *destStr=nullptr;
static size_t destLen=0;

GameString::GameString():charSize(16,32),strWidth(0){}
GameString::GameString(const string &str):GameString(){
	setString(str);
}
GameString::~GameString(){}

void GameString::setString(const string &str){
	//准备缓冲
	memset(stringBuffer,0,bufferSize);
	destStr=stringBuffer;
	destLen=bufferSize;
	//获取字符及其特征
	charset.newString(str.data(),&destStr,destLen);
	//计算特征,生成纹理
	DataBlock block;
	block.dataPointer=(uchar*)destStr;
	block.dataLength=::strlen(destStr);
	arrayCharAttr.setSize(block.dataLength);//申请缓冲
	auto charAmount=0;//统计字符数
	for(uint i=0;i<block.dataLength;++i){
		auto attr=arrayCharAttr.data(charAmount);
		//开始判断
		block.get_uint8(i,u8);
		attr->isAscii=(u8<0x80);
		if(attr->isAscii){
			u16=u8;
		}else{
			block.get_uint16(i,u16);
			++i;
		}
		attr->tex=fontTextureCache.renderCharCode(u16);
		++charAmount;
	}
	arrayCharAttr.setSize(charAmount);//去掉用不着的空间
	strWidth=block.dataLength;
}
uint GameString::stringWidth()const{return charSize.x()*strWidth;}

Point2D<GLfloat> GameString::sizeF()const{
	return Point2D<GLfloat>(stringWidth(),charSize.y());
}

void GameString::renderX()const{
	shapeRenderer.setColor(color);
	point2D=rectF().p0;
	size2D.y()=charSize.y();
	//开始计算
	renderX(0,arrayCharAttr.size());
}

void GameString::renderX(uint from,uint amount)const{
	for(decltype(amount) i=0;i<amount;++i){
		//获取文字纹理
		auto attr=arrayCharAttr.data(from+i);
		if(!attr)break;
		//根据是否ASCII来进行渲染
		if(attr->isAscii){
			w=size2D.x()=charSize.x();//确定宽度
			attr->tex.draw(point2D,size2D,Texture::TexCoord_LeftHalf);//渲染
		}else{
			w=size2D.x()=charSize.x()*2;//确定宽度
			attr->tex.draw(point2D,size2D);
		}
		point2D.x()+=w;//确定下一个字的绘制位置
	}
}