#pragma once
#include "Window.h"
// Ό³Έν :
class AppWindow : public Window
{
public:
	// constrcuter destructer
	AppWindow();
	~AppWindow();

	// delete Function
	AppWindow(const AppWindow& _Other) = delete;
	AppWindow(AppWindow&& _Other) noexcept = delete;
	AppWindow& operator=(const AppWindow& _Other) = delete;
	AppWindow& operator=(AppWindow&& _Other) noexcept = delete;

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

protected:

private:
	class SwapChain* m_SwapChain = nullptr;
};

