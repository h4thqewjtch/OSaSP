#pragma once
#include<list>
#include<conio.h>
#include<iostream>
#include"Exception.h"
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
void PushFront(list<T>& Listok)
{
	T info;
	int i = 0;
	do
	{
		cout << "	Enter an information for the list element:	";
		Input(info, nameof(info));
		Listok.push_front(info);
		cout << "	Do you want to add other element?(1/0)		";
		InputInt(i, "choise parameter <<i>>", 1);
	} while (i);

}

template<class T>
void PushBack(list<T>& Listok)
{
	T info;
	int i = 0;
	do
	{
		cout << "	Enter an information for the list element:	";
		Input(info, nameof(info));
		Listok.push_back(info);
		cout << "	Do you want to add other element?(1/0)		";
		InputInt(i, "choise parameter <<i>>", 1);
	} while (i);
}

template<class T>
void Insert(list<T>& Listok)
{
	T info;
	int number = 0;
	cout << "	Enter a position where you push the element(<" << Listok.size() - 1 << "):	";
	InputInt(number, nameof(number), Listok.size()-1);
	cout << "	Enter an information for the list element:	";
	Input(info, nameof(info));
	Listok.push_back(info);
	list <int>::iterator iter;
	iter = Listok.begin();
	while (number--)
	{
		iter++;
	}
	Listok.insert(iter, info);
}

template<class T>
void PopFront(list<T>& Listok)
{
	Listok.pop_front();
}

template<class T>
void PopBack(list<T>& Listok)
{
	Listok.pop_back();
}

template<class T>
void Erase(list<T>& Listok)
{
	T info;
	int begin = 0;
	int end = 0;
	list <int>::iterator iter_0;
	list <int>::iterator iter_2;
	iter_0 = iter_2 = Listok.begin();
	cout << "	Enter a position where you begin deletion(<" << Listok.size() - 1 << "):	";
	InputInt(begin, nameof(begin), Listok.size() - 2);
	cout << "	Enter a position where you complete deletion(>"<< begin<<" && <"<< Listok.size() << "):		";
	InputInt(end, nameof(begin), Listok.size() - 1);
	while (begin--)
	{
		iter_0++;
	}
	while (end--)
	{
		iter_2++;
	}
	Listok.erase(iter_0, iter_2);
}

template<class T>
void Clear(list<T>& Listok)
{
	Listok.clear();
}

template<class T>
void SortUp(list<T>& Listok)
{
	Listok.sort();
}

template<class T>
void SortDown(list<T>& Listok)
{
	Listok.sort(greater<int>());
}

template<class T>
void Show(list<T>& Listok)
{
	list <int>::iterator iter;
	cout << " The list:" << endl;
	for (iter = Listok.begin(); iter != Listok.end(); iter++)
	{
		if (iter == Listok.begin())
			cout << " NULL <- ";
		else cout << " <-> ";
		cout << *iter;
	}
	cout << " -> NULL";
	cout << endl;
}