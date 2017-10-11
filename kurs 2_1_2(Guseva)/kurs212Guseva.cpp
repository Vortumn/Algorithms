#include "stdafx.h"
#include <iostream>
using namespace std;


struct List
{
	int  info;
	List * prevadress,
		*nextadress;
};

List *CreateList(unsigned Length)
{
	List *current = 0,
		*Next = 0;
	cout << "Input the information about your elements from the end to begin" << endl;
	// Начинаем создавать список с последнего элемента
	for (unsigned i = 1; i <= Length; ++i)
	{
		current = new List;
		cin >> current->info;
		current->nextadress = Next; //последний элемент имеет 0 адрес последующего (логично)

		if (Next) //не равен 0
			Next->prevadress = current;

		Next = current;
	}

	//  Для первого элемента списка адрес предыдущего элемента - 0
	current->prevadress = 0;
	return current;
}

void DeleteList(List * &Beg)
{
	List *Next;
	while (Beg)
	{
		Next = Beg->nextadress;
		delete Beg;
		Beg = Next;
	}
}

List *ListItem(List *Beg, unsigned Index, bool ErrMsg = true) //доступ к элементу по индексу
{
	//  Цикл заканчивается, когда список закончился (Beg == 0) или найден искомый
	while (Beg && (Index--))
		Beg = Beg->nextadress;
	if (ErrMsg && !Beg)
		cout << "Element is not in the list \n";
	return Beg;
}

//длина списка
unsigned LengthList(List * Beg)
{
	unsigned Length = 0;   //  Счетчик элементов списка
						   // Начинаем с начала списка
	while (Beg)
	{
		// Увеличиваем счетчик элементов списка на единицу
		++Length;
		// Перемещаемся на следующий элемент списка
		Beg = Beg->nextadress;
	}
	return Length;
}


//удаление элемента по номеру 
void DelItem(List * &Beg, unsigned Index)
{
	if (Index >= LengthList(Beg))
		return;

	List *Item;

	if (!Index) //если индекс = 0
	{
		Item = Beg->nextadress; //Item - адрес на след элемент
		delete Beg; //удаляем 0 элемент
		Beg = Item; //присваеваем указателю на начало списка новое значение
		Beg->prevadress = 0;
		return;
	}

	
	Item = ListItem(Beg, Index - 1, 0);  //получение элемента, стоящего раньше нужного
	List * DItem = Item->nextadress; //получение адреса нужного элемента (для удаления)
	if (DItem->nextadress != 0)
	{
		
		Item->nextadress = DItem->nextadress; //забиваем в адрес предыдущего адрес следующего за удаляемым
		List *NextItem = ListItem(Beg, Index + 1, 0); //получаем след элемент
		NextItem->prevadress = Item; //забиваем ему в адрес предыдущего адрес элемента до
	}
	else
	{
		Item->nextadress = 0;
	}
	delete DItem;
}


int main()
{
	List *current;
	cout << "Please input the length of the list you want to create " << endl;
	int n;
	cin >> n;
	
	List *DatBoy = CreateList(n); //DatBoy - адрес первого элемента в списке

	current = DatBoy; //вывод на экран
	cout << endl;
	cout << "Your list is ";
	while (current)
	{
		cout << current->info << " ";
		current = current -> nextadress;
	}
	cout << endl;

	//удаление элемента по номеру
	cout << "Input number of element you want to delete " << endl;
	int num;
	cin >> num;
	DelItem(DatBoy, num);

	cout << endl;
	cout << "Here we are " << endl;
	//повторный вывод списка на экран
	current = DatBoy; //вывод на экран
	while (current)
	{
		cout << current->info << " ";
		current = current->nextadress;
	}
	cout << endl;

	//удаление списка
	DeleteList(DatBoy);
	system("pause");

    return 0;
}

