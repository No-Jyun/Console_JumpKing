#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 얼음(미끄러짐) 타일 액터 클래스
class Ice : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Ice, Actor)

public:
	Ice(const Vector2& position);

private:

};

