#pragma once

#include "ClauseFactory.hpp"
#include "Clause.hpp";
#include "Grammar.hpp";
#include "First.hpp";
#include "FollowedBy.hpp";
#include "NotFollowedBy.hpp";
#include "OneOrMore.hpp";
#include "Seq.hpp";
#include "Terminal.hpp";
#include "Rule.hpp";
#include "ParserInfo.hpp";
#include "StringUtils.hpp";

class MetaGrammar
{
private:

    // имена для правил

    string GRAMMAR = "GRAMMAR";
    string WSC = "WSC";
    string COMMENT = "COMMENT";
    string RULE = "RULE";
    string CLAUSE = "CLAUSE";
    string IDENT = "IDENT";
    string PREC = "PREC";
    string NUM = "NUM";
    string NAME_CHAR = "NAME_CHAR";
    string CHAR_SET = "CHARSET";
    string HEX = "Hex";
    string CHAR_RANGE = "CHAR_RANGE";
    string CHAR_RANGE_CHAR = "CHAR_RANGE_CHAR";
    string QUOTED_STRING = "QUOTED_STR";
    string ESCAPED_CTRL_CHAR = "ESCAPED_CTRL_CHAR";
    string SINGLE_QUOTED_CHAR = "SINGLE_QUOTED_CHAR";
    string STR_QUOTED_CHAR = "STR_QUOTED_CHAR";
    string NOTHING = "NOTHING";
    string START = "START";

    // имена для вершин дерева разбора

    string RULE_AST = "RuleAST";
    string PREC_AST = "PrecAST";
    string R_ASSOC_AST = "RAssocAST";
    string L_ASSOC_AST = "LAssocAST";
    string IDENT_AST = "IdentAST";
    string LABEL_AST = "LabelAST";
    string LABEL_NAME_AST = "LabelNameAST";
    string LABEL_CLAUSE_AST = "LabelClauseAST";
    string SEQ_AST = "SeqAST";
    string FIRST_AST = "FirstAST";
    string FOLLOWED_BY_AST = "FollowedByAST";
    string NOT_FOLLOWED_BY_AST = "NotFollowedByAST";
    string ONE_OR_MORE_AST = "OneOrMoreAST";
    string ZERO_OR_MORE_AST = "ZeroOrMoreAST";
    string OPTIONAL_AST = "OptionalAST";
    string SINGLE_QUOTED_CHAR_AST = "SingleQuotedCharAST";
    string CHAR_RANGE_AST = "CharRangeAST";
    string QUOTED_STRING_AST = "QuotedStringAST";
    string START_AST = "StartAST";
    string NOTHING_AST = "NothingAST";

    map<Clause, int> clauseTypeToPrecedence;
    clauseTypeToPrecedence[Terminal] = 7;
    clauseTypeToPrecedence[RuleRef] = 7;
    clauseTypeToPrecedence[OneOrMore] = 6;
    clauseTypeToPrecedence[NotFollowedBy] = 5;
    clauseTypeToPrecedence[FollowedBy] = 5;
    clauseTypeToPrecedence[ASTNodeLabel] = 3;
    clauseTypeToPrecedence[Seq] = 2;
    clauseTypeToPrecedence[First] = 1;

    /*
    * 1) Все эти классы - дочерние для клоза, не совсем понятно как это описать
    * 2) Как-то указать что этот umap не должен изменяться
    */

public:
    vector<Rule> NecessaryRules;
    NecessaryRules.push_back(Rule(GRAMMAR, seq(start(), ruleRef(WSC), oneOrMore(ruleRef(RULE)))));
    NecessaryRules.push_back(Rule(RULE, ast(RULE_AST, Seq(ruleRef(IDENT), ruleRef(WSC), optional(ruleRef(PREC)), str("<-"), ruleRef(WSC), ruleRef(CLAUSE), ruleRef(WSC), c(';'), ruleRef(WSC)))));
    NecessaryRules.push_back(Rule(CLAUSE, 8, /* associativity = */ NONE, Seq(c('('), ruleRef(WSC), ruleRef(CLAUSE), ruleRef(WSC), c(')'))));
    NecessaryRules.push_back(Rule(CLAUSE, 7, /* associativity = */ NONE, First(ruleRef(IDENT), ruleRef(QUOTED_STRING), ruleRef(CHAR_SET), ruleRef(NOTHING), ruleRef(START))));
    NecessaryRules.push_back(Rule(CLAUSE, 6, /* associativity = */ NONE, First(Seq(ast(ONE_OR_MORE_AST, ruleRef(CLAUSE)), ruleRef(WSC), c('+')), Seq(ast(ZERO_OR_MORE_AST, ruleRef(CLAUSE)), ruleRef(WSC), c('*')))));
    NecessaryRules.push_back(Rule(CLAUSE, 5, /* associativity = */ NONE, First(Seq(c('&'), Ast(FOLLOWED_BY_AST, ruleRef(CLAUSE))), Seq(c('!'), ast(NOT_FOLLOWED_BY_AST, ruleRef(CLAUSE))))));
    Rule(CLAUSE, 4, /* associativity = */ NONE, Seq(ast(OPTIONAL_AST, ruleRef(CLAUSE)), ruleRef(WSC), c('?')))
};
