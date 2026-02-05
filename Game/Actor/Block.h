#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Block : public Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(Block, Actor)

public:
	Block(const Vector2& position);

private:

};

