#pragma once

#include "../Util/Vector2.h"

// 전방 선언
// 전역 변수여도 엔진 초기화 시
class Input;
class Renderer;

// 간단한 게임 엔진 클래스
class SimpleEngine
{
	// 엔진 설정 구조체
	struct EngineSetting
	{
		// 프레임 속도 
		float framerate = 0.0f;

		// 화면 너비
		int width = 0;

		// 화면 높이
		int height = 0;
	};

public:
	SimpleEngine(const Vector2& screenSize, int stageNum);
	~SimpleEngine();

	// 엔진 루프(게임 루프)
	void Run();

	// 엔진 종료 함수
	void QuitEngine();

	// 새 레벨을 추가(설정)하는 함수
	void SetNewLevel(class DrawLevel* newLevel);

	// 전역 접근 함수
	static SimpleEngine& Get();

	// 화면 너비 반환 함수
	inline int GetWidth() const { return setting.width; }

	// 화면 높이 반환 함수
	inline int GetHeight() const { return setting.height; }

	inline int GetStageNum() const { return stageNum; }

private:

	// 정리 함수
	void Shutdown();

	// 게임 플레이 시작 함수
	void BeginPlay();

	// 업데이트 함수
	void Tick(float deltaTime);

	// 그리기 함수 (Draw / Render)
	void Draw();

private:
	// 엔진 종료 플래그
	bool isQuit = false;

	EngineSetting setting;

	// 입력 관리자
	Input* input = nullptr;

	// 렌더러 객체
	Renderer* renderer = nullptr;

	int stageNum;

	// 메인 레벨
	class DrawLevel* mainLevel = nullptr;

	// 전역 변수
	static SimpleEngine* instance;

	Vector2 maxScreenSize;
};
