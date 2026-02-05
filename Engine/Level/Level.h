#pragma once

// STL(Standard Template Library - C++ 기본 제공)
// 크기가 알아서 변경되는 배열
#include <vector>
#include "Common/RTTI.h"

namespace Wanted
{
	// 전방 선언
	class Actor;

	// 담당 임무 : 레벨에 있는 모든 액터(물체) 관리
	class WANTED_API Level : public RTTI
	{
		// RTTI 코드 추가
		RTTI_DECLARATIONS(Level, RTTI)

	public:
		Level();
		virtual ~Level();

		// 게임 플레이 이벤트
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 액터 추가 함수
		void AddNewActor(Actor* newActor);

		// 액터 추가/제거 처리 함수
		void ProcessAddAndDestroyActor();

	protected:
		// 액터 배열
		std::vector<Actor*> actors;
		
		// 실행 중에 추가 요청된 액터의 배열
		std::vector<Actor*> addRequestedActors;
	};
}
