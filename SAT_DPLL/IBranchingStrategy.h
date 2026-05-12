#ifndef IBRANCHINGSTRATEGY_H
#define IBRANCHINGSTRATEGY_H

class BoolEquation;

class IBranchingStrategy
{
public:
    virtual ~IBranchingStrategy() = default;
    virtual int chooseColumn(BoolEquation& equation) = 0;
};

#endif // IBRANCHINGSTRATEGY_H
