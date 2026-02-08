#pragma once

#include "Level/Level.h"
#include "Util/Timer.h"

#include <vector>

using namespace Wanted;

// 게임 클리어 화면을 보여주는 레벨 클래스
class ClearMenuLevel : public Level
{
	// RTTI 선언
	 RTTI_DECLARATIONS(ClearMenuLevel, Level)

public:
	ClearMenuLevel();
	~ClearMenuLevel();

private:
	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	// 게임 클리어 로고를 파일에서 읽고 저장하는 함수
	void LoadClearMenuLogo();

	// 게임 클리어 항목을 파일에서 읽고 저장하는 함수
	void LoadClearMenuItem();

	// 벡터 멤버 변수 초기화하는 함수
	void SetVectorPosition();

	// 게임 클리어 로고 그리는 함수
	void DrawLogo();

	// 게임 클리어 항목 그리는 함수
	void DrawItem();

private:
	// 파일로부터 읽은 게임 클리어 문자열을 저장할 배열
	std::vector<const char*> clearMenuLogo;

	// 파일로부터 읽은 게임 클리어 항목 문자열을 저장할 배열
	std::vector<const char*> clearMenuItem;

	// 클리어 화면 로고 크기
	int logoArrayWidth = 0;
	int logoArrayHeight = 0;

	// 콘솔 화면의 정중앙 값을 저장할 좌표
	Vector2 consoleCenter;

	// 클리어 화면 로고 그리는 좌표
	Vector2 logoPosition;

	// 클리어 화면 항목 그리는 좌표
	Vector2 itemPosition;

	// 색상 배열에서 현재 색상의 인덱스
	int currentColorIndex = 0;

	// 게임 클리어 항목 색상 전환 타이머
	Timer timer;
};

