#include "Game.h"
#include "Level/PauseMenuLevel.h"
#include "Level/JumpLevel.h"
#include "Level/MainMenuLevel.h"
#include "Render/Renderer.h"

#include <iostream>

// 정적 변수 초기화
Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	// 세가지 레벨 생성 및 배열에 추가
	levels.emplace_back(new MainMenuLevel());
	levels.emplace_back(new JumpLevel(1));
	levels.emplace_back(new PauseMenuLevel());

	// 시작 상태(레벨) 설정
	state = LevelControl::MainMenu;

	// 게임 시작 시 활성화할 레벨 설정
	mainLevel = levels[0];
	mainLevel->SetLevelFontSize();
}

Game::~Game()
{
	// 중복 제거 방지
	mainLevel = nullptr;

	// 모든 레벨 삭제
	for (Level*& level : levels)
	{
		delete level;
		level = nullptr;
	}

	// 배열 정리
	levels.clear();
}

void Game::ToggleMenu(LevelControl levelControl)
{
	// 화면 지우기
	// system 은 콘솔 명령어 실행 함수. "cls" 명령어 실행
	// cls -> clear screen
	system("cls");

	// 전달받은 레벨로 상태 변경
	state = levelControl;

	// 메인 레벨 변경
	mainLevel = levels[static_cast<int>(state)];
	mainLevel->SetLevelFontSize();
}

Game& Game::Get()
{
	// 예외 처리
	if (!instance)
	{
		// 오류 메시지 출력.
		MessageBoxA(
			nullptr,
			"Game::Get() - instance is null",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 정적 변수 반환
	return *instance;
}