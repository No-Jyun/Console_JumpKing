#pragma once

#include "../Util/Vector2.h"
#include "../Util/Color.h"
#include "../Level/DrawLevel.h"
#include "../Engine/SimpleEngine.h"

class MouseControl
{
public:
	MouseControl(
		DrawLevel* drawLevel = nullptr,
		const char* image = " ",
		const Vector2& position = Vector2::Zero,
		Color color = Color::White
	);
	virtual ~MouseControl();

	// 게임 플레이 이벤트
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Draw();

	void ChangeImage(const char* newImage);

	// 삭제 요청 함수
	void Destroy();

	// 삭제가 될 때 호출될 이벤트 함수
	virtual void OnDestroy();

	// 게임 종료 함수
	void QuitGame();

	// Getter
	inline bool HasBeganPlay() const { return hasBeganPlay; }
	inline bool IsActive() const { return isActive && !destroyRequested; }
	inline bool DestroyRequested() const { return destroyRequested; }
	inline int GetSortingOrder() const { return sortingOrder; }
	inline int GetWidth() const { return width; }

private:
	void SelectPositionClear();

	void DragProcess();

private:
	// 이미 BeginPlay 이벤트를 받았는지 여부
	bool hasBeganPlay = false;

	// 활성화 상태 여부
	bool isActive = true;

	// 현재 프레임에 삭제 요청 받았는지 여부
	bool destroyRequested = false;

	// 그릴 문자(이미지)
	char* image = nullptr;

	// 문자열 길이
	int width = 0;

	// 색상
	Color color = Color::White;

	DrawLevel* drawLevel = nullptr;

	// 그리기 우선 순위 (값이 크면 우선순위가 높음)
	int sortingOrder = 0;

	// 위치
	Vector2 position;

	Vector2 firstSelectedPosition;
	Vector2 lastSelectedPosition;
	
	bool isFirstClicked = true;

	std::vector<Vector2> selectedPosition;
};
