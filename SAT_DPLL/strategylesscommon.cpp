#include "strategylesscommon.h"
#include <vector>
#include "boolinterval.h"
#include "boolequation.h"
#include <algorithm>
int StrategyLessCommon::chooseColumn(BoolEquation& equation)
{
    std::vector<int> indexes;
    std::vector<int> values;

    for (int i = 0; i < equation.mask.getSize(); i++)
    {
        if (equation.mask[i] == 0)
        {
            indexes.push_back(i);
            values.push_back(0);
        }
    }
    for (int i = 0; i < equation.cnfSize; i++)
    {
        BoolInterval* interval = equation.cnf[i];

        if (!interval)
            continue;

        for (size_t idx = 0; idx < indexes.size(); idx++)
        {
            if (interval->getValue(indexes[idx]) != '-')
            {
                values[idx]++;
            }
        }
    }
    if (values.empty())
    {
        throw std::logic_error("No variable available for branching");
    }
    int minElementIndex = std::min_element(values.begin(), values.end()) - values.begin();
    return indexes[minElementIndex];
}
