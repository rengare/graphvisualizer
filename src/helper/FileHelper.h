#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
namespace FileHelper
{
static bool ReadShaderFromFile(string fileName, string *fileContent)
{
	stringstream stream;
	ifstream file;

	file.open(fileName, ios::in);
	if (!file)
	{
		return false;
	}
	stream << file.rdbuf();
	*fileContent = stream.str();

	file.close();
	return true;
}

static void ReadFileToBuffer()
{
}

static void ReadNodeData()
{

}
}

#endif