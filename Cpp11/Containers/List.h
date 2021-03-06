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
	//类型
	typedef function<bool(const T &val)> Condition;//条件函数,用于判断val是否满足condition
	typedef function<bool(const T &a,const T &b)> SortFun;//排序条件函数,返回排序依据
	//变量
	ListNode<T> *head,*tail;//表头指针
	SizeType amount;//列表中的项数

	//获取pos位置的节点,没有则返回空
	ListNode<T>* node(SizeType pos)const{
		if(pos>=amount)return nullptr;
		bool forward=pos<amount/2;//确定搜索方向
		SizeType p = forward ? 0 : amount-1;
		auto n = forward ? head : tail;
		if(forward){
			while(n->next && p<pos){n=n->next;++p;};
		}else{
			while(n->prev && p>pos){n=n->prev;--p;};
		}
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
	//获取满足条件con的首个节点
	ListNode<T>* nodeCondition(Condition con)const{
		auto n=head;
		while(n){
			if(con(n->data))break;
			n=n->next;
		};
		return n;
	}
	//在node之前插入newNode
	void insertNodeBefore(ListNode<T> &newNode,ListNode<T> &node){
		//修改head
		if(head==&node)head=&newNode;
		//修改与node前项的关系
		if(node.prev){
			node.prev->next=&newNode;
		}
		newNode.prev=node.prev;
		//修改与node的关系
		newNode.next=&node;
		node.prev=&newNode;
		//增加数量
		++amount;
	}
	//在node之后插入newNode
	void insertNodeAfter(ListNode<T> &newNode,ListNode<T> &node){
		//修改tail
		if(tail==&node)tail=&newNode;
		//修改与node后项的关系
		if(node.next){
			node.next->prev=&newNode;
		}
		newNode.next=node.next;
		//修改与node的关系
		newNode.prev=&node;
		node.next=&newNode;
		//增加数量
		++amount;
	}
	//移除节点
	void removeNode(ListNode<T> &node){
		if(head==&node)head=node.next;//首项的情况
		if(tail==&node)tail=node.prev;//末项的情况
		if(node.prev)node.prev->next = node.next;//有前项的情况
		if(node.next)node.next->prev = node.prev;//有后项的情况
		--amount;
	}
	//删除节点
	bool deleteNode(ListNode<T> *node){//删除节点
		if(!node)return false;
		//移出队列并删除
		removeNode(*node);
		delete node;
		return true;
	}
	//节点前移
	bool moveNodePrev(ListNode<T> &node){
		auto prevNode=node.prev;
		if(prevNode){
			removeNode(node);//从老位置移除
			insertNodeBefore(node,*prevNode);//插入新位置
			return true;
		}
		return false;
	}
	//节点后移
	bool moveNodeNext(ListNode<T> &node){
		auto nextNode=node.next;
		if(nextNode){
			removeNode(node);//从老位置移除
			insertNodeAfter(node,*nextNode);//插入新位置
			return true;
		}
		return false;
	}
	//交换节点
	void swapNode(ListNode<T> *nodeA,ListNode<T> *nodeB){
		if(!nodeA || !nodeB || nodeA==nodeB)return;
		//修改前后项
		if(nodeA->prev==nodeB){
			moveNodePrev(*nodeA);
		}else if(nodeA->next==nodeB){
			moveNodeNext(*nodeA);
		}else{//nodeA和nodeB不相邻的情况
			auto aPrev=nodeA->prev,aNext=nodeA->next;
			auto bPrev=nodeB->prev,bNext=nodeB->next;
			if((aPrev || aNext) && (bPrev || bNext)){
				removeNode(*nodeA);
				removeNode(*nodeB);
				//重新插入B
				if(aPrev){
					insertNodeAfter(*nodeB,*aPrev);
				}else if(aNext){
					insertNodeBefore(*nodeB,*aNext);
				}
				//重新插入A
				if(bPrev){
					insertNodeAfter(*nodeA,*bPrev);
				}else if(bNext){
					insertNodeBefore(*nodeA,*bNext);
				}
			}
		}
	}
public:
	//构造/析构函数
	List():head(nullptr),tail(nullptr),amount(0){}
	~List(){clear();}

	//迭代器(正向)
	class iterator{
	protected:
		ListNode<T> *node;
	public:
		iterator(ListNode<T> *nd=nullptr):node(nd){}
		bool operator!=(const iterator &itr)const{return node!=itr.node;}
		T& operator*()const{return node->data;}
		T* operator->()const{return &node->data;}
		iterator& operator--(){node=node->prev;return *this;}
		iterator& operator++(){node=node->next;return *this;}
	};
	//迭代函数
	iterator begin()const{return iterator(head);}
	iterator end()const{return iterator();}
	//容量函数
	inline SizeType size()const{return amount;}//元素数量

	//查询数据
	const T* data(SizeType pos)const{//返回pos位置的数据的地址
		auto n=node(pos);
		return n?(&n->data):nullptr;
	}
	T* data(SizeType pos){//返回pos位置的数据的地址,可以用此方法直接修改List中的数据
		auto n=node(pos);
		return n?(&n->data):nullptr;
	}
	T* data(Condition con){//返回满足con条件的首个数据
		auto n=node(con);
		return n?(&n->data):nullptr;
	}
	bool data(SizeType pos,T &value)const{//返回pos位置的数据的副本,修改副本不影响List的数据
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

	//插入数据(在pos位置之前)
	bool insert(SizeType pos,const T &newData){
		if(pos>amount)return false;
		auto newNode=new ListNode<T>();
		newNode->data=newData;
		//数据复制完了,开始寻找位置
		if(amount==0){//首个数据
			head=tail=newNode;
			++amount;
		}else{//已经有老数据了,那么在新的位置插入数据即可
			auto nd=node(pos);
			if(nd){
				insertNodeBefore(*newNode,*nd);
			}else{
				insertNodeAfter(*newNode,*tail);
			}
		}
		return true;
	}
	bool push_front(const T &newData){return insert(0,newData);}
	bool push_back(const T &newData){return insert(amount,newData);}
	//删除数据(删除pos位置的数据)
	bool erase(SizeType pos){return deleteNode(node(pos));}
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
		head=tail=nullptr;
		amount=0;
	}
	//删除数据(根据值)
	void remove(const T &value){
		auto node=head,next=node;
		while(node){
			next=node->next;
			if(node->data==value){
				deleteNode(node);
			}
			node=next;
		};
	}
	//删除数据(根据条件)
	void remove_if(Condition con){
		auto node=head,next=node;
		while(node){
			next=node->next;
			if(con(node->data)){
				deleteNode(node);
			}
			node=next;
		};
	}
	//删除数据(保证唯一性)
	void unique(){
		auto n1=head,n2=head,next=head;
		for(;n1;n1=n1->next){
			n2=n1->next;//两两比较
			while(n2){
				if(n1->data==n2->data){//删除重复
					next=n2->next;
					deleteNode(n2);
					n2=next;
				}else{
					n2=n2->next;
				}
			}
		}
	}
	//移动(根据位置),返回移动是否成功
	bool movePrev(SizeType pos){
		auto nd=node(pos);
		return nd ? moveNodePrev(*nd) : false;
	}
	bool moveNext(SizeType pos){
		auto nd=node(pos);
		return nd ? moveNodeNext(*nd) : false;
	}
	//排序
	void sort(SortFun compareFunc){
		auto n1=head,n2=head,nd=head;//nd为最值
		for(;n1;n1=n1->next){
			n2=n1->next;//两两比较
			nd=n1;
			while(n2){
				//比较,决定排序顺序
				if(!compareFunc(nd->data,n2->data)){
					nd=n2;//更换最值
				}
				//下一个
				n2=n2->next;
			}
			//最值发生变化,则需要交换
			if(nd!=n1){
				swapNode(nd,n1);
				n1=nd;//让n1从最值继续往下遍历
			}
		}
	}
};
#endif