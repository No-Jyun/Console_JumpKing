#include "JumpLevel.h"
#include "Actor/MapTile/Block.h"
#include "Actor/MapTile/Ground.h"
#include "Actor/MapTile/Grass.h"
#include "Actor/MapTile/Wall.h"
#include "Actor/MapTile/Ice.h"
#include "Actor/MapTile/Spike.h"
#include "Actor/MapTile/UpwardGoal.h"
#include "Actor/MapTile/UpwardSpawn.h"
#include "Actor/MapTile/DownwardGoal.h"
#include "Actor/MapTile/DownwardSpawn.h"
#include "Actor/MapTile/Goal.h"
#include "Actor/MapTile/GoalArea.h"
#include "Actor/BulletSpawner.h"
#include "Actor/Bullet.h"
#include "Actor/Player.h"
#include "Util/Util.h"
#include "Core/Input.h"
#include "Game/Game.h"
#include "Render/Renderer.h"

#include <Windows.h>
#include <iostream>

/*
# : 화면 테두리
. : 바닥
, : 풀
/ : 벽
p : 플레이어
* : 얼음
0,1,2,3 : 가시 (방향)
u : 위 스테이지를 향하는 블럭
i : 아래 스테이지에서 위로 올라올때 생생될 위치
d : 아래 스테이지를 향하는 블럭
f : 위 스테이지에서 아래로 내려올때 생성될 위치
G : 게임 클리어 목적지
g : 게임 클리어 목적지의 트리거 박스
*/

// 스테이지를 로드하기 위한 문자열
static const char* stage[] =
{
	" ",
	"Stage1.txt",
	"Stage2.txt",
	"Stage3.txt"
};

JumpLevel::JumpLevel(const int stageIndex)
	: playerSpawnPosition(Vector2::Zero), currentStageNum(stageIndex)
{
	// 메인 화면의 폰트 크기 조절
	fontSize = 15;

	// stageIndex에 맞는 stage 로드
	LoadStage(stage[currentStageNum]);

	// 리스폰 타이머 설정
	playerRespawnTimer.SetTargetTime(playerRepawnTime);
}

void JumpLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 타이머 경과
	timer.Tick(deltaTime);

	// ESC키 처리.
	// 플레이어 사망시에도 입력받기 위해서 레벨에서 처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글
		Game::Get().ToggleMenu(LevelControl::PauseMenuLevel);
		return;
	}

	// 점수 보여주기
	ShowTimer();

	// Todo: Test
	if (Input::Get().GetKeyDown('Q'))
	{
		state = LevelState::NextLevel;
		return;
	}

	//// Todo: Test
	//if (Input::Get().GetKeyDown('W'))
	//{
	//	state = LevelState::PreviousLevel;
	//	return;
	//}

	//// Todo: Test
	//if (Input::Get().GetKeyDown('E'))
	//{
	//	AddNewActor(new Goal(Vector2(player->position.x + 5, player->position.y)));		
	//}

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

	// 탄환 충돌 판정 처리
	ProcessCollisionPlayerAndBullet();

	// 게임 클리어시
	if (isGameClear)
	{
		GameClear();
		return;
	}

	// 현재 레벨 상태에 따라 스테이지 변동
	if (state != LevelState::None)
	{
		PlayerGotoStage();
		return;
	}
}

void JumpLevel::LoadStage(const char* filename)
{
	// 파일 로드
	// 최종 파일 경로 만들기 ("../Assets/Stage/filename")
	char path[2048] = { };
	sprintf_s(path, 2048, "../Assets/Stage/%s", filename);

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

	// 엔진의 중앙으로 그릴 위치 설정
	drawPosition.x = Engine::Get().GetWidth() / 2;
	drawPosition.y = Engine::Get().GetHeight() / 2;

	int levelWidth = 0;
	int levelHeight = 0;

	// 너비 저장할 변수 선언
	int x = 0;

	// 맵 크기 측정을 위한 반복문
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
		x++;

		// 개행 문자가 나오면 높이 증가
		if (mapCharacter == '\n')
		{
			// 너비 갱신
			levelWidth = levelWidth > x ? levelWidth : x;

			// y좌표는 하나 늘리고, x좌표 초기화
			x = 0;

			// 높이 증가
			levelHeight++;

			continue;
		}
	}

	// 맵이 콘솔의 중앙에 오도록 정렬
	drawPosition.x -= levelWidth / 2;
	drawPosition.y -= levelHeight / 2;

	// 처음으로 돌아가 다시 맵 읽기
	index = 0;

	// 객체를 생성할 위치 값
	Vector2 position = drawPosition;

	// 스테이지 통과시 플레이어가 생성될 위치를 찾기위한 인덱스
	int goalIndex = 0;

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
			position.x = drawPosition.x;
			position.y++;

			continue;
		}

		/*
		# : 화면 테두리
		. : 바닥
		, : 풀
		/ : 벽
		p : 플레이어
		* : 얼음
		0,1,2,3 : 가시 (방향)
		u : 위 스테이지를 향하는 블럭
		i : 아래 스테이지에서 위로 올라올때 생생될 위치
		d : 아래 스테이지를 향하는 블럭
		f : 위 스테이지에서 아래로 내려올때 생성될 위치
		G : 게임 클리어 목적지
		g : 게임 클리어 목적지의 트리거 박스
		*/
		// 한 문자씩 처리
		switch (mapCharacter)
		{
		case '#':
			// 블럭(맵 경계) 생성
			AddNewActor(new Block(position));

			// 좌측 상단의 꼭짓점은 맵경계 블럭의 첫 생성시 설정
			if (leftUpPosition == Vector2::Zero)
			{
				leftUpPosition = position;
			}

			// 우측 하단의 꼭짓점은 맵경계 블럭의 x,y 좌표 최대 값
			rightDownPosition.x = rightDownPosition.x > position.x ? rightDownPosition.x : position.x;
			rightDownPosition.y = rightDownPosition.y > position.y ? rightDownPosition.y : position.y;

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

		case '/':
			// 벽 타일 생성
			AddNewActor(new Wall(position));
			break;

		case '0':
		case '1':
		case '2':
		case '3':
			// 가시 방향 인덱스 값 전달
			AddNewActor(new Spike(position, mapCharacter - '0'));
			break;

		case 'G':
			// 게임 클리어 목적지 타일 생성
			AddNewActor(new Goal(position));
			break;

		case 'g':
			// 게임 클리어 목적지 트리거 박스 타일 생성
			AddNewActor(new GoalArea(position));
			break;

		case 'u':
		{
			// 위 스테이지로 향하는 타일 생성
			UpwardGoal* goal = new UpwardGoal(position);
			AddNewActor(goal);

			// 배열에 저장
			upwardGoal.emplace_back(goal);
			break;
		}

		case 'i':
			// 플레이어가 스테이지를 통과하여 맵이 로딩되는 경우
			if (playerData.isSaved)
			{
				// 윗 스테이지를 향한다면
				if (playerData.isUpward)
				{
					// 현재 생성할 생성위치가 통과한 위치와 같다면
					if (playerData.goalIndex == goalIndex)
					{
						// 플레이어 생성 위치 설정
						playerSpawnPosition = position;
					}
					goalIndex++;
				}
			}

			// 위 스테이지 생성위치 생성
			AddNewActor(new UpwardSpawn(position));
			break;

		case 'd':
		{
			// 아래 스테이지로 향하는 타일 생성
			DownwardGoal* goal = new DownwardGoal(position);
			AddNewActor(goal);

			// 배열에 저장
			downwardGoal.emplace_back(goal);
			break;
		}

		case 'f':
			// 플레이어가 스테이지를 통과하여 맵이 로딩되는 경우
			if (playerData.isSaved)
			{
				// 아래 스테이지를 향한다면
				if (!playerData.isUpward)
				{
					// 현재 생성할 생성위치가 통과한 위치와 같다면
					if (playerData.goalIndex == goalIndex)
					{
						// 플레이어 생성 위치 설정
						playerSpawnPosition = position;
					}
					goalIndex++;
				}
			}

			// 아래 스테이지 생성위치 생성
			AddNewActor(new DownwardSpawn(position));
			break;

		case 'p':
			// 플레이어 데이터가 저장되지 않았다면
			if (!playerData.isSaved)
			{
				// 맵 로드시 플레이어 생성 위치 초기화
				playerSpawnPosition = position;
			}

			// 맵 로드시 p 가 존재한다면 1스테이지라는 의미이므로 리스폰 위치 설정
			playerRespawnPosition = position;
			break;
		}

		// x 좌표 증가 처리
		position.x++;
	}

	// 플레이어 생성

	if (!playerData.isSaved && playerSpawnPosition != Vector2(92, 54))
	{
		playerSpawnPosition = Vector2(92, 54);
	}

	player = new Player(playerSpawnPosition);
	AddNewActor(player);

	// 3스테이지라면
	if (currentStageNum > 2)
	{
		// 탄환 생성기 생성
		AddNewActor(new BulletSpawner(Vector2::Zero, this));
	}

	// 사용한 버퍼 해제
	delete[] data;

	// 파일이 정상적으로 열렸으면 닫기
	fclose(file);
}

void JumpLevel::PlayerGotoStage()
{
	// 스테이지 변동
	currentStageNum += static_cast<int>(state);

	// 현재 맵 정리
	ClearLevel();

	// 새로운 맵 로드
	LoadStage(stage[currentStageNum]);

	// 플레이어 상태 로드
	LoadPlayerData();
}

void JumpLevel::SavePlayerData(const int goalIndex)
{
	// 플레이어 상태 저장
	// isSaved가 켜졌다면 앞으로 플레이어를 생성시 로드해야함
	playerData.isSaved = true;
	playerData.isUpward = state == LevelState::NextLevel;

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
	player->velocityX = playerData.velocityX;
	player->velocityY = playerData.velocityY;
	player->isLeft = playerData.isLeft;
	player->isLanding = playerData.isLanding;
	player->isJumping = playerData.isJumping;
	player->isFalling = playerData.isFalling;

	// 플레이어 데이터 초기화
	playerData.Clear();
}

void JumpLevel::ClearLevel()
{
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

	// 레벨 관련 변수 초기화
	isPlayerDead = false;
	isGameClear = false;
	state = LevelState::None;
	upwardGoal.clear();
	downwardGoal.clear();
	bulletSpawnPositions.clear();
	leftUpPosition = Vector2::Zero;
	rightDownPosition = Vector2::Zero;
	player = nullptr;
}

void JumpLevel::GameClear()
{	
	// 클리어 화면으로 메뉴 토글
	Game::Get().ToggleMenu(LevelControl::ClearMenuLevel);
}

void JumpLevel::RespawnPlayer()
{
	// 1스테이지에서 사망했다면 플레이어 생성만 처리
	if (currentStageNum == 1)
	{
		player = new Player(playerRespawnPosition);
		AddNewActor(player);
		return;
	}

	// 스테이지 인덱스 초기화
	currentStageNum = 1;

	// 플레이어 데이터 초기화
	playerData.Clear();

	// 현재 맵 정리
	ClearLevel();

	// 스테이지 1 로드
	LoadStage(stage[currentStageNum]);
}

static const char* screenBuffer = "                                                                                                    ";

void JumpLevel::ShowTimer()
{
	// 점수 문자열 만들기
	sprintf_s(timeString, 128, "Time: %d", static_cast<int>(timer.GetElapsedTime()));

	// 그리기
	Renderer::Get().Submit(timeString, Vector2(leftUpPosition.x, rightDownPosition.y + 1));

	// 화면 오른쪽 아래 보이는 문자열 덮어쓰기
	Renderer::Get().Submit(screenBuffer, (rightDownPosition + Vector2::Right), Color::White, 6);
}

const Vector2& JumpLevel::GetRandomBulletSpawnPosition()
{
	// 배열 길이 확인
	size_t length = bulletSpawnPositions.size();

	// 배열이 비었다면 배열 채우기
	if (length == 0)
	{
		// 액터 순회
		for (Actor* const actor : actors)
		{
			// 맵 경계 타일이 아니라면 스킵
			if (!actor->IsTypeOf<Block>())
			{
				continue;
			}

			// 액터의 위치가 맵의 하단이면 스킵
			if (actor->GetPosition().y >= leftUpPosition.y + ((rightDownPosition.y - leftUpPosition.y) / 2))
			{
				continue;
			}

			// 배열에 위치 추가
			bulletSpawnPositions.emplace_back(actor->GetPosition());
		}

		// 길이 재설정
		length = bulletSpawnPositions.size();
	}

	// 랜덤으로 위치 선택하여 반환
	return bulletSpawnPositions[Util::Random(0, static_cast<int>(length) - 1)];
}

const Vector2& JumpLevel::GetPlayerPosition()
{
	// 플레이어 존재 체크
	if (!player || isPlayerDead)
	{
		return Vector2::Zero;
	}

	return player->GetPosition();
}

void JumpLevel::ProcessCollisionPlayerAndOther()
{
	// 플레이어가 없다면 리턴
	if (!player)
	{
		return;
	}

	// 액터 필터링을 위한 변수
	std::vector<Actor*> others;

	// 액터 필터링
	for (Actor* const actor : actors)
	{
		// 현재 액터가 충돌하지 않을 액터 또는 플레이어라면 스킵
		if (IsCollisionSkipped(actor) || actor == player)
		{
			continue;
		}

		// 배열에 추가
		others.emplace_back(actor);
	}

	// 판정 처리 안해도 되는지 확인
	if (others.size() == 0)
	{
		return;
	}

	// 바닥 충돌 판정용 플래그
	bool isLanding = false;

	// 바닥 충돌한 액터 저장용 변수
	Actor* landedActor = nullptr;

	// 충돌 판정
	for (Actor* const actor : others)
	{
		// 충돌 검사
		Vector2 crashedDir = player->TestIntersect(*actor);

		// 충돌하지 않은 액터 넘기기
		if (crashedDir == Vector2::Zero)
		{			
			continue;
		}

		// 충돌한 액터가 다음 스테이지로 이동하는 통로라면 다음 스테이지 로드
		if (actor->IsTypeOf<UpwardGoal>())
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

		// 충돌한(아래에 있는) 액터가 아래 스테이지로 이동하는 통로라면 이전 스테이지 로드
		else if (actor->IsTypeOf<DownwardGoal>())
		{
			// 다음 레벨로 가도록 레벨 상태 저장
			state = LevelState::PreviousLevel;

			// 통과한 목적지의 인덱스 찾기
			int goalIndex = 0;
			while (downwardGoal[goalIndex] != actor)
			{
				goalIndex++;
			}

			// 플레이어 상태 저장
			SavePlayerData(goalIndex);

			return;
		}

		// 충돌한 액터가 가시라면 사망처리
		// 스테이지 1로 돌아가서 리스폰
		else if (actor->IsTypeOf<Spike>())
		{
			// 플레이어 사망
			PlayerDead();

			return;
		}

		// 충돌한 액터가 게임 클리어 목적지 또는 해당 영역이라면
		else if (actor->IsTypeOf<Goal>() || actor->IsTypeOf<GoalArea>())
		{
			// 게임 클리어!!
			isGameClear = true;

			return;
		}

		// 바닥 충돌한 경우
		if (crashedDir == Vector2::Down)
		{
			// 플래그 On
			isLanding = true;

			// 액터 저장
			landedActor = actor;
		}

		// 플레이어에게 충돌방향과 액터 알리기
 		player->CrashedWithOther(crashedDir, *actor);
		break;
	}

	// 플래그가 켜졌다면
	if (isLanding)
	{
		// 플레이어 착륙
		player->UpdateIsLanding(true);

		// 얼음을 밟은 경우
		if (landedActor->IsTypeOf<Ice>())
		{
			// 얼음을 밟았다고 알림
			player->UpdateIsOnIce(true);
		}
		else
		{
			// 얼음을 밟지 않았다고 알림
			player->UpdateIsOnIce(false);
		}
	}
	else
	{
		// 바닥 충돌을 하지 않았다면 공중 판정
		player->UpdateIsLanding(false);
	}
}

void JumpLevel::ProcessCollisionPlayerAndBullet()
{
	// 플레이어가 없다면 리턴
	if (!player)
	{
		return;
	}

	// 액터 필터링을 위한 변수
	std::vector<Actor*> others;

	// 액터 필터링
	for (Actor* const actor : actors)
	{
		// 현재 액터가 충돌하지 않을 액터 또는 플레이어라면 스킵
		if (!actor->IsTypeOf<Bullet>())
		{
			continue;
		}

		// 배열에 추가
		others.emplace_back(actor);
	}

	// 판정 처리 안해도 되는지 확인
	if (others.size() == 0)
	{
		return;
	}

	// 충돌 판정
	for (Actor* const actor : others)
	{
		// 화면에 그려진 플레이어와 탄환의 위치가 같을때만 충돌 판정
		if (player->GetPosition() == actor->GetPosition())
		{
			// 충돌한 액터가 탄환이라면 사망처리
			// 스테이지 1로 돌아가서 리스폰
			// 플레이어 사망
			PlayerDead();

			// 탄환 제거
			actor->Destroy();

			return;
		}
	}
}

bool JumpLevel::IsCollisionSkipped(Actor* const other)
{
	// 위 스테이지 생성 위치 타일 액터라면 충돌 X
	if (other->IsTypeOf<UpwardSpawn>())
	{
		return true;
	}
	// 아래 스테이지 생성 위치 타일 액터라면 충돌 X
	if (other->IsTypeOf<DownwardSpawn>())
	{
		return true;
	}

	// 그 외는 충돌 검사 필요
	return false;
}

void JumpLevel::PlayerDead()
{
	// 플레이어 죽음 설정
	isPlayerDead = true;

	// 플레이어 사망 처리
	player->Die();

	// 포인터 초기화
	player = nullptr;
}
