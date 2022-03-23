#pragma once
#include <bitset> 
#include "Terminal.hpp"
#include "bits-stdc++.h"

class CharSet : public Terminal
{
private:
	bitset<8> chars;
	bitset<8> invertedChars;
	//�� ��� ���� ����� ��� ������ �� ��������, �� � ��� ������-�� ��� ������������ ���������.

public:
	CharSet()
	{}

	CharSet(vector<char> chars)
	{
		//� ��� ������ �������� ��� ����������� �����, � �� ������ ����� �����.
		for (int i = 0; i < chars.size(); i++)
		{
			this->chars.set(chars[i]);
		}
	}

	CharSet(vector<CharSet> CharSets)
	{
		if (CharSets.size() == 0)
		{
			//����� ����� ����� ������������� ����������
		}
		for (auto charSet : CharSets)
		{
			for (int i = charSet.chars.nextSetBit(0); i >= 0; i = charSet.chars._Find_next(i + 1)) // nextSetBit ���������� ������ ���������� ����, � ��� ����� ������� � std::bitset ���, �������� ���� ������ ��������������, ��� ��� Find_next �� ������������� ������, ������� ������ �� ������������ ������ ��� ��������� �� ��������.
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
		//� ��� ������ �������� ��� ����������� �����, � �� ������ ����� �����.
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

	//������ �����, ����� ���� ����� �������
};