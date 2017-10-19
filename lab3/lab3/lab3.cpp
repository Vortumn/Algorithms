// lab3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include "stack"

using namespace std;

template <typename T>
struct Node //структура узла
{
	int key;
	T value;
	Node <T>* right;
	Node <T>* left;
	Node(int key, T value) : key(key), value(value), left(nullptr), right(nullptr) {};
};
template <typename T>

void PrintNode(Node<T>* Current) //функция вывода узла с потомками
{
	cout << "Current node key: " << Current->key << endl << "Current node value: " << Current->value << endl;
	if (Current->left) cout << "Left child key: " << Current->left->key << endl;
	if (Current->right) cout << "Right child key: " << Current->right->key << endl;
	cout << endl;
}


template <typename T>
class Tree
{
private:
	Node <T>* Root;
	int Tree_Size;
	int cheat = 0;
public:
	Tree() : Root(nullptr), Tree_Size(0) {};

	~Tree() {
		this->ClearTree(Root);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Add(int key, T value)
	{
		if (Tree_Size == 0) //если дерево пустое, то добавляем корневой элемент
		{
			Root = new Node<T>(key, value);
			Tree_Size++;
			if (key < 0) cheat++;
			return;
		}
		else
		{
			Node<T>* thisone = Root;
			Node<T>* nextone = nullptr;
			//1ый случай
			while (thisone)
			{
				if (thisone->key == key)
				{
					thisone->value = value;
					return;
				}
				else {
					nextone = thisone;
					thisone = (key > thisone->key) ? thisone->right : thisone->left;
				}
			}
			if (nextone)
			{
				if (key > nextone->key)
				{
					thisone = new Node<T>(key, value);
					if (key < 0) cheat++;
					nextone->right = thisone;
					Tree_Size++;
				}
				else if (key < nextone->key)
				{
					thisone = new Node<T>(key, value);
					nextone->left = thisone;
					if (key < 0) cheat++;
					Tree_Size++;
					return;
				}
			}
		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Node<T> *FindNode(int key) { //функция на поиск узла
		Node <T>* thisone = Root;
		while (thisone)
		{
			if (key == thisone->key) return thisone;
			else
			{
				thisone = (key > thisone->key) ? thisone->right : thisone->left;
			}
		}
		return nullptr;
	}


	T GetValue(int key) //получение значения элемента по ключу (5)
	{
		Node <T>* itemplace = FindNode(key);
		if (itemplace) return itemplace->value;
		else return NULL;
	}


	Node<T> *FindParent(int key)
	{
		Node <T>* thisone = Root;
		Node <T>* parent = nullptr;
		while (thisone)
		{
			if (key == thisone->key) return parent;
			else
			{
				parent = thisone;
				thisone = (key > thisone->key) ? thisone->right : thisone->left;
			}
		}
	}

	void DelItem(int key) //удаление узла по ключу (2)
	{
		Node <T>* itemplace = FindNode(key);
		if (!itemplace) return;
		else
		{
			Tree_Size--;
			//случай 1: у удаляемого узла нет правого ребенка
			if (!itemplace->right)
			{
				if (!itemplace->left)
				{
					Node <T>* parent = FindParent(itemplace->key);
					if (parent->right == itemplace) parent->right = nullptr;
					else if (parent->left == itemplace) parent->left = nullptr;
					delete itemplace;
					return;
				}
				else {
					Node<T>* nextone = itemplace->left; //попросту переносим левого на его место + удаляем предыдущего левого
					itemplace->value = nextone->value;
					itemplace->key = nextone->key;
					itemplace->left = nextone->left;
					delete nextone;
					return;
				}
			}

			//случай 2: У удаляемого узла есть правый ребенок, у которого, в свою очередь нет левого ребенка
			if (itemplace->right->left == nullptr)
			{
				Node<T>* nextone = itemplace->right; //переносим правого ребенка в itemplace itemplace, сохраняя левого
				itemplace->right = nextone->right;
				itemplace->key = nextone->key;
				itemplace->value = nextone->value;
				delete nextone;
				return;
			}
			//случай 3: все потомки в наличие и жаждут власти за престол
			else
			{
				Node<T>* nextone = itemplace->right;
				Node<T>* curr = nullptr;
				while (nextone)
				{
					curr = nextone;
					nextone = nextone->left;
				}
				Node <T>* parent = FindParent(curr->key);
				itemplace->value = curr->value;
				itemplace->key = curr->key;
				parent->left = nullptr;
				delete(curr);
				delete(nextone);
				return;
			}

		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ClearTree(Node<T>* Current) //функция очистки всего дерева (4)
	{
		if (!Current)
		{
			Tree_Size = 0;
			Root = nullptr;
			return;
		}
		else
		{
			ClearTree(Current->left);
			ClearTree(Current->right);
			delete Current;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ShowTreeSize() //получение количества узлов в дереве (3)
	{
		cout << "Tree Size is :" << Tree_Size << endl;
		cout << endl;
		return;
	}

	int TreeSize()
	{
		return Tree_Size;
	}

	Node<T>* GetRoot()
	{
		return Root;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsKeyInTree(int key) //Проверка, есть ли в дереве узел с заданным ключом (7)
	{
		Node <T>* itemplace;
		itemplace = FindNode(key);
		if (itemplace) return true;
		else return false;
	}

	void IsKeyPrint(int key)
	{
		if (IsKeyInTree(key))
		{
			cout << "Yep, dat key in the tree " << endl;
		}

		else {
			cout << "Nope, dat key is not in the tree " << endl;
		}
	}

	bool IsValueInTree(T value)
	{
		// Обходим в прямом порядке итеративно
		if (!Root) return false;

		stack<Node<T>*> Stack;
		Stack.push(Root);

		while (!Stack.empty()) {
			Node<T>* Current = Stack.top();
			Stack.pop();

			if (Current->value == value) return true;
			if (Current->right) Stack.push(Current->right);
			if (Current->left) Stack.push(Current->left);
		}
		return false;
	}

	void IsValuePrint(T value)
	{
		if (IsValueInTree(value))
		{
			cout << "Yep, dat value in a tree " << endl;
			return;
		}
		else
		{
			cout << "Nope, dat value isnt here" << endl;
			return;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void InOrderPrint(Node<T>* Root, void(*node_func)(Node<T>*)) //вывод дерева в обратном порядке (ЛКП) (8)
	{
		if (Root == nullptr) return;

		InOrderPrint(Root->left, node_func); //рекурсивный вызов правого поддерева
		node_func(Root);
		InOrderPrint(Root->right, node_func);//рекурсивный вызов левого поддерева
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ЗАДАНИЕ ПО ДЕЛЕНИЮ ДЕРЕВА

	void SetRoot(Node<T>* Current)
	{
		Root = Current;
	}

	void SetTreeSize(int n) //ленивое читерство
	{
		Tree_Size = n;
	}

	int Neg()
	{
		return cheat;
	}

	Node<T>* FindNegKey(Node<T>* Root) //для положительного корня
	{
		while (Root->key >= 0)
		{
			Root = Root->left;
		}
		return Root;
	}


	Node<T>* FindPosKey(Node<T>* Root) //для отрицательного корня
	{
		while (Root->key < 0)
		{
			Root = Root->right;
		}
		return Root;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

template <typename T>
void PrintTree(Tree <T>* thisone)
{
	thisone->ShowTreeSize();
	thisone->InOrderPrint(thisone->GetRoot(), PrintNode);
}

template <typename T>
void RealMagic(Tree <T>* thisonetree, Tree<T>* nextonetree)
{
	Node <T>* root = thisonetree->GetRoot();
	Node <T>* Current = nullptr;
	if (root->key >= 0) //если корень >= 0, то отрицательные значения могут находиться только в левом поддереве нашего дерева. Их и будем "отцеплять" в дерево 2
	{
		Current = thisonetree->FindNegKey(root); //найдем первое отрицательное значение левого поддерева 

		nextonetree->SetTreeSize(thisonetree->Neg()); //читерство на размер
		thisonetree->SetTreeSize(thisonetree->TreeSize() - nextonetree->TreeSize());
	}
	else //тоже самое, только с отрицательным корнем и положительными значениями в правом поддереве
	{
		Current = thisonetree->FindPosKey(root);
		
		nextonetree->SetTreeSize(thisonetree->TreeSize() - thisonetree->Neg()); //читерство на размер
		thisonetree->SetTreeSize(thisonetree->Neg());
	}
	Node <T>* Par = thisonetree->FindParent(Current->key);	//найдём его предка по ключу
	if (Par->left == Current) Par->left = nullptr;//сделаем nullptr у предка на данное значение
	else if (Par->right == Current) Par->right = nullptr;

	nextonetree->SetRoot(Current);//пильнём в корень второго дерева полученное значение

	
}


using namespace std;

int main()
{
	Tree<int> tree;
	Tree<int> tree_two;
	int k;
	cout << "Input the size of the tree " << endl;
	int TREE_SIZE;
	cin >> TREE_SIZE;

	for (int i = 0; i < TREE_SIZE; i++)
	{
		cout << "Input key/value " << endl;
		cin >> k;
		tree.Add(k, k);
	}

	PrintTree(&tree); //вывели дерево (8) с количеством узлов (3)

	RealMagic(&tree, &tree_two);

	cout << "LADIES AND GENTLEMEN " << endl;
	cout << "First tree! " << endl << endl;
	PrintTree(&tree);
	cout << endl;
	cout << "Second tree! " << endl << endl;
	PrintTree(&tree_two);
	


	//int key;
	//cout << "Input key of the node to del" << endl;
	//cin >> key;
	//tree.DelItem(key); //удалили (2)
	//PrintTree(&tree); //вывели дерево (8) с количеством узлов (3)
	
	//int val; 
	//cout << "Input the key of node which value you want to see" << endl;
	//cin >> key;
	//val = tree.GetValue(key); //значение по ключу (5)
	//cout << val << endl;

	//cout << "Input the key you want to find in tree " << endl;
	//cin >> key;
	//tree.IsKeyPrint(key); //проверка, есть ли ключ в дереве (6)

	//cout << "Input the value you want to find in tree " << endl;
	//cin >> val;
	//tree.IsValuePrint(val); //(7)

	//tree.ClearTree(tree.GetRoot());

	system("pause");
    return 0;
}

