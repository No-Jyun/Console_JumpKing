#include "Goal.h"
#include "Render/Renderer.h"

// 목적지 타일의 이미지 배열
static const char* images[] =
{
	"|>",
	"|",
	"A"
};

// 목적지 타일의 색상 배열
static const Color colors[] =
{
	Color::LightRed,
	Color::LightYellow,
	Color::LightGreen,
	Color::LightSky,
	Color::LightBlue,
	Color::LightPurple,
};

Goal::Goal(const Vector2& position)
	: super(" ", position, Color::Green)
{
	sortingOrder = 2;

	// 타이머 설정
	timer.SetTargetTime(0.1f);
}

void Goal::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 타이머 경과
	timer.Tick(deltaTime);

	// 타이머 끝나면
	if (timer.IsTimeOut())
	{
		// 타이머 리셋
		timer.Reset();

		// 색상 배열의 크기
		int colorLength = static_cast<int>(sizeof(colors) / sizeof(colors[0]));

		// 현재 색상 인덱스 증가 및 모듈러 연산
		currentColorIndex = (currentColorIndex + 1) % colorLength;
	}
}

void Goal::Draw()
{
	super::Draw();

	// 이미지 배열의 크기
	int imageLength = static_cast<int>(sizeof(images) / sizeof(images[0]));

	// 배열 순회
	for (int i = 0;i < imageLength;i++)
	{
		// i번 이미지는 y좌표의 i 만큼 아래에 Draw
		Renderer::Get().Submit(images[i], Vector2(position.x, position.y + i), colors[currentColorIndex], sortingOrder);
	}
}
