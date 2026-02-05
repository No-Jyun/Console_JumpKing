#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Util/Util.h"

Player::Player(const Vector2& position)
	: super("b", position, Color::Green), state(PlayerState::IdleR)
{
	sortingOrder = 10;

	x = static_cast<float>(position.x);
	y = static_cast<float>(position.y);

	// 점프 타이머 세팅
	jumpTimer.SetTargetTime(1.2f);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
	}

	// 점프 중에는 입력 방지
	if (!isJumping)
	{
		// 점프키 입력 중에는 좌우 이동 불가
		if (!isJumpKeyDown)
		{
			// 좌우 방향키 입력처리.
			if (Input::Get().GetKey(VK_LEFT))
			{
				MoveLeft(deltaTime);
			}

			if (Input::Get().GetKey(VK_RIGHT))
			{
				MoveRight(deltaTime);
			}
		}

		// 점프 입력 처리
		// 첫 점프키 입력시
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			JumpKeyDown(deltaTime);
		}
		// 점프키를 누르고 있는 동안
		else if (Input::Get().GetKey(VK_SPACE))
		{
			JumpKey(deltaTime);
		}
		// 점프키를 뗀 경우 점프!
		else if (Input::Get().GetKeyUp(VK_SPACE))
		{
			Jump();
			Jumping(deltaTime);
		}
	}
	// 점프 중이라면 해당 방향으로 힘을 가지고 이동 중
	else
	{
		Jumping(deltaTime);
	}

	// 상태 변화에 따른 문자열, 색상 변경
	ChangeImageAndColor();
}

void Player::MoveLeft(float deltaTime)
{
	// 이동속도에 따른 왼쪽 이동 처리
	x -= moveSpeed * deltaTime;

	// 좌표 검사
	if (x < 0.0f)
	{
		x = 0.0f;
	}

	// position 벡터에 갱신
	position.x = static_cast<int>(x);

	// 상태 업데이트
	isLeft = true;
	state = PlayerState::IdleL;
}

void Player::MoveRight(float deltaTime)
{
	// 이동속도에 따른 오른쪽 이동 처리
	x += moveSpeed * deltaTime;

	// 좌표 검사
	if (static_cast<int>(x) + width > Engine::Get().GetWidth())
	{
		x = static_cast<float>(Engine::Get().GetWidth() - 1);
	}

	// position 벡터에 갱신
	position.x = static_cast<int>(x);

	// 상태 업데이트
	isLeft = false;
	state = PlayerState::IdleR;
}

void Player::JumpKeyDown(float deltaTime)
{
	isJumpKeyDown = true;

	// 점프 충전 상태로 변경
	state = PlayerState::Charging;
}

void Player::JumpKey(float deltaTime)
{
	// 점프 충전 시간 경과
	jumpTimer.Tick(deltaTime);

	// 1.2초 이상 누른 경우 강제 점프
	if (jumpTimer.IsTimeOut())
	{
		Jump();
		Jumping(deltaTime);
	}
}

void Player::Jump()
{
	const int jumpPower = GetJumpPower(jumpTimer.GetElapsedTime());
	jumpTimer.Reset();
	isJumpKeyDown = false;

	// 현재 방향으로 특정 점프력으로 점프를 하기위해
	// x와 y 방향으로 가해야하는 가속도 계산

	// 플레이어가 점프키를 입력한 시간만큼 점프력이 결정되며,
	// 결정된 점프력을 Z라고 하고, 원점에서 점프를 한다고 가정할때,
	// 플레이어는 (0,0) -> (Z,Z) -> (2*Z,0) 을 포물선을 그리며 2초 동안 이동한다
	// 이때 정점에 도달 했을때 y좌표의 가속도는 
	// 0 = y좌표의 초기 속도 - (중력 가속도) * (정점까지 걸리는 시간)
	// 그러므로 정점까지 걸리는 시간 = y좌표의 초기 속도 / 중력 가속도
	// 위치 함수에 정점 도달을 대입하면
	// 정점y = (y좌표의 초기 속도)^2 / (2 * 중력 가속도) 이므로
	// y좌표의 초기 속도 = (2 * 중력 가속도 * 정점y)의 제곱근 이다.
	// 콘솔에서 점프, 위로 올라가려면 y좌표가 낮아져야 하므로 뺀다.
	jumpVelocityY -= Util::Sqrt(2.0f * gravity * static_cast<float>(jumpPower));

	// x좌표는 중력의 영향을 안받는 등가속도 운동을 해야한다.
	// 위치식 : x(t) = x(0) + v*t 
	// 원점에서 정점에 도달하는 시간 = 정점에서 도착점에 도달하는 시간 이므로
	// 전체 점프 시간 = 2* 정점까지 걸리는 시간, 위에서 구한 정점까지 걸리는 시간을 대입하면
	// 전체 점프 시간 = (2 * y좌표의 초기 시간) / 중력 가속도 이다.
	// x좌표의 전체 이동거리는 속도 * 시간이므로
	// x좌표의 초기 속도 = x좌표의 전체 이동거리 / 전체 점프 시간 으로 위에서 구한 전체 점프 시간을 대입하면
	// x좌표의 초기 속도 = ((x좌표의 전체 이동거리) * (중력 가속도)) / (2 * y좌표의 초기 속도)
	float absVelocity = (2.0f * static_cast<float>(jumpPower) * gravity) / jumpVelocityY;

	// 방향에 따라 가속도 설정
	// Todo: 원인 찾기
	jumpVelocityX = isLeft ? absVelocity : -absVelocity;

	// 점프 중
	isJumping = true;

	// 점프 상태로 변경
	state = PlayerState::Upward;
}

void Player::Jumping(float deltaTime)
{
	// y좌표 속도는 중력의 영향을 받음
	jumpVelocityY += gravity * deltaTime;

	// 이동
	x += jumpVelocityX * deltaTime;
	y += jumpVelocityY * deltaTime;

	// 좌표 처리
	if (x < 0.0f)
	{
		x = 0.0f;
	}
	else if (static_cast<int>(x) + width > Engine::Get().GetWidth())
	{
		x = static_cast<float>(Engine::Get().GetWidth() - 1);
	}
	position.x = static_cast<int>(x);

	// Todo: 맵의 아래쪽으로 떨어지는 기믹이 필요할까?
	if (static_cast<int>(y) > Engine::Get().GetHeight())
	{
		y = static_cast<float>(Engine::Get().GetHeight() - 1);
	}
	// 버퍼의 맨 위에 도달했다면 아래로 추락
	else if (y < 0.0f)
	{
		jumpVelocityY = 0.0f;
		y = 0.0f;
	}
	position.y = static_cast<int>(y);

	// 가속도가 양수가 되면(정점을 지나면) 내려가는 상태로 변경
	if (jumpVelocityY >= 0.0f)
	{
		state = PlayerState::Downward;
	}
}

const int Player::GetJumpPower(const float chargedTime)
{
	if (chargedTime <= 0.2f)
	{
		return 1;
	}
	else if (chargedTime <= 0.4f)
	{
		return 2;
	}
	else if (chargedTime <= 0.6f)
	{
		return 3;
	}
	else if (chargedTime <= 0.8f)
	{
		return 5;
	}
	else if (chargedTime <= 1.0f)
	{
		return 7;
	}
	else
	{
		return 10;
	}
}

const Vector2 Player::TestIntersect(const Actor* const other)
{
	// AABB (Axis Aligned Bounding Box)
	// 물체의 특정 한 지점을 기준으로 끝단을 구함
	// 예 : 사각형의 좌상단, 우하단 / 우상단, 좌하단

	// 자기자신의 x좌표 정보
	float xMin = x;
	float xMax = x + width - 1;

	// 자기자신의 y좌표 정보
	float yMin = y;
	float yMax = y + 1;

	// 충돌을 비교할 다른 액터의 x좌표 정보
	float otherXMin = static_cast<float>(other->GetPosition().x);
	float otherXMax = static_cast<float>(other->GetPosition().x + other->GetWidth() - 1);

	// 충돌을 비교할 다른 액터의 y좌표 정보
	float otherYMin = static_cast<float>(other->GetPosition().y);
	float otherYMax = static_cast<float>(other->GetPosition().y + 1);


	// 안겹치는 조건 확인
	// 다른 액터의 왼쪽 좌표가 내 오른쪽 좌표보다 더 오른쪽에 있는 경우
	if (xMax <= otherXMin)
	{
		return Vector2::Zero;
	}

	// 다른 액터의 오른쪽 좌표가 내 왼쪽 좌표보다 더 왼쪽에 있는 경우
	if (xMin >= otherXMax)
	{
		return Vector2::Zero;
	}

	// 다른 액터의 위 좌표가 내 아래 좌표보다 더 아래에 있는 경우
	if (yMax <= otherYMin)
	{
		return Vector2::Zero;
	}

	// 다른 액터의 아래 좌표가 내 위 좌표보다 더 위에 있는 경우
	if (yMin >= otherYMax)
	{
		return Vector2::Zero;
	}

	// 겹친 거리 계산
	float overlapX = min(xMax, otherXMax) - max(xMin, otherXMin);
	float overlapY = min(yMax, otherYMax) - max(yMin, otherYMin);

	// 어느 방향 충돌인지 판정
	if (overlapX < overlapY)
	{
		// x축으로 더 적게 겹침 -> 좌/우 충돌
		// 다른 액터의 오른쪽 좌표가 내 왼쪽 좌표보다 큰 경우 
		if (xMin < otherXMax) 
		{
			// 플레이어 기준 왼쪽에서 충돌
			return Vector2::Left;
		}
		// 다른 액터의 왼쪽 좌표가 내 오른쪽 좌표보다 큰 경우 
		else 
		{
			// 플레이어 기준 오른쪽에서 충돌
			return Vector2::Right;
		}
	}
	else
	{
		// y축으로 더 적게 겹침 -> 상/하 충돌
		// 다른 액터의 아래 좌표가 내 위 좌표보다 큰 경우 
		if (yMin < otherYMax) 
		{
			// 플레이어 기준 위에서 충돌
			return Vector2::Up;
		}
		// 다른 액터의 위 좌표가 내 아래 좌표보다 큰 경우 
		else
		{
			// 플레이어 기준 아래에서 충돌
			return Vector2::Down;
		}
	}

	// 예외상황을 위한 리턴
	return Vector2::Zero;
}


void Player::OnGround(const Actor& other)
{
	// 레벨이 호출해줌

	isJumping = false;

	// 점프 가속도 초기화
	jumpVelocityX = 0.0f;
	jumpVelocityY = 0.0f;

	// Todo: x, y 좌표 보정
	x = static_cast<float>(position.x);
	y = static_cast<float>(position.y);

	// 방향에 맞춰 상태 변경
	state = isLeft ? PlayerState::IdleL : PlayerState::IdleR;
}

void Player::CrashedWithOther(const Vector2& crashedDirection, const Actor& other)
{
}

void Player::ChangeImageAndColor()
{
	ChangeImage(GetPlayerStringByState(state));
	color = GetPlayerColorByState(state);
}

const char* Player::GetPlayerStringByState(const PlayerState& state)
{
	switch (state)
	{
	case PlayerState::IdleL:
		return "d";

	case PlayerState::IdleR:
		return "b";

	case PlayerState::Charging:
		return "o";

	case PlayerState::Upward:
		return "U";

	case PlayerState::Downward:
		return "D";

	case PlayerState::Die:
		return "X";
	default:
		return " ";
	}
}

const Color Player::GetPlayerColorByState(const PlayerState& state)
{
	switch (state)
	{
	case PlayerState::IdleL:
	case PlayerState::IdleR:
		return Color::Green;

	case PlayerState::Charging:
		return Color::LightGreen;

	case PlayerState::Upward:
		return Color::Green;

	case PlayerState::Downward:
		return Color::Yellow;

	case PlayerState::Die:
		return Color::Red;

	default:
		return Color::White;
	}
}

