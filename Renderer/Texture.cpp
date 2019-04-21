#include "Texture.h"

Texture::Texture():texture(0),width(0),height(0){}
Texture::~Texture(){}

static GLfloat vertex[]={0,0,0,0,0,0,0,0};
static GLfloat texCoord_Default[]  ={0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0};
static GLfloat texCoord_LeftHalf[] ={0.0,0.0, 0.5,0.0, 0.5,1.0, 0.0,1.0};
static GLfloat texCoord_RightHalf[]={0.5,0.0, 1.0,0.0, 1.0,1.0, 0.5,1.0};
static GLfloat texCoord_UpHalf[]   ={0.0,0.5, 1.0,0.5, 1.0,1.0, 0.0,1.0};
static GLfloat texCoord_DownHalf[] ={0.0,0.0, 1.0,0.0, 1.0,0.5, 0.0,0.5};

//创建纹理
void Texture::texImage2D(GLsizei width, GLsizei height, const GLvoid *pixels){
	if(!glIsTexture(texture)){//防止多次申请从而导致原texture值丢失
		glGenTextures(1,&texture);//申请纹理序号
	}
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	this->width=width;
	this->height=height;
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
}
void Texture::deleteTexture(){
	if(glIsTexture(texture)){
		glDeleteTextures(1,&texture);
	}
}
//创建纹理(根据不同的类)
void Texture::texImage2D(const FileBMP &fileBmp){
	Bitmap_32bit bitmap;
	fileBmp.decodeTo(bitmap);
	texImage2D(bitmap);
	bitmap.deleteBitmap();
}
void Texture::texImage2D(const FilePNG &filePng){
	Bitmap_32bit bitmap;
	filePng.decodeTo(bitmap);
	texImage2D(bitmap);
	bitmap.deleteBitmap();
}
void Texture::texImage2D(const Pixmap<bool> &pixmap){
	Bitmap_32bit bitmap;
	pixmap.toBitmap(bitmap);
	texImage2D(bitmap);
	bitmap.deleteBitmap();
}
void Texture::texImage2D(const Pixmap<uint8> &pixmap){
	Bitmap_32bit bitmap;
	pixmap.toBitmap(bitmap);
	texImage2D(bitmap);
	bitmap.deleteBitmap();
}
void Texture::texImage2D(const Bitmap_32bit &bitmap){
	texImage2D(bitmap.getWidth(),bitmap.getHeight(),bitmap.dataPointer);
}
//创建纹理(根据文件名)
void Texture::texImage2D_FileBMP(const string &filename){
	FileBMP fileBmp;
	if(fileBmp.loadFile(filename)){
		fileBmp.parseData();
		texImage2D(fileBmp);
		fileBmp.deleteDataPointer();
	}
}
void Texture::texImage2D_FilePNG(const string &filename){
	FilePNG filePng;
	if(filePng.loadFile(filename)){
		filePng.parseData();
		texImage2D(filePng);
		filePng.deleteDataPointer();
	}
}
void Texture::texImage2D_FileName(const string &filename){
	//根据扩展名来识别
	auto pos=filename.find_last_of(".");
	auto suffix=filename.substr(pos);
	auto len=suffix.length();
	if(len!=4)return;//目前图片的扩展名包括点共4个
	for(decltype(len) i=0;i<len;++i){//转换成小写再比对
		suffix[i]=tolower(suffix[i]);
	}
	//开始比对
	if(suffix==".bmp"){
		texImage2D_FileBMP(filename);
	}else if(suffix==".png"){
		texImage2D_FilePNG(filename);
	}
}

void Texture::draw(const Point2D<GLfloat> &p,TexCoord coord)const{
	draw(p,sizeF()-Point2D<float>(1,1),coord);
}
void Texture::draw(const Point2D<GLfloat> &p,const Point2D<GLfloat> &size,TexCoord coord)const{
	draw(Rectangle2D<GLfloat>(p,p+size),coord);
}
void Texture::draw(const Rectangle2D<GLfloat> &rect,TexCoord coord)const{
	if(!glIsTexture(texture))return;
	glBindTexture(GL_TEXTURE_2D,texture);
	//顶点数组
	rect2vertex(rect,vertex);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	//纹理坐标数组
#define TEXTURE_TEXCOORD(name) case TexCoord_##name:glTexCoordPointer(2,GL_FLOAT,0,texCoord_##name);break;
	switch(coord){
		TEXTURE_TEXCOORD(Default)
		TEXTURE_TEXCOORD(LeftHalf)
		TEXTURE_TEXCOORD(RightHalf)
		TEXTURE_TEXCOORD(UpHalf)
		TEXTURE_TEXCOORD(DownHalf)
#undef TEXTURE_TEXCOORD
		default:;
	}
	//绘制
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

GLsizei Texture::getWidth()const{return width;}
GLsizei Texture::getHeight()const{return height;}
Point2D<GLsizei> Texture::size()const{return Point2D<GLsizei>(width,height);}
Point2D<GLfloat> Texture::sizeF()const{return Point2D<GLfloat>(width,height);}

void Texture::rect2vertex(const Rectangle2D<GLfloat> &rect,GLfloat vertex[]){
	vertex[0]=vertex[6]=rect.p0.x();
	vertex[1]=vertex[3]=rect.p0.y();
	vertex[2]=vertex[4]=rect.p1.x();
	vertex[5]=vertex[7]=rect.p1.y();
}

void TextureCache::clearCache(){
	for(auto &item:*this){
		item.value.deleteTexture();
	}
	clear();
}