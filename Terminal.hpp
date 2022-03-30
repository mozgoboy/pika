#pragma once
#include "Clause.hpp"

class Terminal : public Clause
{
public:
	TypesofClauses TypeOfClause = TypesofClauses::Terminal;
	Terminal() : Clause(vector<Clause*> {} )
	{}
	//выше немного бред написан, нужно переработать родительский конструктор от клоза, так как почему то он имеет конструктор с вектором
};