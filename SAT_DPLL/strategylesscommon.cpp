#include "strategylesscommon.h"
#include <vector>
#include "boolinterval.h"
#include "boolequation.h"
#include <algorithm>
int StrategyLessCommon::chooseColumn(BoolEquation& equation)
{
    std::vector<int> indexes;
    std::vector<int> values;
    bool rezInit = false;

    for (int i = 0; i < (equation.mask).getSize(); i++)
    {
        if ((equation.mask)[i] == 0)
        {
            indexes.push_back(i);
        }
    }

    for (int i = 0; i < equation.cnfSize; i++)
    {
        BoolInterval* interval = equation.cnf[i];
        if (interval != nullptr)
        {
            if (!rezInit) {
                for (int k : indexes)
                {
                    char val = interval->getValue(k);
                    int pushValue = 0;
                    if (val == '-')
                    {
                        pushValue = 1;
                    }
                    values.push_back(pushValue);
                }
                rezInit = true;
            } else {
                for (int idx = 0; idx < (int)indexes.size(); idx++)
                {
                    if (interval->getValue(indexes[idx]) == '-')
                    {
                        values[idx]++;
                    }
                }
            }
        }
    }

    int minElementIndex = std::min_element(values.begin(), values.end()) - values.begin();
    return indexes[minElementIndex];
}
