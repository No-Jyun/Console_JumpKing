#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

// 전방 선언
class JumpLevel;

// 플레이어를 향해 날아갈 탄환을 생성하는 클래스
class BulletSpawner : public Actor
{
	// RTTI 선언
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

