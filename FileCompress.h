//FileCompress.h

#pragma once

#include "Heap.h"
#include "HuffmanTree.h"
#include <fstream>
#include <cstring>
#include <algorithm>

typedef long long LongType;

struct Chinfo
{
	unsigned char _ch;
	size_t _count;
	string _code;

	Chinfo(): _ch(0), _count(0)
	{}
	Chinfo(const size_t& count): _ch(0), _count(count)
	{}
	Chinfo(const Chinfo& x)
	{
		_ch = x._ch;
		_count = x._count;
		_code = x._code;
	}
	//赋值运算符重载？？？？

	bool operator< (const Chinfo& x)
	{
		if(_count < x._count)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator!= (const Chinfo& x)
	{
		return _count != x._count;
	}
	Chinfo operator+ (const Chinfo& x)
	{
		Chinfo tmp;
		tmp._count = _count + x._count;
		return tmp;
	}
};
//template<class T>
class FileCompress
{
	protected:
		Chinfo _infos[256];
	protected:
		void _HufCode(HuffmanTreeNode<Chinfo>* root)
		{
			if(root)
			{
				if(root->_left == NULL && root->_right == NULL)
				{
					HuffmanTreeNode<Chinfo>* child = root;
					HuffmanTreeNode<Chinfo>* parent = root->_parent;
					string s;
					while(parent)
					{
						if(child == parent->_left)
							s.push_back('0');
						else
							s.push_back('1');
						child = parent;
						parent = parent->_parent;
					}
					reverse(s.begin(), s.end());
					_infos[root->_weight._ch]._code = s;
					return ;
				}
				_HufCode(root->_left);
				_HufCode(root->_right);
			}
		}
		string _Compress(const char* filename, const char* desfile)
		{
			ifstream fin(filename, ios::binary);
			assert(fin);
			ofstream fout;
			string newfilename = string(filename);
			size_t pos = newfilename.rfind('.');
			string suffix = newfilename.substr(pos+1);
			//newfilename = newfilename.substr(0, pos);
			//newfilename += ".hufcompress";
			fout.open(desfile, ios::binary);
			//向压缩文件中填入配置信息
			size_t sum = 0;
			char buf[256];
			for(size_t i = 0; i < 256; i++)
			{
				if(_infos[i]._count != 0)
					sum++;
			}
			string line = suffix;
			line += '\n';
			fout<<line.c_str();
			line = "";
			string tmp(_itoa(sum, buf, 10));
			line += tmp;
			line += '\n';
			fout<<line.c_str();
			line = "";
			//fout<<'w';
			for(size_t i = 0; i < 256; i++)
			{
				if(_infos[i]._count != 0)
				{
					line += _infos[i]._ch;
					tmp = _itoa(_infos[i]._count, buf, 10);
					line += tmp;
					line += '\n';
					//fout<<line.c_str();
					for(size_t k = 0; k < line.size(); k++)
					{
						fout<<line[k];
					}
					line = "";
				}
			}
			///////////////////////////
			char value = 0;
			size_t index = 0;
			char ch;
			while( !fin.eof() )
			{
				fin.get(ch);
				string code = _infos[unsigned char(ch)]._code;
				for(size_t i = 0; i < code.size(); i++)
				{
					value <<= 1;
					if(code[i] == '1')
					{
						value |= 1;
					}
					++index;
					if(index >= 8)
					{
						fout<<value;
						value = 0;
						index = 0;
					}
				}
			}
			if(index)
			{
				value <<= (8-index);
				fout<<value;
			}
			fin.close();
			fout.close();
			return newfilename;
		}
	public:
		FileCompress()
		{
			for(size_t i = 0; i < 256; i++)
			{
				_infos[i]._ch = i;
			}
		}
		string Compress(const char* filename, const char* desfile)
		{
			//统计字符个数
			ifstream fin(filename, ios::binary);
			char ch;
			while( fin.get(ch) )
			{
				_infos[unsigned char(ch)]._count++;
			}
			fin.close();
			//构建哈弗曼树
			Chinfo invalid;
			HuffmanTree<Chinfo> huf(_infos, 256, invalid);
			//哈弗曼编码
			_HufCode(huf.GetRoot());
			//文件压缩
			string newfilename = _Compress(filename, desfile);
			return newfilename;
		}
		string Uncompress(string filename, string desFileName = "")
		{
			ifstream fin(filename, ios::binary);
			assert(fin);
			ofstream fout;
			
			if( desFileName.compare("") == 0 )
			{
				desFileName = filename;
			}
			else
			{
				size_t pos = filename.rfind('\\');
				string str = filename.substr(pos-1);
				desFileName += str;
			}
			size_t pos = desFileName.rfind('.');
			desFileName = desFileName.substr(0, pos);
			desFileName += "-2.";
			string line;
			char ch;
			char c;
			while(fin.get(ch))
			{
				if(ch == '\n')
				{
					break;
				}
				line += ch;
			}
			desFileName += line;
			line = "";
			fout.open(desFileName.c_str(), ios::binary);

			while(fin.get(ch))
			{
				if(ch == '\n')
				{
					break;
				}
				line += ch;
			}
			size_t lines = atoi(line.c_str());
			line = "";
			for(size_t i = 0; i < lines; i++)
			{
				fin.get(ch);
				while(fin.get(c))
				{
					if(c == '\n')
					{
						break;
					}
					line += c;
				}
				_infos[unsigned char(ch)]._count = atoi(line.c_str());
				line = "";
			}
			int shift = 7;
			Chinfo invalid;
			HuffmanTree<Chinfo> huf(_infos, 256, invalid);
			HuffmanTreeNode<Chinfo>* root = huf.GetRoot();
			HuffmanTreeNode<Chinfo>* cur = root;
		
			LongType count;
			if(root == NULL)
				count = 0;
			else
				count = root->_weight._count;
			while(fin.get(ch))
			{
				for(shift = 7; shift >= 0; --shift)
				{
					c = (ch >> shift) & 1;
					if(c)
					{
						cur = cur->_right;
					}
					else
					{
						cur = cur->_left;
					}
					if(cur->_left == NULL && cur->_right == NULL)
					{
						fout<<cur->_weight._ch;
						cur = root;
						--count;
						if(count <= 0)
							break;
					}
				}
				if(count <= 0)
					break;
			}
			fin.close();
			fout.close();
			return desFileName;
		}
};
