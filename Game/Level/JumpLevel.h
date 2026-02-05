#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

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

private:
	// 게임 클리어 여부를 알려주는 변수
	bool isGameClear = false;	

	// Todo: 플레이어가 죽은 위치 (Draw에서 처리하기 위해 Tick에서 저장)
	Vector2 playerDeadPosition;

};

