#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Util/Util.h"
#include "Level/Level.h"
#include "Actor/PlayerDeadEffect.h"
#include "Game/Game.h"

Player::Player(const Vector2& position)
	: super("p", position, Color::Green), state(PlayerState::IdleR)
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

	//// Todo: debug
	//if (Input::Get().GetKeyDown('R'))
	//{
	//	__debugbreak(); 
	//	return;
	//}

	// 점프 중이 아니고 바닥에 있고 빙판길이 아니라면 입력
	if (!isJumping && isLanding && !isOnIce)
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

		// 점프 입력 처리
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

	// 점프 중이 아닌데 바닥에 있지 않다면 공중에 있다는 뜻이므로 추락 처리
	else if (!isJumping && !isLanding)
	{
		// 추락 처리
		Fall(deltaTime);
	}

	// 점프 중이라면 해당 방향으로 힘을 가지고 이동 중
	else if(isJumping && !isLanding)
	{
		Jumping(deltaTime);
	}

	// 빙판길 위라면 해당 방향으로 자동 이동
	else if (isOnIce)
	{
		MoveOnIce(deltaTime);
	}

	// 상태 변화에 따른 문자열, 색상 변경
	ChangeImageAndColor();
}

void Player::UpdateIsLanding(bool isLanding)
{
	this->isLanding = isLanding;

	// 점프 중이였고, 착륙했거나, 추락 중에 바닥에 닿았다면
	if ((isJumping && isLanding) || (isFalling && isLanding))
	{
		// x, y 좌표 보정
		x = static_cast<float>(position.x);
		y = static_cast<float>(position.y);

		// 착륙한 경우
		isJumping = false;
		isFalling = false;

		// 속도 초기화
		velocityX = 0.0f;
		velocityY = 0.0f;

		// 방향에 맞춰 상태 변경
		state = isLeft ? PlayerState::IdleL : PlayerState::IdleR;
	}
	// 바닥 판정이후, 점프하지 않았는데 바닥에 없다면
	// 추락 해야하므로 추락 기본값 세팅
	else if (!isJumping && !isLanding && !isFalling)
	{
		// 상태 변수 설정
		isFalling = true;
		isOnIce = false;

		// 방향에 따라 x속도 설정
		velocityX = isLeft ? -5.0f : 5.0f;

		// y속도 초기화
		velocityY = 0.0f;

		// x, y 좌표 보정
		x = static_cast<float>(position.x);
		y = static_cast<float>(position.y);

		// 상태 변경
		state = PlayerState::Downward;
	}
}

void Player::Fall(float deltaTime)
{
	// y좌표 속도는 중력의 영향을 받음
	velocityY += gravity * deltaTime;

	// 이동
	x += velocityX * deltaTime;
	y += velocityY * deltaTime;

	// 좌표 처리
	x = CheckXPosition(x);

	// position 벡터에 갱신
	position.x = Util::FloatCastInt(x);

	// 추락은 떨어지기만 하므로 아래 체크만 하면 됨 
	if (Util::FloatCastInt(y) > GetOwner()->GetLevelRightDownPosition().y)
	{
		y = static_cast<float>(GetOwner()->GetLevelRightDownPosition().y - 1);
	}
	
	// position 벡터에 갱신
	position.y = Util::FloatCastInt(y);

	// 상태 변경
	state = PlayerState::Downward;
}

void Player::MoveLeft(float deltaTime)
{
	// 점프키 입력 중에는 좌우 방향 지정만 가능
	if (isJumpKeyDown)
	{
		// 방향 상태 변경
		isLeft = true;

		// 플레이어가 왼쪽 보도록 상태 변경
		state = PlayerState::ChargingL;
		return;
	}

	// 이동속도에 따른 왼쪽 이동 처리
	x -= moveSpeed * deltaTime;

	// 좌표 검사
	x = CheckXPosition(x);

	// position 벡터에 갱신
	position.x = Util::FloatCastInt(x);

	// 상태 업데이트
	isLeft = true;
	state = PlayerState::IdleL;
}

void Player::MoveRight(float deltaTime)
{
	// 점프키 입력 중에는 좌우 방향 지정만 가능
	if (isJumpKeyDown)
	{
		// 방향 상태 변경
		isLeft = false;

		// 플레이어가 오른쪽 보도록 상태 변경
		state = PlayerState::ChargingR;
		return;
	}

	// 이동속도에 따른 오른쪽 이동 처리
	x += moveSpeed * deltaTime;

	// 좌표 검사
	x = CheckXPosition(x);

	// position 벡터에 갱신
	position.x = Util::FloatCastInt(x);

	// 상태 업데이트
	isLeft = false;
	state = PlayerState::IdleR;
}

void Player::MoveOnIce(float deltaTime)
{
	// 이동거리 계산
	float moveDistance = moveSpeed * deltaTime;

	// 방향에 따라 이동
	x = isLeft ? x - moveDistance : x + moveDistance;

	// 좌표 검사
	x = CheckXPosition(x);
	
	// position 벡터에 갱신
	position.x = Util::FloatCastInt(x);
}

const float Player::CheckXPosition(float nowX)
{
	// 로드된 스테이지의 꼭짓점 가져오기
	Vector2 leftUp = GetOwner()->GetLevelLeftUpPosition();
	Vector2 rightDown = GetOwner()->GetLevelRightDownPosition();

	// 좌표 검사
	if (nowX <= leftUp.x)
	{
		nowX = leftUp.x + 1;
	}
	else if (Util::FloatCastInt(nowX) + width > rightDown.x)
	{
		nowX = rightDown.x - 1;
	}

	return nowX;
}

void Player::JumpKey(float deltaTime)
{
	// 상태 설정
	isJumpKeyDown = true;

	// 점프 충전 상태로 변경
	state = isLeft ? PlayerState::ChargingL : PlayerState::ChargingR;

	// 점프 충전 시간 경과
	jumpTimer.Tick(deltaTime);

	// 1.2초 이상 누른 경우 강제 점프
	if (jumpTimer.IsTimeOut())
	{
		// 점프관련 변수 세팅
		Jump();

		// 점프 실행
		Jumping(deltaTime);
	}
}

void Player::Jump()
{
	// 누른 시간에 비례해서 점프력 가져오기
	const int jumpPower = GetJumpPower(jumpTimer.GetElapsedTime());

	// 타이머 리셋
	jumpTimer.Reset();

	// 점프 누르는 상태 초기화
	isJumpKeyDown = false;

	// 현재 방향으로 특정 점프력으로 점프를 하기위해
	// x와 y 방향으로 가해야하는 가속도 계산
	// 중력 가속도와 점프력(정점 높이)를 알고 있다.

	// 플레이어가 점프키를 입력한 시간만큼 점프력이 결정되며,
	// 결정된 점프력을 Z라고 하고, 원점에서 점프를 한다고 가정할때,
	// 플레이어는 (0,0) -> (Z*0.75,Z) -> (Z*1.5,0) 을 포물선을 그리며 이동한다

	// 이때 정점에 도달 했을때 y좌표의 가속도는 
	// 0 = (y좌표의 초기 속도) - (중력 가속도) * (정점까지 걸리는 시간)
	// 그러므로 정점까지 걸리는 시간 = y좌표의 초기 속도 / 중력 가속도
	// 위치 함수에 정점 도달을 대입하면
	// 정점y = (y좌표의 초기 속도)^2 / (2 * 중력 가속도) 이므로
	// y좌표의 초기 속도 = (2 * 중력 가속도 * 정점y)의 제곱근 이다.
	// 콘솔에서 점프, 위로 올라가려면 y좌표가 낮아져야 하므로 뺀다.
	velocityY = -(Util::Sqrt(2.0f * gravity * static_cast<float>(jumpPower)));

	// x좌표는 중력의 영향을 안받는 등가속도 운동을 해야한다.
	// 위치식 : x(t) = x(0) + v*t 
	// 원점에서 정점에 도달하는 시간 = y좌표의 초기 속도 / 중력 가속도 이므로
	float peakT = abs(velocityY) / gravity;

	// 전체 점프 시간 = 2 * 정점까지 걸리는 시간
	float totalT = 2.0f * peakT;

	// x좌표의 전체 이동 거리 = x좌표의 초기 속도 * 전체 점프 시간
	// x좌표의 초기 속도 = x좌표의 전체 이동 거리 / 전체 점프 시간
	velocityX = (1.5f * static_cast<float>(jumpPower)) / totalT;

	// 방향에 따라 가속도 설정
	velocityX = isLeft ? -velocityX : velocityX;

	// 점프 상태로 변경
	isJumping = true;
	state = PlayerState::Upward;
}

void Player::Jumping(float deltaTime)
{
	// y좌표 속도는 중력의 영향을 받음
	velocityY += gravity * deltaTime;

	// 이동
	x += velocityX * deltaTime;
	y += velocityY * deltaTime;

	// 좌표 처리
	x = CheckXPosition(x);

	// position 벡터에 갱신
	position.x = Util::FloatCastInt(x);

	// 좌표 검사
	if (Util::FloatCastInt(y) > GetOwner()->GetLevelRightDownPosition().y)
	{
		y = static_cast<float>(GetOwner()->GetLevelRightDownPosition().y - 1);
	}

	// 버퍼의 맨 위에 도달했다면 아래로 추락
	else if (y < 0.0f)
	{
		velocityY = 0.0f;
		y = 0.0f;
	}

	// position 벡터에 갱신
	position.y = Util::FloatCastInt(y);

	// 속도가 양수가 되면(정점을 지나면) 내려가는 상태로 변경
	if (velocityY >= 0.0f)
	{
		state = PlayerState::Downward;
	}

	//  속도가 음수면 올라가는 상태
	else
	{
		state = PlayerState::Upward;
	}
}

const int Player::GetJumpPower(const float chargedTime)
{
	// 점프력 조정 가능
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
		return 4;
	}
	else if (chargedTime <= 0.8f)
	{
		return 5;
	}
	else if (chargedTime <= 1.0f)
	{
		return 6;
	}
	else
	{
		return 8;
	}
}

const Vector2 Player::TestIntersect(Actor& const other)
{
	// AABB (Axis Aligned Bounding Box)
	// 물체의 특정 한 지점을 기준으로 끝단을 구함
	// 예 : 사각형의 좌상단, 우하단 / 우상단, 좌하단

	// 현재 액터들은 한 문자 크기 이므로
	// 크기가 1인 정사각형으로 간주한다.
	// 따라서 각 꼭지점의 좌표에 해당하는 x,y값을 구하여 비교한다

	// 자기자신의 x좌표 정보
	float xMin = x;
	float xMax = x + 1.0f;

	// 자기자신의 y좌표 정보
	float yMin = y;
	float yMax = y + 1.0f;

	// 충돌을 비교할 다른 액터의 x좌표 정보
	float otherXMin = static_cast<float>(other.GetPosition().x);
	float otherXMax = static_cast<float>(other.GetPosition().x + other.GetWidth());

	// 충돌을 비교할 다른 액터의 y좌표 정보
	float otherYMin = static_cast<float>(other.GetPosition().y);
	float otherYMax = static_cast<float>(other.GetPosition().y + 1);

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
	if (yMax < otherYMin)
	{
		return Vector2::Zero;
	}

	// 다른 액터의 아래 좌표가 내 위 좌표보다 더 위에 있는 경우
	if (yMin > otherYMax)
	{
		return Vector2::Zero;
	}

	// 플레이어의 바닥이 상대 액터의 윗면과 맞닿아있는 경우
	if (yMax == otherYMin)
	{
		// 렌더링에 그려진 / 화면에 보이는 플레이어와 발판의 x좌표가 같을 때만 충돌 감지
		if (position.x == other.GetPosition().x)
		{
			return Vector2::Down;
		}
	}

	// 겹친 거리 계산
	float overlapX = min(xMax, otherXMax) - max(xMin, otherXMin);
	float overlapY = min(yMax, otherYMax) - max(yMin, otherYMin);

	// 어느 방향 충돌인지 판정
	if (overlapX < overlapY)
	{
		// x축으로 더 적게 겹침 -> 좌/우 충돌		
		// x의 속도가 양수라면 오른쪽 충돌
		if (velocityX > 0 || !isLeft)
		{
			return Vector2::Right;
		}

		// x의 속도가 음수라면 왼쪽 충돌
		else
		{
			return Vector2::Left;
		}
	}
	else
	{
		// y축으로 더 적게 겹침 -> 상/하 충돌
		// y의 속도가 양수 또는 0 이라면 하단 충돌
		if (velocityY >= 0)
		{
			// 렌더링에 그려진 / 화면에 보이는 플레이어와 발판의 x좌표가 같을 때만 충돌 감지
			if (position.x == other.GetPosition().x)
			{
				return Vector2::Down;
			}
		}

		// y의 속도가 음수라면 상단 충돌
		else
		{
			return Vector2::Up;
		}
	}

	// 예외상황을 위한 리턴
	return Vector2::Zero;
}

void Player::CrashedWithOther(const Vector2& crashedDirection, const Actor& other)
{
	// 위에 충돌한 경우
	if (crashedDirection == Vector2::Up)
	{
		// y 가속도 0으로 바로 추락
		velocityY = 0.0f;

		// y 좌표 보정
		y = static_cast<float>(position.y);
	}
	else if (crashedDirection == Vector2::Left)
	{
		// x 가속도 0
		velocityX = 0.0f;

		// x 좌표 보정
		x = static_cast<float>(position.x);
	}
	else if (crashedDirection == Vector2::Right)
	{
		// x 가속도 0
		velocityX = 0.0f;

		// x 좌표 보정
		x = static_cast<float>(position.x);
	}
}

void Player::Die()
{
	// 액터 제거
	Destroy();

	// 이펙트 생성 (재생을 위해)
	GetOwner()->AddNewActor(new PlayerDeadEffect(position));
}

void Player::ChangeImageAndColor()
{
	// 현 상태에 맞게 문자열(이미지) 변경
	ChangeImage(GetPlayerStringByState(state));

	// 현 상태에 맞게 문자열(이미지) 색상 변경
	color = GetPlayerColorByState(state);
}

const char* Player::GetPlayerStringByState(const PlayerState& state)
{
	// 현 상태에 맞는 문자열 (이미지) 반환
	switch (state)
	{
	case PlayerState::IdleL:
		return "q";

	case PlayerState::IdleR:
		return "p";

	case PlayerState::ChargingL:
		return "d";

	case PlayerState::ChargingR:
		return "b";

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
	// 현 상태에 맞는 문자열 (이미지) 색상 반환
	switch (state)
	{
	case PlayerState::IdleL:
	case PlayerState::IdleR:
		return Color::Green;

	case PlayerState::ChargingL:
	case PlayerState::ChargingR:
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

