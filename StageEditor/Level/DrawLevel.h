#pragma once
#include <vector>
#include <string>
#include "../Render/Renderer.h"

// 전방 선언
class MouseControl;

class DrawLevel
{
	friend MouseControl;

public:
	DrawLevel(const Vector2& screenSize);
	virtual ~DrawLevel();

	// 게임 플레이 이벤트
	void BeginPlay();
	void Tick(float deltaTime);
	void Draw();

private:
	void SetLevelBase();

	void DrawInstruction();

	void DrawStageLevel();

private:
	MouseControl* mouseActor;

	Vector2 screenSize;

	std::vector<std::string> stage;
};
