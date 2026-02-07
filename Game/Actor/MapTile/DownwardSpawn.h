#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 하위 스테이지 이동시 플레이어가 생성될 통로의 타일 액터 클래스
class DownwardSpawn : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(DownwardSpawn, Actor)

public:
	DownwardSpawn(const Vector2& position);

private:


};

