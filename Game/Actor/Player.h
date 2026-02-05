#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class Player : public Actor
{
	// 플레이어 상태
	enum class PlayerState
	{
		None,
		IdleL,
		IdleR,
		Charging,
		Upward,
		Downward,
		Die,
	};

	// RTTI 선언
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	// 플레이어가 바닥에 있다고 알려주는 함수
	void OnGround(const Actor& other);

	// 플레이어 기준 벡터 방향으로 충돌하고 충돌 조치하는 함수
	void CrashedWithOther(const Vector2& crashedDirection, const Actor& other);

	// 충돌 여부 확인 함수, 충돌하는 경우가 플레이어와 다른 액터뿐이므로 
	// 플레이어의 함수로 이동
	const Vector2 TestIntersect(const Actor* const other);

	// 상태에 따른 애니메이션 (문자열) 변경
	void ChangeImageAndColor();

	// 플레이어 상태에 따른 문자열 Getter
	const char* GetPlayerStringByState(const PlayerState& state);

	// 플레이어 상태에 따른 문자열 색상 Getter
	const Color GetPlayerColorByState(const PlayerState& state);

private:
	// 점프 로직
	// 첫 점프키 입력에 대한 설정
	void JumpKeyDown(float deltaTime);

	// 점프키 입력 중을 처리하는 함수
	void JumpKey(float deltaTime);

	// 점프를 하기위한 사전준비 함수
	void Jump();

	void Jumping(float deltaTime);

	// 점프력을 구하는 함수
	const int GetJumpPower(const float chargedTime);

	// 왼쪽으로 이동하는 함수
	void MoveLeft(float deltaTime);

	// 오른쪽으로 이동하는 함수
	void MoveRight(float deltaTime);

private:
	// 좌우 구분 변수
	bool isLeft = false;

	// 공중에 있는지 판단하는 변수
	bool isJumping = false;

	// 플레이어가 점프키를 눌렀는지 판단하는 변수
	bool isJumpKeyDown = false;

	float x = 0.0f;
	float y = 0.0f;

	// Todo: 이동속도 조정
	float moveSpeed = 30.0f;
	//float maxSpeed = 5.0f;

	// 점프 관련 변수
	// 중력 가속도 설정
	const float gravity = 1.5f;

	// 현재 속도
	float jumpVelocityX = 0.0f;
	float jumpVelocityY = 0.0f;

	// 타이머 변수
	Timer jumpTimer;

	// 플레이어 상태
	PlayerState state;
};

