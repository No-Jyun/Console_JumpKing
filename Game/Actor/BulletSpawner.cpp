#include "BulletSpawner.h"
#include "Actor/Bullet.h"
#include "Util/Util.h"
#include "Level/JumpLevel.h"

BulletSpawner::BulletSpawner(const Vector2& position, JumpLevel* level)
	: super(" ", position), jumpLevel(level)
{
	// 탄환 생성기는 보이지 않게 설정
	sortingOrder = -1;
}

void BulletSpawner::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 타이머 경과
	spawnTimer.Tick(deltaTime);

	// 타이머 끝난 경우
	if (spawnTimer.IsTimeOut())
	{
		// 타이머 리셋
		spawnTimer.Reset();

		// 플레이어 위치 받아오기
		Vector2 playerPosition = jumpLevel->GetPlayerPosition();

		// 플레이어가 존재하면
		if (playerPosition != Vector2::Zero)
		{
			// 설정값으로 탄환 생성
			jumpLevel->AddNewActor(new Bullet(bulletSpawnPosition, playerPosition, bulletSpeed));
		}

		// 다시 랜덤으로 설정값 갱신
		SetRandomSetting();
	}
}

void BulletSpawner::SetRandomSetting()
{
	// 생성될 탄환의 속도 랜덤 결정
	bulletSpeed = Util::RandomRange(3.0f, 6.0f);

	// 생성될 속도 랜덤 결정
	spawnTimer.SetTargetTime(Util::RandomRange(4.0f, 8.0f));

	// 생성될 위치 랜덤 결정
	bulletSpawnPosition = jumpLevel->GetRandomBulletSpawnPosition();
}
