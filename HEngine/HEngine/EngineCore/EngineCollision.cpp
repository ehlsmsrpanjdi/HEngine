#include "pch.h"
#include "EngineCollision.h"
#include "Actor.h"

std::unordered_map<int, std::unordered_set<EngineCollision*>> EngineCollision::Collisions = {};

void EngineCollision::SetOwner(Actor* _Actor) {
	Owner = _Actor;
}

void EngineCollision::CollisionDraw(HDC _hdc) {
	float2D Location = Owner->GetLocation() + GetLocation();
	float2D Scale = Owner->GetScale() * GetScale();

	Int2D _Location = Location.FloatToInt();
	Int2D _Scale = Scale.FloatToInt();

	Rectangle(_hdc, _Location.X, _Location.Y, _Scale.X + _Location.X, _Scale.Y + _Location.Y);
}

void EngineCollision::ChangeType(int _type) {
	std::unordered_set<EngineCollision*>& Set = Collisions[this->GetType()];
	std::unordered_set<EngineCollision*>::iterator Iter = Set.find(this);
	if (Set.end() != Iter) {
		Set.erase(Iter);
	}
	this->SetType(_type);
	Collisions[this->GetType()].insert(this);
}

EngineCollision* EngineCollision::CreateCollision(int _Type) {
	EngineCollision* Collision = new EngineCollision(_Type);
	Collisions[_Type].insert(Collision);
	return Collision;
}

EngineCollision* EngineCollision::CollisionCheck(int _Select) {
	for (EngineCollision* _Collision : Collisions[_Select]) {
		if (_Collision != nullptr) {
			if (isColliding(_Collision)) {
				return _Collision;
			}
		}
	}
	return nullptr;
}

void EngineCollision::Collisioning() {
	for (std::function<void(void)> Fun : Funs) {
		if (Fun != nullptr) {
			Fun();
		}
	}
}

bool EngineCollision::isColliding(EngineCollision* _Collision) {

	float2D LocationA = _Collision->GetOwner()->GetLocation() + _Collision->GetLocation();
	float2D ScaleA = _Collision->GetOwner()->GetScale() + _Collision->GetScale();

	float2D LocationB = GetOwner()->GetLocation() + GetLocation();
	float2D ScaleB = GetOwner()->GetScale() * GetScale();

	float ARight = LocationA.X + ScaleA.X;
	float ABottom = LocationA.Y + ScaleA.Y;
	float BRight = LocationB.X + ScaleB.X;
	float BBottom = LocationB.Y + ScaleB.Y;

	return !(LocationA.X > BRight || // A�� �������� B�� ���ʺ��� ���ʿ� ����
		ARight < LocationB.X || // A�� ������ B�� �����ʺ��� �����ʿ� ����
		LocationA.Y > BBottom || // A�� ������ B�� �Ʒ��ʺ��� �Ʒ��� ����
		ABottom < LocationB.Y); // A�� �Ʒ����� B�� ���ʺ��� ���� ����

}