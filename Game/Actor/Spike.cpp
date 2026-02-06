#include "Spike.h"

// 가시 방향에 따른 문자열
static const char* spikeString[] =
{
	"A",
	">",
	"V",
	"<"	
};

Spike::Spike(const Vector2& position, const int direction)
	: super(spikeString[direction], position, Color::Red)
{

}
