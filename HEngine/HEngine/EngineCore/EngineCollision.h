#pragma once
#include "EngineHelper/EngineTransform.h"
#include "BaseObject.h"
#include <unordered_map>
#include "functional"
class Actor;

class EngineCollision : public BaseObject{
public:
	EngineCollision() {

	}

	~EngineCollision() {

	}


	static std::unordered_map<int, std::function<bool(float4D, float4D)>> Collisions;

	void SetOwner(Actor* _Actor);
private:
	Actor* Owner = nullptr;
};
