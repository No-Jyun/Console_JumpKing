#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 스테이지의 목표지점 타일 액터 클래스
class Goal : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Goal, Actor)

public:
	Goal(const Vector2& position);

private:

};

