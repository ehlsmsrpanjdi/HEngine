#pragma once
#include "EnginePath.h"
#include "filesystem"
#include <map>
#include "HString.h"


class EngineFile : public EnginePath {
public:
	EngineFile()
		:EnginePath()
	{
	}

	EngineFile(fs::path _path)
		:EnginePath(_path)
	{

	}

	EngineFile(std::string _str)
		:EnginePath(_str)
	{

	}

	bool MovetoFile(fs::path _path);


	void AllExtendFileRecursive();
	void FindAllFile(std::string _str, fs::path _pa);
	std::string GetFile(std::string _str, std::string _FileName);
	const std::map<std::string, std::string>& GetAllFile(std::string _str);


private:
	std::map<std::string, std::map<std::string, std::string>> AllFiles;

};