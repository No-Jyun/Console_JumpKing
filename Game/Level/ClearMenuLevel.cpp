#include "ClearMenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"

#include <iostream>

// 게임 클리어 로고 파일 위치 문자열
const char* clearMenuLogoFileName = "../Assets/ClearMenu/ClearMenuLogo.txt";

// 게임 클리어 항목 파일 위치 문자열
const char* clearMenuItemFileName = "../Assets/ClearMenu/BackToMainMenu.txt";

// 게임 클리어 배너 파일 위치 문자열
const char* clearMenuBannerFileName = "../Assets/ClearMenu/GameClear.txt";

// 선택된 항목 색상 배열
static const Color itemColors[] =
{
	Color::Sky,
	Color::LightSky,
};

// 게임 클리어 배너의 색상 배열
static const Color bannerColors[] =
{	
	Color::LightRed,
	Color::Red,
	Color::LightYellow,
	Color::Yellow,
	Color::LightGreen,
	Color::Green,
	Color::LightSky,
	Color::Sky,
	Color::LightBlue,
	Color::Blue,
	Color::LightPurple,
	Color::Purple,
};

ClearMenuLevel::ClearMenuLevel()
{
	// 클리어 화면의 폰트 크기 조절
	fontSize = 15;

	// 파일로부터 게임 클리어 로고 문자열 로드
	LoadClearMenuLogo();

	// 파일로부터 항목의 문자열 로드
	LoadClearMenuItem();

	// 파일로부터 배너의 문자열 로드
	LoadClearMenuBanner();

	// 벡터 초기화
	SetVectorPosition();

	// 색상 타이머 세팅
	itemColorTimer.SetTargetTime(0.5f);

	// 배너 색상 타이머 세팅
	bannerColorTimer.SetTargetTime(0.1f);
}

ClearMenuLevel::~ClearMenuLevel()
{
	// 클리어 화면 로고 문자열 제거
	for (const char*& string : clearMenuLogo)
	{
		delete string;
		string = nullptr;
	}

	// 클리어 화면 항목 문자열 제거
	for (const char*& string : clearMenuItem)
	{
		delete string;
		string = nullptr;
	}

	// 클리어 화면 항목 문자열 제거
	for (const char*& string : clearMenuBanner)
	{
		delete string;
		string = nullptr;
	}

	// 배열 초기화
	clearMenuLogo.clear();
	clearMenuItem.clear();
	clearMenuBanner.clear();
}

void ClearMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 타이머 경과
	itemColorTimer.Tick(deltaTime);
	bannerColorTimer.Tick(deltaTime);

	// 항목 색상 타이머 끝나면
	if (itemColorTimer.IsTimeOut())
	{
		// 타이머 리셋
		itemColorTimer.Reset();

		// 색상 인덱스 돌리기
		currentItemColorIndex = 1 - currentItemColorIndex;
	}

	// 배너 색상 타이머 끝나면
	if (bannerColorTimer.IsTimeOut())
	{
		// 타이머 리셋
		bannerColorTimer.Reset();

		// 색상 인덱스 돌리기
		// 색상 배열의 크기
		int colorLength = static_cast<int>(sizeof(bannerColors) / sizeof(bannerColors[0]));

		// 현재 색상 인덱스 증가 및 모듈러 연산
		currentBannerColorIndex = (currentBannerColorIndex + 1) % colorLength;
	}

	// 입력 처리 (엔터키)
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 클리어 화면에서 메인 메뉴로 돌아가기
		Game::Get().ToggleMenu(LevelControl::MainMenuLevel);
	}
}

void ClearMenuLevel::Draw()
{
	super::Draw();

	// 배너 출력
	DrawBanner();

	// 로고 출력
	DrawLogo();

	// 항목 출력
	DrawItem();
}

void ClearMenuLevel::LoadClearMenuLogo()
{
	// 클리어 화면 로고 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, clearMenuLogoFileName, "rt");

	// 예외 처리
	if (!file)
	{
		std::cout << "Failed to open clear menu logo file\n";
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
		clearMenuLogo.emplace_back(text);
	}

	// 파일 닫기
	fclose(file);
}

void ClearMenuLevel::LoadClearMenuItem()
{
	// 클리어 메뉴 항목 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, clearMenuItemFileName, "rt");

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
		clearMenuItem.emplace_back(text);
	}

	// 파일 닫기
	fclose(file);
}

void ClearMenuLevel::LoadClearMenuBanner()
{
	// 클리어 메뉴 배너 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, clearMenuBannerFileName, "rt");

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
		clearMenuBanner.emplace_back(text);
	}

	// 파일 닫기
	fclose(file);
}

void ClearMenuLevel::SetVectorPosition()
{
	// 기본 값으로 초기화
	consoleCenter = Vector2::Zero;
	logoPosition = Vector2::Zero;
	itemPosition = Vector2::Zero;
	bannerPosition = Vector2::Zero;

	// 콘솔의 정중앙 설정
	consoleCenter.x = Engine::Get().GetWidth() / 2;
	consoleCenter.y = Engine::Get().GetHeight() / 2;

	// 배너 y위치 설정 (맨 위에 그리기)
	bannerPosition.y = 1;

	// 저장된 클리어 메뉴 배너 문자열에서 가장 긴 문자열 크기 저장
	size_t maxSize = 0;
	for (const char* text : clearMenuBanner)
	{
		size_t textLength = strlen(text);
		maxSize = maxSize > textLength ? maxSize : textLength;
	}

	// 배너 x위치 설정 (중앙 정렬)
	bannerPosition.x = consoleCenter.x - (static_cast<int>(maxSize) / 2);

	// 클리어 메뉴 로고는 배너 아래로 설정
	logoPosition.y = bannerPosition.y + static_cast<int>(clearMenuBanner.size()) + 2;

	// 저장된 클리어 메뉴 로고 문자열에서 가장 긴 문자열 크기 저장
	maxSize = 0;
	for (const char* text : clearMenuLogo)
	{
		size_t textLength = strlen(text);
		maxSize = maxSize > textLength ? maxSize : textLength;
	}

	// 그릴 문자열이 콘솔의 중앙에 오도록 정렬
	logoPosition.x = consoleCenter.x - (static_cast<int>(maxSize) / 2);

	// 클리어 메뉴 항목은 클리어 메뉴 로고의 아래에 오도록 y좌표 저장
	itemPosition.y = logoPosition.y + static_cast<int>(clearMenuLogo.size()) + 2;

	// 클리어 메뉴 항목 문자열에서 가장 긴 문자열 크기 저장
	maxSize = 0;
	for (const char* text : clearMenuItem)
	{
		size_t textLength = strlen(text);
		maxSize = maxSize > textLength ? maxSize : textLength;
	}

	// 클리어 메뉴 항목 문자열을 그릴 위치 저장
	itemPosition.x = consoleCenter.x - (static_cast<int>(maxSize) / 2);
}

void ClearMenuLevel::DrawLogo()
{
	// 배열을 순회하며 Draw
	for (int i = 0; i < static_cast<int>(clearMenuLogo.size()); i++)
	{
		// 로고를 그리기로 정한 위치에서
		Vector2 drawPosition = logoPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		Renderer::Get().Submit(clearMenuLogo[i], drawPosition);
	}
}

void ClearMenuLevel::DrawItem()
{
	// 현재 항목의 문자열 Draw
	for (int i = 0; i < static_cast<int>(clearMenuItem.size()); i++)
	{
		// 클리어 항목을 그리기로 정한 위치에서
		Vector2 drawPosition = itemPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		// 아이템 색상 설정
		Color textColor = itemColors[currentItemColorIndex];

		Renderer::Get().Submit(clearMenuItem[i], drawPosition, textColor);
	}
}

void ClearMenuLevel::DrawBanner()
{
	// 현재 항목의 문자열 Draw
	for (int i = 0; i < static_cast<int>(clearMenuBanner.size()); i++)
	{
		// 클리어 배너를 그리기로 정한 위치에서
		Vector2 drawPosition = bannerPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		// 아이템 색상 설정
		Color textColor = bannerColors[currentBannerColorIndex];

		Renderer::Get().Submit(clearMenuBanner[i], drawPosition, textColor);
	}
}
