#pragma once
class EngineImage;
class Actor;

class Renderer {
	friend Actor;
public:
	Renderer() {

	}

	void SetImage(EngineImage* _Image);
	void SetOwner(Actor* _Actor);
	void Rendering();
private:
	EngineImage* RImage = nullptr;
	Actor* Owner = nullptr;
};