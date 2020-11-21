#include <iostream>
#include <vector>

#define MAX 0x7FFFFFFF

using namespace std;

#define uli long int
#define skip cout<<endl
//#define skip(n) for(int x = 0; x < n; ++x){cout << endl;}
#define co cout<<
#define ci cin>>
#define en <<endl
#define MAXSIZE 5
//#define HASHSIZE 5

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class BTreeX
{
public:
	int size;

	vector<T> keyB;
	vector<BTreeX*> nd;

	BTreeX* parent;

	BTreeX() { nd.resize(MAXSIZE + 1); }
	BTreeX(BTreeX* p) :parent(p) { nd.resize(MAXSIZE + 1); }

	void become_to_nodeB(T k, BTreeX* x, BTreeX* y);
	void splitB();
	void sortB();
	void insert_to_keyB(T k);
	bool is_nullB(vector<BTreeX*> t);
	void insertB(T k);
	void showB();
	bool findB(T k);
	BTreeX<T>* searchB(T k);

	//void remove(T k);
	//void insert_for(BTreeX* t, T k);




	BTreeX<T>* search(T k);
	BTreeX<T>* search_min();
};



template<typename T>
void BTreeX<T>::sortB()
{
	for (size_t i = 0; i < keyB.size() - 1; i++)
	{
		for (size_t j = i + 1; j < keyB.size(); j++)
		{
			if (keyB[i] > keyB[j]) swap(keyB[i], keyB[j]);
		}
	}
}

template<typename T>
void BTreeX<T>::insert_to_keyB(T k)
{
	keyB.push_back(k);
	size++;
	sortB();
}

template<typename T>
void BTreeX<T>::insertB(T k)
{
	if (this == NULL) return;
	else if (is_nullB(nd))	this->insert_to_keyB(k);
	else if (k <= keyB[0]) nd[0]->insertB(k);
	else
	{
		for (size_t i = 1; i < keyB.size() - 1; i++)
		{
			if (keyB[i - 1] < k && keyB[i] >= k)
			{
				nd[i]->insertB(k);
				return this->splitB();
			}
		}
		if (keyB[keyB.size() - 1] < k)
		{
			nd[keyB.size()]->insertB(k);
		}
	}
	return this->splitB();
}

template<typename T>
void BTreeX<T>::splitB()
{
	if (size < MAXSIZE) return;

	BTreeX* x = new BTreeX(parent);
	BTreeX* y = new BTreeX(parent);

	for (size_t i = 0; i < (MAXSIZE - 1) / 2; i++) x->insertB(keyB[i]);
	for (size_t i = (MAXSIZE - 1) / 2 + 1; i < MAXSIZE; i++) y->insertB(keyB[i]);

	for (size_t i = 0; i < nd.size() / 2; i++) x->nd[i] = nd[i];
	for (size_t i = nd.size() / 2; i < nd.size(); i++) y->nd[i - nd.size() / 2] = nd[i];

	for (size_t i = 0; i < x->nd.size(); i++)
	{
		if (x->nd[i] != nullptr) x->nd[i]->parent = x;
	}

	for (size_t i = 0; i < y->nd.size(); i++)
	{
		if (y->nd[i] != nullptr) y->nd[i]->parent = y;
	}

	if (parent)
	{
		parent->insert_to_keyB(keyB[(keyB.size() - 1) / 2]);

		for (int i = nd.size() - 1; i >= 0; --i)
		{
			if (parent->nd[i] != nullptr && parent->nd[i] != this)
			{
				for (int j = i; j >= 0; --j)
				{
					if (parent->nd[j] != this)
					{
						parent->nd[j + 1] = parent->nd[j];
					}
					else
					{
						parent->nd[j] = x;
						parent->nd[j + 1] = y;
						break;
					}
				}
				break;
			}
			else if (parent->nd[i] == this)
			{
				parent->nd[i] = x;
				parent->nd[i + 1] = y;
				break;
			}

		}
	}
	else
	{
		x->parent = this;
		y->parent = this;
		become_to_nodeB(keyB[(keyB.size() - 1) / 2], x, y);
	}
}

template<typename T>
void BTreeX<T>::become_to_nodeB(T k, BTreeX* x, BTreeX* y)
{
	keyB.clear();
	keyB.push_back(k);
	nd[0] = x;
	nd[1] = y;
	for (size_t i = 2; i < nd.size(); i++) nd[i] = nullptr;

	size = 1;
}

template<typename T>
bool BTreeX<T>::is_nullB(vector<BTreeX*> t)
{
	for (size_t i = 0; i < t.size(); i++)
	{
		if (t[i] != nullptr) return false;
	}
	return true;
}


template<typename T>
void BTreeX<T>::showB()
{
	if (this != nullptr)
	{
		for (int i = 0; i < nd.size(); i++)
		{
			this->nd[i]->showB();
		}

		for (int j = 0; j < keyB.size(); j++)
		{
			co " " << this->keyB[j] en;
		}
	}
	else return;
}


template<typename T>
bool BTreeX<T>::findB(T k)
{
	for (int i = 0; i < size; ++i)
		if (keyB[i] == k) return true;
	return false;
}



template<typename T>
BTreeX<T>* BTreeX<T>::searchB(T k)
{
	if (!this) return nullptr;
	for (int i = 0; i < nd.size(); i++)
	{
		if (this->findB(k)) return this;
		else if (k <= keyB[0]) return this->nd[0]->searchB(k);
		else
		{
			for (size_t i = 1; i < keyB.size() - 1; i++)
			{
				if (keyB[i - 1] < k && keyB[i] >= k)
				{
					return this->nd[i]->searchB(k);
				}
			}
			if (keyB[keyB.size() - 1] < k)
			{
				return this->nd[keyB.size()]->searchB(k);
			}
		}
	}
}





//template<typename T>
//void BTreeX<T>::insert_for(BTreeX* t, T k)
//{
//	if (this != nullptr)
//	{
//		if (this->key[0] != NULL && this->key[0] != k) t->insert(this->key[0]);
//		if (this->key[1] != NULL && this->key[1] != k) t->insert(this->key[1]);
//		if (this->key[2] != NULL && this->key[2] != k) t->insert(this->key[2]);
//
//		this->first->insert_for(t, k);
//		this->second->insert_for(t, k);
//		this->third->insert_for(t, k);
//	}
//	else return;
//}
//
//template<typename T>
//void BTreeX<T>::remove(T k)
//{
//	BTreeX* item = this->search(k); // Ищем узел, где находится ключ k
//
//	BTreeX* p = item->parent;
//
//	if (!item->first)
//	{
//		if (item->size == 2 && item->key[1] == k) item->key[1] = NULL;
//		else if (item->size == 2)
//		{
//			swap(item->key[0], item->key[1]);
//			item->key[1] = NULL;
//		}
//		else
//		{
//			BTreeX* buf = new BTreeX();
//			insert_for(buf, k);
//			if (p->parent->first == p) p->parent->first = buf;
//			else if (p->parent->second == p) p->parent->second = buf;
//			else if (p->parent->third == p) p->parent->third = buf;
//			delete p;
//		}
//	}
//	else
//	{
//		BTreeX* buf = new BTreeX();
//		item->insert_for(buf, k);
//		if (p->first == item) p->first = buf;
//		else if (p->second == item) p->second = buf;
//		else if (p->third == item) p->third = buf;
//		delete item;
//	}
//
//}
