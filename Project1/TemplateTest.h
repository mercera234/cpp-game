#pragma once

template <class T>
class TemplateTest
{
private:
public:
	T mbr;
};

template <class T>
struct TemplateStruct
{
	T mbr;
};

template <class T>
class TTChild : public TemplateTest<T> //if inheriting from template class, then must either use the template <> syntax, or use a concrete type
{
private:
public:
	T data;
};

class TTChild2 : public TemplateTest<int>
{
private:
public:

};