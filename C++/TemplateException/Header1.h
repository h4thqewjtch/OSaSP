#pragma once
#include"Header.h"
#include<fstream>
#include<string>

class File
{
protected:
	string file_name = "";
	ifstream file_i;
	ofstream file_o;
public:
	File() = default;
	~File() = default;
};

class TextFile : public File
{
public:
	TextFile() = default;
	TextFile(string name) 
	{
		file_name = name;
	};
	~TextFile() = default;
	void Write(char info)
	{
		file_o.open(file_name, ios::app);
		if (!file_o.is_open())
		{
			cout << "TextFile: file_o error" << endl;
			_getch();
			return;
		}
		file_o << info;
		file_o.close();
	}
	void Read(char info)
	{
		file_i.open(file_name);
		if (!file_i.is_open())
		{
			cout << "TextFile: file_i error" << endl;
			_getch();
			return;
		}
		file_i.seekg(0, ios::beg);
		file_i >> info;
	    file_i.close();
	}
	string SetFile_Name()
	{
		return file_name;
	}
};

class BinFile : public File
{
public:
	BinFile() = default;
	BinFile(string name) 
	{
		file_name = name;
	};
	~BinFile() = default;
	void Write(BiList& list)
	{
		file_o.open(file_name, ios::binary | ios::app);
		if (!file_o.is_open())
			cout << "BinFile: file_o error" << endl;
		file_o.write(reinterpret_cast<char*> (&list), sizeof(BiList));
		file_o.close();
	}
	void Read(BiList& list)
	{
		file_i.open(file_name, ios::binary);
		if (!file_i.is_open())
			cout << "BinFile: file_i error" << endl;
		file_i.read(reinterpret_cast<char*> (&list), sizeof(BiList));
		file_i.close();
	}
	string SetFile_Name()
	{
		return file_name;
	}
	
};

void menu()
{
	BiList list;
	string textName = "";
	cout << "Enter a name for text file: ";
	rewind(stdin);
	cin >> textName;
	string binName = "";
	cout << "Enter a name for binary file: ";
	rewind(stdin);
	cin >> binName;
	TextFile textFile(textName);
	BinFile binFile(binName);
	char c='R';
	while (1)
	{
		cout << "What do you want to do?" << endl;
		cout << "	0: change the type of data" << endl;
		cout << "	1: create/add an element of the list" << endl;
		cout << "	2: delete the elements of the list" << endl;
		cout << "	3: delete the list" << endl;
		cout << "	4: show an element of the list" << endl;
		cout << "	5: show the list" << endl;
		cout << "	6: write the list into a text file" << endl;
		cout << "	7: read the list from the text file" << endl;
		cout << "	8: write the list into a binary file" << endl;
		cout << "	9: read the list from the binary file" << endl;
		cout << "Your choise is ";
		int i = 0;
		InputInt(i, "choise parameter <<i>>", 9);
		switch (i)
		{
		case 0:
		{
			cout << "0: change the type of data" << endl;
			return;
		}
		case 1:
		{
			cout << "1: create the list/add an element of the list" << endl;
			list.Push();
			break;
		}
		case 2:
		{
			cout << "2: delete elements of the list" << endl;
			bool flag = 0;
			list.Pop(flag);
			break;
		}
		case 3:
		{
			cout << "3: delete the list" << endl;
			bool flag = 1;
			list.Pop(flag);
			break;
		}
		case 4:
		{
			system("cls");
			cout << "4: show an element of the list" << endl;
			bool all = 0;
			bool exit = 0;
			cout << "	0 - Return to a menu" << endl;
			cout << "	1 - show head" << endl;
			cout << "	2 - show tail" << endl;
			InputInt(i, "choise parameter <<i>>", 2);
			switch (i)
			{
			case 0:
			{
				exit = 1;
				break;
			}
			case 1:
			{
				all = 0;
				list.SetIntoHead(true);
				list.Print(*list, all);
				all = 0;
				break;
			}
			case 2:
			{
				all = 0;
				list.SetIntoHead(false);
				list.Print(*list, all);
				all = 0;
				break;
			}
			}
			break;
		}
		case 5:
		{
			cout << "5: show the list" << endl;
			bool all = 1;
			list.SetIntoHead(true);
			list.Print(*list, all);
			break;
		}
		case 6:
		{
			cout<< "6: write the list into a text file" << endl;
			list.SetIntoHead(true);
			ofstream file(textFile.SetFile_Name());
			file.seekp(0, ios::beg);
			file.close();
			do
			{
				textFile.Write((*list)->info);
			} while (++list);
			break;
		}
		case 7:
		{
			cout<<"7: read the list from the text file" << endl;
			list.SetIntoHead(true);
			ifstream file(textFile.SetFile_Name());
			file.seekg(0, ios::beg);
			file.close();
			do
			{
				//file >> c;
				textFile.Read(c);
				cout << "c :" << c <<endl;
			} while (++list);
				//file.close();
			break;
		}
		case 8:
		{
			cout << "8: write the list into a binary file" << endl;
			list.SetIntoHead(true);
			ofstream file(binFile.SetFile_Name(), ios::binary);
			file.seekp(0, ios::beg);
			file.close();
			binFile.Write(list);
			list.SetIntoHead(true);
			do
			{
				(*list)->info = ' ';
			} while (++list);
			break;
		}
		case 9:
		{
			cout << "9: read the list from the binary file" << endl;
			list.SetIntoHead(false);
			ifstream file(binFile.SetFile_Name(), ios::binary);
			file.seekg(0, ios::beg);
			file.close();
			binFile.Read(list);
			break;
		}
		}
	}
}
