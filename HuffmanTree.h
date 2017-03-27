//HuffmanTree.h

#pragma once

#include "Heap.h"
#include "FileCompress.h"

template<class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T> *_left;
	HuffmanTreeNode<T> *_right;
	HuffmanTreeNode<T> *_parent;

	HuffmanTreeNode(): _left(NULL), _right(NULL), _parent(NULL)
	{}
	HuffmanTreeNode(const T& x): _weight(x), _left(NULL), _right(NULL), _parent(NULL)
	{}

	//string对象的拷贝构造？？？

	bool operator< (const HuffmanTreeNode<T>& x)
	{
		if(_weight < x._weight)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
	template<class T>
	struct LessPtr
	{
		bool operator() (const T& l, const T& r)
		{
			if(*l < *r)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
	protected:
		Node* _root;

	protected:
		void _Destroy(Node* root)
		{
			if(root)
			{
				_Destroy(root->_left);
				_Destroy(root->_right);
				delete root;
			}
		}
	public:
		HuffmanTree(): _root(NULL)
		{}
		HuffmanTree(T *arr, size_t size, const T& invalid)
		{
			assert(arr);
			assert(size);
			//建堆
			Heap<Node*, LessPtr<Node*> > hp;
			for(size_t i = 0; i < size; i++)
			{
				if(arr[i] != invalid)
				{
					Node* ptr = new Node(arr[i]);
					hp.Push(ptr);
				}
			}
			while(hp.Size() > 1)
			{
				Node* left = hp.Top();
				hp.Pop();
				Node* right = hp.Top();
				hp.Pop();
				Node* parent = new Node(left->_weight + right->_weight);
				parent->_left = left;
				parent->_right = right;
				left->_parent = parent;
				right->_parent = parent;
				hp.Push(parent);
			}
			if(hp.Size() == 0)
				_root = NULL;
			else
				_root = hp.Top();
		}
		~HuffmanTree()
		{
			if(_root)
			{
				_Destroy(_root);
				_root = NULL;
			}
		}
		Node* GetRoot()
		{
			return _root;
		}
};
