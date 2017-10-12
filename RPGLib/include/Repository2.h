#pragma once
#include <map>
#include <functional>

template <class Key>
bool lessThan(Key& a, Key& b);

/*
This class doesn't work!
*/
template <class Key, class Val>
class Repository2
{
private:
	std::map<Key, Val*, std::function<bool(Key, Key)>> objects;
//	std::map<Key, Val*, Compare = less<Key>> objects;
	//std::map<Key, Val*> objects;
public:
	//Repository2(std::function<bool(Key, Key)> keyCompare = std::less<Key>);
	Repository2(std::function<bool(Key, Key)>& keyCompare);
	void add(Key& key, Val& obj); //add one object of type T
	Val* find(Key& key);
	std::map<Key, Val*>* getInternalMap() { return &objects; }
};

template <class Key, class Val>
Repository2<Key, Val>::Repository2(std::function<bool(Key, Key)>& keyCompare)
	: objects(keyCompare)
{

}

template <class Key, class Val>
void Repository2<Key, Val>::add(Key& key, Val& obj) //add one object of type T
{
	std::pair<Key, Val*> aMap(key, &obj);
	objects.insert(aMap);
}

template <class Key, class Val>
Val* Repository2<Key, Val>::find(Key& key)
{
	std::map<Key, Val*>::iterator it = objects.find(key);
	return it == objects.end() ? NULL : &(*it->second);
}

template <class Key>
bool lessThan(Key& a, Key& b)
{
	return a < b;
}