#pragma once
#include "Clause.hpp"

class Terminal : public Clause
{
public:
	Terminal() : Clause()
	{}
	Terminal(vector<Clause> x) : Clause(x)
	{}
	//выше немного бред написан, нужно переработать родительский конструктор от клоза, так как почему то он имеет конструктор с вектором
};