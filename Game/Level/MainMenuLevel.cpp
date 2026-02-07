#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"

#include <iostream>

const char* mainMenuLogoFileName = "../Assets/MainMenu/MainMenuLogo.txt";

MainMenuLevel::MainMenuLevel()
{
	// 메뉴 아이템 생성
	mainMenuItems.emplace_back(new MainMenuItem(
		"../Assets/MainMenu/GameStart.txt",
		[]()
		{
			// 메뉴 토글 함수 호출
			Game::Get().ToggleMenu(LevelControl::GameMenu);
		}
	));

	mainMenuItems.emplace_back(new MainMenuItem(
		"../Assets/MainMenu/QuitGame.txt",
		[]()
		{
			// 게임 종료
			Game::Get().QuitEngine();
		}
	));

	// 메인 화면의 폰트 크기 조절
	fontSize = 15;

	// 파일로부터 메인 메뉴 문자열 로드
	LoadMainMenuLogo();

	// 파일로부터 각 항목의 문자열 로드
	LoadMainMenuItem();

	// 벡터 초기화
	SetVectorPosition();
}

MainMenuLevel::~MainMenuLevel()
{
	// 메뉴 아이템 제거
	for (MainMenuItem*& item : mainMenuItems)
	{
		delete item;
		item = nullptr;
	}

	// 메인 메뉴 문자열 제거
	for (const char*& string : mainMenuLogo)
	{
		delete[] string;
		string = nullptr;
	}

	// 배열 초기화
	mainMenuItems.clear();
	mainMenuLogo.clear();
}

void MainMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리 (방향키 위/아래키, 엔터키, ESC)
	// 배열 길이
	static int length = static_cast<int>(mainMenuItems.size());
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
		mainMenuItems[currentIndex]->onSelected();
	}
}

void MainMenuLevel::Draw()
{
	super::Draw();

	// 로고 출력
	DrawLogo();

	// 메뉴 아이템 출력
	DrawItem();
}

void MainMenuLevel::LoadMainMenuLogo()
{
	// 메인 메뉴 로고 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, mainMenuLogoFileName, "rt");

	// 예외 처리
	if (!file)
	{
		std::cout << "Failed to open main menu logo file\n";
		__debugbreak();
		return;
	}

	// 파일에서 읽은 데이터 담을 버퍼
	char buffer[2048] = {};

	while (true)
	{
		// 문자열 읽기
		if (fgets(buffer, 2048, file) == nullptr)
		{
			break;
		}

		// 개행문자를 \0으로 교체
		size_t length = strlen(buffer);
		if (length > 0 && buffer[length - 1] == '\n') 
		{
			buffer[length - 1] = '\0';
		}

		// 문자열 복사
		char* text = new char[length + 1];
		strcpy_s(text, length + 1, buffer);

		// 배열에 저장
		mainMenuLogo.emplace_back(text);
	}

	// 파일 닫기
	fclose(file);
}

void MainMenuLevel::LoadMainMenuItem()
{
	// 각 메인 메뉴 항목이 그릴 문자열 저장
	for (MainMenuItem* menuItem : mainMenuItems)
	{
		// 메인 메뉴 항목 파일 열기
		FILE* file = nullptr;
		fopen_s(&file, menuItem->filename, "rt");

		// 예외 처리
		if (!file)
		{
			std::cout << "Failed to open main menu item file\n";
			__debugbreak();
			return;
		}

		// 파일에서 읽은 데이터 담을 버퍼
		char buffer[2048] = {};

		while (true)
		{
			// 문자열 읽기
			if (fgets(buffer, 2048, file) == nullptr)
			{
				break;
			}

			// 개행문자를 \0으로 교체
			size_t length = strlen(buffer);
			if (length > 0 && buffer[length - 1] == '\n')
			{
				buffer[length - 1] = '\0';
			}

			// 문자열 복사
			char* text = new char[length + 1];
			strcpy_s(text, length + 1, buffer);

			// 배열에 저장
			menuItem->texts.emplace_back(text);
		}

		// 파일 닫기
		fclose(file);
	}
}

void MainMenuLevel::SetVectorPosition()
{
	// 기본 값으로 초기화
	consoleCenter = Vector2::Zero;
	logoPosition = Vector2::Zero;

	// 콘솔의 정중앙 설정
	consoleCenter.x = Engine::Get().GetWidth() / 2;
	consoleCenter.y = Engine::Get().GetHeight() / 2;

	// 메인 메뉴 로고 높이 설정
	logoArrayHeight = static_cast<int>(mainMenuLogo.size());

	// 저장된 메인 메뉴 로고 문자열에서 가장 긴 문자열 크기 저장
	size_t maxSize = 0;
	for (const char* text : mainMenuLogo)
	{
		size_t textLength = strlen(text);
		maxSize = maxSize > textLength ? maxSize : textLength;
	}

	// 메인 메뉴 로고 너비 설정
	logoArrayWidth = static_cast<int>(maxSize + 1);

	// 그릴 문자열이 콘솔의 중앙에 오도록 정렬
	logoPosition.x = consoleCenter.x - (logoArrayWidth / 2);

	// 메인 메뉴 로고는 위 가운데 정렬
	logoPosition.y = 0;

	// 가운데로 오도록 x 좌표 정렬
	for (MainMenuItem* mainMenuItem : mainMenuItems)
	{
		// 저장된 메인 메뉴 항목 문자열에서 가장 긴 문자열 크기 저장
		maxSize = 0;
		for (const char* text : mainMenuItem->texts)
		{
			size_t textLength = strlen(text);
			maxSize = maxSize > textLength ? maxSize : textLength;
		}

		// 메인 메뉴 항목 문자열을 그릴 위치 저장
		mainMenuItem->position.x = consoleCenter.x - (static_cast<int>(maxSize) / 2);
	}

	// 1번 항목은 메인 메뉴 로고의 아래에 오도록 y좌표 저장
	mainMenuItems[0]->position.y = logoPosition.y + logoArrayHeight + 2;

	// 2번 항목은 1번 항목 아래에 오도록 y좌표 저장
	mainMenuItems[1]->position.y = mainMenuItems[0]->position.y + static_cast<int>(mainMenuItems[0]->texts.size()) + 2;
}

void MainMenuLevel::DrawLogo()
{
	// 배열을 순회하며 Draw
	for (int i = 0; i < static_cast<int>(mainMenuLogo.size()); i++)
	{
		// 로고를 그리기로 정한 위치에서
		Vector2 drawPosition = logoPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		Renderer::Get().Submit(mainMenuLogo[i], drawPosition);
	}
}

void MainMenuLevel::DrawItem()
{
	// 배열을 순회하며 각 항목을 Draw
	for (int i = 0; i < static_cast<int>(mainMenuItems.size()); i++)
	{
		MainMenuItem* item = mainMenuItems[i];

		// 현재 항목의 문자열 Draw
		for (int j = 0; j < static_cast<int>(item->texts.size()); j++)
		{
			// 메뉴 아이템을 그리기로 정한 위치에서
			Vector2 drawPosition = item->position;

			// y값을 증가시켜 아래로 한칸씩 그리기
			drawPosition.y += j;

			// 아이템 색상 확인 (선택됐는지 여부)
			Color textColor =
				(i == currentIndex) ? selectedColor : unselectedColor;

			Renderer::Get().Submit(item->texts[j], drawPosition, textColor);
		}
	}
}
