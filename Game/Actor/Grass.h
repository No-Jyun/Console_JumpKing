#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 풀(바닥) 타일 액터 클래스
class Grass : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Grass, Actor)

public:
	Grass(const Vector2& position);

private:


};

