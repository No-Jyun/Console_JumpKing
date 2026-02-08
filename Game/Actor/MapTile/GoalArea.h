#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// Goal 액터가 그리는 영역 (목적지 트리거) 타일 액터 클래스
class GoalArea : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(GoalArea, Actor)

public:
	GoalArea(const Vector2& position);

private:

};

