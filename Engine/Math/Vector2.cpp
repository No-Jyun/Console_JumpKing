#include "Vector2.h"
#include "Util/Util.h"

#include <iostream>

namespace Wanted
{
	Vector2 Vector2::Zero(0, 0);
	Vector2 Vector2::One(1, 1);
	Vector2 Vector2::Up(0, 1);
	Vector2 Vector2::Down(0, -1);
	Vector2 Vector2::Right(1, 0);
	Vector2 Vector2::Left(-1, 0);


	Vector2::Vector2()
	{
	}

	Vector2::Vector2(int x, int y)
		: x(x), y(y)
	{
	}

	Vector2::~Vector2()
	{
		if (string)
		{
			delete[] string;
			string = nullptr;
		}
	}

	const char* Vector2::ToString()
	{
		// 기존 문자열이 있다면 제거
		if (string)
		{
			delete[] string;
			string = nullptr;
		}

		string = new char[128];
		memset(string, 0, sizeof(char) * 128);
		sprintf_s(string, 128, "(%d, %d)", x, y);

		return string;
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	const float Vector2::Dot(const float otherX, const float otherY) const
	{
		float xF = static_cast<float>(x);
		float yF = static_cast<float>(y);

		float length = Util::Sqrt(xF * xF + yF * yF);

		// 내적을 위해 벡터 정규화
		xF /= length;
		yF /= length;

		return xF * otherX + yF * otherY;
	}

	Vector2::operator COORD() const
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}
}