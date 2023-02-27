#pragma once
#include<iostream>

using namespace std;

class Exception : public exception
{
	string error_text = "Unknown Error! Please try again\n";
public:
	Exception() = default;
	~Exception() = default;
	string CetException()
	{
		return error_text;
	}
};

class ArgumentException : public Exception
{
	string _error = " was entered incorrectly! Please try again\n";
	string error_text = "";
public:
	ArgumentException() = default;
	ArgumentException(string error) : error_text(error + _error) {};
	~ArgumentException() = default;
	string GetException()
	{
		return error_text;
	}
};

class ExceptionEmpty : public Exception
{
	string _error = " is empty! Please try again\n";
	string error_text = "";
public:
	ExceptionEmpty() {};
	ExceptionEmpty(string error) : error_text(error + _error) {};
	~ExceptionEmpty() {};
	string GetException()
	{
		return error_text;
	}
};

class ConditionException : public Exception
{
	string _error = " does not satisfy the condition! Please try again\n";
	string error_text = "";
public:
	ConditionException() = default;
	ConditionException(string error) : error_text(error + _error) {};
	~ConditionException() = default;
	string GetException()
	{
		return error_text;
	}
};


//template<class T>
//void BiList<T>::Algorithm::SortUp(BiList<T>& list)
//{
//	list.node = list.head;
//
//	Node<T>* before = list.node->prev;
//	Node<T>* _node = list.node->next;
//	Node<T>* after = _node->next;
//	while (_node)
//	{
//		if (list.node->info > _node->info)
//		{
//			_node->prev = before;
//			before->next = _node;
//			after->prev = list.node;
//			list.node->next = after;
//			list.node->prev = _node;
//			_node->next = list.node;
//		}
//		else
//		{
//			list.node = list.node->next;
//			_node = _node->next;
//		}
//	}
//}