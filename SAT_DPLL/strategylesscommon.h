#ifndef STRATEGYLESSCOMMON_H
#define STRATEGYLESSCOMMON_H
#include "IBranchingStrategy.h"
class StrategyLessCommon : public IBranchingStrategy
{
public:
    int chooseColumn(const BoolEquation& equation) override;
};

#endif // STRATEGYLESSCOMMON_H
