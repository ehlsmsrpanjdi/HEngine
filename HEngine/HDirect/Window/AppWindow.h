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

	void updateQuadPosition();

protected:
	std::shared_ptr<class EngineKey> KeyManager = nullptr;
	std::shared_ptr<class EngineTime> TimeManager = nullptr;

private:
	class SwapChain* m_swap_chain;
	class VertexBuffer* m_vb;
	class VertexShader* m_vs;
	class PixelShader* m_ps;
	class ConstantBuffer* m_cb;
	class IndexBuffer* m_ib;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;
};