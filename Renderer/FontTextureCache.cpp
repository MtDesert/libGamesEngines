#include"FontTextureCache.h"

FontTextureCache::FontTextureCache(){}
FontTextureCache::~FontTextureCache(){clearCache();}

Texture FontTextureCache::renderCharCode(uint16 character){
	Texture tex;
	auto val=mapTextures.value(character);//找重复
	if(val){
		tex=*val;
	}else{//找不到,应该生成新的
		if(character<0x80){
			bitmapFontAscii.renderCharCode(character);//绘制文字,绘制的结果在内存中
			tex.texImage2D(bitmapFontAscii.charPixmap);//生成纹理,此时数据在显存中
		}else{
			bitmapFontGb2312.renderCharCode(character);//绘制文字,绘制的结果在内存中
			tex.texImage2D(bitmapFontGb2312.charPixmap);//生成纹理,此时数据在显存中
		}
		//rendererTTF.renderCharCode(character);//绘制文字,绘制的结果在内存中
		//tex.texImage2D(rendererTTF.charPixmap);//生成纹理,此时数据在显存中
		mapTextures.insert(character,tex);//缓存起来,以防重复绘制
	}
	return tex;
}
bool FontTextureCache::renderString(const string &str,Charset::EnumCharset charset){
	auto block=Charset::newString(str.data(),Charset::UTF8,Charset::GB2312);//得到转换结果
	uint16 charCode;
	for(size_t i=2;i<block.dataLength;i+=2){
		if(block.get_uint16(i,charCode)){
			renderCharCode(charCode);//渲染每个文字,不必担心重复问题
		}
	}
	block.deleteDataPointer();//清理
	return true;
}

void FontTextureCache::clearCache(){
	auto kv=mapTextures.data(0);
	while(kv){
		kv->value.deleteTexture();
		mapTextures.erase(0);
		kv=mapTextures.data(0);
	}
	mapTextures.clear();
}