#pragma once
#include "Clause.hpp"

class Terminal : public Clause
{
public:
	TypesofClauses TypeOfClause = TypesofClauses::Terminal;
	Terminal() : Clause(vector<Clause*> {} )
	{}
	//���� ������� ���� �������, ����� ������������ ������������ ����������� �� �����, ��� ��� ������ �� �� ����� ����������� � ��������
};