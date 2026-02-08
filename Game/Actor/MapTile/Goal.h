#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

// 게임 클리어를 판단할 목적지 클래스
class Goal : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Goal, Actor)

public:
	Goal(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	virtual void Draw() override;

private:
	// 색상 배열에서 현재 색상의 인덱스
	int currentColorIndex = 0;
	
	// 색상을 전환할 타이머
	Timer timer;
};

