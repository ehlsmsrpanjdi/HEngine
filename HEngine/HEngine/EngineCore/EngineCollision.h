#pragma once
#include "EngineHelper/EngineTransform.h"
#include "BaseObject.h"
#include <unordered_map>
#include "functional"
#include "list"
#include "unordered_set"
class Actor;

class EngineCollision : public BaseObject {
public:
	EngineCollision() {

	}

	EngineCollision(int _type)
		:Type(_type)
	{

	}

	~EngineCollision() {

	}

	static std::unordered_map<int, std::unordered_set<EngineCollision*>> Collisions;

	EngineCollision* CollisionCheck(int _Select);

	static EngineCollision* CreateCollision(int _Type);

	void SetOwner(Actor* _Actor);

	int GetType() {
		return Type;
	}

	void ChangeType(int _type);

	bool isColliding(EngineCollision* _Collision);

	void Collisioning();

	void CollisionDraw(HDC _hdc);

	void PushFunction(std::function<void(void)> _Fun) {
		Funs.push_back(_Fun);
	}

	Actor* GetOwner() {
		if (Owner != nullptr) {
			return Owner;
		}
		CodeError;
		return nullptr;
	}

private:
	std::list<std::function<void(void)>> Funs;

	Actor* Owner = nullptr;
	void SetType(int _type) {
		Type = _type;
	}
	int Type = -1;
};
