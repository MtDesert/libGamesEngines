#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include"List.h"
#include"Array.h"

//数组表,Array和List结合使用
//当插入新的数据时,先检查存储空间(数组),如果不够则扩容,然后再放入数据
//适用场合:元素尺寸小,用量大,经常读取,只在队列尾添加
template<typename T>
class ArrayList{
	//类型
	typedef function<bool(const T &val)> condition;//条件函数,用于判断val是否满足condition
	typedef function<void(T &val)> forEachCallback;//遍历函数
	//变量
	List< Array<T> > arrayList;
public:
	uint arraySize;//数组大小,扩容大小按照此值来进行
	//构造/析构函数
	ArrayList():arraySize(256){}
	~ArrayList(){deleteData();}
	//获取数据
	T* data(condition con)const{
		T* ret=nullptr;
		for(auto &arr:arrayList){
			ret=arr.data(con);
			if(ret)break;
		}
		return ret;
	}
	//遍历
	void forEach(forEachCallback callback){
		for(auto &arr:arrayList){
			for(auto &val:arr){
				callback(val);
			}
		}
	}
	//插入
	void push_back(const T &newData){
		//寻找未填满的数组
		Array<T> *pArray=nullptr;
		for(auto &arr:arrayList){
			if(!arr.isFull()){
				pArray=&arr;
				break;
			}
		}
		//根据情况添加新数组
		if(!pArray){
			arrayList.push_back(Array<T>());//创建数组
			pArray=arrayList.data(arrayList.size()-1);
			pArray->setArraySize(arraySize);//给新数组申请空间
		}
		pArray->push_back(newData);
	}
	//删除
	void clear(){//清除数据,不回收空间
		for(auto &arr:arrayList){
			arr.clear();
		}
	}
	void deleteData(){//删除数据并回收空间
		for(auto &arr:arrayList){arr.deleteData();}
		arrayList.clear();
	}
};
#endif