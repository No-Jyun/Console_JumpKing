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
	
	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;

private:
	// 탄환을 발사하기 위한 설정값을 랜덤으로 설정해주는 함수
	void SetRandomSetting();

private:
	// 탄환 생성기를 소환한 레벨을 가리키는 포인터
	JumpLevel* jumpLevel;

	// 탄환이 생성될 위치
	Vector2 bulletSpawnPosition;

	// 탄환 속도
	float bulletSpeed = 0.0f;

	// 탄환 생성 속도
	Timer spawnTimer;
};

