#include "JumpLevel.h"
#include "Actor/Block.h"
#include "Actor/Ground.h"
#include "Actor/Player.h"
#include "Util/Util.h"

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
	: playerDeadPosition(Vector2::Zero)
{
	// stageIndex에 맞는 stage 로드
	LoadMap(stage[stageIndex]);
}

void JumpLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 충돌 판정 처리
	ProcessCollisionPlayerAndOther();

	// 발판 확인 
	CheckGround();

	// Draw 필요??
	// 플레이어가 죽었을 경우 / 클리어 했을 경우
}

void JumpLevel::LoadMap(const char* filename)
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
			"JumpLevel::LoadMap() - Failed to load map",
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
		case '1':
			AddNewActor(new Block(position));
			break;

		case '.':
			AddNewActor(new Ground(position));
			break;

		case 'p':
			AddNewActor(new Player(position));
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

void JumpLevel::CheckGameClear()
{
	// Todo: 플레이어가 Flag에 닿았는지 확인
}

void JumpLevel::ProcessCollisionPlayerAndOther()
{
	// 액터 필터링을 위한 변수.
	Player* player = nullptr;
	std::vector<Actor*> others;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (!actor->IsTypeOf<Block>())
		{
			others.emplace_back(actor);
		}
	}

	// 판정 처리 안해도 되는지 확인.
	if (others.size() == 0 || !player)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const actor : others)
	{
		// 충돌 검사
		Vector2 crashedDir = player->TestIntersect(actor);

		// 충돌하지 않은 액터 넘기기
		if (crashedDir == Vector2::Zero)
		{
			continue;
		}

		// 플레이어에게 충돌방향과 액터 알리기
		player->CrashedWithOther(crashedDir, *actor);
		break;
	}
}

void JumpLevel::CheckGround()
{
	// 액터 필터링을 위한 변수.
	Player* player = nullptr;
	std::vector<Actor*> grounds;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (!actor->IsTypeOf<Block>())
		{
			grounds.emplace_back(actor);
		}
	}

	// 판정 처리 안해도 되는지 확인.
	if (grounds.size() == 0 || !player)
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
			return;
		}
	}

	// 발판 판정이후 리턴되지 않았다면 플레이어 아래에
	// 발판이 없다는 뜻이므로 isLanding 업데이트
	player->UpdateIsLanding(false);
}
