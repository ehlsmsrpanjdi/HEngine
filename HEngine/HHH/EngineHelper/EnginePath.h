#pragma once
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;



class EnginePath {
public:
	EnginePath() {
		pa = fs::current_path();
	}

	EnginePath(fs::path _path)
		:pa(_path)
	{

	}

	EnginePath(std::string _str)
		:pa(_str)
	{

	}

	void Move_parent() {
		pa = pa.parent_path();
	}

	fs::path GetPath() {
		return pa;
	}
protected:
	fs::path pa;
};