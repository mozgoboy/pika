#pragma once

#include <set>
#include "LabeledClause.hpp";
#include "Clause.hpp";
#include "RuleRef.hpp";
#include "First.hpp";
#include "Terminal.hpp";
#include "Rule.hpp";

class GrammarUtils
{
private:
    void findTerminals(Clause* clause, set<Clause*> visited, vector<Clause*> terminalsOut) 
    {
        if (visited.insert(clause).second) 
        {
            if (clause->TypeOfClause == TypesOfClauses::Terminal) 
            {
                terminalsOut.push_back(clause);
            }
            else 
            {
                for (auto labeledSubClause : clause->labeledSubClauses) 
                {
                    auto subClause = labeledSubClause->clause;
                    findTerminals(subClause, visited, terminalsOut);
                }
            }
        }
    }

    void findReachableClauses(Clause* clause, set<Clause*> visited, vector<Clause*> revTopoOrderOut) 
    {
        if (visited.insert(clause).second) 
        {
            for (auto labeledSubClause : clause->labeledSubClauses) 
            {
                auto subClause = labeledSubClause->clause;
                findReachableClauses(subClause, visited, revTopoOrderOut);
            }
            revTopoOrderOut.push_back(clause);
        }
    }

    void findCycleHeadClauses(Clause* clause, set<Clause*> discovered, set<Clause*> finished, set<Clause*> cycleHeadClausesOut) 
    {
        if (clause->TypeOfClause == TypesOfClauses::RuleRef)
        {
            /*
            throw new IllegalArgumentException(
                "There should not be any " + RuleRef.class.getSimpleName() + " nodes left in grammar");
                */
        }
        /** Ќе совсем пон€тно что такое RuleRef, почему мы должны пробрасывать исключени€.*/
        discovered.insert(clause);
        for (auto labeledSubClause : clause->labeledSubClauses) 
        {
            auto subClause = labeledSubClause->clause;
            if (discovered.contains(subClause))
            {
                // Reached a cycle
                cycleHeadClausesOut.insert(subClause);
            }
            else if (!finished.contains(subClause))
            {
                findCycleHeadClauses(subClause, discovered, finished, cycleHeadClausesOut);
            }
        }
        discovered.erase(clause);
        finished.insert(clause);
    }

    int countRuleSelfReferences(Clause* clause, string ruleNameWithoutPrecedence)
    {
        if (clause->TypeOfClause == TypesOfClauses::RuleRef && ((RuleRef*)clause)->refdRuleName == ruleNameWithoutPrecedence)
        {
            return 1;
        }
        else
        {
            auto numSelfRefs = 0;
            for (auto labeledSubClause : clause->labeledSubClauses)
            {
                auto subClause = labeledSubClause->clause;
                numSelfRefs += countRuleSelfReferences(subClause, ruleNameWithoutPrecedence);
            }
            return numSelfRefs;
        }
    }

    int rewriteSelfReferences(Clause* clause, Rule::Associativity associativity, int numSelfRefsSoFar, int numSelfRefs, string selfRefRuleName, bool isHighestPrec, string currPrecRuleName, string nextHighestPrecRuleName) 
    {
        if (numSelfRefsSoFar < numSelfRefs) 
        {
            for (int i = 0; i < clause->labeledSubClauses.size(); i++)
            {
                auto labeledSubClause = clause->labeledSubClauses[i];
                auto subClause = labeledSubClause->clause;
                if (subClause->TypeOfClause == TypesOfClauses::RuleRef) {
                    if (((RuleRef*)subClause)->refdRuleName == selfRefRuleName) 
                    {
                        if (numSelfRefs >= 2) 
                        {
                            // Change name of self-references to implement precedence climbing:
                            // For leftmost operand of left-recursive rule:
                            // E[i] <- E X E  =>  E[i] = E[i] X E[i+1]
                            // For rightmost operand of right-recursive rule:
                            // E[i] <- E X E  =>  E[i] = E[i+1] X E[i]
                            // For non-associative rule:
                            // E[i] = E E  =>  E[i] = E[i+1] E[i+1]
                            RuleRef X((associativity == Rule::Associativity::LEFT && numSelfRefsSoFar == 0) || (associativity == Rule::Associativity::RIGHT && numSelfRefsSoFar == numSelfRefs - 1)
                                ? currPrecRuleName
                                : nextHighestPrecRuleName);
                            clause->labeledSubClauses[i]->clause = &X;
                        }
                        else /* numSelfRefs == 1 */ 
                        {
                            if (!isHighestPrec) 
                            {
                                // Move subclause (and its AST node label, if any) inside a First clause that
                                // climbs precedence to the next level:
                                // E[i] <- X E Y  =>  E[i] <- X (E[i] / E[(i+1)%N]) Y
                                ((RuleRef*)subClause)->refdRuleName = currPrecRuleName;
                                auto Y = new RuleRef(nextHighestPrecRuleName);
                                vector<Clause*> Z {subClause, Y};
                                First X(Z);
                                clause->labeledSubClauses[i]->clause = &X;

                                /* “ут странный момент, у них конструктор First определЄн от Clause, но они в него суют и RuleRef. ѕочему так можно.*/
                            }
                            else
                            {
                                // Except for highest precedence, just defer back to lowest-prec level:
                                // E[N-1] <- '(' E ')'  =>  E[N-1] <- '(' E[0] ')'        
                                ((RuleRef*)subClause)->refdRuleName = nextHighestPrecRuleName;
                            }
                        }
                        numSelfRefsSoFar++;
                    }
                    // Else don't rewrite the RuleRef, it is not a self-ref
                }
                else
                {
                    numSelfRefsSoFar = rewriteSelfReferences(subClause, associativity, numSelfRefsSoFar,
                        numSelfRefs, selfRefRuleName, isHighestPrec, currPrecRuleName, nextHighestPrecRuleName);
                }
                subClause->toStringCached = "";
            }
        }
        return numSelfRefsSoFar;
    }

    
public:
    vector<Clause*> findClauseTopoSortOrder(Rule* topLevelRule, vector<Rule*> allRules, vector<Clause*> lowestPrecedenceClauses)
    {
        vector<Clause*> allClausesUnordered;
        set<Clause*> topLevelVisited;

        if (topLevelRule != nullptr)
        {
            allClausesUnordered.push_back(topLevelRule->labeledClause->clause);
            topLevelVisited.insert(topLevelRule->labeledClause->clause);
        }

        for (auto rule : allRules)
        {
            findReachableClauses(rule->labeledClause->clause, topLevelVisited, allClausesUnordered);
        }
        /**“ут тупо занесли в ќл лозесјнордеред все клозы, в которые вообще можно прийти при парсинге грамматики*/
        auto topLevelClauses = allClausesUnordered; // Ќа вс€кий случай спросить на семинаре.
        for (auto clause : allClausesUnordered)
        {
            for (auto labeledSubClause : clause->labeledSubClauses)
            {
                topLevelClauses.erase(remove(topLevelClauses.begin(), topLevelClauses.end(), labeledSubClause->clause));
            }
        }
        /** “ут, в начале занесли все клозы в топлевеледклозес, а потом удалили все, у которых есть родители.*/
        auto dfsRoots = topLevelClauses;

        for (auto x : lowestPrecedenceClauses)
        {
            dfsRoots.push_back(x);
        }

        /** ƒобавл€ем в dfsRoots какие то клозы, непон€тно что такое lowestPrecedence, какой то входной параметр пока что*/

        set<Clause*> cycleDiscovered;
        set<Clause*> cycleFinished;
        set<Clause*> cycleHeadClauses;

        for (auto clause : topLevelClauses)
        {
            findCycleHeadClauses(clause, cycleDiscovered, cycleFinished, cycleHeadClauses);
        }
        /** ƒл€ каждого хэдклоза из грамматики нашли список всех вершин, в которых начинаютс€ циклы*/
        for (auto rule : allRules)
        {
            findCycleHeadClauses(rule->labeledClause->clause, cycleDiscovered, cycleFinished, cycleHeadClauses);
        }
        /** «ачем то делаем то же самое дл€ всех правил.*/
        for (auto x : cycleHeadClauses)
        {
            dfsRoots.push_back(x);
        }
        /**«акидываем все вершины, €вл€ющиес€ началоми циклов в список dfsRoots.*/

        set<Clause*> terminalsVisited;
        vector<Clause*> terminals;
        for (auto rule : allRules)
        {
            findTerminals(rule->labeledClause->clause, terminalsVisited, terminals);
        }
        /** ѕосле этого в terminals будут лежать все терминалы, достижимые в наших правилах.*/
        vector<Clause*> allClauses(terminals);
        set<Clause*> reachableVisited;
        for (auto x : terminals)
        {
            reachableVisited.insert(x);
        }

        for (auto topLevelClause : dfsRoots)
        {
            findReachableClauses(topLevelClause, reachableVisited, allClauses);
        }
        /** ¬ allClauses закидываем все достижимые классы, правда почему то не из топлевелклозов а из dfsRoots, правда наличие visited не даЄт создавать дубликаты.*/

        for (int i = 0; i < allClauses.size(); i++)
        {
            allClauses[i]->clauseIdx = i;
        }
        /** ѕочему то в allClauses классы располагаютс€ в пор€дке увеличени€ индекса.*/
        return allClauses;
    }

    void checkNoRefCycles(Clause* clause, string selfRefRuleName, set<Clause*> visited) 
    {
        if (visited.insert(clause).second) 
        {
            for (auto labeledSubClause : clause->labeledSubClauses) 
            {
                auto subClause = labeledSubClause->clause;
                checkNoRefCycles(subClause, selfRefRuleName, visited);
            }
        }
        else 
        {
            /*
            throw new IllegalArgumentException(
                "Rules should not contain cycles when they are created: " + selfRefRuleName);
                */
        }
        visited.erase(clause);
    }

    void handlePrecedence(string ruleNameWithoutPrecedence, vector<Rule*> rules, vector<Clause*> lowestPrecedenceClauses, unordered_map<string, string> ruleNameToLowestPrecedenceLevelRuleName) 
    {
        // Rewrite rules
        // 
        // For all but the highest precedence level:
        //
        // E[0] <- E (Op E)+  =>  E[0] <- (E[1] (Op E[1])+) / E[1] 
        // E[0,L] <- E Op E   =>  E[0] <- (E[0] Op E[1]) / E[1] 
        // E[0,R] <- E Op E   =>  E[0] <- (E[1] Op E[0]) / E[1]
        // E[3] <- '-' E      =>  E[3] <- '-' (E[3] / E[4]) / E[4]
        //
        // For highest precedence level, next highest precedence wraps back to lowest precedence level:
        //
        // E[5] <- '(' E ')'  =>  E[5] <- '(' E[0] ')'

        // Check there are no duplicate precedence levels
        unordered_map<int, Rule*> precedenceToRule;
        for (auto rule : rules) 
        {
            /*
            if (precedenceToRule.insert({ rule->precedence, rule }) != nullptr)
            {
                throw new IllegalArgumentException("Multiple rules with name " + ruleNameWithoutPrecedence
                    + (rule.precedence == -1 ? "" : " and precedence " + rule.precedence));
            }*/
            // ¬ыше функци€ работает следующим образом, пытаетс€ засунуть в меп пару ключ значение, если такой ключ уже был то возвращает его значение, если его не было то заносит и возвращает null, пока не €сно, нужно нам это или нет.
        }
        // Get rules in ascending order of precedence
        vector<Rule*> precedenceOrder;
        for (auto &x : precedenceToRule)
        {
            precedenceOrder.push_back(x.second);
        }

        // Rename rules to include precedence level
        auto numPrecedenceLevels = rules.size();
        for (int precedenceIdx = 0; precedenceIdx < numPrecedenceLevels; precedenceIdx++) 
        {
            // Since there is more than one precedence level, update rule name to include precedence
            auto rule = precedenceOrder.get(precedenceIdx);
            rule.ruleName += "[" + rule.precedence + "]";
        }

        // Transform grammar rule to handle precence
        for (int precedenceIdx = 0; precedenceIdx < numPrecedenceLevels; precedenceIdx++) {
            var rule = precedenceOrder.get(precedenceIdx);

            // Count the number of self-reference operands
            var numSelfRefs = countRuleSelfReferences(rule.labeledClause.clause, ruleNameWithoutPrecedence);

            var currPrecRuleName = rule.ruleName;
            var nextHighestPrecRuleName = precedenceOrder.get((precedenceIdx + 1) % numPrecedenceLevels).ruleName;

            // If a rule has 1+ self-references, need to rewrite rule to handle precedence and associativity
            var isHighestPrec = precedenceIdx == numPrecedenceLevels - 1;
            if (numSelfRefs >= 1) {
                // Rewrite self-references to higher precedence or left- and right-recursive forms.
                // (the toplevel clause of the rule, rule.labeledClause.clause, can't be a self-reference,
                // since we already checked for that, and IllegalArgumentException would have been thrown.)
                rewriteSelfReferences(rule.labeledClause.clause, rule.associativity, 0, numSelfRefs,
                    ruleNameWithoutPrecedence, isHighestPrec, currPrecRuleName, nextHighestPrecRuleName);
            }

            // Defer to next highest level of precedence if the rule doesn't match, except at the highest level of
            // precedence, which is assumed to be a precedence-breaking pattern (like parentheses), so should not
            // defer back to the lowest precedence level unless the pattern itself matches
            if (!isHighestPrec) {
                // Move rule's toplevel clause (and any AST node label it has) into the first subclause of
                // a First clause that fails over to the next highest precedence level
                var first = new First(rule.labeledClause.clause, new RuleRef(nextHighestPrecRuleName));
                // Move any AST node label down into first subclause of new First clause, so that label doesn't
                // apply to the final failover rule reference
                first.labeledSubClauses[0].astNodeLabel = rule.labeledClause.astNodeLabel;
                rule.labeledClause.astNodeLabel = null;
                // Replace rule clause with new First clause
                rule.labeledClause.clause = first;
            }
        }

        // Map the bare rule name (without precedence suffix) to the lowest precedence level rule name
        var lowestPrecRule = precedenceOrder.get(0);
        lowestPrecedenceClauses.add(lowestPrecRule.labeledClause.clause);
        ruleNameToLowestPrecedenceLevelRuleName.put(ruleNameWithoutPrecedence, lowestPrecRule.ruleName);
    }

};