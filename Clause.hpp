#pragma once
#include <vector>
#include <string>

using namespace std;

class Clause
{
private:
public:
	vector<LabeledClause> labeledSubClauses;
	vector<Rule> rules;
	vector<Clause> seedParentClauses;
	bool CanMatchZeroChars; //По идее удалим, пока не понятно зачем нужно
	int ClauseIdx;
	string toStringCached;
	string toStringWithRuleNameCached;
	int TypeOfClause;

	Clause()
	{}

	Clause(vector<Clause> SubClauses)
	{
		this->labeledSubClauses.clear();
		for (int i = 0; i < SubClauses.size(); i++)
		{
			Clause SubClause = SubClauses[i];
			if (SubClause.type = 0)
			{
				// Здесь долэна быть какая-то обработка при условии что SubClause это astNodeLabel, но пока не понятно какая
			}
			LabeledClause X{ subClause, astNodeLabel };
			this->labeledSubClauses.push_back(X);
		}
	}

	void registerRule(Rule rule)
	{
		rules.push_back(rule);
	}

	void unregisterRule(Rule rule)
	{
		rules.pop_back(rule);
	}

	void addAsSeedParentClause()
	{
		vector<Clause> added;
		for (auto &x : labeledSubClauses)
		{
			if (notAlreadyExist(added, x))
			{
				added.push_back(x);
				x.clause.seedParentClauses.push_back(*this);
			}
		}
	}

	void determineWhetherCanMatchZeroChars()
	{}

	Match match(MemoTable memoTable, MemoKey memoKey, String input)
	{}

	// Две верхние функции пока не до конца понятно как работают и нужны ли?

	String getRuleNames()
	{}

	String toString()
	{}

	String toStringWithRuleNames()
	{}

	//Выше просто функции для вывода, нам они пока что не нужны.
};