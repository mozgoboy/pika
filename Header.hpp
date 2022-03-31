#pragma once
#include <string>

using namespace std;

class Clause
{
	public:
		string name;
		string toStringCached;
		Clause()
		{
			this->name = "Govno";
		}
		Clause(string name)
		{
			this->name = name;
		}
};

class RuleRef : public Clause
{
public:
	string refdRuleName;
	RuleRef(string refdRuleName)
	{
		RuleRef::Clause();
		this->refdRuleName = refdRuleName;
	}

	void determineWhetherCanMatchZeroChars() 
	{}

	string toString() 
	{
		if (this->toStringCached == "") 
		{
			toStringCached = refdRuleName;
		}
		return toStringCached;
	}
};