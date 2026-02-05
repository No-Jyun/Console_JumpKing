#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Ground : public Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(Ground, Actor)

public:
	Ground(const Vector2& position);

private:


};

