#pragma once
#include "MenuItem.h"

template <typename T>
class ObjectLinkedItem : public MenuItem
{
protected:
	T crossRef;
public:
	ObjectLinkedItem(T crossRefIn);

	//getters/setters
	T getCrossRef() { return crossRef; }
};

template <typename T>
ObjectLinkedItem<T>::ObjectLinkedItem(T crossRefIn)
{
	crossRef = crossRefIn;
}

//
//template <typename T>
//class ObjectLinkedItem<T*> : public MenuItem
//{
//protected:
//	T* crossRef;
//public:
//	ObjectLinkedItem(T* crossRefIn);
//	//void draw() {}
//
//	//getters/setters
//	T* getCrossRef() { return crossRef; }
//};
//
//template <typename T>
//ObjectLinkedItem<T*>::ObjectLinkedItem(T* crossRefIn)
//{
//	crossRef = crossRefIn;
//}

