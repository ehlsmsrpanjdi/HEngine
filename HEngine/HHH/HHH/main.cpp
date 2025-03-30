#include <crtdbg.h>
#include "Window/AppWindow.h"
#include "Window/Window.h"
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