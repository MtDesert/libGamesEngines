#ifndef TEXTURE_H
#define TEXTURE_H

#include"GL/gl.h"
#include"FileBMP.h"
#include"FilePNG.h"
#include"Pixmap.h"

#include<map>
using namespace std;

/*纹理主要是指在显存中的东西,而如果在内存或者外存中我们应该叫做图像,
纹理由显卡直接调度,可以进行快速渲染
这是OpenGL所使用的纹理,在OpenGL的标准中,操作纹理的依据是GLuint类型的一个值,通过不同的值来区分不同的纹理*/
class Texture{
public:
	//构造/析构
	Texture();
	~Texture();

	//创建/删除纹理
	void texImage2D(GLsizei width,GLsizei height,const GLvoid* pixels);
	void deleteTexture();
	//创建纹理(通过图像文件或者内存图像来创建)
	void texImage2D(const FileBMP &fileBmp);
	void texImage2D(const FilePNG &filePng);
	void texImage2D(const Pixmap<bool> &pixmap);
	void texImage2D(const Pixmap<uint8> &pixmap);
	void texImage2D(const Bitmap_32bit &bitmap);
	//创建纹理(通过各种文件名)
	void texImage2D_FileBMP(const string &filename);
	void texImage2D_FilePNG(const string &filename);
	void texImage2D_FileName(const string &filename);

	//局部绘制枚举
	enum TexCoord{
		TexCoord_Default,//默认绘制整个纹理
		TexCoord_LeftHalf,//左半部分(可用于绘制ASCII字母)
		TexCoord_RightHalf,//右半部分
		TexCoord_UpHalf,//上半部分
		TexCoord_DownHalf//下半部分
	};
	//绘制纹理(从p开始绘制,绘制大小为size,若没指定size则以本身的宽和高来作为默认size)
	void draw(const Point2D<GLfloat> &p,TexCoord coord=TexCoord_Default)const;
	void draw(const Point2D<GLfloat> &p,const Point2D<GLfloat> &size,TexCoord coord=TexCoord_Default)const;
	void draw(const Rectangle2D<GLfloat> &rect,TexCoord coord=TexCoord_Default)const;

	//纹理的几何尺寸
	GLsizei getWidth()const;//宽
	GLsizei getHeight()const;//高
	Point2D<GLsizei> size()const;//宽高
	Point2D<GLfloat> sizeF()const;//宽高

	static void rect2vertex(const Rectangle2D<GLfloat> &rect,GLfloat vertex[]);
private:
	GLuint texture;
	GLsizei width,height;
};

/*纹理缓冲,缓存各种纹理,为游戏提供各种支持*/
class TextureCache:public Map<int,Texture>{
public:
	void clearCache();
};

#endif // TEXTURE_H