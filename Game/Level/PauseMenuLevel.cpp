#include "PauseMenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Render/Renderer.h"

#include <iostream>

// 메뉴 로고 문자열
static const char* menuLogo[] =
{
	" /$$$$$$$                                              ",
	"| $$__  $$                                             ",
	"| $$  \\ $$ /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$       ",
	"| $$$$$$$/|____  $$| $$  | $$ /$$_____/ /$$__  $$      ",
	"| $$____/  /$$$$$$$| $$  | $$|  $$$$$$ | $$$$$$$$      ",
	"| $$      /$$__  $$| $$  | $$ \\____  $$| $$_____/      ",
	"| $$     |  $$$$$$$|  $$$$$$/ /$$$$$$$/|  $$$$$$$      ",
	"|__/      \\_______/ \\______/ |_______/  \\_______/      "
};

PauseMenuLevel::PauseMenuLevel()
{
	// 메뉴 아이템 생성
	items.emplace_back(new MenuItem(
		"Resume Game",
		[]() 
		{
			// 메뉴 토글 함수 호출
			Game::Get().ToggleMenu();
		}
	));

	items.emplace_back(new MenuItem(
		" Quit Game ",
		[]()
		{
			// 게임 종료
			Game::Get().QuitEngine();
		}
	));

	// 배열 크기 초기화
	size_t arrayLength = sizeof(menuLogo) / sizeof(menuLogo[0]);
	logoArrayLength = static_cast<int>(arrayLength);

	// 벡터 초기화
	SetVectorPosition();
}

PauseMenuLevel::~PauseMenuLevel()
{
	// 메뉴 아이템 제거
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화
	items.clear();
}

void PauseMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	
	// 입력 처리 (방향키 위/아래키, 엔터키, ESC)
	// 배열 길이
	static int length = static_cast<int>(items.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 돌리기 (위 방향으로 - 빼기)
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 돌리기 (아래 방향으로 - 더하기)
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장한 함수 포인터 호출
		items[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글
		Game::Get().ToggleMenu();

		// 인덱스 초기화
		currentIndex = 0;
	}
}

void PauseMenuLevel::Draw()
{
	// 로고 출력
	DrawLogo();

	// 메뉴 아이템 출력
	DrawItem();
}

void PauseMenuLevel::SetVectorPosition()
{
	// 기본 값으로 초기화
	consoleCenter = Vector2::Zero;
	logoPosition = Vector2::Zero;
	itemPosition = Vector2::Zero;

	// 콘솔의 정중앙 설정
	consoleCenter.x = Engine::Get().GetWidth() / 2;
	consoleCenter.y = Engine::Get().GetHeight() / 2;

	// 로고 문자열 하나의 길이
	int logoStringLength = static_cast<int>(strlen(menuLogo[0]));

	// 그릴 문자열이 콘솔의 중앙에 오도록 정렬
	logoPosition.x = consoleCenter.x - (logoStringLength / 2);

	// 로고가 크므로 중앙의 4칸 위로 설정
	logoPosition.y = consoleCenter.y - 4;

	// 로고의 중앙 위치 계산
	int logoCenter = logoPosition.x + (logoStringLength / 2);

	// 메뉴 아이템 문자열 하나의 길이
	int itemLength = static_cast<int>(strlen(items[0]->text));

	// 메뉴 아이템 문자열이 로고와 같이 중앙에 오도록 정렬
	itemPosition.x = logoCenter - (itemLength / 2);

	// 로고의 2칸 아래에 그리도록 설정
	itemPosition.y = logoPosition.y + logoArrayLength + 2;
}

void PauseMenuLevel::DrawLogo()
{
	// 배열을 순회하며 Draw
	for (int i = 0; i < logoArrayLength; i++)
	{
		// 로고를 그리기로 정한 위치에서
		Vector2 drawPosition = logoPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		Renderer::Get().Submit(menuLogo[i], drawPosition);
	}
}

void PauseMenuLevel::DrawItem()
{
	for (int ix = 0; ix < static_cast<int>(items.size()); ix++)
	{
		// 메뉴 아이템을 그리기로 정한 위치에서
		Vector2 drawPosition = itemPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += ix;

		// 아이템 색상 확인 (선택됐는지 여부)
		Color textColor =
			(ix == currentIndex) ? selectedColor : unselectedColor;

		Renderer::Get().Submit(items[ix]->text, drawPosition, textColor);
	}
}
