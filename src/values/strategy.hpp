#ifndef STRATEGY_VALUE_HPP
#define STRATEGY_VALUE_HPP

#include <string>

class StrategyValue 
{
public:
    enum class Strategy
    {
        NotSet,
        KeepAll,
        KeepNth,
        RemovePrevious,
        Difference
    };

    explicit StrategyValue() = default;

    StrategyValue(const std::string& strat_value, int nth);
    StrategyValue(const StrategyValue& src);
    bool operator==(const StrategyValue::Strategy src) const;

    StrategyValue::Strategy value();

    bool is_set() const;

private:
    StrategyValue::Strategy  from_string(const std::string& strat_value);

private:
    StrategyValue::Strategy m_strategy{StrategyValue::Strategy::NotSet};
    int m_nth;
};

#endif
