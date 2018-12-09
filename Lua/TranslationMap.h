#ifndef TRANSLATIONMAP_H
#define TRANSLATIONMAP_H

#include<string>
using namespace std;

//翻译映射,主要用于保存源语言(主要是英文)和目标语言的映射
class TranslationMap{
	//成员变量
	struct StrMap{string src,dest;};//源语言和目标语言的映射
	StrMap *strMapArray;//映射表
	uint mapAmount;//映射数量
	uint current;//临时变量,用于支持addStringMap()
public:
	TranslationMap();
	~TranslationMap();
	//类型
	typedef decltype(mapAmount) amountType;

	void setAmount(amountType amount);//设置数量(这会清除所有空间)
	amountType getAmount()const;//获取枚举数,可用于判断越界
	void clear();

	void addStringMap(const string &key,const string &value);
};

#endif // TRANSLATIONMAP_H