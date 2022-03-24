#pragma once
#include "Terminal.hpp"

class Start : public Terminal
{
public:
	string START_STR = "^";

	Start()
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
	//������� ���� ���� �� ���� ��� ��������

	string toString() 
	{
		if (toStringCached == "")
		{
			toStringCached = START_STR;
		}
		return toStringCached;
	}
	//������� ���� ������ �����, �� ����� �����
};