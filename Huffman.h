#pragma once

#include<assert.h>
#include"heap.h"

using namespace std;

template<class T>
struct HuffmanNode
{
	T _weight;
	HuffmanNode<T>* _pLeft;
	HuffmanNode<T>* _pRight;
	HuffmanNode<T>* _pParent;

	HuffmanNode(const T& weight = T())
		:_weight(weight)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
	typedef Node* pNode;
public:
	HuffmanTree()
		:_pRoot(NULL)
	{}

	~HuffmanTree()
	{
		Destroy(_pRoot);
	}

	HuffmanTree(T* array, size_t size, const T& invalid = T())
	{
		struct HuffmanNodeLess
		{
			bool operator()(pNode a, pNode b)
			{
				assert(a!=NULL && b!=NULL);
				return (a->_weight < b->_weight);
			}
		};

		Heap<pNode, HuffmanNodeLess> MinHeap;

		for (size_t i = 0; i < size; ++i)
		{
			if (array[i] != invalid)//�Ѳ�������Чֵ�����������
			{
				pNode NewNode = new Node(array[i]);
				MinHeap.Push(NewNode);
			}
		}

		if (!MinHeap.Empty())
		{
			while (MinHeap.Size() > 1)
			{
				//ȡ��Ȩֵ��С�������ڵ�ֱ���Ϊ��������,�丸�ڵ��ȨֵΪ����������Ȩֵ֮��
				pNode pLeft = MinHeap.Top();
				MinHeap.Pop();
				pNode pRight = MinHeap.Top();
				MinHeap.Pop();

				pNode pParent = new Node(pLeft->_weight + pRight->_weight);
				pParent->_pLeft = pLeft;
				pParent->_pRight = pRight;

				MinHeap.Push(pParent);//������Ͷ����С����
			}

			_pRoot = MinHeap.Top();
		}
	}

	pNode Root()
	{
		return _pRoot;
	}

private:
	void Destroy(pNode pd)
	{
		if (pd)
		{
			Destroy(pd->_pLeft);
			Destroy(pd->_pRight);
			delete pd;
			pd = NULL;
		}
	}

private:
	pNode _pRoot;
};

void TestHuffmanTree()
{
	int array[] = {7,5,3,1};
	size_t sz = sizeof(array) / sizeof(array[0]);

	HuffmanTree<int> HuffTree(array,sz);
}