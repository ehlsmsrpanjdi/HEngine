#include <crtdbg.h>
#include "Window/AppWindow.h"
#include "Window/Window.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AppWindow app;
	app.SetSize(1024,768);
	if (app.init()) {
		while (app.isRun()) {
			app.broadcase();
		}
	}
	return 0;
}

