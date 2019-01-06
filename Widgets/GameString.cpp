#include"GameString.h"
#include"Charset.h"
#include"extern.h"

FontTextureCache GameString::fontTextureCache;
//渲染用的变量
static uint8 u8;//存放单字节的字符
static uint16 u16;//存放双字节的字符
static size_t w;//渲染时候计算字符长度用的变量
static Point2D<float> p;//渲染纹理用的点

GameString::GameString():charSize(16,32){}
GameString::GameString(const string &str):GameString(){
	setString(str);
}
GameString::~GameString(){stringCode.memoryFree();}

DataBlock GameString::newString(const string &str){//这里写死了字符集,后期可以修改
	return Charset::newString(str.data(),Charset::UTF8,Charset::GB2312);
}
void GameString::setString(const string &str){
	stringCode.memoryFree();//删除掉旧的数据
	stringCode=newString(str);
}
uint GameString::stringWidth()const{
	uint ret=0;
	for(uint i=0;i<stringCode.dataLength;++i){
		if(stringCode.get_uint8(i,u8) && u8<0x80){//ASCII
			if(u8==0x00)break;//字符串结束
			++ret;
		}else if(stringCode.get_uint16(i,u16)){
			ret+=2;
			++i;
		}
	}
	return ret*charSize.x();
}

Point2D<GLfloat> GameString::sizeF()const{
	return Point2D<GLfloat>(stringWidth(),charSize.y());
}

void GameString::render()const{
	shapeRenderer.setColor(color);
	p=rectF().p0;
	size2D.y()=charSize.y();
	//开始计算
	w=0;
	for(size_t i=0;i<stringCode.dataLength;++i){//渲染每个字
		if(stringCode.get_uint8(i,u8) && u8<0x80){//ASCII
			if(u8==0x00)break;//字符串结束
			w=size2D.x()=charSize.x();//确定宽度
			::texture=fontTextureCache.renderCharCode(u8);//取纹理
			::texture.draw(p,size2D,Texture::TexCoord_LeftHalf);//渲染
		}else if(stringCode.get_uint16(i,u16)){//非ASCII
			w=size2D.x()=charSize.x()*2;//确定宽度
			::texture=fontTextureCache.renderCharCode(u16);//取纹理
			::texture.draw(p,size2D);//渲染
			++i;
		}
		p.x()+=w;//确定下一个字的绘制位置
	}
}
