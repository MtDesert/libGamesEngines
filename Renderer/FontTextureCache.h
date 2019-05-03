#ifndef FONTTEXTURECACHE_H
#define FONTTEXTURECACHE_H

#include"Texture.h"
#include"BitmapFont.h"
#include"Charset.h"
#include"Map.h"

/*字体纹理缓冲,负责管理特定字体中每个字的纹理
在程序或者游戏中,并非所有的文字都会用到,我们只需要保存好经常用到的文字的字体,
再次用到的时候直接从本类对象中查询,若有则直接调出,没有则进行绘制再调出*/
class FontTextureCache{
public:
	FontTextureCache();
	~FontTextureCache();

	//成员对象
	BitmapFont_Ascii bitmapFontAscii;
	BitmapFont_GB2312 bitmapFontGb2312;//点阵字体生成模块,负责读取点阵数据(暂时使用gb2312)
	Map<uint16,Texture> mapTextures;//存放纹理的容器
	//渲染过程
	Texture renderCharCode(uint16 character);//渲染文字(Unicode字符),返回文字对应的纹理
	void clearCache();//清除缓冲
};

#endif
