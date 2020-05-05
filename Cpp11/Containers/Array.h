#ifndef ARRAY_H
#define ARRAY_H

#include"Point.h"
#include"DataBlock.h"

#include<list>
#include<string.h>

//数组,存储空间连续
//和std::array不同的是,本类可以在存数据前先设定尺寸
template<typename T>
struct Array{
protected:
	//类型
	typedef function<bool(const T &val)> condition;//条件函数,用于判断val是否满足condition
	typedef function<bool(const T &a,const T &b)> SortFun;//排序条件函数,返回排序依据
	//变量
	T *dataPtr;//数据指针
	SizeType length;//长度,占内存的T类型数据个数
	SizeType usedLength;//实际使用的数量,不要大于长度,否则会发生越界
public:
	Array():dataPtr(nullptr),length(0),usedLength(0){}
	~Array(){deleteData();}

	//迭代器(正向)
	class iterator{
	protected:
		const Array<T> *arr;
		SizeType pos;
	public:
		iterator(const Array<T> *arr=nullptr,SizeType pos=0):arr(arr),pos(pos){}
		bool operator!=(const iterator &itr)const{return pos!=itr.pos;}
		T& operator*()const{return *(iterator::operator->());}
		T* operator->()const{return arr->data(pos);}
		iterator& operator--(){--pos;return *this;}
		iterator& operator++(){++pos;return *this;}
	};
	//运算符
	Array<T>& operator=(const Array<T> &another){
		clear();
		setArraySize(another.size());
		for(auto &dat:another){
			push_back(dat);
		}
		return *this;
	}
	//迭代函数
	iterator begin()const{return iterator(this,0);}
	iterator end()const{return iterator(this,this->size());}

	//存储空间
	SizeType arraySize()const{return length;}//能存的元素个数
	void setArraySize(SizeType size,bool enlargeOnly=false){//设置数组空间,注意:数组大小改变的话会清除内部的数据
		if(length==size)return;//无变化
		if(enlargeOnly && length>size)return;//扩容模式
		//申请新空间,并从旧空间复制数据
		auto newPtr=new T[size];
		auto copyAmount=min(length,size);
		for(SizeType i=0;i<copyAmount;++i){
			newPtr[i]=dataPtr[i];
		}
		//清除旧空间,更新大小
		delete []dataPtr;
		dataPtr=newPtr;
		length=size;
		usedLength=min(usedLength,length);
	}
	//元素个数
	SizeType size()const{return usedLength;}//当前存储的元素个数
	void setSize(SizeType size,bool needResetArraySize=false){//设置需要存储的元素个数
		if(size>length){//考虑是否需要扩容
			if(!needResetArraySize)return;
			setArraySize(size);
		}
		usedLength=size;
	}
	bool isFull()const{return usedLength>=length;}
	void fill(const T &val){
		if(!dataPtr)return;
		for(SizeType i=0;i<usedLength;++i){
			dataPtr[i]=val;
		}
	}
	//访问数据
	T* data(SizeType pos)const{//根据位置获取数据
		if(dataPtr && pos<usedLength){
			return &dataPtr[pos];
		}
		return nullptr;
	}
	bool setData(SizeType pos,const T &val){
		if(dataPtr && pos < usedLength){
			dataPtr[pos] = val;
			return true;
		}
		return false;
	}
	T* data(condition con)const{//根据条件获取数据
		for(SizeType i=0;i<usedLength;++i){
			if(con(dataPtr[i]))return &dataPtr[i];//找到了
		}
		return nullptr;//没找到
	}
	T* firstData()const{return data(0);}
	T* lastData()const{return data(size()-1);}
	//数据位置
	int indexOf(const T &value,SizeType from=0)const{//获取首个value值在表中的索引,找不到返回-1
		if(from<usedLength){
			for(auto i=from;i<usedLength;++i){
				if(dataPtr[i]==value)return i;
			}
		}
		return -1;
	}
	int indexOf(condition con,SizeType from=0)const{//获取首个满足条件con的值在表中的索引,找不到返回-1
		if(from<usedLength){
			for(auto i=from;i<usedLength;++i){
				if(con(dataPtr[i]))return i;
			}
		}
		return -1;
	}
	bool contain(const T &val)const{return indexOf(val)>=0;}//判断数组中是否包含数据val
	//遍历
	void forEach(function<void(T &val,SizeType index)> callback){
		for(SizeType i=0;i<usedLength;++i){
			callback(dataPtr[i],i);
		}
	}
	void forEach(function<void(const T &val,SizeType index)> callback)const{
		for(SizeType i=0;i<usedLength;++i){
			callback(dataPtr[i],i);
		}
	}
	void forEach(function<void(T &val)> callback){
		for(SizeType i=0;i<usedLength;++i){
			callback(dataPtr[i]);
		}
	}
	void forEach(function<void(const T &val)> callback)const{
		for(SizeType i=0;i<usedLength;++i){
			callback(dataPtr[i]);
		}
	}
	//添加
	void insert(SizeType pos,const T &value){
		if(isFull()){
			setArraySize(length+1);
			usedLength=length;
		}else{
			++usedLength;
		}
		//数据后移(多次复制)
		for(SizeType i=usedLength-1;i>pos;--i){
			dataPtr[i]=dataPtr[i-1];
		}
		dataPtr[pos]=value;
	}
	void push_front(const T &value){insert(0,value);}
	void push_back(const T &value){insert(usedLength,value);}
	//删除
	void erase(SizeType pos){//按位置删除
		if(!dataPtr || usedLength<=0)return;
		for(SizeType i=pos+1;i<usedLength;++i){//数据前移
			dataPtr[i-1]=dataPtr[i];
		}
		--usedLength;
	}
	void remove(const T &val){//按值删除,只删除一个
		auto pos=indexOf(val);
		if(pos>=0)erase(pos);
	}
	void remove(const T* ptr){//按指针删除
		if(!dataPtr)return;
		auto pos = ptr-dataPtr;
		erase(pos);
	}
	void remove_if(condition con){//删除所有符合条件con的值
		if(!dataPtr)return;
		SizeType pos=0,remain=0;
		while(pos<usedLength){
			if(con(dataPtr[pos])){//符合删除条件
			}else{//不符合条件,保留
				dataPtr[remain]=dataPtr[pos];
				++remain;
			}
			++pos;
		}
		usedLength=remain;
	}
	void unique(){//删除所有重复的值(参考std::list::unique)
		SizeType len=0,a=0,b=0;
		for(;a<usedLength;++a){
			for(b=0;b<len;++b){
				if(dataPtr[a]==dataPtr[b])break;//有重复
			}
			if(b==len){//无重复
				dataPtr[len]=dataPtr[a];
				++len;
			}
		}
		usedLength=len;
	}
	void pop_front(){erase(0);}
	void pop_back(){erase(usedLength-1);}
	//排序
	void sort(SortFun compareFunc){
		if(!dataPtr || !compareFunc)return;
		SizeType posToExchange=0,sz=size(),a=0,b=0;
		for(;a<sz;++a){
			posToExchange=a;
			for(b=a+1;b<sz;++b){
				//比较,决定排序顺序
				if(!compareFunc(dataPtr[posToExchange],dataPtr[b])){//两两比较
					posToExchange=b;//更换最值
				}
			}
			if(posToExchange!=a){//最值发生变化,则需要交换
				T tmp=dataPtr[a];
				dataPtr[a]=dataPtr[posToExchange];
				dataPtr[posToExchange]=tmp;
			}
		}
	}
	//清除与释放
	void clear(){usedLength=0;}//清除,但不释放所占内存
	void deleteData(){//删除数据,释放数组所占内存
		if(dataPtr){
			delete []dataPtr;
		}
		dataPtr=nullptr;
		usedLength=length=0;
	}
};

//二维数组模板
template<typename T>
struct Array2D{
	explicit Array2D():width(0),height(0){}
	explicit Array2D(int length,int width):width(length),height(width){newData(length,width);}
	explicit Array2D(const Array2D &another){newData(another);}
	~Array2D(){deleteData();}

	//operator
	Array2D& operator=(const Array2D<T> &another){newData(another);return *this;}
	//init and clear
	virtual void newData(SizeType length,SizeType width,const T &val=T()){}
	virtual void newData(const Array2D<T> &another){}
	virtual void deleteData(){}
	//size
	SizeType getWidth()const{return width;}
	SizeType getHeight()const{return height;}
	SizeType getSize()const{return width*height;}
	//range
	bool isXInRange(SizeType x)const{return x<width;}
	bool isYInRange(SizeType y)const{return y<height;}
	bool isInRange(SizeType x,SizeType y)const{return isXInRange(x)&&isYInRange(y);}
	bool isInRange(const Point2D<SizeType> &p)const{return isInRange(p.x,p.y);}
	//locate
	SizeType offset(SizeType x,SizeType y)const{return y*height+x;}
	SizeType offset(const Point2D<SizeType> &p)const{return offset(p.x,p.y);}
	//read/write
	virtual bool getValue(SizeType x,SizeType y,T& value)const{return false;}
	bool getValue(const Point2D<SizeType> &p,T &value)const{return getValue(p.x,p.y,value);}
	virtual bool setValue(SizeType x,SizeType y,const T &value){return false;}
	bool setValue(const Point2D<SizeType> &p,const T &value){return setValue(p.x,p.y,value);}

	virtual void fill(const T &value){}
	//deal with list
	void removePoints_NoInRange(list< Point2D<SizeType> > &pointList)const{
		for(auto itr=pointList.begin();itr!=pointList.end();){
			isInRange(*itr)?++itr:itr=pointList.erase(itr);
		}
	}
	/**
	 * @brief seedFill 种子填充算法
	 * @param pointList 保存输出结果
	 * @param seedPoint 开始填充的点
	 * @param isMatch 用于判断两个点上的值是否匹配的函数
	 */
	void seedFill(list< Point2D<SizeType> > &pointList,const Point2D<SizeType> &seedPoint,std::function<bool(const T &a,const T &b)> &isMatch)const
	{
		pointList.clear();
		if(!isInRange(seedPoint))return;
		pointList.push_back(seedPoint);
		//
		Point2D<SizeType> around[4]={Point2D<SizeType>(-1,0),Point2D<SizeType>(1,0),Point2D<SizeType>(0,-1),Point2D<SizeType>(0,1)};
		Point2D<SizeType> p;

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
	SizeType width,height;
	virtual T* pointer(SizeType x,SizeType y)const{return nullptr;}
};

template<typename T>
struct Array2D_LV1_Pointer:public Array2D<T>{
	Array2D_LV1_Pointer():data(nullptr){}
	//init and clear
	void newData(SizeType length,SizeType width,const T &val=T()){
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
	bool getValue(SizeType x,SizeType y,T& value)const{
		if(!this->isInRange(x,y))return false;
		value=data[this->offset(x,y)];
		return true;
	}
	bool setValue(SizeType x,SizeType y,const T &value){
		if(!this->isInRange(x,y))return false;
		data[this->offset(x,y)]=value;
		return true;
	}
	void fill(const T &value){
		SizeType size=this->getSize();
		for(SizeType i=0;i<size;++i)data[i]=value;
	}
protected:
	virtual T* pointer(SizeType x, SizeType y)const{
		if(!this->isInRange(x,y))return nullptr;
		return &data[this->offset(x,y)];
	}
	T *data;
};

template<typename T>
struct Array2D_LV2_Pointer:public Array2D<T>{
	Array2D_LV2_Pointer():data(nullptr){}

	//运算符
	Array2D_LV2_Pointer<T>& operator=(const Array2D_LV2_Pointer<T> &another){
		newData(another);
		return *this;
	}
	//init and clear
	void newData(SizeType length,SizeType width,const T &val=T()){
		deleteData();
		this->width=length;
		this->height=width;
		data=new T*[this->width];
		for(SizeType i=0;i<this->width;++i)data[i]=new T[this->height];
		fill(val);
	}
	void newData(const Array2D_LV2_Pointer<T> &another){
		deleteData();
		this->width=another.width;
		this->height=another.height;
		data=new T*[this->width];
		for(SizeType i=0;i<this->width;++i){
			data[i]=new T[this->height];
			memcpy(data[i],another.data[i],this->height*sizeof(T));
		}
	}
	void deleteData(){
		if(data){
			for(SizeType i=0;i<this->width;++i)delete data[i];
			delete data;
		}
		data=nullptr;
		this->width=0;
		this->height=0;
	}
	bool getValue(SizeType x,SizeType y,T& value)const{
		if(!this->isInRange(x,y))return false;
		value=data[x][y];
		return true;
	}
	bool setValue(SizeType x,SizeType y,const T &value){
		if(!this->isInRange(x,y))return false;
		data[x][y]=value;
		return true;
	}
	void fill(const T &value){
		for(SizeType x=0;x<this->width;++x){
			for(SizeType y=0;y<this->height;++y)data[x][y]=value;
		}
	}
protected:
	virtual T* pointer(SizeType x,SizeType y)const{
		if(!this->isInRange(x,y))return nullptr;
		return &(data[x][y]);
	}
	T **data;
};
#endif