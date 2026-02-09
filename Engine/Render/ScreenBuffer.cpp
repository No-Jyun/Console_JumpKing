#include "ScreenBuffer.h"
#include <iostream>	
#include "Renderer.h"
#include "Windows.h"
#include <wchar.h>

namespace Wanted
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		// Console Output 생성
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		// 예외 처리
		if (buffer == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(
				nullptr,
				"ScreenBuffer - Failed to create buffer.",
				"Buffer creation error",
				MB_OK
			);
			__debugbreak();
		}

		// 윈도우 버퍼 최대 크기 측정
		//COORD test = GetLargestConsoleWindowSize(buffer);
		//std::cout << test.X << ' ' << test.Y << "\n";

		//ChangeFontSize(8);


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

		// 버퍼 크기 설정 (콘솔 전체 공간 크기)
		// Screen Buffer: 실제 텍스트가 저장되는 전체 2D 공간
		if (!SetConsoleScreenBufferSize(buffer, screenSize))
		{
			std::cerr << "Failed to set console buffer size\n";
			__debugbreak();
		}


		// 커서 끄기
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(buffer, &info);

		//끄도록 설정
		info.bVisible = false;
		SetConsoleCursorInfo(buffer, &info);

		//ChangeFontSize(2.0f);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		// 버퍼 해제
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}

	void ScreenBuffer::Clear()
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

	void ScreenBuffer::Draw(CHAR_INFO* charInfo)
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
	void ScreenBuffer::ChangeFontSize(const SHORT fontSize)
	{
		// 현재 폰트 정보 가져오기
		CONSOLE_FONT_INFOEX fontInfo;
		fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		GetCurrentConsoleFontEx(buffer, FALSE, &fontInfo);

		// 폰트 크기 변경
		fontInfo.dwFontSize.X = static_cast<SHORT>(fontSize);
		fontInfo.dwFontSize.Y = static_cast<SHORT>(fontSize);


		wcscpy_s(fontInfo.FaceName, L"SimSun-ExtB");

		// 적용
		SetCurrentConsoleFontEx(buffer, FALSE, &fontInfo);
	}
}