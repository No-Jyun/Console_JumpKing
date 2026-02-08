#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 벽 타일 액터 클래스
class Wall : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Wall, Actor)

public:
	Wall(const Vector2& position);

private:

};

