//test.cpp

#include "FileCompress.h"
#include "FileCompressFunc.h"
#include <Windows.h>

int main()
{
	long long start;
	long long finish;
	start = GetTickCount();

	JudgeCompress("1");
	//JudgeUncompress("1.hufcompress");
	
	//JudgeCompress("Input.BIG");
	//JudgeUncompress("Input.hufcompress");

	finish = GetTickCount();
	cout<<(finish - start)<<endl;
	return 0;
}
