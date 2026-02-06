#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 땅(바닥) 타일 액터 클래스
class Ground : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Ground, Actor)

public:
	Ground(const Vector2& position);

private:


};

