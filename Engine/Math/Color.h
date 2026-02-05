#pragma once

#include "Common/Common.h"

#include <Windows.h>

namespace Wanted
{
	// 콘솔에 텍스트 색상 등을 지정할 때 사용할
	// 색상 열거형
	enum class WANTED_API Color : unsigned short
	{
		Black = 0,
		// 텍스트 색상임. 배경색상 또한 내부에 존재하므로 필요하면 사용
		Red = FOREGROUND_RED,
		LightRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
		Green = FOREGROUND_GREEN,
		Blue = FOREGROUND_BLUE,
		White = Red | Green | Blue,
		Sky = FOREGROUND_BLUE | FOREGROUND_GREEN,
		Purple = FOREGROUND_RED | FOREGROUND_BLUE,
		Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
		LightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	};
}