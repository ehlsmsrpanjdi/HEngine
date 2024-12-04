#pragma once
class EngineImage;
class Actor;

class Renderer {
	friend Actor;
	class Bitmap;
public:
	Renderer() {

	}

	void SetImage(Bitmap* _bitmap);
	void SetOwner(Actor* _Actor);
	void Rendering();
private:
	/*EngineImage* Image = nullptr;*/
	Bitmap* RBitmap = nullptr;
	Actor* Owner = nullptr;
};