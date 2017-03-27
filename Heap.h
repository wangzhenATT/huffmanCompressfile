//heap.h
#pragma once

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

template<class T>
struct Greater
{
	bool operator() (const T &a, const T &b)
	{
		if(a > b)
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
struct Less
{
	bool operator() (const T &a, const T &b)
	{
		if(a < b)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

template<class T, class Cmp = Greater<T> >
class Heap
{
protected:
	vector<T> _v;
public:
	void AdjustDown(size_t parent)
	{
		Cmp cmp;
		size_t child = parent*2 + 1;
		while(child < _v.size())
		{
			if(child+1 < _v.size() && cmp(_v[child+1], _v[child]) )
			{
				++child;
			}
			if( cmp(_v[child], _v[parent]) )
			{
				std::swap(_v[child], _v[parent]);
				parent = child;
				child = parent*2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void AdjustUp(size_t child)
	{
		Cmp cmp;
		size_t parent = (child-1) / 2;
		while(child > 0)
		{
			if( cmp(_v[child], _v[parent]) )
			{
				std::swap(_v[child], _v[parent]);
				child = parent;
				parent = (child-1) / 2;
			}
			else
			{
				break;
			}
		}
	}
public:
	Heap()
	{}
	Heap(const T* arr, size_t size)
	{
		for(size_t i = 0; i < size; i++)
		{
			_v.push_back(arr[i]);
		}
		int parent = (_v.size()-2) / 2;
		for(; parent >= 0; --parent)
		{
			AdjustDown(parent);
		}
	}
	void Push(const T &x)
	{
		_v.push_back(x);
		size_t child = _v.size() - 1;
		AdjustUp(child);
	}
	void Pop()
	{
		assert(_v.size() > 0);
		std::swap(_v[0], _v[_v.size()-1]);
		_v.pop_back();
		AdjustDown(0);
	}
	const T& Top()
	{
		assert(_v.size());
		return _v[0];
	}
	size_t Size()
	{
		return _v.size();
	}
	bool Empty()
	{
		return _v.empty();
	}
	void Print()
	{
		for(size_t i = 0; i < _v.size(); i++)
		{
			cout<<_v[i]<<' ';
		}
		cout<<endl;
	}
};