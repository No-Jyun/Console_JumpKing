#pragma once
// dll에서 템플릿을 사용할때 발생하는 오류 코드 C4251
// dll에선 원칙적으로 사용하면 안됨. 그러나 답이 없음
#pragma warning(disable: 4251)	// 미봉책 (유니티, 언리얼 다 사용함)
#pragma warning(disable: 4172)	// 위험성 인지한 상태로 사용

// 아래 설정을 껏다 키며 조절해도 됨
//#define ENGINE_BUILD_DLL 0

#define DLLEXPORT   __declspec( dllexport )
#define DLLIMPORT   __declspec( dllimport )

// ENGINE_BUILD_DLL 이 선언되어 있는지에 따라 분기
#if ENGINE_BUILD_DLL
#define WANTED_API DLLEXPORT
#else
#define WANTED_API DLLIMPORT
#endif