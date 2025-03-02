#pragma once
#include "EnginePath.h"
#include "filesystem"
#include <unordered_map>
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


	virtual bool FindFile(fs::path _path);

	bool MovetoFile(fs::path _path);

	virtual fs::path FindFileRecursive(fs::path _path, fs::path _my);

	//virtual fs::path AllFileRecursive(fs::path _my);

	void AllExtendFileRecursive(const fs::path& _extend, const fs::path& _my = "");

	std::unordered_map<std::string, std::string> Files;


};