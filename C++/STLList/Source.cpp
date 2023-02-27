#include<iostream>
#include <list>
#include <iterator>
#include"Header.h"
using namespace std;

int main() {
    list <int> foliage;
    list <int> listmerge = { 7, 8, 9 };
    int info = 0;
	while (1)
	{
		system ("cls");
		cout << "What do you want to do?" << endl;
		cout << "	0: exit" << endl;
		cout << "	1: add some elements into a list" << endl;
		cout << "	2: delete some elements from the list" << endl;
		cout << "	3: clear the list" << endl;
		cout << "	4: sort the list" << endl;
		cout << "	5: show the list" << endl;
		cout << "Your choise is ";
		int i = 0;
		InputInt(i, "choise parameter <<i>>", 5);
		switch (i)
		{
		case 0:
		{
			cout << "0: exit" << endl;
			return 0;
		}
		case 1:
		{
			system("cls");
			cout << "1: add an element into a list into a begin" << endl;
			cout << "	0: return to the menu" << endl;
			cout << "	1: add an element into a list's begin" << endl;
			cout << "	2: add an element into a list's end" << endl;
			cout << "	3: add some elements into a list" << endl;
			cout << "Your choise is ";
			InputInt(i, "choise parameter <<i>>", 3);
			switch (i)
			{
			case 0:
			{
				cout << "1.0: return to the menu" << endl;
				break;
			}
			case 1:
			{
				system("cls");
				cout << "1.1: add an element into a list's begin" << endl;
				PushFront(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			case 2:
			{
				system("cls");
				cout << "1.2: add an element into a list's end" << endl;
				PushBack(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			case 3:
			{
				system("cls");
				cout << "1.3: add some elements into a list" << endl;
				Insert(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			}
			break;
		}
		case 2:
		{
			system("cls");
			cout << "2: delete some elements from the list " << endl;
			cout << "	0: return to the menu" << endl;
			cout << "	1: delete the element from the list's begin" << endl;
			cout << "	2: delete the element from the list's end" << endl;
			cout << "	3: delete some elements from the list" << endl;
			cout << "Your choise is ";
			InputInt(i, "choise parameter <<i>>", 3);
			switch (i)
			{
			case 0:
			{
				cout << "2.0: return to the menu" << endl;
				break;
			}
			case 1:
			{
				system("cls");
				cout << "2.1: delete the element from the list's begin" << endl;
				PopFront(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			case 2:
			{
				system("cls");
				cout << "2.2: delete the element from the list's end" << endl;
				PopBack(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			case 3:
			{
				system("cls");
				cout << "2.3: delete some elements from the list" << endl;
				Erase(foliage);
				cout << "The operation is performed!" << endl;
				_getch();
				break;
			}
			}
			break;
		}
		case 3:
		{
			system("cls");
			cout << "3: clear the list" << endl;
			Clear(foliage);
			cout << "The list is empty!" << endl;
			_getch();
			break;
		}
		case 4:
		{
			system("cls");
			cout << "4: sort the list" << endl;
			cout << "	0: return to the menu" << endl;
			cout << "	1: sort the list up" << endl;
			cout << "	2: sort the list down" << endl;
			cout << "Your choise is ";
			InputInt(i, "choise parameter <<i>>", 2);
			switch (i)
			{
			case 0:
			{
				cout << "4.0: return to the menu" << endl;
				break;
			}
			case 1:
			{
				system("cls");
				cout << "4.1: sort the list up" << endl;
				SortUp(foliage);
				cout << "The list is sorted!" << endl;
				_getch();
				break;
			}
			case 2:
			{
				system("cls");
				cout << "4.2: sort the list down" << endl;
				SortDown(foliage);
				cout << "The list is sorted!" << endl;
				_getch();
				break;
			}
			}
			break;
		}
		case 5:
		{
			system("cls");
			cout << "5: show the list" << endl;
			if (foliage.empty())
				cout << "The list is empty" << endl;
			else Show(foliage);
			_getch();
			break;
		}
		}
	}
    return 0;
}