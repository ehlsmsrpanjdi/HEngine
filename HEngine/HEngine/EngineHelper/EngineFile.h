#pragma once
#include "EnginePath.h"
class EngineFile : public EnginePath {
	EngineFile() {
		pa = fs::current_path();
	}

	EngineFile(fs::path _path)
		:EnginePath(_path)
	{

	}

	EngineFile(std::string _str)
		:EnginePath(_str)
	{

	}


	bool FindFile(fs::path _path);

	fs::path FindFileRecursive(fs::path _path, fs::path _my);

};