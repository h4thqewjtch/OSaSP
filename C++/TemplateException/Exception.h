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
