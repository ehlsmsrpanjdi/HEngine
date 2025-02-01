#pragma once
#include "Window.h"
class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

protected:
	class EngineKey* KeyManager = nullptr;
	class EngineTime* TimeManager = nullptr;

private:
};