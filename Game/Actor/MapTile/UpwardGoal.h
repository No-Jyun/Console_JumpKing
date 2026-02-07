#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 상위 스테이지 이동의 통로 타일 액터 클래스
class UpwardGoal : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(UpwardGoal, Actor)

public:
	UpwardGoal(const Vector2& position);

private:

};

