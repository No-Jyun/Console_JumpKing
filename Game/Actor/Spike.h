#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Spike : public Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(Spike, Actor)

public:
	Spike(const Vector2& position, const int direction);

private:

};

