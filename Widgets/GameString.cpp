#include"GameString.h"
#include"Charset.h"
#include"extern.h"

FontTextureCache GameString::fontTextureCache;

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
	uint8 u8;uint16 u16;
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
	auto pos=rectF().p0;
	//开始计算
	uint8 u8;uint16 u16;
	size_t w=0;
	Texture tex;
	for(size_t i=0;i<stringCode.dataLength;++i){//渲染每个字
		if(stringCode.get_uint8(i,u8) && u8<0x80){//ASCII
			if(u8==0x00)break;//字符串结束
			w=charSize.x();
			tex=fontTextureCache.renderCharCode(u8);
			tex.draw(pos,Point2D<GLfloat>(w,charSize.y()),Texture::TexCoord_LeftHalf);
		}else if(stringCode.get_uint16(i,u16)){//非ASCII
			w=charSize.x()*2;//确定宽度
			tex=fontTextureCache.renderCharCode(u16);
			tex.draw(pos,Point2D<GLfloat>(w,charSize.y()));
			++i;
		}
		pos.x()+=w;//确定下一个字的绘制位置
	}
}
