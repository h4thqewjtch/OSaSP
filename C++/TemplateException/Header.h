#pragma once
#include<iostream>
#include"Exception.h"
#include<conio.h>
#define nameof(name) #name
#define EXCEPTION_OUTPUT(error)\
(\
(cout << "Error text: " << error.GetException() ) \
)
using namespace std;


struct Node
{
	char info='1';
	Node* prev = nullptr;
	Node* next = nullptr;
};

class BiList
{
	Node* head = nullptr;
	Node* tail = nullptr;
	Node* current = nullptr;
public:
	BiList() = default;
	~BiList() = default;
	void Push();
	void Pop(bool);
	void Print(Node*, bool);
	void SetIntoHead(bool);
	bool operator ++();
	bool operator --();
	Node* operator *();
};

void Input(char info, string s)
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

void BiList::Push()
{
	Node* t = nullptr;
	int i = 0;
	do
	{
		try
		{
			if(!(t = new Node))
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
		//Input(t->info, nameof(info));
		rewind(stdin);
		cin >> t->info;
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

void BiList::Pop(bool flag)
{
	Node* t = nullptr, * t1 = nullptr;
	int n = 0, i = 0;
	if (!head || !tail)
	{
		cout << " Something went wrong" << endl;
		return;
	}
	if (!flag)
	{
		cout << " Enter a count of <<delete>> operations" << endl;
		rewind(stdin);
		cin >> n;
		cout << " Delete from" << endl;
		cout << " 0: head" << endl;
		cout << " 1: tail" << endl;
		InputInt(i, "choise parameter <<i>>", 1);
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
			t1 = t->next;
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
	else
	{
		t = head->next;
		t1 = t->prev;
		do
		{
			delete t1;
			t = t->next;
			t1 = t->prev;
		} while (t);
		head = nullptr;
		tail = nullptr;
	}
}

void BiList::Print(Node* el, bool flag)
{
	if (!head || !tail)
	{
		cout << " Something went wrong" << endl;
		return;
	}
	if (!flag)
	{
		cout << " Element with this <<info>>:" << endl;
		cout << el->info << endl;
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
	}
	char c=_getch();
}

void BiList::SetIntoHead(bool flag)
{
	if (!head || !tail)
	{
		cout << " Something went wrong" << endl;
		return;
	}
	flag ? current = head : current = tail;
}

bool BiList::operator ++()
{
	if (current->next)
	{
		current = current->next;
		return true;
	}
	return false;
}

bool BiList::operator --()
{
	if (current->prev)
	{
		current = current->prev;
		return true;
	}
	return false;
}

Node* BiList::operator *()
{
	return this->current;
}
