#pragma once
#include "Terminal.hpp"

class Nothing : public Terminal
{
public:
	TypesofClauses TypeOfClause = TypesofClauses::Nothing;
	string NOTHING_STR = "()";

	Nothing() : Terminal()
	{}

	void determineWhetherCanMatchZeroChars()
	{
		canMatchZeroChars = true;
	}

	Match* match(MemoTable* memoTable, MemoKey* memoKey, string input)
	{
		if (memoKey->startPos == 0)
		{
			Match mast(memoKey);
			return &mast;
		}
		return nullptr;
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