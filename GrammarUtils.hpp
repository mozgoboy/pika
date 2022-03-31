#pragma once

#include "LabeledClause.hpp";
#include "Clause.hpp";
#include "RuleRef.hpp";
#include "First.hpp";
#include "Terminal.hpp";
#include "Rule.hpp";

class GrammarUtils
{
private:
    void findTerminals(Clause* clause, vector<Clause*> visited, vector<Clause*> terminalsOut) 
    {
        if (visited.push_back(clause)) 
        {
            if (clause instanceof Terminal) 
            {
                terminalsOut.add(clause);
            }
            else 
            {
                for (var labeledSubClause : clause.labeledSubClauses) 
                {
                    var subClause = labeledSubClause.clause;
                    findTerminals(subClause, visited, terminalsOut);
                }
            }
        }
    }
public:

};