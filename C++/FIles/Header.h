#pragma once
#include"Exception.h"
#include<iostream>
#include<conio.h>
#define nameof(name) #name
#define EXCEPTION_OUTPUT(error)\
(\
(cout << "Error text: " << error.GetException() ) \
)
using namespace std;

template<class T>
void Input(T& info, string s)
{
	while (1)
	{
		try
		{
			rewind(stdin);
			cin >> info;
			if (cin.fail())
			{
				cin.clear();
				rewind(stdin);
				throw ExceptionEmpty(s);
			}
			else break;
		}
		catch (ExceptionEmpty error)
		{
			EXCEPTION_OUTPUT(error);
		}
		catch (...)
		{
			Exception error;
			cout << error.CetException();
		}
	}
}

void InputInt(int& info, string s, int count)
{
	while (1)
	{
		try
		{
			rewind(stdin);
			cin >> info;
			if (cin.fail())
			{
				cin.clear();
				rewind(stdin);
				throw ArgumentException(s);
			}
			else if (info > count || info < 0)
				throw ConditionException(s);
			else break;
		}
		catch (ArgumentException error)
		{
			EXCEPTION_OUTPUT(error);
		}
		catch (ConditionException error)
		{
			EXCEPTION_OUTPUT(error);
		}
		catch (...)
		{
			Exception error;
			cout << error.CetException();
		}
	}
}

template<class T>
struct Node
{
	T info = 0;
	Node<T>* prev = nullptr;
	Node<T>* next = nullptr;
};

template<class T>
class BiList
{
	Node<T>* head = nullptr;
	Node<T>* tail = nullptr;
	Node<T>* node = nullptr;
public:
	BiList() = default;
	~BiList() = default;
	void Push();
	void Pop(int, int);
	Node<T>* Peek(int);
	class Iterator;
	class Algorithm;
	
};

template<class T>
void BiList<T>::Push()
{
	Node<T>* t = nullptr;
	int i = 0;
	do
	{
		try
		{
			if (!(t = new Node<T>))
				throw bad_alloc();
		}
		catch (bad_alloc error)
		{
			cout << "No memory allocated!";
			return;
		}
		catch (...)
		{
			Exception error;
			cout << error.CetException();
		}
		cout << " Enter information in <<info>>:" << endl;
		Input(t->info, nameof(info));
		if (!tail || !head)
		{
			tail = t;
			head = t;
		}
		else
		{
			t->prev = tail;
			tail->next = t;
			tail = t;
		}
		cout << " Do you want to add other element?(1/0)" << endl;
		InputInt(i, "choise parameter <<i>>", 1);
	} while (i);
}

template<class T>
void BiList<T>::Pop(int i, int n)
{
	Node<T>* t = nullptr, * t1 = nullptr;
	if (!head || !tail)
	{
		cout << " Something went wrong" << endl;
		return;
	}
	switch (i)
	{
	case 0:
	{
		t = head->next;
		t1 = t->prev;
		while (n)
		{
			delete t1;
			t->prev = nullptr;
			t = t->next;
			t1 = t->prev;
			n--;
		}
		head = t1;
		break;
	}
	case 1:
	{
		t = tail->prev;
		t1 = t->prev;
		while (n)
		{
			delete t1;
			t->next = nullptr;
			t = t->prev;
			t1 = t->next;
			n--;
		}
		tail = t1;
		break;
	}
	}
}

template<class T>
Node<T>* BiList<T>::Peek(int i)
{
	if (i == 0)
		return head;
	if (i == 2)
		return tail;
}

template<class T>
class BiList<T>::Iterator 
{
	Node<T>* current = nullptr;
public:
	Iterator() = default;
	~Iterator() = default;
	void operator() (Node<T>*);
	bool operator ++();
	bool operator --();
	Node<T>* operator *();
};

template<class T>
void BiList<T>::Iterator::operator ()(Node<T>* node)
{
	current = node;
}

template<class T>
bool BiList<T>::Iterator::operator ++()
{
	if (current->next)
	{
		current = current->next;
		return true;
	}
	return false;
}

template<class T>
bool BiList<T>::Iterator::operator --()
{
	if (current->prev)
	{
		current = current->prev;
		return true;
	}
	return false;
}

template<class T>
Node<T>* BiList<T>::Iterator::operator *()
{
	return this->current;
}

template<class T>
class BiList<T>::Algorithm
{
public:
	Algorithm() = default;
	~Algorithm() = default;
	int Size(BiList<T>::Iterator&);
	bool IsEmpty(BiList<T>::Iterator&);
	void PrintList(Node<T>*);
	void PrintElement(BiList<T>::Iterator&, int);
};

template<class T>
int BiList<T>::Algorithm::Size(BiList<T>::Iterator& iter)
{
	int size = 0;
	do 
	{
		size++;
	} while (++iter);
	return size;
}

template<class T>
bool BiList<T>::Algorithm::IsEmpty(BiList<T>::Iterator& iter)
{
	return (*iter)==nullptr;
}

template<class T>
void BiList<T>::Algorithm::PrintList(Node<T>* el)
{
	if (!el)
	{
		cout << "This element is not exists" << endl;
	}
	else
	{
		cout << " The list:" << endl;
		do
		{
			if (!el->prev)
				cout << " NULL <- ";
			cout << el->info;
			if (!el->next)
				cout << " -> NULL";
			else cout << " <-> ";
			el = el->next;
		} while (el);
		cout << endl;
	}
	char c = _getch();
}

template<class T>
void BiList<T>::Algorithm::PrintElement(BiList<T>::Iterator& iter, int number)
{
	int i = 0;
	while(i!=number)
	{
		++iter;
		i++;
	}
	cout << " Element of the list with this number: " << (*iter)->info << endl;
}