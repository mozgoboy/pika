#pragma once
#include "Clause.hpp"

class Rule
{
public:
    string ruleName;

    int precedence;

    LabeledClause labeledClause;

    enum Associativity
    {
        LEFT, RIGHT, NONE
    };

    Associativity associativity;

    Rule(string ruleName, int precedence, Associativity associativity, Clause clause) 
    {
        this->ruleName = ruleName;
        this->precedence = precedence;
        this->associativity = associativity;

        string astNodeLabel = "";
        auto clauseToUse = clause;
        if (clause.TypeOfClause == 0) {
            // Transfer ASTNodeLabel.astNodeLabel to astNodeLabel
            //astNodeLabel = ((ASTNodeLabel)clause).astNodeLabel; Тут перевод в тип astNodeLabel, пока до конца не ясно как его оформлять.
            // skip over ASTNodeLabel node when adding subClause to subClauses array
            clauseToUse = clause.labeledSubClauses[0].clause;
        }
        LabeledClause(clauseToUse, astNodeLabel) X;
        this->labeledClause = X;
    }

    Rule(string ruleName, Clause clause) {
        Rule(ruleName, -1, NONE, clause);
    }

    string toString()
    {}
    // Опять же нужно просто для вывода.
};