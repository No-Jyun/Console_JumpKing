#include "MouseControl.h"
#include "../Util/Util.h"
#include "../Core/Input.h"

MouseControl::MouseControl(DrawLevel* drawLevel, const char* image, const Vector2& position, Color color)
	: color(color), drawLevel(drawLevel)
{
	this->position = Vector2(0, SimpleEngine::Get().GetHeight() + 1);

	// 문자열 복사
	width = static_cast<int>(strlen(image));
	this->image = new char[width + 1];
	strcpy_s(this->image, width + 1, image);

	SelectPositionClear();
}

MouseControl::~MouseControl()
{
	// 메모리 해제
	SafeDeleteArray(image);
}

void MouseControl::BeginPlay()
{
	// 이벤트를 받은 후에는 플래그 설정
	hasBeganPlay = true;
}

void MouseControl::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown('Q'))
	{
		QuitGame();
	}

	// 마우스 입력
	if (Input::Get().GetMouseButton(0))
	{
		Vector2 mousePosition = Input::Get().MousePosition();
		char buffer[256] = {};
		sprintf_s(
			buffer,
			256,
			"cursor: (%d, %d)",
			mousePosition.x,
			mousePosition.y
		);

		ChangeImage(buffer);

		if (isFirstClicked)
		{
			isFirstClicked = false;
			firstSelectedPosition = mousePosition;
		}

		lastSelectedPosition = mousePosition;

		DragProcess();
	}

	if (Input::Get().GetMouseButtonUp(0))
	{
		isFirstClicked = true;
	}

	if (Input::Get().GetKeyDown('R'))
	{
		SelectPositionClear();
	}
	/*if (Input::Get().GetKeyDown('R'))
	{

	}
	if (Input::Get().GetKeyDown('R'))
	{

	}
	if (Input::Get().GetKeyDown('R'))
	{

	}
	if (Input::Get().GetKeyDown('R'))
	{

	}
	if (Input::Get().GetKeyDown('R'))
	{

	}
	if (Input::Get().GetKeyDown('R'))
	{

	}*/

}

void MouseControl::Draw()
{
	// 렌더러에 데이터 제출
	Renderer::Get().Submit(image, position, color, sortingOrder);

	for (Vector2 selPos : selectedPosition)
	{
		Renderer::Get().Submit(" ", selPos, Color::BackGoundWhite, sortingOrder + 3);
	}
}

void MouseControl::ChangeImage(const char* newImage)
{
	// 기존 메모리 해제.
	SafeDeleteArray(image);

	// 새로운 문자열 복사.
	width = static_cast<int>(strlen(newImage));
	image = new char[width + 1];
	strcpy_s(image, width + 1, newImage);
}

void MouseControl::Destroy()
{
	// 삭제 플래그 설정
	destroyRequested = true;

	// 삭제 이벤트 호출
	OnDestroy();
}

void MouseControl::OnDestroy()
{

}

void MouseControl::QuitGame()
{
	// 엔진 종료 요청
	SimpleEngine::Get().QuitEngine();
}

void MouseControl::SelectPositionClear()
{
	firstSelectedPosition = Vector2::Zero;
	lastSelectedPosition = Vector2::Zero;

	selectedPosition.clear();
}

void MouseControl::DragProcess()
{
	selectedPosition.clear();

	int iIndex = 0;
	int jIndex = 0;

	int finishIIndex = 0;
	int finishJIndex = 0;

	if (firstSelectedPosition.y <= lastSelectedPosition.y)
	{
		iIndex = firstSelectedPosition.x;
		jIndex = firstSelectedPosition.y;

		finishIIndex = lastSelectedPosition.x;
		finishJIndex = lastSelectedPosition.y;
	}
	else
	{
		iIndex = lastSelectedPosition.x;
		jIndex = lastSelectedPosition.y;

		finishIIndex = firstSelectedPosition.x;
		finishJIndex = firstSelectedPosition.y;
	}

	int maxWidth = drawLevel->screenSize.x;

	for (;jIndex <= finishJIndex;jIndex++)
	{
		for (;iIndex < maxWidth;iIndex++)
		{
			if (drawLevel->stage[jIndex][iIndex] == '#')
			{
				continue;
			}

			selectedPosition.emplace_back(Vector2(iIndex, jIndex));

			if (jIndex == finishJIndex && iIndex == finishIIndex)
			{
				break;
			}
		}
		iIndex = 0;
	}
}
