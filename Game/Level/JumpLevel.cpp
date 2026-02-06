#include "JumpLevel.h"
#include "Actor/Block.h"
#include "Actor/Ground.h"
#include "Actor/Player.h"
#include "Actor/Grass.h"
#include "Actor/Ice.h"
#include "Actor/Spike.h"
#include "Util/Util.h"
#include "Core/Input.h"
#include "Game/Game.h"
#include "Actor/Goal.h"

#include <Windows.h>
#include <iostream>

/*
# : 화면 테두리
. : 바닥
p : 플레이어
*/

// 스테이지를 로드하기 위한 문자열
static const char* stage[] =
{
	" ",
	"Stage1.txt"
	//"Stage2.txt"
};

JumpLevel::JumpLevel(const int stageIndex)
	: playerSpawnPosition(Vector2::Zero), currentStageNum(stageIndex)
{
	// stageIndex에 맞는 stage 로드
	LoadStage(stage[stageIndex]);

	// 리스폰 타이머 설정
	playerRespawnTimer.SetTargetTime(playerRepawnTime);
}

void JumpLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// ESC키 처리.
	// 플레이어 사망시에도 입력받기 위해서 레벨에서 처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글
		Game::Get().ToggleMenu();
		return;
	}

	// 플레이어 사망시
	if (isPlayerDead)
	{
		// 리스폰 타이머 증가
		playerRespawnTimer.Tick(deltaTime);

		// 리스폰 타이머 경과시
		if (playerRespawnTimer.IsTimeOut())
		{
			// 플레이어 사망 변수 초기화
			isPlayerDead = false;

			// 리스폰 타이머 초기화
			playerRespawnTimer.Reset();

			// 플레이어 리스폰
			RespawnPlayer();
		}
		return;
	}

	// 충돌 판정 처리
	ProcessCollisionPlayerAndOther();

	// 발판 확인 
	CheckGround();

	if (state == LevelState::NextLevel)
	{
		PlayerGotoNextStage();		
	}
	else if (state == LevelState::PreviousLevel)
	{
		// Todo: 이전 스테이지로 
	}

	// Draw 필요??
	// 플레이어가 죽었을 경우 / 클리어 했을 경우
}

void JumpLevel::LoadStage(const char* filename)
{
	// 파일 로드
	// 최종 파일 경로 만들기 ("../Assets/filename")
	char path[2048] = { };
	sprintf_s(path, 2048, "../Assets/%s", filename);

	// 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, path, "rt");

	// 예외 처리
	if (!file)
	{
		// 오류 메시지 출력.
		MessageBoxA(
			nullptr,
			"JumpLevel::LoadStage() - Failed to load stage",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 맵 읽기
	// 맵 크기 파악: File Position 포인터를 파일의 끝으로 이동
	fseek(file, 0, SEEK_END);

	// 이 위치 읽기
	size_t fileSize = ftell(file);

	// File Position 처음으로 되돌리기
	rewind(file);

	// 파일에서 데이터를 읽어올 버퍼 생성
	char* data = new char[fileSize + 1];

	// 데이터 읽기
	size_t readSize = fread(data, sizeof(char), fileSize, file);

	// 읽어온 문자열을 분석 (파싱-Parcing) 해서 출력
	// 인덱스를 사용해 한 문자씩 읽기
	int index = 0;

	// 객체를 생성할 위치 값
	Vector2 position;

	while (true)
	{
		// 종료 조건
		if (index >= fileSize)
		{
			break;
		}

		// 캐릭터 읽기
		char mapCharacter = data[index];
		index++;

		// 개행 문자 처리
		if (mapCharacter == '\n')
		{
			// y좌표는 하나 늘리고, x좌표 초기화
			position.x = 0;
			position.y++;

			continue;
		}

		/*
		# : 화면 테두리
		. : 바닥
		p : 플레이어
		*/
		// 한 문자씩 처리
		switch (mapCharacter)
		{
		case '#':
			// 블럭(맵 경계) 생성
			AddNewActor(new Block(position));
			break;

		case '.':
			// 땅(바닥) 타일 생성
			AddNewActor(new Ground(position));
			break;

		case ',':
			// 잔디(땅과 같은 바닥) 타일 생성
			AddNewActor(new Grass(position));
			break;

		case '*':
			// 얼음(미끄러짐) 타일 생성
			AddNewActor(new Ice(position));
			break;

		case '0':
		case '1':
		case '2':
		case '3':
			// 가시 방향 인덱스 값 전달
			AddNewActor(new Spike(position, mapCharacter - '0'));
			break;

		case 'g':
			// 목적지 타일 생성
			Goal * goal = new Goal(position);
			AddNewActor(goal);

			// 배열에 저장
			upwardGoal.emplace_back(goal);
			break;

		case 'p':
			// 맵 로드시 플레이어 생성 위치 초기화
			playerSpawnPosition = position;

			// 플레이어 생성
			player = new Player(position);
			AddNewActor(player);
			break;
		}

		// x 좌표 증가 처리
		position.x++;
	}

	// 사용한 버퍼 해제
	delete[] data;

	// 파일이 정상적으로 열렸으면 닫기
	fclose(file);
}

void JumpLevel::PlayerGotoNextStage()
{
	// 현재 맵에 그려진거 지우기
	// 메모리 정리
	for (Actor*& actor : actors)
	{
		if (actor)
		{
			delete actor;
			actor = nullptr;
		}
	}

	// 배열 초기화
	actors.clear();

	//system("cls");

	// 스테이지 변동
	currentStageNum += static_cast<int>(state);

	// 레벨 관련 변수 초기화
	state = LevelState::None;
	upwardGoal.clear();
	downwardGoal.clear();
	player = nullptr;

	// 새로운 맵 로드
	// Todo: 여기부터 시작
	// 플레이어 생성 및 상태 로드
}

void JumpLevel::PlayerGotoPreviousStage()
{
}

void JumpLevel::LoadNextStage(const char* filename)
{

}

void JumpLevel::SavePlayerData(const int goalIndex)
{
	// 플레이어 상태 저장
	// isSaved가 켜졌다면 앞으로 플레이어를 생성시 로드해야함
	playerData.isSaved = true;

	playerData.goalIndex = goalIndex;
	playerData.velocityX = player->velocityX;
	playerData.velocityY = player->velocityY;
	playerData.isLeft = player->isLeft;
	playerData.isLanding = player->isLanding;
	playerData.isJumping = player->isJumping;
	playerData.isFalling = player->isFalling;	
}

void JumpLevel::LoadPlayerData()
{
	
}

void JumpLevel::CheckGameClear()
{
	// Todo: 플레이어가 Flag에 닿았는지 확인
}

void JumpLevel::ProcessCollisionPlayerAndOther()
{
	if (!player)
	{
		return;
	}

	// 액터 필터링을 위한 변수.
	std::vector<Actor*> others;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		// 맵 경계와 플레이어를 제외한 액터 배열에 추가
		if (actor != player && !actor->IsTypeOf<Block>())
		{
			others.emplace_back(actor);
		}
	}

	// 판정 처리 안해도 되는지 확인.
	if (others.size() == 0)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const actor : others)
	{
		// 충돌 검사
		Vector2 crashedDir = player->TestIntersect(*actor);

		// 충돌하지 않은 액터 넘기기
		if (crashedDir == Vector2::Zero)
		{
			continue;
		}

		// 충돌한 액터가 목적지라면 다음 스테이지 로드
		if (actor->IsTypeOf<Goal>())
		{
			// 다음 레벨로 가도록 레벨 상태 저장
			state = LevelState::NextLevel;

			// 통과한 목적지의 인덱스 찾기
			int goalIndex = 0;
			while (upwardGoal[goalIndex] != actor)
			{
				goalIndex++;
			}

			// 플레이어 상태 저장
			SavePlayerData(goalIndex);

			return;
		}

		// 충돌한 액터가 가시라면 사망처리
		// Todo: 스테이지 1로 돌아가서 리스폰
		if (actor->IsTypeOf<Spike>())
		{
			// 플레이어 죽음 설정
			isPlayerDead = true;

			// 플레이어 사망 처리
			player->Die();

			// 포인터 초기화
			player = nullptr;

			return;
		}

		// 플레이어에게 충돌방향과 액터 알리기
		player->CrashedWithOther(crashedDir, *actor);
		break;
	}
}

void JumpLevel::CheckGround()
{
	if (!player)
	{
		return;
	}

	// 액터 필터링을 위한 변수.
	std::vector<Actor*> grounds;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		// 맵 경계와 플레이어를 제외한 액터 배열에 추가
		if (actor != player && !actor->IsTypeOf<Block>())
		{
			grounds.emplace_back(actor);
		}
	}

	// 판정 처리 안해도 되는지 확인.
	if (grounds.size() == 0)
	{
		return;
	}

	// 발판 판정.
	for (Actor* const ground : grounds)
	{
		// x좌표가 같지 않은 액터 넘기기
		if (player->GetPosition().x != ground->GetPosition().x)
		{
			continue;
		}

		// 플레이어 아래의 발판 검사
		// 플레이어의 높이가 1이므로 1을 더한다
		float playerFoot = player->GetYposition() + 1.0f;
		float groundTop = Util::FloatCastInt(ground->GetPosition().y);

		// 플레이어가 한칸 내려간다면 겹치는 여부 판단 
		if (playerFoot == groundTop)
		{
			player->UpdateIsLanding(true);

			// 얼음을 밟은 경우
			if (ground->IsTypeOf<Ice>())
			{
				player->UpdateIsOnIce(true);
			}
			else
			{
				player->UpdateIsOnIce(false);
			}

			// 가시를 밟은 경우
			if (ground->IsTypeOf<Spike>())
			{
				// 플레이어 죽음 설정
				isPlayerDead = true;

				// 플레이어 사망 처리
				player->Die();

				// 포인터 초기화
				player = nullptr;
			}

			return;
		}
	}

	// 발판 판정이후 리턴되지 않았다면 플레이어 아래에
	// 발판이 없다는 뜻이므로 isLanding 업데이트
	player->UpdateIsLanding(false);
}

void JumpLevel::RespawnPlayer()
{
	player = new Player(playerSpawnPosition);
	AddNewActor(player);
}
