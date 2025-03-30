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


	//virtual bool FindFile(fs::path _path);

	bool MovetoFile(fs::path _path);

	//virtual fs::path FindFileRecursive(fs::path _path, fs::path _my);

	//virtual fs::path AllFileRecursive(fs::path _my);

	//void AllExtendFileRecursive(const fs::path& _extend, const fs::path& _my = "");
	void AllExtendFileRecursive();
	void FindAllFile(std::string _str, fs::path _pa);
	std::string GetFile(std::string _str, std::string _FileName);
	const std::map<std::string, std::string>& GetAllFile(std::string _str);


	//std::string GetFile(std::string _str) {
	//	std::string str = HString::Upper(_str);
	//	if (Files.find(str) == Files.end()) {
	//		return "";
	//	}
	//	return Files[str];
	//}


private:
	std::map<std::string, std::map<std::string, std::string>> AllFiles;

};