#pragma once
#include "Terminal.hpp"

class Nothing : public Terminal
{
public:
	string NOTHING_STR = "()";

	Nothing()
	{}

	void determineWhetherCanMatchZeroChars()
	{
		CanMatchZeroChars = true;
	}

	Match match(MemoTable memoTable, MemoKey memoKey, string input)
	{
		if (memoKey.startPos == 0)
		{
			return Match(memoKey);
		}
		return NULL;
	}
	//Функция выше пока не ясно как работает

	string toString()
	{
		if (toStringCached == "")
		{
			toStringCached = NOTHING_STR;
		}
		return toStringCached;
	}
	//Функция выше просто вывод, мб потом уберём
};