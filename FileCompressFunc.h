#pragma once

#include "FileCompress.h"
#include <Windows.h>
#include <io.h>
#include <direct.h>


void FileCompressFunc(string srcFileName, string desFileName)
{
	long handle;
	struct _finddata32_t fileInfo;
	string srcFileName2 = srcFileName;
	string desFileName2 = desFileName;
	if( (handle = _findfirst32(srcFileName2.append("\\\\*").c_str(), &fileInfo)) != -1 )
	{
		do 
		{
			srcFileName2 = srcFileName;
			desFileName2 = desFileName;
			srcFileName2.append("\\\\").append(fileInfo.name);
			if(fileInfo.attrib & _A_SUBDIR)
			{
				if( strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0 )
				{
					desFileName2.append("\\\\").append(fileInfo.name);
					desFileName2 += ".hufcompress";
					mkdir(desFileName2.c_str());
					FileCompressFunc(srcFileName2, desFileName2);
				}
			}
			else
			{
				desFileName2.append("\\\\").append(fileInfo.name);
				size_t pos = desFileName2.rfind('.');
				desFileName2 = desFileName2.substr(0, pos);
				desFileName2 += ".hufcompress";
				FileCompress fc;
				fc.Compress(srcFileName2.c_str(), desFileName2.c_str());
			}
		} while ( _findnext32(handle, &fileInfo) == 0 );
		_findclose(handle);
	}
}
void JudgeCompress(string srcFileName)
{
	long handle;
	struct _finddata32_t fileInfo;
	string desFileName = "";
	if( (handle = _findfirst32(srcFileName.c_str(), &fileInfo)) != -1 )
	{
		if(fileInfo.attrib & _A_SUBDIR)
		{
			desFileName = srcFileName;
			desFileName += ".hufcompress";
			mkdir(desFileName.c_str());
			FileCompressFunc(srcFileName, desFileName);
		}
		else
		{
			desFileName = srcFileName;
			size_t pos = desFileName.rfind('.');
			desFileName = desFileName.substr(0, pos);
			desFileName += ".hufcompress";
			FileCompress fc;
			fc.Compress(srcFileName.c_str(), desFileName.c_str());
		}
		_findclose(handle);
	}
}


void FileUncompressFunc(string srcFileName, string desFileName)
{
	long handle;
	struct _finddata32_t fileInfo;
	string srcFileName2 = srcFileName;
	string desFileName2 = desFileName;
	if( (handle = _findfirst32(srcFileName2.append("\\\\*").c_str(), &fileInfo)) != -1 )
	{
		do 
		{
			srcFileName2 = srcFileName;
			desFileName2 = desFileName;
			srcFileName2.append("\\\\").append(fileInfo.name);
			if(fileInfo.attrib & _A_SUBDIR)
			{
				if(strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
				{
					desFileName2.append("\\\\").append(fileInfo.name);
					size_t pos = desFileName2.rfind('.');
					desFileName2 = desFileName2.substr(0, pos);
					desFileName2 += "-2";
					mkdir(desFileName2.c_str());
					FileUncompressFunc(srcFileName2, desFileName2);
				}
			}
			else
			{
				FileCompress fc;
				fc.Uncompress(srcFileName2, desFileName2);
			}

		} while (_findnext32(handle, &fileInfo) == 0);
		_findclose(handle);
	}
}
void JudgeUncompress(string srcFileName)
{
	long handle;
	struct _finddata32_t fileInfo;
	string desFileName = "";
	if( (handle = _findfirst32(srcFileName.c_str(), &fileInfo)) != -1 )
	{
		if(fileInfo.attrib & _A_SUBDIR)
		{
			size_t pos = srcFileName.rfind('.');
			desFileName = srcFileName.substr(0, pos);
			desFileName += "-2";
			mkdir(desFileName.c_str());
			FileUncompressFunc(srcFileName, desFileName);
		}
		else
		{
			FileCompress fc;
			fc.Uncompress(srcFileName, desFileName);
		}
		_findclose(handle);
	}

}
