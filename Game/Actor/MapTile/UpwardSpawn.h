#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 상위 스테이지 이동시 플레이어가 생성될 통로의 타일 액터 클래스
class UpwardSpawn : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(UpwardSpawn, Actor)

public:
	UpwardSpawn(const Vector2& position);

private:

};

