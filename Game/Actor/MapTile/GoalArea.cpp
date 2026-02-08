#include "GoalArea.h"

GoalArea::GoalArea(const Vector2& position)
	: super(" ", position)
{
	// Goal 액터가 그리는 영역을 담당하므로 우선순위 낮게 설정
	sortingOrder = -1;
}
