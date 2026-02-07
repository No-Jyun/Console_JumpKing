#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 하위 스테이지 이동의 통로 타일 액터 클래스
class DownwardGoal : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(DownwardGoal, Actor)

public:
	DownwardGoal(const Vector2& position);

private:


};

