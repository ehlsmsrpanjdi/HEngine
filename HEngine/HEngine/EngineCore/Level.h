#pragma once
#include "list"
#include "BaseObject.h"
class Actor;
class Renderer;
class EngineCollision;
class Level : public BaseObject{
public:
	friend Actor;

	Level();
	~Level();

	virtual void BeginPlay() override;

	virtual void Tick(float _deltatime) override;

	void RenderTick(float _deltatime);

	template <typename Act>
	Act* SpawnActor(std::string_view _str) {
		Act* Actor = new Act();
		Actor->SetName(_str);
		Actor->Current_Level = this;
		Actor->BeginPlay();
		Actors.push_back(Actor);
		return Actor;
	}

	std::list<Actor*> Actors;

private:
	std::map<int, Renderer*> Renderers;
	std::map<int, EngineCollision*> Collisions;
};
