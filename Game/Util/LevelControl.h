#pragma once

namespace Wanted
{
	// 레벨 전환시 사용할 열거형
	enum class LevelControl : int
	{
		MainMenuLevel = 0,
		GameLevel = 1,
		PauseMenuLevel = 2,
		ClearMenuLevel = 3,
		None,
	};
}
