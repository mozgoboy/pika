#pragma once
#include <bitset> 
#include "Terminal.hpp"
#include "bits-stdc++.h"

class CharSet : public Terminal
{
private:
	bitset<8> chars;
	bitset<8> invertedChars;
	//“о что выше вроде как аналог их варианта, но у них почему-то это безразмерные множества.

public:
	CharSet()
	{}

	CharSet(vector<char> chars)
	{
		//” них первой строчкой идЄт конструктор супер, € не совсем пон€л зачем.
		for (int i = 0; i < chars.size(); i++)
		{
			this->chars.set(chars[i]);
		}
	}

	CharSet(vector<CharSet> CharSets)
	{
		if (CharSets.size() == 0)
		{
			//«десь потом будет пробрасывание исключений
		}
		for (auto charSet : CharSets)
		{
			for (int i = charSet.chars.nextSetBit(0); i >= 0; i = charSet.chars._Find_next(i + 1)) // nextSetBit возвращает индекс следующего бита, € так пон€л аналога в std::bitset нет, наверное надо писать самосто€тельно, так как Find_next из подключенного хэдера, который почему то подключаетс€ только как локальный не работает.
			{
				this->chars.set(i);
			}
			for (int i = charSet.invertedChars.nextSetBit(0); i >= 0; i = charSet.invertedChars.nextSetBit(i + 1)) 
			{
				this->invertedChars.set(i);
			}
		}
	}

	CharSet(bitset<8> chars) 
	{
		//” них первой строчкой идЄт конструктор супер, € не совсем пон€л зачем.
		this->chars = chars;
	}

	CharSet invert() 
	{
		auto tmp = chars;
		chars = invertedChars;
		invertedChars = tmp;
		toStringCached = "";
		return *this;
	}

	void determineWhetherCanMatchZeroChars() 
	{}

	Match match(MemoTable memoTable, MemoKey memoKey, string input) 
	{
		if (memoKey.startPos < input.length()) 
		{
			char c = input[memoKey.startPos];
			bitset<8> b(c);
			if ((chars != NULL && (chars == b)) || (invertedChars != NULL && !(invertedChars == b))) 
			{
				return new Match(memoKey, /* len = */ 1, Match.NO_SUBCLAUSE_MATCHES);
			}
		}
		return NULL;
	}

	void toString(bitset<8> chars, int cardinality, bool inverted, string buf)
	{}

	//просто вывод, потом если нужно добавим
};