#include"Header.h"
#include"Header1.h"
#include<iostream>

using namespace std;

int main()
{
	BiList<char> list;
	BiList<char>::Iterator iter;
	BiList<char>::Algorithm algor;
	TextFile<char> textFile;
	BinFile<char> binFile;
	while (1)
	{
		cout << "What do you want to do?" << endl;
		cout << "	0: exit" << endl;
		cout << "	1: create/add an element of the list" << endl;
		cout << "	2: delete some elements of the list" << endl;
		cout << "	3: show an element of the list" << endl;
		cout << "	4: show the list" << endl;
		cout << "	5: write the list into a text file" << endl;
		cout << "	6: read the list from the text file" << endl;
		cout << "	7: write the list into a binary file" << endl;
		cout << "	8: read the list from the binary file" << endl;
		cout << "Your choise is ";
		int i = 0;
		InputInt(i, "choise parameter <<i>>", 8);
		switch (i)
		{
		case 0:
		{
			cout << "0: exit" << endl;
			return 0;
		}
		case 1:
		{
			cout << "1: create the list/add an element of the list" << endl;
			list.Push();
			break;
		}
		case 2:
		{
			cout << "2: delete some elements of the list" << endl;
			int n = 0;
			cout << " Enter a count of <<delete>> operations" << endl;
			rewind(stdin);
			cin >> n;
			cout << " Delete from" << endl;
			cout << " 0: head" << endl;
			cout << " 1: tail" << endl;
			InputInt(i, "choise parameter <<i>>", 1);
			list.Pop(i, n);
			break;
		}
		case 3:
		{
			cout << "3: show an element of the list" << endl;
			int number;
			cout << " Enter a number of the element you want to see" << endl;
			iter(list.Peek(0));
			InputInt(number, "number of the element", algor.Size(iter)-1);
			iter(list.Peek(0));
			algor.PrintElement(iter, number);
			break;
		}
		case 4:
		{
			cout << "4: show the list" << endl;
			iter(list.Peek(0));
			if (algor.IsEmpty(iter))
				cout << " list is not exists" << endl;
			else
			{
				iter(list.Peek(0));
				algor.PrintList(*iter);
			}
			break;
		}
		case 5:
		{
			cout << "5: write the list into a text file" << endl;
			iter(list.Peek(0));
			textFile.OfstreamOpen();
			do
			{
				textFile.Write((*iter)->info);
			} while (++iter);
			textFile.OfstreamClose();
			break;
		}
		case 6:
		{
			cout << "6: read the list from the text file" << endl;
			iter(list.Peek(2));
			textFile.IfstreamOpen();
			do
			{
				textFile.Read((*iter)->info);
			} while (--iter);
			textFile.IfstreamClose();
			break;
		}
		case 7:
		{
			cout << "7: write the list into a binary file" << endl;
			iter(list.Peek(0));
			binFile.OfstreamOpen();
			do
			{
				binFile.Write(*iter);
			} while (++iter);
			binFile.OfstreamClose();
			break;
		}
		case 8:
		{
			cout << "8: read the list from the binary file" << endl;
			iter(list.Peek(2));
			binFile.IfstreamOpen();
			do
			{
				binFile.Read(*iter);
			} while (--iter);
			binFile.IfstreamClose();
			break;
		}
		}
	}
	return 0;
}