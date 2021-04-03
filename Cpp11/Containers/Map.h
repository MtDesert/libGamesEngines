#ifndef MAP_H
#define MAP_H

#include"Set.h"

/*键值对*/
template<typename K,typename V>
struct KeyValue{
	K key;V value;
	KeyValue(){}
	KeyValue(const K &k):key(k){}
	KeyValue(const K &k,const V &v):key(k),value(v){}
	bool operator==(const KeyValue<K,V> kv){return key==kv.key;}//只要键一样,我们就认为是一样,这样可以直接用Set的特性
};

/*映射是集合的一种扩展,即每个元素多了*/
template<typename K,typename V>
class Map:public Set<KeyValue<K,V> >{
public:
	bool insert(const K &key,const V &val){//插入键值对
		return Set<KeyValue<K,V> >::insert(KeyValue<K,V>(key,val));
	}
	V* value(const K &key)const{//根据键查询值,没有键的时候返回nullptr
		auto kv = Set<KeyValue<K,V> >::nodeValue(KeyValue<K,V>(key));
		return kv?&kv->data.value:nullptr;
	}
};
#endif // MAP_H