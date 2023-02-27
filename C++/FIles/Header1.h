#pragma once
#include"Header.h"
#include<fstream>
#include<string>

template<class T>
class File
{
protected:
	string text_name = "D:/text.txt";
	string bin_name = "D:/bin.txt";
	ifstream file_i;
	ofstream file_o;
public:
	File() = default;
	~File() = default;
	string TextName()
	{
		return text_name;
	}
	string BinName()
	{
		return bin_name;
	}
};

template<class T>
class TextFile : protected File<T>
{
public:
	TextFile() = default;
	~TextFile() = default;
	void IfstreamOpen();
	void OfstreamOpen();
	void Write(T&);
	void Read(T&);
	void IfstreamClose();
	void OfstreamClose();
};

template<class T>
class BinFile : protected File<T>
{
public:
	BinFile() = default;
	~BinFile() = default;
	void IfstreamOpen();
	void OfstreamOpen();
	void Write(Node<T>*);
	void Read(Node<T>*);
	void IfstreamClose();
	void OfstreamClose();

};

template<class T>
void TextFile<T>::IfstreamOpen()
{
	(this->file_i).open(this->text_name);
}

template<class T>
void TextFile<T>::OfstreamOpen()
{
	(this->file_o).open(this->text_name);
}

template<class T>
void TextFile<T>::Write(T& info)
{
	if (!(this->file_o).is_open())
	{
		cout << "TextFile: file_o error" << endl;
		_getch();
		return;
	}
	this->file_o << info;
}

template<class T>
void TextFile<T>::Read(T& info)
{
	if (!(this->file_i).is_open())
	{
		cout << "TextFile: file_i error" << endl;
		_getch();
		return;
	}
	this->file_i >> info;
}

template<class T>
void TextFile<T>::IfstreamClose()
{
	if ((this->file_i).is_open())
		(this->file_i).close();
}

template<class T>
void TextFile<T>::OfstreamClose()
{
	if ((this->file_o).is_open())
		(this->file_o).close();
}

template<class T>
void BinFile<T>::IfstreamOpen()
{
	(this->file_i).open(this->bin_name, ios::binary);
}

template<class T>
void BinFile<T>::OfstreamOpen()
{
	(this->file_o).open(this->bin_name, ios::binary);
}

template<class T>
void BinFile<T>::Write(Node<T>* node)
{
	if (!(this->file_o).is_open())
		cout << "BinFile: file_o error" << endl;
	(this->file_o).write(reinterpret_cast<char*> (node), sizeof(Node<T>*));
}

template<class T>
void BinFile<T>::Read(Node<T>* node)
{
	if (!(this->file_i).is_open())
		cout << "BinFile: file_i error" << endl;
	(this->file_i).read(reinterpret_cast<char*> (node), sizeof(Node<T>*));
}

template<class T>
void BinFile<T>::IfstreamClose()
{
	if ((this->file_i).is_open())
		(this->file_i).close();
}

template<class T>
void BinFile<T>::OfstreamClose()
{
	if ((this->file_o).is_open())
		(this->file_o).close();
}