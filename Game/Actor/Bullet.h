#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

// 플레이어를 향해 날아가는 탄환 클래스
class Bullet : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Bullet, Actor)

public:
	Bullet(const Vector2& position, const Vector2& playerPosition, float moveSpeed = 5.0f);

private:
	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;

	// 탄환이 날아갈 위치 및 속도 설정
	void SetBulletDirectionAndSpeed();

private:
	// 이동 속도 변수
	float moveSpeed = 0.0f;
	float xDirection = 0.0f;
	float yDirection = 0.0f;

	// x, y 이동 위치 처리를 위한 float 변수
	float xPosition = 0.0f;
	float yPosition = 0.0f;

	// 탄환의 목적지 벡터
	Vector2 targetPosition;
};

