#pragma once
#include <iostream>
using namespace std;
#define TH template<typename Type>
TH
class BST
{
	struct Node
	{
		Node* left = nullptr;
		Node* right = nullptr;
		Type data;
	};
	Node* root = nullptr;
public:	
	BST() { root = nullptr; }
	~BST() { clear(); }
	BST& operator=(const BST& that);
	BST(const BST& that) { *this = that; }
	void insert(const Type& v);
	bool findAndRemove(const Type& v);
	bool find(const Type& v) const;
	void clear();
	void printInOrder() const;
	void InOrder(const Node*) const;
	void PreOrder(const Node*);
	void PostOrder(const Node*);
	void PreOrderToArray();
};
TH
bool BST<Type>::findAndRemove(const Type& v)
{
	Node* iter = root;
	Node* parent = nullptr;

	while (iter)
	{
		if (v < iter->data)
		{
			parent = iter;
			iter = iter->left;
		}
		else if (v > iter->data)
		{
			parent = iter;
			iter = iter->right;
		}
		else
			break;
	}
	if (iter == nullptr)
		return false;

	if (iter->left && iter->right)
	{
		Node* min = iter->right;
		while (min->left)
		{
			parent = min;
			min = min->left;
		}
		swap(iter->data, min->data);
		iter = min;
	}
	if (iter->left == nullptr && iter->right == nullptr)
	{
		if (parent == nullptr)
		{
			delete iter;
			root = nullptr;
		}
		else if (parent->left == iter)
		{
			parent->left = nullptr;
			delete iter;

		}
		else
		{
			parent->right = nullptr;
			delete iter;
		}
	}
	else if (iter->left && iter->right == nullptr)
	{
		if (parent == nullptr)
		{
			root = iter->left;
			delete iter;
		}
		else if (parent->left == iter)
		{
			parent->left = iter->left;
			delete iter;

		}
		else
		{
			parent->right = iter->left;
			delete iter;
		}
	}
	else
	{
		if (parent == nullptr)
		{
			root = iter->right;
			delete iter;
		}
		else if (parent->left == iter)
		{
			parent->left = iter->right;
			delete iter;

		}
		else
		{
			parent->right = iter->right;
			delete iter;
		}
	}
	return true;
	
}
TH
BST<Type>& BST<Type>::operator=(const BST& that)
{
	if (this != &that)
	{
		this->clear();

		PreOrder(that.root);
	}
	return *this;
}
TH
void BST<Type>::PreOrderToArray()
{
	Node* temp;
	temp = root;
	PreOrder(temp);
}
TH
void BST<Type>::insert(const Type& v)
{
	Node* tmp = new Node;
	tmp->data = v;
	Node* iter;
	iter = root;
	
	if (root == nullptr)
	{
		root = tmp;
		return;
	}
	while (true)
	{
		if (iter->data > v) // if v is less than root
		{
			if (iter->left == nullptr)
			{
				iter->left = tmp;
				return;
			}
			if (iter->left != nullptr)
			{
				iter = iter->left;
			}
		}
		else if (iter->data <= v) // if v is greater than root
		{
			if (iter->right == nullptr)
			{
				iter->right = tmp;
				return;
			}
			if (iter->right != nullptr)
			{
				iter = iter->right;
			}
		}
	}
}
TH
void BST<Type>::printInOrder() const
{
	Node* temp;
	temp = root;
	InOrder(temp);
	cout << endl;
}
TH
void BST<Type>::InOrder(const Node* n) const
{
	if (n != nullptr)
	{
		
		InOrder(n->left);
		cout << n->data << " ";
		InOrder(n->right);
	}
}
TH
bool BST<Type>::find(const Type& v) const
{
	Node* iter;	
	iter = root;
	
	if (iter == nullptr)
	{
		return false;		
	}
	if (iter->data == v)
	{
		return true;
	}
	while (true)
	{
		if (v < iter->data)
		{
			//check iter left for null
			if (iter->left != nullptr)
			{
				if (iter->left->data == v)
				{
					return true;				
				}
				else
				{
					iter = iter->left;					
				}
			}
			else if (iter->left == nullptr)
				return false;
			//if not then make iter equal to 
		}
		else if (v >= iter->data)
		{
			if (iter->right != nullptr)
			{
				if (iter->right->data == v)
				{
					return true;					
					break;
				}
				else
				{
					iter = iter->right;
					
				}
			}
			else if (iter->right == nullptr)
				return false;
		}
	}
}
TH
void BST<Type>::clear()
{
	Node* n;
	n = root;
	PostOrder(n);
	root = nullptr;
}
TH
void BST<Type>::PostOrder(const Node* n)
{
	if (n != nullptr)
	{		
		PostOrder(n->left);
		PostOrder(n->right);
		delete n;
	}
}
TH
void BST<Type>::PreOrder(const Node* n)
{
	if (n != nullptr)
	{		
		insert(n->data);
		PreOrder(n->left);
		PreOrder(n->right);
		
	}
}