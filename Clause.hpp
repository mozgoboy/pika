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
	bool CanMatchZeroChars; //�� ���� ������, ���� �� ������� ����� �����
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
				// ����� ������ ���� �����-�� ��������� ��� ������� ��� SubClause ��� astNodeLabel, �� ���� �� ������� �����
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
		vector<Clause*> added;
		for (auto &x : labeledSubClauses)
		{
			if (notAlreadyExist(added, x))
			{
				added.push_back(x);
				x.clause.seedParentClauses.push_back(*this);
			}
		}
	}

	virtual void determineWhetherCanMatchZeroChars() = 0;

	Match match(MemoTable memoTable, MemoKey memoKey, string input)
	{}

	// ��� ������� ������� ���� �� �� ����� ������� ��� �������� � ����� ��?

	string getRuleNames()
	{}

	string toString()
	{}

	string toStringWithRuleNames()
	{}

	//���� ������ ������� ��� ������, ��� ��� ���� ��� �� �����.
};