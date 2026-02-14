#include <iostream>
#include <Windows.h>

#include "Engine/SimpleEngine.h"
#include "Util/Vector2.h"

int main()
{
	std::cout << "제작할 스테이지의 너비와 높이를 입력하세요\n";

	int width, height;
	std::cout << "높이를 입력하세요 (최소 높이: 10 / 최대 높이: 50) : ";
	std::cin >> height;

	std::cout << "너비를 입력하세요 (최소 너비: 10 / 최대 너비: 50) : ";
	std::cin >> width;

	std::cout << height << ' ' << width;

	//Renderer* renderer = new Renderer(Vector2(51, 51));
	//Input* input = new Input(Vector2(51, 51));
	//
	//Renderer::Get().Submit("Hello", Vector2(10, 10));
	//Renderer::Get().Draw();
	////////////////////////////////////////////////////////////////// 여기부터

	SimpleEngine* engine = new SimpleEngine(Vector2(width, height));
	engine->Run();


	delete engine;
	engine = nullptr;
}