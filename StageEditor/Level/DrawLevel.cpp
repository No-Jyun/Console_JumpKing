#include "DrawLevel.h"
#include "../Actor/MouseControl.h"

const char* instructionString[] =
{
	". : 바닥",
	", : 풀",
	"/ : 벽",
	"* : 얼음",
	"0,1,2,3 : 가시(북,동,남,서)",
	"u : 위 스테이지 통로",
	"i : 아래 스테이지에서 위 스테이지로 올라올때 생성될 위치",
	"d : 아래 스테이지 통로",
	"f : 위 스테이지에서 아래 스테이지로 내려올대 생성되는 위치",
	"G : 게임 클리어 목적지",
	"g : 게임 클리어 목적지의 트리거 박스"
};

DrawLevel::DrawLevel(const Vector2& screenSize)
	: screenSize(screenSize)
{
	mouseActor = new MouseControl(this);

	SetLevelBase();
}

DrawLevel::~DrawLevel()
{
	delete mouseActor;
	mouseActor = nullptr;
}

void DrawLevel::BeginPlay()
{
	if (mouseActor == nullptr)
	{
		return;
	}

	if(mouseActor->HasBeganPlay())
	{
		mouseActor->BeginPlay();
	}
}

void DrawLevel::Tick(float deltaTime)
{
	if (mouseActor == nullptr)
	{
		return;
	}

	mouseActor->Tick(deltaTime);
}

void DrawLevel::Draw()
{
	if (!mouseActor->IsActive())
	{
		return;
	}
	
	mouseActor->Draw();

	DrawInstruction();
	DrawStageLevel();
}

void DrawLevel::SetLevelBase()
{
	for (int i = 0;i < screenSize.y;i++)
	{
		std::string t(screenSize.x, ' ');
		stage.emplace_back(t);
	}

	// 맵 경계 설정
	for (int i = 0;i < screenSize.x;i++)
	{
		stage[0][i] = '#';
		stage[screenSize.y - 1][i] = '#';
	}

	for (int j = 0;j < screenSize.y;j++)
	{
		stage[j][0] = '#';
		stage[j][screenSize.x - 1] = '#';
	}
}

void DrawLevel::DrawInstruction()
{
	size_t length = sizeof(instructionString) / sizeof(instructionString[0]);

	for (int i = 0;i < length;i++)
	{
		Vector2 drawPosition = Vector2(52, 0);
		drawPosition.y += i;

		Renderer::Get().Submit(instructionString[i], drawPosition);
	}
}

void DrawLevel::DrawStageLevel()
{
	for (int i = 0;i < screenSize.y;i++)
	{		
		Vector2 drawPosition = Vector2::Zero;
		drawPosition.y += i;
		
		Renderer::Get().Submit(stage[i].c_str(), drawPosition);
	}
}
