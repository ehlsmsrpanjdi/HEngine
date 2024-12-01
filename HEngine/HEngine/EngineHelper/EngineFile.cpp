#include "EngineFile.h"

bool EngineFile::FindFile(fs::path _path) {
	for (const fs::directory_entry& entry : fs::directory_iterator(pa)) {
		if (entry.path().filename() == _path) {
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