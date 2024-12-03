#pragma once
#include "EnginePath.h"
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

	virtual fs::path FindFileRecursive(fs::path _path, fs::path _my);


};