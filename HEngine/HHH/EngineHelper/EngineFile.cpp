#include "EngineFile.h"
#include "filesystem"
#include "HString.h"

bool EngineFile::FindFile(fs::path _path) {
	for (const fs::directory_entry& entry : fs::directory_iterator(pa)) {
		if (entry.path().filename().stem() == _path) {
			pa = entry.path();
			return true;
		}
	}
	return false;
}

fs::path EngineFile::FindFileRecursive(fs::path _path, fs::path _my) {
	fs::path pp("");
	for (const fs::directory_entry& entry : fs::directory_iterator(_my)) {
		if (pp != "") {
			return pp;
		}
		if (entry.is_directory()) {
			pp = FindFileRecursive(_path, entry.path());
		}
		else if (entry.is_regular_file()) {
			if (entry.path().filename().stem() == _path) {
				pp = entry.path();
			}
		}
	}
	return pp;
}

void EngineFile::AllExtendFileRecursive(const fs::path& _extend, const fs::path& _my)
{
	fs::path dirPath;
	if (_my == "") {
		dirPath = pa;  // 현재 디렉토리
	}
	else {
		dirPath = _my;  // 현재 디렉토리
	}
	for (const fs::directory_entry& entry : fs::directory_iterator(dirPath)) {
		if (entry.is_directory() == true) {
			AllExtendFileRecursive(_extend, entry.path());
		}
		else {
			fs::path ppath = entry.path().extension();

			if (entry.path().extension() == _extend) {
				Files[HString::Upper(entry.path().filename().string())] = entry.path().string();
			}
		}
	}
	return;
}


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

const std::map<std::string, std::string>& EngineFile::GetAllFile(std::string _str)
{
	return AllFiles[HString::Upper(_str)];
}









