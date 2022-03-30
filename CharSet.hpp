#pragma once
#include "bits-stdc++.h"
#include "Terminal.hpp"
#include <bitset> 



class CharSet : public Terminal
{
private:
	bitset<8> chars;
	bitset<8> invertedChars;
	//“о что выше вроде как аналог их варианта, но у них почему-то это безразмерные множества.

public:
	TypesofClauses TypeOfClause = TypesofClauses::CharSet;
	CharSet()
	{}

	CharSet(vector<char> chars) : Terminal()
	{
		//” них первой строчкой идЄт конструктор супер, € не совсем пон€л зачем.
		for (int i = 0; i < chars.size(); i++)
		{
			this->chars.set(chars[i]);
		}
	}

	CharSet(vector<CharSet*> CharSets)
	{
		if (CharSets.size() == 0)
		{
			//«десь потом будет пробрасывание исключений
		}
		for (auto charSet : CharSets)
		{
			for (int i = charSet->chars.nextSetBit(0); i >= 0; i = charSet->chars._Find_next(i + 1)) // nextSetBit возвращает индекс следующего бита, € так пон€л аналога в std::bitset нет, наверное надо писать самосто€тельно, так как Find_next из подключенного хэдера, который почему то подключаетс€ только как локальный не работает.
			{
				this->chars.set(i);
			}
			for (int i = charSet->invertedChars.nextSetBit(0); i >= 0; i = charSet->invertedChars.nextSetBit(i + 1)) 
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

	Match* match(MemoTable* memoTable, MemoKey* memoKey, string input) 
	{
		if (memoKey->startPos < input.length()) 
		{
			char c = input[memoKey->startPos];
			bitset<8> b(c);
			if ((chars != NULL && (chars == b)) || (invertedChars != NULL && !(invertedChars == b))) 
			{
				Match mast(memoKey, /* len = */ 1, Match::NO_SUBCLAUSE_MATCHES);
				return &mast
			}
		}
		return nullptr;
	}

	void toString(bitset<8> chars, int cardinality, bool inverted, string buf) {
		bool isSingleChar = !inverted && cardinality == 1;
		if (isSingleChar) {
			char c = (char)chars.nextSetBit(0);
			buf.append("'");
			buf.append(StringUtils.escapeQuotedChar(c));
			buf.append("'");
		}
		else {
			buf.append("[");
			if (inverted) {
				buf.append(" ^ ");
			}
			for (int i = chars.nextSetBit(0); i >= 0; i = chars.nextSetBit(i + 1)) {
				buf.append(StringUtils.escapeCharRangeChar((char)i));
				if (i < chars.size() - 1 && chars.get(i + 1)) {
					// Contiguous char range
					int end = i + 2;
					while (end < chars.size() && chars.get(end)) {
						end++;
					}
					int numCharsSpanned = end - i;
					if (numCharsSpanned > 2) {
						buf.append(" - ");
					}
					buf.append(StringUtils.escapeCharRangeChar((char)(end - 1)));
					i = end - 1;
				}
			}
			buf.append("]");
		}
	}

	string toString() {
		if (toStringCached.empty()) {
			string buf;
			auto charsCardinality = chars == NULL ? 0 : chars.cardinality();
			auto invertedCharsCardinality = invertedChars == NULL ? 0 : invertedChars.cardinality();
			auto invertedAndNot = charsCardinality > 0 && invertedCharsCardinality > 0;
			if (invertedAndNot) {
				buf.append("(");
			}
			if (charsCardinality > 0) {
				toString(chars, charsCardinality, false, buf);
			}
			if (invertedAndNot) {
				buf.append(" | ");
			}
			if (invertedCharsCardinality > 0) {
				toString(invertedChars, invertedCharsCardinality, true, buf);
			}
			if (invertedAndNot) {
				buf.append(")");
			}
			toStringCached = buf;
		}
		return toStringCached;
	}

	//просто вывод, потом если нужно добавим
};