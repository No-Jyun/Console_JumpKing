#pragma once

#include "Vector2.h"
#include "Color.h"

#include <time.h>
#include <cmath>

// 헬퍼(Helper) 기능 제공.
namespace Util
{
	// 콘솔 커서 위치 이동(설정)하는 함수.
	inline void SetConsolePosition(const Vector2& position)
	{
		SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<COORD>(position)
		);
	}

	// 콘솔 텍스트 색상 설정 함수.
	inline void SetConsoleTextColor(Color color)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<unsigned short>(color)
		);
	}

	// 커서 끄기.
	inline void TurnOffCursor(HANDLE handle)
	{
		// 커서 끄기.
		CONSOLE_CURSOR_INFO info = {};
		info.bVisible = false;
		SetConsoleCursorInfo(handle, &info);
	}

	// 커서 켜기.
	inline void TurnOnCursor()
	{
		// 커서 켜기.
		CONSOLE_CURSOR_INFO info = {};
		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = true;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}

	// 어떤 값을 두 수 사이로 고정할 때 사용하는 함수.
	template<typename T>
	T Clamp(T value, T min, T max)
	{
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}

		return value;
	}
}

// 메모리 정리 함수.
template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

template<typename T>
void SafeDeleteArray(T*& t)
{
	if (t)
	{
		delete[] t;
		t = nullptr;
	}
}