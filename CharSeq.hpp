#pragma once
#include "Terminal.hpp"

class CharSeq : public Terminal
{
private:
	
public:
	string str;
	bool ignoreCase;

	public CharSeq(string str, bool ignoreCase) 
	{
		Terminal(); // super ��� ��� �� ����������� �� ���������????
		this->str = str;
		this->ignoreCase = ignoreCase;
	}

	void determineWhetherCanMatchZeroChars() 
	{}

	Match match(MemoTable memoTable, MemoKey memoKey, string input) 
	{
		if (memoKey.startPos <= input.length() - str.length() && (input.substr(memoKey.startPos, str.length()) == str)) // ���� �������� ��� �������� ignoreCase, �.�. ���� �� true �� ��������� ��������� ���� ������ ������� true, � ����� ������������ ��������� ���������.
		{
			return new Match(memoKey, /* len = */ str.length());
		}
		return NULL;
	}

	string toString()
	{}
};