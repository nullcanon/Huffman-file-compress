#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include"Huffman.h"
#include<string>
#include<assert.h>
#include<cstdlib>
#include<cstdio>
#include<iostream>

using namespace std;

struct FileInfo
{
	unsigned char _ch;
	long long _chCount;
	string _code;

	FileInfo(const long long& count = 0)
		:_chCount(count)
	{}

	FileInfo(const char ch)
		:_ch(ch)
	{}

	bool operator<(const FileInfo& f)
	{
		return _chCount < f._chCount;
	}

	FileInfo operator+(const FileInfo& f)
	{
		return _chCount + f._chCount;
	}

	bool operator!=(const FileInfo& f)
	{
		return _chCount != f._chCount;
	}
};

class FileCompress
{
	typedef HuffmanNode<FileInfo> Node;
public:
	void Compress(const char* FileName)
	{
		assert(FileName != NULL);
		FILE* pFileRead = fopen(FileName, "r");
		assert(NULL != pFileRead);

		for (int i = 0; i < 256; ++i)
		{
			_Info[i]._ch = i;
		}

		//统计字符的个数
		int ch = fgetc(pFileRead);
		while (EOF != ch)
		{
			_Info[ch]._chCount++;
			ch = fgetc(pFileRead);
		}
		fseek(pFileRead, 0, SEEK_SET);

		//建Huffman树
		HuffmanTree<FileInfo> FileInfoTree(_Info, 256, FileInfo());
		Node* root = FileInfoTree.Root();

		string code;
		int line = 0;
		_GetTreeNode(root, code, line);//生成Huffman编码

		//生成配置信息
		string OldFileNamePostfix = _GetFileNamePostfix(FileName);
		string NewFileName = _ChangeFileNamePostfix(FileName,".ZPF");

		FILE* pFileWrite = fopen(NewFileName.c_str(),"w");
		assert(pFileWrite != NULL);
		fputs(OldFileNamePostfix.c_str(), pFileWrite);
		fputc('\n',pFileWrite);
		char Line[4];
		_itoa(line,Line,10);
		fputs(Line,pFileWrite);
		fputc('\n',pFileWrite);

		for (size_t i = 0; i < 256; ++i)
		{
			if (_Info[i]._chCount)
			{
				fputc(_Info[i]._ch,pFileWrite);
				char a[126];
				_itoa(_Info[i]._chCount,a,10);
				fputc(' ',pFileWrite);
				fputs(a,pFileWrite);
				fputc('\n',pFileWrite);
			}
		}

		//将Huffman编码写入文件
		unsigned char data = 0;
		int pos = 7;
		ch = fgetc(pFileRead);
		while (EOF != ch)
		{
			const char* pStr = _Info[ch]._code.c_str();
			while (*pStr)
			{
				if (pos >= 0)
				{
					data = data | ((*pStr-'0')<<pos);
					pos--;
					pStr++;
				}
				else
				{
					fputc(data,pFileWrite);
					data = 0;
					pos = 7;
				}
			}
			ch = fgetc(pFileRead);
		}
		fputc(data,pFileWrite);
		fputc('\n',pFileWrite);

		fclose(pFileRead);
		fclose(pFileWrite);
	}

	void UnCompress(const char* FileName)
	{
		assert(FileName != NULL);
		string FileNamePostfix = _GetFileNamePostfix(FileName);
		if (FileNamePostfix != ".ZPF")
			return;

		FILE* pFileRead = fopen(FileName,"rb");
		assert(pFileRead != NULL);
		string OldFileNamePostfix;
		string OldFileName;
		if(_ReadLine(pFileRead,OldFileNamePostfix))
			OldFileName = _ChangeFileNamePostfix(FileName,OldFileNamePostfix);

		long long chKindCount = 0;//获取文件中字符种类的个数,即行数
		string Line;
		_ReadLine(pFileRead,Line);
		chKindCount = atoi(Line.c_str());

		//根据配置文件重建哈夫曼树
		for (size_t i = 0; i < 256; ++i)
		{
			_Info[i]._ch = i;
		}
		while (chKindCount--)
		{
			Line.clear();
			_ReadLine(pFileRead,Line);
			unsigned char ch = Line[0];
			_Info[ch]._chCount = atoi(Line.substr(2).c_str());
		}

		HuffmanTree<FileInfo> NewTree(_Info,256,FileInfo());

		//解压
		FILE* pFileWrite = fopen(OldFileName.c_str(),"w");
		assert(pFileWrite != NULL);
		HuffmanNode<FileInfo>*  pCur = NewTree.Root();
		char ch = fgetc(pFileRead);
		char pos = 7;
		int chCount = pCur->_weight._chCount;
		while (chCount)
		{
			int tmp = (unsigned char)ch&(1 << pos);
			if (0 == tmp)
				pCur = pCur->_pLeft;
			else
				pCur = pCur->_pRight;

			--pos;

			if (NULL == pCur->_pLeft&&NULL == pCur->_pRight)
			{
				fputc(pCur->_weight._ch, pFileWrite);
				pCur = NewTree.Root();
				if (--chCount == 0)
					break;
			}
			if (-1 == pos)
			{
				pos = 7;
				ch = fgetc(pFileRead);
			}
		}

		fclose(pFileRead);
		fclose(pFileWrite);
	}

private:
	void _GetTreeNode(Node* root, string& code, int& line)
	{
		if (NULL == root)
			return;

		_GetTreeNode(root->_pLeft, code + '0', line);
		_GetTreeNode(root->_pRight, code + '1', line);

		if (NULL == root->_pLeft && NULL == root->_pRight)
		{
			_Info[root->_weight._ch]._code = code.c_str();

			if (root->_weight._chCount != 0)
				line++;
		}
	}

	string _GetFileNamePostfix(const char* FileName)
	{
		string name(FileName);
		size_t find = name.rfind('.');
		name = name.substr(find,name.size());
		return name;
	}

	string _ChangeFileNamePostfix(const char* FileName,const string Postfix)
	{
		string NewFileName(FileName);
		size_t find = NewFileName.rfind('.');
		NewFileName = NewFileName.substr(0,find)+Postfix;
		return NewFileName;
	}
	
	bool _ReadLine(FILE* pFileRead, string& line)
	{
		char ch = fgetc(pFileRead);
		if (EOF == ch)
		{
			return false;
		}
		while (ch != EOF && ch != '\n')
		{
			if (ch == '\r')
			{
				ch = fgetc(pFileRead);
				continue;
			}
			line += ch;
			ch = fgetc(pFileRead);
		}
		return true;
	}

private:
	FileInfo _Info[256];
};