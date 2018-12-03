#include"BitmapFont.h"
#include"Charset_GB2312.h"

//构造/析构函数
BitmapFont::BitmapFont(){
	charPixmap.colorMap.insert(false,0x00000000);//底色
	charPixmap.colorMap.insert(true,0xFFFFFFFF);//字体色
}
BitmapFont::~BitmapFont(){}
BitmapFont_Ascii::BitmapFont_Ascii(){
	charPixmap.newData(32,32);//在设计游戏的时候可以统一字体大小,请根据实际情况修改
}
BitmapFont_Ascii::~BitmapFont_Ascii(){}
BitmapFont_GB2312::BitmapFont_GB2312(){
	charPixmap.newData(32,32);//在设计游戏的时候可以统一字体大小,请根据实际情况修改
}
BitmapFont_GB2312::~BitmapFont_GB2312(){}

//各种渲染函数
bool BitmapFont_Ascii::renderCharCode(uint8 charCode){
	uint8 index;
	if(!Charset_GB2312::charIndex_Ascii(charCode,index))return false;
	auto charData=charBlock.subDataBlock(index*64,64);
	if(charData.dataLength!=64)return false;
	charPixmap.fill(0);//开始绘图
	for(int y=0;y<32;++y){
		for(int x=0;x<16;++x){
			index= y*2 + x/8;
			if(charData.dataPointer[index] & ( 1<<(x%8) )){
				charPixmap.setColor(x,y,1);
			}
		}
	}
	return true;
}
bool BitmapFont_GB2312::renderCharCode(uint16 charCode){
	uint16 index;
	DataBlock *block=nullptr;
	//查询字符表
	if(Charset_GB2312::charIndex_Symbol(charCode,index)){//符号区
		block=&symbolBlock;
	}else if(Charset_GB2312::charIndex_lv1Chinese(charCode,index)){//1级汉字
		block=&lv1Chinese;
	}else if(Charset_GB2312::charIndex_lv2Chinese(charCode,index)){//2级汉字
		block=&lv2Chinese;
	}else{//无字符
		return false;
	}
	//获取字形数据
	auto charData=block->subDataBlock(index*128,128);//32*32/8==128,请根据实际情况修改
	if(charData.dataLength!=128)return false;
	charPixmap.fill(0);
	for(int y=0;y<32;++y){
		for(int x=0;x<32;++x){
			index= y*4 + x/8;
			if(charData.dataPointer[index] & ( 1<<(x%8) )){
				charPixmap.setColor(x,y,1);
			}
		}
	}
	return true;
}
