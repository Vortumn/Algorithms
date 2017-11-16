#include "stdafx.h"
#include "iostream"
#include "stack"

#define red 1
#define black 0
using namespace std;

template <typename T>
struct Node //структура узла
{
	int key;
	T value;
	bool color;
	Node <T>* right;
	Node <T>* left;
	Node <T>* parent;
	Node(int key, T value, bool color) : key(key), value(value), color(color), left(nullptr), right(nullptr){};
};


template <typename T>

void PrintNode(Node<T>* Current) //функция вывода узла с потомками
{
	if (Current->key == 0 && Current->value == 0) return;
	cout << "Current node key: " << Current->key << endl << "Current node value: " << Current->value << endl;
	cout << "Current node color: ";
	if (Current->color == red) cout << "red" << endl;
	if (Current->color == black) cout << "black" << endl;
	if (Current->left) cout << "Left child key: " << Current->left->key << endl;
	if (Current->right) cout << "Right child key: " << Current->right->key << endl;
	if (Current->parent) cout << "Parent key: " << Current->parent->key << endl;
	cout << endl;
}


template <typename T>
class Tree
{
private:
	Node <T>* Root;
	Node <T>* NIL = new Node<T>(0, 0, black); //ограничитель 
	int Tree_Size;

public:
	Tree() : Root(nullptr), Tree_Size(0) {};

	~Tree() {
		this->ClearTree(Root);
	}


	void NilChildren(Node<T>* ournode) //создание черных потомков для узла
	{
		ournode->left = NIL;
		ournode->right = NIL;
	}


	void Add(int key, T value) //функция добавления элемента в КЧ-дерево (почти аналогична добавлению в бинарное)
	{
		if (Tree_Size == 0) //если дерево пустое, то добавляем корневой элемент
		{
			Root = new Node<T>(key, value, black);
			Root->parent = NIL; //используем ограничитель и для родителя корневого узла
			NilChildren(Root);
			Tree_Size++;
			return;
		}
		else
		{
			Node<T>* thisone = Root;
			Node<T>* nextone = NIL;
			while(thisone != NIL)
			{
				if (thisone->key == key) //проверка на совпадения
				{
					thisone->value = value;
					return;
				}
				else
				{
					nextone = thisone;
					thisone = (key < thisone->key) ? thisone->left : thisone->right;
				}
			}
			Node<T>* ournode = new Node<T>(key, value, red);//создаём новый дочерний с дефолтными значениями параметров
			ournode->parent = nextone; // родителя 
			NilChildren(ournode); // черных NIL-детей
			Tree_Size++;
			if (key < nextone->key) nextone->left = ournode;
			else nextone->right = ournode;
			FixTreeBalance(ournode); //и фиким баланс в КЧ-дереве после определения места вставки
			return;
		}
	}


	void FixTreeBalance(Node<T>* datboy) //функция восстановления КЧ-свойств 
	{
		Node<T> * uncle = NIL; //вводим переменную для дяди
		Node <T> * P = NIL; 
		Node <T> * G = NIL;
		{
			while (datboy->parent->color == red && datboy != Root) //цикл пока родитель текущего элемента красный
			{
				uncle = (datboy->parent == datboy->parent->parent->left) ? datboy->parent->parent->right : datboy->parent->parent->left;
				P = datboy->parent;
				G = datboy->parent->parent;

				//случаи 1,2: красный родитель, красный дядя (перекраска)
				if (uncle->color == red)
				{
					G->color = red;
					P->color = black;
					uncle->color = black;
					datboy = G; //переносим внимание цикла на деда, чтобы проверить КЧ-свойства для всего дерева
				}

				//случаи 3,4,5,6: красный родитель, черный дядя
				//если речь идёт о левом поддереве, а дядя является правым по отношению к G

				else if (uncle->color == black && uncle == G->right)
				{
					//случай 3: если datboy - правый потомок
					if (datboy == datboy->parent->right)
					{
						datboy = datboy->parent;
						LeftRotate(datboy);
					}
					else //случай 4: если datboy - левый потомок
					{
						datboy->parent->color = black;
						datboy->parent->parent->color = red;
						RightRotate(datboy->parent->parent);
					}
				}
				//если речь идёт о правом поддереве, а дядя является левым по отношению к G
				else if (uncle->color == black && uncle == G->left)
				{
					//случай 5: если datboy - левый потомок
					if (datboy == datboy->parent->left)
					{
						datboy = datboy->parent;
						RightRotate(datboy);
					}
					//случай 6: если datboy - правый потомок
					else
					{
						datboy->parent->color = black;
						datboy->parent->parent->color = red;
						LeftRotate(datboy->parent->parent);
					}
				}

			} //кц while
		}	
		Root->color = black;
		return;
	}


	void LeftRotate(Node<T>* p)
	{
		Node <T> *z = p->right; //правый потомок P

		if (p->parent != NIL) //ежели родитель есть
		{
			if (p == p->parent->left) p->parent->left = z; //разбираемся с родителем P
			else p->parent->right = z;
			z->parent = p->parent;	

		}
		else {							//ежели родителя нет, то меняем и глобальный корень дерева
			z->parent=NIL;
			Root = z;
		}

		if (z->left != NIL || z->left != nullptr) //обмен дочерними элементами
		{
			p->right = z->left;
			z->left->parent = p;
		}

		else p->right = NIL;
		z->left = p;
		p->parent = z;
	}

	void RightRotate(Node<T>* p)
	{
		{
			Node <T> *z = p->left; //левый потомок P
			if (p->parent != NIL)
			{
				if (p == p->parent->left) p->parent->left = z; //разбираемся с родителем P
				else p->parent->right = z;
				z->parent = p->parent;

			}
			else {							//ежели родителя нет, то меняем и глобальный корень дерева
				z->parent=NIL;
				Root = z;
			}

			if (z->right != NIL || z->right != nullptr) //разбираемся с обменом потомками
			{
				p->left = z->right;
				z->right->parent = p;
			}
			else p->left = NIL;

			z->right = p;
			p->parent = z;
		}
	}


	Node<T> *FindNode(int key) { //функция на поиск указателя на узел по ключу
		Node <T>* thisone = Root;
		while (thisone != NIL)
		{
			if (key == thisone->key) return thisone;
			else
			{
				thisone = (key > thisone->key) ? thisone->right : thisone->left;
			}
		}
		return NIL;
	}



	void Swap(Node <T> *Parent, Node<T>*Child) //функция замены ссылки у дочернего дедушки на ребенка удаляемого элемента
	{
		if (Parent->parent != NIL)
		{
			if (Parent->parent->left == Parent)
			{
				Parent->parent->left = Child;
				Child->parent = Parent->parent;
			}
			else
			{
				Parent->parent->right = Child;
				Child->parent = Parent->parent;
			}
		}
		else //если родителя нет, следовательно, элемент - корень
		{
			Child->parent = NIL;
			Root = Child;
		}
	}



	void Del(int key)
	{
		Node <T>* itemplace = FindNode(key);
		Node <T>* y; //переменная под потомка в некоторых случаях
		bool color = itemplace->color; //и цвет (если он черный, то нужно будет восстанавливать свойства дерева)
		Node <T>* x = NIL; //еще одна доп переменная для потомка потомка (в частности - определяет необходимость фикса)

		if (itemplace == NIL) return;
		else
		{
			Tree_Size--;
			//случай 1: у удаляемого элемента оба потомка - NIL
			if (itemplace->right == NIL && itemplace->left == NIL)
			{
				if (itemplace->parent->left == itemplace) itemplace->parent->left = NIL; //зануляем у родителя ссылку на удаляемый элемент
				else if (itemplace->parent->right == itemplace) itemplace->parent->right = NIL; //
				else Root = NIL; //ну или дерево превращается в NIL
				delete itemplace;
			}
			//случай 2: у удаляемого элемента нет левого или правого потомка
			else if (itemplace->right == NIL || itemplace->left == NIL)
			{
				y = (itemplace->left == NIL) ? itemplace->right : itemplace->left; //находим непустого ребенка
				Swap(itemplace, y);
				//так как ссылка на ребенка изменена, то можно просто удалить текущий элемент
				delete itemplace;
			}

			//случай 3: у удаляемого элемента есть оба потомка, но нет левого ребенка у правого
			else if (itemplace->right != NIL && itemplace->left != NIL && itemplace->right->left == NIL)
			{
				y = itemplace->right;
				x = y->right;
				Swap(itemplace, y); //изменение ссылок родителей
				y->left = itemplace->left; //присваиваем y ребенка itemplace
				y->left->parent = y;
				delete itemplace;
			}

			//случай 4: есть оба ребенка и нет исключений
			else if (itemplace->right != NIL && itemplace->left != NIL && itemplace->right->left != NIL)
			{
				x = itemplace->right;
				y = NIL;
				while (x != NIL) //находим левый минимум у правого потомка удаляемого элемента
				{
					y = x;
					x = x->left;
				}

				x = y->right; //меняем значение переменной x, использовавшейся ранее как расходный материал поиска минимума
				y->parent->left = x; //переприсваиваем родительские функции левому ребенку y
				x->parent = y->parent;
				itemplace->left->parent = y; //отбираем детишек у удаляемого элемента
				itemplace->right->parent = y;
				y->left = itemplace->left; //меняем детишек у y
				y->right = itemplace->right;
				Swap(itemplace, y); //меняем родителей
				delete itemplace;
			}

			if (color == black) FixDelBalance(x);
		}
	}




	void FixDelBalance(Node<T> *x)
	{
		Node <T>* w; //переменная под дядю
		while (x != NIL && x->color == black)
		{
			//определяем переменную дяди
			w = (x == x->parent->left) ? x->parent->right : x->parent->left;

			//случай 1: красный дядя
			if (w->color == red)
			{
				w->color = black; //перекрас дяди
				x->parent->color = red;

				//для x - левого потомка
				if (x == x->parent->left)
				{
					LeftRotate(x->parent); // делаем левый поворот через родителя
					w = x->parent->right; // меняем переменную дяди на новую (чтобы он все ещё оставался братом x)
				}

				//для x - правого потомка
				else
				{
					RightRotate(x->parent);
					w = x->parent->left;

				}
			}

			else
			{
			//случай 2: дядя черный и его потомки тоже (от положения х не зависит)
			if (w->left->color == black && w->right->color == black)
			{
				w->color = red; //тупо перекрас
				x = x->parent; //и меняем х уже на родителя
			}

			//случай 3: дядя черный, его левый ребенок - красный, правый - черный (если х - слева)
			else if (w == w->parent->right && w->left->color == red && w->right->color == black) //если дядя справа (х - слева)
			{
				w->left->color = black; //тупо перекрас и кручение вокруг родителя
				w->color = red;
				RightRotate(w);
				w = x->parent->right; //плюс переприсваивание w новому положению
			}
			//случай 3 : дядя черный, его правый ребенок - красный, левый - черный (если х - справа)
			else if (w == w->parent->left && w->right->color == red && w->left->color == black) //если дядя слева (х - справа)
			{
				w->right->color = black;
				w->color = red;
				LeftRotate(w);
				w = x->parent->left;
			}

			//случай 4: дядя черный, его правый ребенок - красный (если х - слева)
			else if (w->right->color == red && x == x->parent->left)
			{
				w->color = x->parent->color;
				x->parent->color = black;
				w->right->color = black;
				LeftRotate(x->parent);
				x = Root; //меняем значение x на корень
			}
			//случай 4: дядя черный, его левый ребенок - красный (если х - справа)
			else if (w->left->color == red && x == x->parent->right)
			{
				w->color = x->parent->color;
				x->parent->color = black;
				w->left->color = black;
				RightRotate(x->parent);
				x = Root;
			}
			}
		}
		x->color = black;
	}



	T GetValue(int key) //получение значения элемента по ключу (5)
	{
		Node <T>* itemplace = FindNode(key);
		if (itemplace) return itemplace->value;
		else return NULL;
	}

	Node<T>* GetRoot()
	{
		return Root;
	}

	void ShowTreeSize() //получение количества узлов в дереве (3)
	{
		cout << "Tree Size is :" << Tree_Size << endl;
		cout << endl;
		return;
	}

	void ClearTree(Node<T>* Current) //функция очистки всего дерева (4)
	{
		if (!Current || Current == NIL)
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

	void InOrderPrint(Node<T>* Root, void(*node_func)(Node<T>*)) //вывод дерева в обратном порядке (ЛКП) (8)
	{
		if (Root == nullptr) return;

		InOrderPrint(Root->left, node_func); //рекурсивный вызов правого поддерева
		node_func(Root);
		InOrderPrint(Root->right, node_func);//рекурсивный вызов левого поддерева
	}

};

template <typename T>
void PrintTree(Tree <T>* thisone)
{
	thisone->ShowTreeSize();
	thisone->InOrderPrint(thisone->GetRoot(), PrintNode);
}

using namespace std;

int main()
{
	Tree<int> tree;
	int k;
	cout << "Input the size of the tree " << endl;
	int TREE_SIZE;
	cin >> TREE_SIZE;

	for (int i = 0; i < TREE_SIZE; i++)
	{
		cout << "Input key/value " << endl;
		cin >> k;
		tree.Add(k, k+2);
	}

	PrintTree(&tree);

	cout << "Add someone new" << endl;
	cin >> k;
	tree.Add(k, k + 2);
	cout << "----------------------------------------------------------" << endl;
	PrintTree(&tree);

	cout << "Add someone new" << endl;
	cin >> k;
	tree.Add(k, k + 2);
	cout << "----------------------------------------------------------" << endl;
	PrintTree(&tree);

	cout << "Add someone new" << endl;
	cin >> k;
	tree.Add(k, k + 2);
	cout << "----------------------------------------------------------" << endl;
	PrintTree(&tree);


	cout << "Delete someone" << endl;
	cin >> k;
	tree.Del(k);
	PrintTree(&tree);

	cout << "Delete someone" << endl;
	cin >> k;
	tree.Del(k);
	PrintTree(&tree);

	cout << "Delete someone" << endl;
	cin >> k;
	tree.Del(k);
	PrintTree(&tree);


	cout << "Delete someone" << endl;
	cin >> k;
	tree.Del(k);
	PrintTree(&tree);


	system("pause");
	return 0;
}
