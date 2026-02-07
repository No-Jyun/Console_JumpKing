#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Util/Timer.h"

using namespace Wanted;

class Player;
// 게임을 플레이하는 레벨 클래스
class JumpLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(JumpLevel, Level)

	// 레벨 관리를 위한 열거형
	enum class LevelState
	{
		PreviousLevel = -1,
		None = 0,
		NextLevel = 1
	};

	// 플레이어의 데이터를 저장/로드 하기위한 데이터 구조체
	struct PlayerData
	{
		// 플레이어를 생성할지 로드할지 판단하는 변수
		bool isSaved = false;

		// 통과한 지점의 인덱스
		int goalIndex = 0;

		// 윗 스테이지를 향하는지 판단하는 변수
		bool isUpward = true;

		// 통과했을때 x좌표 속도
		float velocityX = 0.0f;

		// 통과했을때 y좌표 속도
		float velocityY = 0.0f;

		// 플레이어의 상태 변수 저장
		// 좌우 구분 변수
		bool isLeft = false;

		// 바닥에 있는지 판단하는 변수
		bool isLanding = true;

		// 점프 중인지 판단하는 변수
		bool isJumping = false;

		// 추락 중인지 판단하는 변수
		bool isFalling = false;

		// 데이터 초기화 함수
		void Clear()
		{
			isSaved = false;
			goalIndex = 0;
			isUpward = true;
			velocityX = 0.0f;
			velocityY = 0.0f;
			isLeft = false;
			isLanding = true;
			isJumping = false;
			isFalling = false;
		}
	};

public:
	JumpLevel(const int stageIndex);

	virtual void Tick(float deltaTime) override;

private:
	// 스테이지를 로드하는 함수
	void LoadStage(const char* filename);

	// 플레이어가 다음/이전 스테이지 통로에 도달했을때 처리하는 함수
	void PlayerGotoStage();

	// 플레이어가 맵의 경계를 지나면 플레이어의 상태 저장하는 함수
	void SavePlayerData(const int goalIndex);

	// 새로운 스테이지 입장시 플레이어의 상태를 로드하는 함수
	void LoadPlayerData();

	// 스테이지 로드시 레벨을 정리하는 함수
	void ClearLevel();

	// 게임 클리어 확인 함수
	void CheckGameClear();

	// 충돌 판정 처리 함수
	// Todo: 탄환과 충돌
	void ProcessCollisionPlayerAndOther();
	//void ProcessCollisionPlayerAndSpike();

	// 플레이어의 발판을 확인하는 함수
	void CheckGround();

	// 충돌을 무시할 액터인지 판단하는 함수
	bool IsCollisionSkipped(Actor* const other);

	// 플레이어 리스폰 함수
	void RespawnPlayer();

private:
	// 현재 로드된 스테이지 넘버
	int currentStageNum = 0;

	// 게임 클리어 여부를 알려주는 변수
	bool isGameClear = false;	

	// 플레이어가 죽었는지 확인.
	bool isPlayerDead = false;

	// 레벨 관리 상태 변수
	LevelState state = LevelState::None;

	// 위 스테이지로 향하는 타일을 저장할 배열
	std::vector<Actor*> upwardGoal;

	// 아래 스테이지로 향하는 타일을 저장할 배열
	std::vector<Actor*> downwardGoal;
	
	// 현재 맵의 플레이어를 가리키는 포인터
	Player* player = nullptr;
	
	// 맵 이동시 저장되고 로드할 플레이어 데이터
	PlayerData playerData;

	// 플레이어 생성 위치
	Vector2 playerSpawnPosition;

	// 플레이어 재생성 위치 (플레이어 리스폰 위해 생성시 저장)
	Vector2 playerRespawnPosition;

	// 플레이어 리스폰 대기시간
	const float playerRepawnTime = 1.5f;

	// 플레이어 리스폰 타이머
	Timer playerRespawnTimer;

	// 콘솔 중앙에 오도록 그릴 위치
	Vector2 drawPosition;
};

