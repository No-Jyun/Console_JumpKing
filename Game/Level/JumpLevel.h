#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Util/Timer.h"

using namespace Wanted;

class JumpLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(JumpLevel, Level)

public:
	JumpLevel(const int stageIndex);

	virtual void Tick(float deltaTime) override;

private:
	// 스테이지에 맞는 맵을 로드하는 함수
	void LoadMap(const char* filename);

	// 게임 클리어 확인 함수
	void CheckGameClear();

	// 충돌 판정 처리 함수
	// Todo: 탄환과 충돌
	void ProcessCollisionPlayerAndOther();
	// Todo: 가시와 충돌
	//void ProcessCollisionPlayerAndSpike();

	// 플레이어의 발판을 확인하는 함수
	void CheckGround();

	// 플레이어 리스폰 함수
	void RespawnPlayer();

private:
	// 게임 클리어 여부를 알려주는 변수
	bool isGameClear = false;	

	// 플레이어가 죽었는지 확인.
	bool isPlayerDead = false;

	// 플레이어 생성 위치 (플레이어 리스폰 위해 생성시 저장)
	Vector2 playerSpawnPosition;

	// 플레이어 리스폰 대기시간
	const float playerRepawnTime = 1.5f;

	// 플레이어 리스폰 타이머
	Timer playerRespawnTimer;
};

