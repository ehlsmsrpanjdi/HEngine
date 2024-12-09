#include "pch.h"
#include "EngineCollision.h"
#include "Actor.h"

std::unordered_map<int, std::function<bool(float4D, float4D)>> EngineCollision::Collisions = {};

void EngineCollision::SetOwner(Actor* _Actor) {
	Owner = _Actor;
}