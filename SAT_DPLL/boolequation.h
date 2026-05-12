#ifndef BOOLEQUATION_H
#define BOOLEQUATION_H

#include "boolinterval.h"
#include "IBranchingStrategy.h"
class BoolEquation
{
public:

    BoolEquation(BoolInterval **cnf, BoolInterval *root, int cnfSize, int count, BBV mask,IBranchingStrategy* strategy = nullptr);
    BoolEquation(BoolEquation &equation,IBranchingStrategy* strategy = nullptr);
	int CheckRules();

	void Simplify(int ixCol, char value);
	int ChooseColForBranching();

    BoolInterval **cnf;//множество интервалов
    BoolInterval *root;//Корень уравнения
    int cnfSize; // Размер КНФ
    int count; //количество дизъюнкций
    BBV mask; //маска для столбцов

    bool Rule1Row1(BoolInterval *interval);
    bool Rule2RowNull(BoolInterval *interval);
    void Rule3ColNull(BBV vector);
    bool Rule4Col0(BBV vector);
    bool Rule5Col1(BBV vector);

    void setBranchingStrategy(IBranchingStrategy* strategy)
    {
        if(strategy != nullptr) _strategy = strategy;
        else return;
    }
private:
    IBranchingStrategy* _strategy;
};

#endif // BOOLEQUATION_H
