#pragma once
#include "ObjectLinkedItem.h"

template <typename T>
class MockObjectLinkedItem : public ObjectLinkedItem<T>
{
public:
	MockObjectLinkedItem(T crossRefIn);
	void draw() {}
};

template <typename T>
MockObjectLinkedItem<T>::MockObjectLinkedItem(T crossRefIn) : ObjectLinkedItem<T>(crossRefIn)
{
	
}




template <typename T>
class MockObjectLinkedItem<T*> : public ObjectLinkedItem<T*>
{
public:
	MockObjectLinkedItem(T* crossRefIn);
	void draw() {};
};

template <typename T>
MockObjectLinkedItem<T*>::MockObjectLinkedItem(T* crossRefIn) : ObjectLinkedItem<T*>(crossRefIn)
{
}

