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
		Terminal(); // super это так же конструктор по умолчанию????
		this->str = str;
		this->ignoreCase = ignoreCase;
	}

	void determineWhetherCanMatchZeroChars() 
	{}

	Match match(MemoTable memoTable, MemoKey memoKey, string input) 
	{
		if (memoKey.startPos <= input.length() - str.length() && (input.substr(memoKey.startPos, str.length()) == str)) // надо спросить про параметр ignoreCase, т.к. если он true то последнее сравнение тоже должно вернуть true, а иначе возвращается результат сравнения.
		{
			return new Match(memoKey, /* len = */ str.length());
		}
		return NULL;
	}

	string toString()
	{}
};