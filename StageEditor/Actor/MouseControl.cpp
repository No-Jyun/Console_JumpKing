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
	// 마우스 입력 테스트.
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
	}

	if (Input::Get().GetKeyDown('Q'))
	{
		QuitGame();
	}
}

void MouseControl::Draw()
{
	// 렌더러에 데이터 제출
	Renderer::Get().Submit(image, position, color, sortingOrder);
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
