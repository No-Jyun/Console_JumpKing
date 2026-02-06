#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 맵 경계 타일 액터 클래스
class Block : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Block, Actor)

public:
	Block(const Vector2& position);

private:

};

