#pragma once
#include<vector>
using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& a,const T& b)
	{
		return (a <= b);
	}
};

template<class T>
struct Great
{
	bool operator()(const T& a, const T& b)
	{
		return (a > b);
	}
};

template<class T,class Compare  = Less<T> >
class Heap
{
public:
	Heap()
	{}
	~Heap()
	{
		_hp.clear();
	}
	Heap(T* array, size_t size)
		:_hp(NULL)
		,_size(size)
	{
		_hp.resize(_size);
		for (size_t i = 0; i < _size; ++i)
		{
			_hp[i] = array[i];
		}
	
		_CreatHeap();
	}

	void Push(const T& data)
	{
		//将元素插入到最后一个位置,进行调整
		_hp.push_back(data);
		++_size;	
		_AdjustUp();
	}

	void Pop()
	{
		//交换堆顶元素和最后一个元素,然后调整
		swap(_hp[0],_hp[_size-1]);
		_hp.pop_back();
		--_size;
		_AdjustDown(0);
	}

	bool Empty()const
	{
		return _hp.empty();
	}

	size_t Size()const
	{
		return _hp.size();
	}

	T Top()
	{
		return _hp[0];
	}

private:
	void _CreatHeap()
	{
		size_t parent = (_size - 2) >> 1;
		for (size_t i = 0; i <= (_size - 2) >> 1; ++i)
		{
			//子堆父节点的位置向前移动,使当前子堆满足最小(大)堆,直到父节点走完根节点的位置
			size_t parent = ((_size - 2) >> 1) - i;
			_AdjustDown(parent);
		}
		
	}
	
	void _AdjustDown(size_t parent)
	{
		size_t child = parent * 2 + 1;
		while (child < _size)
		{
			//从最后一个子堆开始调整,即找到最后一个节点的父亲节点
			if ((child + 1 < _size) && Compare()(_hp[child + 1], _hp[child]))
				child += 1;

			if (Compare()(_hp[child], _hp[parent]))
			{
				swap(_hp[parent], _hp[child]);

				//继续向前一个子堆
				parent = child;
				child = parent * 2 + 1;
			}
			else//否则说明整个子堆满足最小(大)堆,跳出此次循环
				return;
		}
	}

	void _AdjustUp()
	{
		size_t parent = (_size - 2) >> 1;
		size_t child = _size - 1;

		while (child>0)
		{
			if (Compare()(_hp[child], _hp[parent]))
			{
				swap(_hp[parent], _hp[child]);
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
				return;
		}
	}
private:
	vector<T> _hp;
	size_t _size;
};

void TestHeap()
{
	int arr[] = {53,24,78,9,45,65,87,23};
	size_t sz = sizeof(arr) / sizeof(arr[0]);
	Heap<int> hp(arr,sz);
	hp.Push(8);
	hp.Pop();
}
