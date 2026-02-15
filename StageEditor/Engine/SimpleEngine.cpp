#include "SimpleEngine.h"
#include "../Level/DrawLevel.h"
#include "../Core/Input.h"
#include "../Util/Util.h"
#include "../Render/Renderer.h"

#include <iostream>
#include <Windows.h>


// 전역 변수 초기화
SimpleEngine* SimpleEngine::instance = nullptr;

SimpleEngine::SimpleEngine(const Vector2& screenSize, int stageNum)
	: stageNum(stageNum)
{
	// 전역 변수 값 초기화
	instance = this;

	maxScreenSize = Vector2(51, 51);

	// 입력 관리자 생성
	input = new Input(screenSize);

	// 엔진 설정
	setting.framerate = 120.0f;
	setting.width = screenSize.x;
	setting.height = screenSize.y;

	// 렌더러 객체 생성
	renderer = new Renderer(Vector2(maxScreenSize.x + 50, maxScreenSize.y + 3));

	SetNewLevel(new DrawLevel(screenSize));
}

SimpleEngine::~SimpleEngine()
{
	// 메인 레벨 제거
	if (mainLevel)
	{
		delete mainLevel;
		mainLevel = nullptr;
	}

	// 입력 관리자 제거
	if (input)
	{
		delete input;
		input = nullptr;
	}

	// 렌더러 객체 제거
	SafeDelete(renderer);
}

void SimpleEngine::Run()
{
	// 시계의 정밀도
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 컴파일러 제공
	//__int64 curentTime = 0;
	// 프레임 계산용 변수
	// 타입 재정의 (stl에서)
	int64_t currentTime = 0;
	int64_t previousTime = 0;

	// 기준 프레임 (단위 : 초)
	//float targetFrameRate = 120.0f;
	setting.framerate = setting.framerate == 0 ? 60.0f : setting.framerate;
	float oneFrameTime = 1.0f / setting.framerate;

	// 하드웨어 타이머로 시간 구하기
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	// 엔진 시작 직전에는 두 시간 값을 같게 맞춤
	currentTime = time.QuadPart;
	previousTime = currentTime;

	// 엔진 루프(게임 루프)
	while (!isQuit)
	{
		// 현재 시간 구하기
		QueryPerformanceCounter(&time);
		currentTime = time.QuadPart;

		// 프레임 시간 계산
		float deltaTime
			= static_cast<float>(currentTime - previousTime);

		// 초단위 변환
		deltaTime = deltaTime
			/ static_cast<float>(frequency.QuadPart);

		// 고정 프레임 기법
		if (deltaTime >= oneFrameTime)
		{
			input->ProcessInput();

			// 프레임 처리
			BeginPlay();
			Tick(deltaTime);
			Draw();

			// 이전 시간 값 갱신
			previousTime = currentTime;

			input->SavePreviousInputStates();
		}
	}

	// 정리
	Shutdown();
}

void SimpleEngine::QuitEngine()
{
	isQuit = true;
}

void SimpleEngine::SetNewLevel(DrawLevel* newLevel)
{
	// 기존 레벨 있는지 확인
	// 있으면 기존 레벨 제거
	// Todo : 임시 코드. 레벨 전환할 때는 바로 제거하면 안됨
	if (mainLevel)
	{
		delete mainLevel;
		mainLevel = nullptr;
	}

	// 레벨 설정
	mainLevel = newLevel;
}

SimpleEngine& SimpleEngine::Get()
{
	// 예외처리
	if (!instance)
	{
		// Silent is violent
		std::cout << "Error : Engine::Get(). instance is null\n";
		__debugbreak();
	}

	return *instance;
}

void SimpleEngine::Shutdown()
{
	// 정리 작업
	std::cout << "Engine has been shutdown...\n";
}

void SimpleEngine::BeginPlay()
{
	// 레벨이 있으면 이벤트 전달
	if (!mainLevel)
	{
		std::cout << "Error : Engine::BeginPlay(). mainLevel is empty \n";
		return;
	}

	mainLevel->BeginPlay();
}

void SimpleEngine::Tick(float deltaTime)
{
	//std::cout 
	//	<< "DeltaTime : " << deltaTime
	//	<< ", FPS : " << (1.0f / deltaTime) << "\n";

	// 레벨에 이벤트 흘리기
	// 예외처리
	if (!mainLevel)
	{
		std::cout << "Error : Engine::Tick(). mainLevel is empty\n";
		return;
	}

	mainLevel->Tick(deltaTime);
}

void SimpleEngine::Draw()
{
	// 레벨에 이벤트 흘리기
	// 예외처리
	if (!mainLevel)
	{
		std::cout << "Error : Engine::Draw(). mainLevel is empty\n";
		return;
	}

	// 레벨의 모든 액터가 렌더 데이터를 제출
	mainLevel->Draw();

	// 렌더러에 그리기 명령 전달
	renderer->Draw();
}
