#include "Bullet.h"
#include "Util/Util.h"
#include "Level/Level.h"

static const Vector2 directionVector[4] =
{
	Vector2::Down,
	Vector2::Right,
	Vector2::Up,
	Vector2::Left
};

Bullet::Bullet(const Vector2& position, const Vector2& playerPosition, float moveSpeed)
	: super("/", position, Color::LightYellow),
	moveSpeed(moveSpeed), targetPosition(playerPosition),
	xPosition(static_cast<float>(position.x)), yPosition(static_cast<float>(position.y))
{
	// 벽을 관통하여 추적하므로 그리기 우선순위 높임
	sortingOrder = 5;

	// 탄환이 날아갈 방향과 속도 설정
	SetBulletDirectionAndSpeed();
}

void Bullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// x위치 업데이트
	xPosition += xDirection * moveSpeed * deltaTime;

	// y위치 업데이트
	yPosition += yDirection * moveSpeed * deltaTime;

	// 좌표 변환시 그려질 위치 임시 저장
	int tempXpos = Util::FloatCastInt(xPosition);
	int tempYpos = Util::FloatCastInt(yPosition);

	Vector2 leftUp = GetOwner()->GetLevelLeftUpPosition();
	Vector2 rightDown = GetOwner()->GetLevelRightDownPosition();
	
	// 좌표 검사 (레벨 위,아래, 양끝 지점을 벗어났는지 확인
	if (tempXpos < leftUp.x || tempXpos > rightDown.x ||
		tempYpos < leftUp.y || tempYpos > rightDown.y)
	{
		Destroy();
		return;
	}

	// x 좌표 업데이트
	position.x = tempXpos;

	// y 좌표 업데이트
	position.y = tempYpos;
}

void Bullet::SetBulletDirectionAndSpeed()
{
	// 랜덤으로 4방향 중 하나로 설정
	int direction = Util::Random(0, 3);

	// 타겟 벡터 업데이트
	targetPosition = targetPosition + directionVector[direction];

	// 방향 벡터
	Vector2 targetDirection = targetPosition - position;

	// 방향 벡터의 길이 구하기
	float length = Util::Sqrt(static_cast<float>(
		(targetDirection.x * targetDirection.x) + (targetDirection.y * targetDirection.y)));

	// 방향 벡터를 정규화하여 x좌표의 방향 결정
	xDirection = static_cast<float>(targetDirection.x) / length;

	// 방향 벡터를 정규화하여 y좌표의 방향 결정
	yDirection = static_cast<float>(targetDirection.y) / length;

	// y방향 좌표의 절대값이 특정 값 이상이면 수직 방향으로 변경
	if (fabs(yDirection) > 0.8f)
	{
		ChangeImage("|");
	}

	// x방향 * y방향이 양수라면 콘솔 화면에서 우하단 또는 좌상단을 향함
	else if (xDirection * yDirection > 0)
	{
		ChangeImage("\\");
	}

	// x방향 * y방향이 음수라면 콘솔 화면에서 우상단 또는 좌하단을 향함
	else
	{
		ChangeImage("/");
	}
}
