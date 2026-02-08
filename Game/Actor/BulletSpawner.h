#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class JumpLevel;
class BulletSpawner : public Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(BulletSpawner, Actor)

public:
	BulletSpawner(const Vector2& position, JumpLevel* level);
	
	virtual void Tick(float deltaTime) override;

private:
	void SetRandomSetting();

private:
	JumpLevel* jumpLevel;

	Vector2 bulletSpawnPosition;

	float bulletSpeed = 0.0f;

	Timer spawnTimer;
};

