#pragma once
#include "Clause.hpp"

class Terminal : public Clause
{
public:
	Terminal() : Clause()
	{}
	Terminal(vector<Clause> x) : Clause(x)
	{}
	//���� ������� ���� �������, ����� ������������ ������������ ����������� �� �����, ��� ��� ������ �� �� ����� ����������� � ��������
};