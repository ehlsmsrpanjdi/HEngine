#include "AppWindow.h"
#include <crtdbg.h>

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AppWindow app;
	if (app.init()) {
		while (app.isRun()) {
			app.broadcase();
		}
	}
	return 0;
}