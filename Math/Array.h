#ifndef ARRAY_H
#define ARRAY_H

#include<stddef.h>
#include<string.h>
#include<list>
#include<functional>
using namespace std;

#include"typedef.h"
#include"Point.h"

template<typename T>
struct Array2D
{
	explicit Array2D():width(0),height(0){}
	explicit Array2D(int length,int width):width(length),height(width){newData(length,width);}
	explicit Array2D(const Array2D &another){newData(another);}
	~Array2D(){deleteData();}

	//operator
	Array2D& operator=(const Array2D<T> &another){newData(another);return *this;}
	//init and clear
	virtual void newData(uint length,uint width,const T &val=T()){}
	virtual void newData(const Array2D<T> &another){}
	virtual void deleteData(){}
	//size
	uint getWidth()const{return width;}
	uint getHeight()const{return height;}
	uint getSize()const{return width*height;}
	//range
	bool isXInRange(uint x)const{return x<width;}
	bool isYInRange(uint y)const{return y<height;}
	bool isInRange(uint x,uint y)const{return isXInRange(x)&&isYInRange(y);}
	bool isInRange(const Point2D<uint> &p)const{return isInRange(p.x,p.y);}
	//locate
	uint offset(uint x,uint y)const{return y*height+x;}
	uint offset(const Point2D<uint> &p)const{return offset(p.x,p.y);}
	//read/write
	virtual bool getValue(uint x,uint y,T& value)const{return false;}
	bool getValue(const Point2D<uint> &p,T &value)const{return getValue(p.x,p.y,value);}
	virtual bool setValue(uint x,uint y,const T &value){return false;}
	bool setValue(const Point2D<uint> &p,const T &value){return setValue(p.x,p.y,value);}

	virtual void fill(const T &value){}
	//deal with list
	void removePoints_NoInRange(list< Point2D<uint> > &pointList)const
	{
		for(auto itr=pointList.begin();itr!=pointList.end();)
		{
			isInRange(*itr)?++itr:itr=pointList.erase(itr);
		}
	}
	/**
	 * @brief seedFill 种子填充算法
	 * @param pointList 保存输出结果
	 * @param seedPoint 开始填充的点
	 * @param isMatch 用于判断两个点上的值是否匹配的函数
	 */
	void seedFill(list< Point2D<uint> > &pointList,const Point2D<uint> &seedPoint,std::function<bool(const T &a,const T &b)> &isMatch)const
	{
		pointList.clear();
		if(!isInRange(seedPoint))return;
		pointList.push_back(seedPoint);
		//
		Point2D<uint> around[4]={Point2D<uint>(-1,0),Point2D<uint>(1,0),Point2D<uint>(0,-1),Point2D<uint>(0,1)};
		Point2D<uint> p;

		auto itr=pointList.begin();
		for(;itr!=pointList.end();++itr){
			for(int i=0;i<4;++i){
				p=(*itr)+around[i];
				//compare whether p & around[i] matched
				if(!isInRange(p))continue;
				const T *a=pointer(itr->x,itr->y);
				const T *b=pointer(p.x,p.y);
				if(!a || !b || !isMatch(*a,*b))continue;
				//filter(scan back to find whether repeated)
				auto backItr=pointList.rbegin();
				for(;backItr!=pointList.rend();++backItr){
					if(*backItr==p)break;
				}
				if(backItr!=pointList.rend())continue;
				//ok, new point
				pointList.push_back(p);
			}
		}
	}
protected:
	uint width,height;
	virtual T* pointer(uint x,uint y){return nullptr;}
};

template<typename T>
struct Array2D_LV1_Pointer:public Array2D<T>{
	Array2D_LV1_Pointer():data(nullptr){}
	//init and clear
	void newData(uint length,uint width,const T &val=T()){
		deleteData();
		this->width=length;
		this->height=width;
		data=new T[length*width];
		fill(val);
	}
	void newData(const Array2D_LV1_Pointer<T> &another){
		deleteData();
		this->width=another.width;
		this->height=another.height;
		data=new T[this->width*this->height];
		memcpy(data,another.data,this->getSize()*sizeof(T));
	}
	void deleteData(){
		if(data)delete []data;
		data=nullptr;
		this->width=0;
		this->height=0;
	}
	bool getValue(uint x,uint y,T& value)const{
		if(!this->isInRange(x,y))return false;
		value=data[this->offset(x,y)];
		return true;
	}
	bool setValue(uint x,uint y,const T &value){
		if(!this->isInRange(x,y))return false;
		data[this->offset(x,y)]=value;
		return true;
	}
	void fill(const T &value){
		uint size=this->getSize();
		for(uint i=0;i<size;++i)data[i]=value;
	}
protected:
	T* pointer(uint x, uint y){
		if(!this->isInRange(x,y))return nullptr;
		return &data[this->offset(x,y)];
	}
	T *data;
};

template<typename T>
struct Array2D_LV2_Pointer:public Array2D<T>{
	Array2D_LV2_Pointer():data(nullptr){}
	//init and clear
	void newData(uint length,uint width,const T &val=T()){
		deleteData();
		this->width=length;
		this->height=width;
		data=new T*[this->width];
		for(uint i=0;i<this->width;++i)data[i]=new T[this->height];
		fill(val);
	}
	void newData(const Array2D_LV2_Pointer<T> &another){
		deleteData();
		this->width=another.width;
		this->height=another.height;
		data=new T*[this->width];
		for(uint i=0;i<this->width;++i)
		{
			data[i]=new T[this->height];
			memcpy(data[i],another.data[i],this->height*sizeof(T));
		}
	}
	void deleteData(){
		if(data)
		{
			for(uint i=0;i<this->width;++i)delete data[i];
			delete data;
		}
		data=nullptr;
		this->width=0;
		this->height=0;
	}
	bool getValue(uint x,uint y,T& value)const
	{
		if(!this->isInRange(x,y))return false;
		value=data[x][y];
		return true;
	}
	bool setValue(uint x,uint y,const T &value)
	{
		if(!this->isInRange(x,y))return false;
		data[x][y]=value;
		return true;
	}
	void fill(const T &value)
	{
		for(uint x=0;x<this->width;++x)
		{
			for(uint y=0;y<this->height;++y)data[x][y]=value;
		}
	}
protected:
	T **data;
};

/*template<typename T>
class Array3D:public Array2D_LV1_Pointer<T>
{
public:
	explicit Array3D():Array2D<T>(),height(0){}
	explicit Array3D(int length,int width,int height)
	{
		this->length=length;
		this->width=width;
		this->height=height;
		this->data=new T[length*width*height];
	}
	//size
	uint getHeight()const{return height;}
	uint getSize()const{return Array2D<T>::getSize()*height;}
	//range
	bool isZInRange(uint z)const{return z<height;}
	bool isInRange(uint x,uint y,uint z)const{return this->isInRange(x,y)&&isZInRange(z);}
	bool isInRange(const Point3D<uint> &p)const{return isInRange(p.x,p.y,p.z);}
	//locate
	uint offset(uint x,uint y,uint z)const{return Array2D<T>::offset(x,y)*height+z;}
	uint offset(const Point3D<uint> &p)const{return offset(p.x,p.y,p.z);}
	//read/write
	bool getValue(uint x,uint y,uint z,T& value)const
	{
		if(!isInRange(x,y,z))return false;
		value=this->data[offset(x,y,z)];
		return true;
	}
	bool getValue(const Point3D<uint> &p,T &value)const{return getValue(p.x,p.y,p.z,value);}
	bool setValue(uint x,uint y,uint z,const T &value)
	{
		if(!isInRange(x,y,z))return false;
		this->data[offset(x,y,z)]=value;
		return true;
	}
	bool setValue(const Point3D<uint> &p,const T &value){return setValue(p.x,p.y,p.z,value);}
protected:
	uint height;
};*/

#endif // ARRAY_H
