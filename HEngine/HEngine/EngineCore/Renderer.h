#pragma once

class EngineImage;
class Actor;

class Renderer {
	friend Actor;
public:
	Renderer() {

	}
private:
	EngineImage* Image;
	Actor* Owner = nullptr;
};