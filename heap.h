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
		//��Ԫ�ز��뵽���һ��λ��,���е���
		_hp.push_back(data);
		++_size;	
		_AdjustUp();
	}

	void Pop()
	{
		//�����Ѷ�Ԫ�غ����һ��Ԫ��,Ȼ�����
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
			//�ӶѸ��ڵ��λ����ǰ�ƶ�,ʹ��ǰ�Ӷ�������С(��)��,ֱ�����ڵ�������ڵ��λ��
			size_t parent = ((_size - 2) >> 1) - i;
			_AdjustDown(parent);
		}
		
	}
	
	void _AdjustDown(size_t parent)
	{
		size_t child = parent * 2 + 1;
		while (child < _size)
		{
			//�����һ���Ӷѿ�ʼ����,���ҵ����һ���ڵ�ĸ��׽ڵ�
			if ((child + 1 < _size) && Compare()(_hp[child + 1], _hp[child]))
				child += 1;

			if (Compare()(_hp[child], _hp[parent]))
			{
				swap(_hp[parent], _hp[child]);

				//������ǰһ���Ӷ�
				parent = child;
				child = parent * 2 + 1;
			}
			else//����˵�������Ӷ�������С(��)��,�����˴�ѭ��
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
