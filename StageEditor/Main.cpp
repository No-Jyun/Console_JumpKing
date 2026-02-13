#include <iostream>
#include <Windows.h>

#include "Render/Renderer.h"
#include "Util/Vector2.h"
#include "Core/Input.h"

int main()
{
	std::cout << "제작할 스테이지의 너비와 높이를 입력하세요\n";
	
	int width, height;
	std::cout << "높이를 입력하세요 (최대 높이: 50) : ";
	std::cin >> height;

	std::cout << "너비를 입력하세요 (최대 너비: 50) : ";
	std::cin >> width;

	std::cout << height << ' ' << width;

	Renderer* renderer = new Renderer(Vector2(51, 51));
	Input* input = new Input(Vector2(51, 51));

	Renderer::Get().Submit("Hello", Vector2(10, 10));
	Renderer::Get().Draw();
	////////////////////////////////////////////////////////////////// 여기부터
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

			// 레벨에 요청된 추가/제거 처리
			if (mainLevel)
			{
				mainLevel->ProcessAddAndDestroyActor();
			}
		}
	}

	// 정리
	Shutdown();
}

	delete renderer;
	renderer = nullptr;

	delete input;
	input = nullptr;
}