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
		ChargingL,
		ChargingR,
		Upward,
		Downward,
		Die,
	};

	// RTTI 선언
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	// 플레이어 기준 벡터 방향으로 충돌하고 충돌 조치하는 함수
	void CrashedWithOther(const Vector2& crashedDirection, const Actor& other);

	// 충돌 여부 확인 함수, 충돌하는 경우가 플레이어와 다른 액터뿐이므로 
	// 플레이어의 함수로 이동
	const Vector2 TestIntersect(const Actor* const other);

	// 추락 판단을 위해 플레이어 아래의 블럭여부를 업데이트하는 함수
	void UpdateIsLanding(bool isLanding);

	// 플레이어의 세부 좌표 Getter
	inline const float GetXPosition() { return x; }
	inline const float GetYposition() { return y; }

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

	// 추락 시키는 함수
	void Fall(float deltaTime);

	// 왼쪽으로 이동하는 함수
	void MoveLeft(float deltaTime);

	// 오른쪽으로 이동하는 함수
	void MoveRight(float deltaTime);

private:
	// 좌우 구분 변수
	bool isLeft = false;

	// 바닥에 있는지 판단하는 변수
	bool isLanding = true;

	// 점프 중인지 판단하는 변수
	bool isJumping = false;

	// 추락 중인지 판단하는 변수
	bool isFalling = false;

	// 플레이어가 점프키를 눌렀는지 판단하는 변수
	bool isJumpKeyDown = false;

	// float를 사용하여 가속도 운동 처리
	float x = 0.0f;
	float y = 0.0f;

	// Todo: 이동속도 조정
	float moveSpeed = 10.0f;
	//float maxSpeed = 5.0f;

	// 점프 관련 변수
	// 중력 가속도 설정
	const float gravity = 15.0f;
	//const float maxFallSpeed = 10.0f;

	// 현재 속도
	float velocityX = 0.0f;
	float velocityY = 0.0f;

	// 타이머 변수
	Timer jumpTimer;

	// 플레이어 상태
	PlayerState state;
};

