#pragma once

#include "Engine/Engine.h"
#include "Util/LevelControl.h"
#include <vector>

using namespace Wanted;

class Game : public Engine
{
public:
	Game();
	~Game();

	// 메뉴/게임 레벨을 전환하는 함수
	void ToggleMenu(LevelControl levelControl);

	// 새로운 게임을 시작하는 함수
	void StartNewGame();

	static Game& Get();

private:
	// 게임에서 관리하는 레벨을 저장할 배열
	std::vector<Level*> levels;
	
	// 현재 활성화된 레벨을 나타내는 상태 변수
	LevelControl state = LevelControl::None;

	// 싱글톤 구현을 위한 정적 변수
	static Game* instance;

};
