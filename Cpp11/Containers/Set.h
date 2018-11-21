#ifndef SET_H
#define SET_H

#include"List.h"

/*Set是数学上的集合,和List不同的是,Set不会插入重复的值,即保证唯一性
std::set有个很麻烦的问题就是放到set里面的类型必须可比较大小(即T类型必须有operator<(),operator>()的操作),不然在模板展开的时候就会报错
按照数学理论来说,集合不需要进行排列,不过计算机上的集合排列无非是解决搜索上的效能问题*/
template<typename T>
class Set:public List<T>{
	//只利用List的局部特性,故某些函数的功能将被禁用
	bool insert(uint pos,const T &newData){return List<T>::insert(pos,newData);}
	bool push_front(const T &newData);
	bool push_back(const T &newData);
public:
	bool insert(const T &newData){//直接插入数据
		if(this->contain(newData))return false;
		return this->insert(List<T>::size(),newData);
	}
};

#endif // SET_H
