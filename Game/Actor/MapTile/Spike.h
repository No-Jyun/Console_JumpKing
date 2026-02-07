#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 가시(닿으면 사망) 타일 액터 클래스
class Spike : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Spike, Actor)

public:
	Spike(const Vector2& position, const int direction);

private:

};

