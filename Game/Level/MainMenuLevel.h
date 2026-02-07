#pragma once

#include "Level/Level.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

#include <vector>

using namespace Wanted;

// 메뉴 아이템 구조체
struct MainMenuItem
{
	// 메뉴 선택됐을 때 실행될 함수의 타입
	using OnSelected = void(*)();

	MainMenuItem(const char* filename, OnSelected onSelected)
		: onSelected(onSelected), position(Vector2::Zero)
	{
		// 텍스트 복사
		size_t length = strlen(filename) + 1;
		this->filename = new char[length];
		strcpy_s(this->filename, length, filename);
	}

	~MainMenuItem()
	{
		if (filename)
		{
			delete[] filename;
			filename = nullptr;
		}

		// 메뉴 문자열 해제
		for (const char*& text : texts)
		{
			delete[] text;
			text = nullptr;
		}

		// 배열 초기화
		texts.clear();
	}

	// 메뉴 항목을 그릴 파일이름 텍스트
	char* filename = nullptr;
	
	// 메뉴 항목을 그릴 문자열 배열
	std::vector<const char*> texts;

	// 메뉴가 그려질 위치
	Vector2 position;

	// 메뉴 선택됐을 때 실행될 로직(함수 - 함수 포인터)
	OnSelected onSelected = nullptr;
		
};

// 메인 메뉴를 보여주는 레벨 클래스
class MainMenuLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(MainMenuLevel, Level)

public:
	MainMenuLevel();
	~MainMenuLevel();

private:
	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	// 메인 메뉴 로고를 파일에서 읽고 저장하는 함수
	void LoadMainMenuLogo();

	// 메인 메뉴 아이템을 파일에서 읽고 저장하는 함수
	void LoadMainMenuItem();

	// 벡터 멤버 변수 초기화하는 함수
	void SetVectorPosition();

	// 메인 메뉴 로고 그리는 함수
	void DrawLogo();

	// 메인 메뉴 아이템 그리는 함수
	void DrawItem();

private:
	// 파일로부터 읽은 메인메뉴 문자열을 저장할 배열
	std::vector<const char*> mainMenuLogo;

	// 현재 활성화된 메뉴 아이템 인덱스
	int currentIndex = 0;

	// 선택된 아이템의 색상
	Color selectedColor = Color::Green;

	// 선택되지 않은 아이템의 색상
	Color unselectedColor = Color::White;

	// 메뉴 아이템 배열
	std::vector<MainMenuItem*> mainMenuItems;

	// 메인 메뉴 로고 크기
	int logoArrayWidth = 0;
	int logoArrayHeight = 0;

	// 콘솔 화면의 정중앙 값을 저장할 좌표
	Vector2 consoleCenter;

	// 메뉴 로고 그리는 좌표
	Vector2 logoPosition;
};

