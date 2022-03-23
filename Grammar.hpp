#pragma once

#include <unordered_map>;
#include "Rule.hpp";

class Grammar
{
private:

public:
	vector<Rule> allRules;
	unordered_map<string, Rule> ruleNameWithPrecedenceToRule;
	vector<Clause> allClauses;
	bool DEBUG = false;

	Grammar(vector<Rule> rules)
	{
		Rule topLevelRule = rules[0];
		unordered_map<string, vector<Rule>> ruleNameToRules;
		vector<Rule> rulesWithName;
		for (auto rule : rules)
		{
			if (ruleNameToRules.find(rule.ruleName) == ruleNameToRules.end()) 
			{
				rulesWithName.clear();
				ruleNameToRules[rule.ruleName] = rulesWithName;
			}
			else
			{
				rulesWithName = ruleNameToRules[rule.ruleName];
			}

			// тут есть моментик что rulesWithName достаём из анордеред мапа если существует, а если нет то создаём 

			rulesWithName.push_back(rule); 
		}

		for (auto x : rules)
		{
			allRules.push_back(x);
		}

	}
	
};