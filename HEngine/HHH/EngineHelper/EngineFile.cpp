#include "EngineFile.h"
#include "filesystem"
#include "HString.h"

bool EngineFile::MovetoFile(fs::path _path)
{
	fs::path dirPath = pa;
	for (fs::directory_entry entry : fs::directory_iterator(dirPath)) {
		if (entry.path().filename() == _path) {
			pa = entry.path();
			return true;
		}
	}
	return false;
}


void EngineFile::AllExtendFileRecursive() {
	fs::path dirpath = pa;
	for (fs::directory_entry entry : fs::directory_iterator(dirpath)) {
		if (entry.is_directory() == true) {
			FindAllFile(entry.path().filename().string(), entry.path());
		}
	}
}


void EngineFile::FindAllFile(std::string _str, fs::path _pa)
{
	AllFiles[HString::Upper(_str)];
	for (fs::directory_entry entry : fs::directory_iterator(_pa)) {
		std::string str = HString::Upper(entry.path().filename().stem().string());
		AllFiles[_str].emplace(std::make_pair(str, entry.path().string()));
	}
}



std::string EngineFile::GetFile(std::string _str, std::string _FileName)
{
	return AllFiles[HString::Upper(_str)][HString::Upper(_FileName)];
}

const std::map<std::string, std::string>& EngineFile::GetAllFile(std::string _str)
{
	return AllFiles[HString::Upper(_str)];
}









