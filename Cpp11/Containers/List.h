#ifndef LIST_H
#define LIST_H

#include"typedef.h"

template<typename T>
struct ListNode{//表项
	T data;//数据本体
	ListNode *prev,*next;//指向相邻的表项
	ListNode():prev(nullptr),next(nullptr){}
};

//列表模板,用于存储任何类型(实现部分std::list接口)
//设计这个列表是因为c++的list有平台差异(实现部分的差异)
template<typename T>
class List{
protected:
	//变量
	ListNode<T> *head;//表头指针
	uint amount;//列表中的项数

	//获取pos位置的节点,没有则返回空
	ListNode<T>* node(uint pos)const{
		if(pos>=amount)return nullptr;
		uint p=0;
		auto n=head;
		while(n->next && p<pos){
			n=n->next;++p;
		};
		return n;
	}
	//获取值为value的首个节点
	ListNode<T>* nodeValue(const T &value)const{
		auto n=head;
		while(n){
			if(n->data==value)break;
			n=n->next;
		};
		return n;
	}
	bool removeNode(ListNode<T> *node){//删除节点
		if(!node)return false;
		if(head==node)head=nullptr;//首项的情况
		if(node->prev)node->prev->next = node->next;//有前项的情况
		if(node->next)node->next->prev = node->prev;//有后项的情况
		//现在可以删除了
		delete node;
		--amount;
		return true;
	}
public:
	//构造/析构函数
	List():head(nullptr),amount(0){}
	~List(){clear();}

	//迭代器,用于支持C++11的for(auto item:list){...}
	class iterator{
		ListNode<T> *node;
		iterator():node(nullptr){}
		friend class List;
	public:
		bool operator!=(const iterator &itr)const{return node!=itr.node;}
		iterator& operator++(){node=node->next;return *this;}
		T& operator*()const{return node->data;}
	};
	iterator begin()const{
		iterator itr;
		itr.node=head;
		return itr;
	}
	iterator end()const{return iterator();}

	//查询数据
	const T* data(uint pos)const{
		auto n=node(pos);
		return n?(&n->data):nullptr;
	}
	T* data(uint pos){//返回pos位置的数据的地址,可以用此方法直接修改List中的数据
		auto n=node(pos);
		return n?(&n->data):nullptr;
	}
	bool data(uint pos,T &value)const{//返回pos位置的数据的副本,修改副本不影响List的数据
		auto dataPtr=data(pos);
		if(dataPtr){
			value=dataPtr->data;
		}
		return dataPtr;
	}
	bool contain(const T &value)const{return nodeValue(value);}
	int indexOf(const T &value,int from=0){
		int ret=from;
		auto n=node(from);
		if(n){//开始搜索
			do{
				if(n->data==value)return ret;//找到了
				n=n->next;++ret;//下一个
			}while(n);
		}
		return -1;//找不到
	}
	//表当前项数
	inline uint size()const{return amount;}

	//插入数据(在pos位置之前)
	bool insert(uint pos,const T &newData){
		if(pos>amount)return false;
		auto newNode=new ListNode<T>();
		newNode->data=newData;
		//数据复制完了,开始寻找位置
		auto nd=node(pos);
		if(nd){
			auto ndPrev=nd->prev;
			if(ndPrev){//与前者建立链接
				ndPrev->next=newNode;
				newNode->prev=ndPrev;
			}else{//与head建立链接
				head=newNode;
			}
			//与后者建立链接
			nd->prev=newNode;
			newNode->next=nd;
		}else{
			if(amount==0)head=newNode;//与head建立联系
			else{
				auto ndPrev=node(amount-1);
				if(ndPrev){//与前者建立联系
					ndPrev->next=newNode;
					newNode->prev=ndPrev;
				}
			}
		}
		++amount;
		return true;
	}
	bool push_front(const T &newData){return insert(0,newData);}
	bool push_back(const T &newData){return insert(amount,newData);}
	//删除数据(删除pos位置的数据)
	bool erase(uint pos){return removeNode(node(pos));}
	inline bool pop_front(){return erase(0);}
	inline bool pop_back(){return amount>0?erase(amount-1):false;}
	//清除数据
	void clear(){
		auto node=head;
		while(node){
			auto next=node->next;
			delete node;
			node=next;
		}
		head=nullptr;
		amount=0;
	}
	//删除数据(根据值),返回删除的个数
	void remove(const T &value){
		auto node=head,next=node;
		while(node){
			next=node->next;
			if(node->data==value){
				removeNode(node);
			}
			node=next;
		};
	}
	//删除数据(保证唯一性)
	void unique(){
		auto n1=head,n2=head,next=head;
		for(;n1;n1=n1->next){
			n2=n1->next;
			while(n2){
				next=n2->next;
				if(n1->data==n2->data){
					removeNode(n2);
				}
				n2=next;
			}
		}
	}
};
#endif