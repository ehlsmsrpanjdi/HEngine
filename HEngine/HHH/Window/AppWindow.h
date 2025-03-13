#pragma once
#include "Window.h"
#include "memory"
class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	void OnSize() override;


protected:
	std::shared_ptr<class EngineKey> KeyManager = nullptr;
	std::shared_ptr<class EngineTime> TimeManager = nullptr;
	std::shared_ptr<class EngineFile> FileManager = nullptr;
	
private:
};