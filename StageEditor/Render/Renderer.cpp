#include "Renderer.h"
#include "../Util/Util.h"

#include <iostream>

Renderer::Frame::Frame(int bufferCount)
{
	// 배열 생성 및 초기화
	charInfoArray = new CHAR_INFO[bufferCount];
	memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);

	sortingOrderArray = new int[bufferCount];
	memset(sortingOrderArray, 0, sizeof(int) * bufferCount);
}

Renderer::Frame::~Frame()
{
	// Util 템플릿 활용
	SafeDeleteArray(charInfoArray);
	SafeDeleteArray(sortingOrderArray);
}

void Renderer::Frame::Clear(const Vector2& screenSize)
{
	// 2차원 배열로 다루는 1차원 배열을 순회하면서
	// 빈 문자(' ')를 설정
	const int width = screenSize.x;
	const int height = screenSize.y;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// 배열 인덱스 구하기
			// 2차원 -> 1차원
			const int index = (y * width) + x;

			// 글자 값 및 속성 설정
			CHAR_INFO& info = charInfoArray[index];
			info.Char.AsciiChar = ' ';
			info.Attributes = 0;

			// 그리기 우선순위 초기화
			sortingOrderArray[index] = -1;
		}
	}

}

//----------------Frame--------------//

// 정적 변수 초기화
Renderer* Renderer::instance = nullptr;

Renderer::Renderer(const Vector2& screenSize)
	: screenSize(screenSize)
{
	instance = this;

	// 프레임 객체 생성
	const int bufferCount = screenSize.x * screenSize.y;
	frame = new Frame(bufferCount);

	// 프레임 초기화
	frame->Clear(screenSize);

	// 버퍼 세팅
	SetBuffer();
}

Renderer::~Renderer()
{
	SafeDelete(frame);

	CloseHandle(buffer);
}

void Renderer::Draw()
{
	// 화면 지우기
	Clear();
	BufferClear();

	// 전제조건: 레벨의 모든 액터가 렌더러에 Submit을 완료
	// 렌더 큐 순회하면서 프레임(Frame) 채우기
	for (const RenderCommand& command : renderQueue)
	{
		// 화면에 그릴 텍스트가 없으면 건너뜀
		if (!command.text)
		{
			continue;
		}

		// 세로(y좌표) 기준 화면 벗어났는지 확인
		if (command.position.y < 0 || command.position.y >= screenSize.y)
		{
			continue;
		}

		// 화면에 그릴 문자열 길이
		const int length = static_cast<int>(strlen(command.text));

		// 안그려도 되면 건너뜀
		if (length <= 0)
		{
			continue;
		}

		// 가로(x좌표) 기준으로 화면에서 벗어났는지 확인
		const int startX = command.position.x;
		// "abcde" endX가 e를 가리키려면 1을 빼야함
		const int endX = command.position.x + length - 1;

		if (endX < 0 || startX >= screenSize.x)
		{
			continue;
		}

		// 시작 위치			
		const int visibleStart = startX < 0 ? 0 : startX;
		const int visibleEnd = endX >= screenSize.x ? screenSize.x - 1 : endX;

		// 문자열 설정
		for (int x = visibleStart; x <= visibleEnd; x++)
		{
			// 문자열 안의 문자 인덱스
			const int sourceIndex = x - startX;

			// 프레임 (2차원 문자 배열) 인덱스
			const int index = (command.position.y * screenSize.x) + x;

			// 그리기 우선순위 비교 (같다면 늦게 들어온 사람 출력)
			if (frame->sortingOrderArray[index] > command.sortingOrder)
			{
				continue;
			}

			// 데이터 기록
			frame->charInfoArray[index].Char.AsciiChar = command.text[sourceIndex];
			frame->charInfoArray[index].Attributes = (WORD)command.color;

			// 우선순위 업데이트
			frame->sortingOrderArray[index] = command.sortingOrder;
		}
	}

	// 그리기(ScreenBuffer)
	BufferDraw(frame->charInfoArray);

	// 렌더 큐 비우기
	renderQueue.clear();
}

void Renderer::BufferDraw(CHAR_INFO* charInfo)
{
	SMALL_RECT writeRegion = {};
	writeRegion.Left = 0;
	writeRegion.Top = 0;
	writeRegion.Right = static_cast<short>(screenSize.x - 1);
	writeRegion.Bottom = static_cast<short>(screenSize.y - 1);

	// 버퍼에 전달 받은 글자 배열 설정
	WriteConsoleOutputA(
		buffer,
		charInfo,
		screenSize,
		Vector2::Zero,
		&writeRegion
	);
}

Renderer& Renderer::Get()
{
	if (!instance)
	{
		MessageBoxA(
			nullptr,
			"Renderer::Get() - instance is null",
			"Error",
			MB_OK
		);
		__debugbreak();
	}

	return *instance;
}

void Renderer::Clear()
{
	// 화면 지우기
	// 1. 프레임(2차원 배열 데이터) 지우기
	frame->Clear(screenSize);

	// 2. 콘솔 버퍼 지우기
	BufferClear();
}

void Renderer::BufferClear()
{
	// 실제로 화면을 지우고 난 뒤에 
	// 몇 글자를 썼는지 반환 받는데 사용
	DWORD writtenCount = 0;

	// 콘솔 버퍼에 있는 화면 지우기
	// 그래픽스 -> 지우기 = 한 색상(또는 값)으로 덮어쓰기
	FillConsoleOutputCharacterA(
		buffer,
		' ',
		screenSize.x * screenSize.y,
		Vector2::Zero,
		&writtenCount
	);
}

void Renderer::Submit(const char* text, const Vector2& position, Color color, int sortingOrder)
{
	// 렌더 데이터 생성 후 큐에 추가
	RenderCommand command = {};
	command.text = text;
	command.position = position;
	command.color = color;
	command.sortingOrder = sortingOrder;

	renderQueue.emplace_back(command);
}

void Renderer::SetBuffer()
{
	buffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		CONSOLE_TEXTMODE_BUFFER,
		nullptr
	);

	// 버퍼 생성 후에는 크기 지정 (현재 화면에 보이는 창 크기)
	// Console Window: 그중 일부를 "카메라처럼" 보여주는 창
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = static_cast<short>(screenSize.x - 1);
	rect.Bottom = static_cast<short>(screenSize.y - 1);

	if (!SetConsoleWindowInfo(buffer, TRUE, &rect))
	{
		DWORD errorCode = GetLastError();
		std::cerr << errorCode << "\n" << "Failed to set console window info\n";
		__debugbreak();
	}

	if (!SetConsoleScreenBufferSize(buffer, screenSize))
	{
		std::cerr << "Failed to set console buffer size\n";
		__debugbreak();
	}

	SetConsoleActiveScreenBuffer(buffer);
}
