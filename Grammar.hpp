#pragma once

#include <unordered_map>;
#include <unordered_set>;
#include <queue>
#include <set>
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
			GrammarUtils.checkNoRefCycles(rule.labeledClause.clause, rule.ruleName, new unordered_set<Clause>());
		}

		allRules.clear();
		for (auto x : rules)
		{
			allRules.push_back(x);
		}
		vector<Clause> lowestPrecedenceClauses;

		/*
		for (auto ent : ruleNameToRules.entrySet()) 
		{
			var rulesWithName = ent.getValue();
			if (rulesWithName.size() > 1) 
			{
				var ruleName = ent.getKey();
				GrammarUtils.handlePrecedence(ruleName, rulesWithName, lowestPrecedenceClauses,
					ruleNameToLowestPrecedenceLevelRuleName);
			}
		}*/

		// Закоменченное выше вызывает метод entrySet, который вовращает что-то типа набора пар ключ значение, я если честно не понимаю как оно должно прописываться.

		unordered_set<string, Rule> ruleNameWithPrecedenceToRule;

		for (auto rule : allRules) 
		{
			rule.labeledClause.clause.registerRule(rule);
		}

		unordered_map<string, Clause> toStringToClause;
		for (auto rule : allRules) 
		{
			rule.labeledClause.clause = GrammarUtils.intern(rule.labeledClause.clause, toStringToClause);
		}

		set<Clause> clausesVisitedResolveRuleRefs;
		for (auto rule : allRules) 
		{
			GrammarUtils.resolveRuleRefs(rule.labeledClause, ruleNameWithPrecedenceToRule, ruleNameToLowestPrecedenceLevelRuleName, clausesVisitedResolveRuleRefs);
		}

		allClauses = GrammarUtils.findClauseTopoSortOrder(topLevelRule, allRules, lowestPrecedenceClauses);

		for (Clause clause : allClauses) 
		{
			clause.determineWhetherCanMatchZeroChars();
		}

		for (auto clause : allClauses) 
		{
			clause.addAsSeedParentClause();
		}
	}

	MemoTable parse(string input) {
		auto priorityQueue = new priority_queue<Clause>((c1, c2)->c1.clauseIdx - c2.clauseIdx);

		auto memoTable = new MemoTable(this, input);

		/* auto terminals = allClauses.stream().filter(clause->clause instanceof Terminal
			// Don't match Nothing everywhere -- it always matches
			&& !(clause instanceof Nothing)) //
			.collect(Collectors.toList()); */

		// То что сверху пока тоже не очень ясно

		// Main parsing loop
		for (int startPos = input.length() - 1; startPos >= 0; --startPos) {
			priorityQueue.addAll(terminals);
			while (!priorityQueue.isEmpty()) {
				// Remove a clause from the priority queue (ordered from terminals to toplevel clauses)
				auto clause = priorityQueue.remove();
				auto memoKey = new MemoKey(clause, startPos);
				auto match = clause.match(memoTable, memoKey, input);
				memoTable.addMatch(memoKey, match, priorityQueue);
			}
		}
		return memoTable;
	}


	Rule getRule(string ruleNameWithPrecedence) 
	{
		auto rule = ruleNameWithPrecedenceToRule[ruleNameWithPrecedence];
		return rule;
	}

	vector<Match> getNonOverlappingMatches(string ruleName, MemoTable memoTable) 
	{
		return memoTable.getNonOverlappingMatches(getRule(ruleName).labeledClause.clause);
	}
	
	/*
	NavigableMap<Integer, Match> getNavigableMatches(String ruleName, MemoTable memoTable) 
	{
		return memoTable.getNavigableMatches(getRule(ruleName).labeledClause.clause);
	}
	*/

	//Пока не ясно с аналогом на плюсах у навигаблмэп.
};