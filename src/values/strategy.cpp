#include "strategy.hpp"

#include <iostream>

StrategyValue::StrategyValue(const std::string& strat_value)
{
    m_strategy = from_string(strat_value);
}

StrategyValue::StrategyValue(const StrategyValue& src)
{
    m_strategy = src.m_strategy;
}

StrategyValue::Strategy StrategyValue::value()
{
    return m_strategy;
}

StrategyValue::Strategy StrategyValue::from_string(const std::string& strat_value)
{
    if (!strat_value.compare("keep_all") || !strat_value.compare("keep-all")) {
        return StrategyValue::Strategy::KeepAll;
    }

    else if (!strat_value.compare("keep_nth") || !strat_value.compare("keep-nth")) {
        return StrategyValue::Strategy::KeepNth;
    }

    else if (!strat_value.compare("remove_previous") || !strat_value.compare("remove-previous")) {
        return StrategyValue::Strategy::RemovePrevious;
    }

    else if (!strat_value.compare("difference")) {
        return StrategyValue::Strategy::Difference;
    }

    std::cerr << "Warning: unknown strategy: \"" << strat_value << "\". Using default \"Keep App\".\n";

    return StrategyValue::Strategy::KeepAll;
}

bool StrategyValue::is_set() const
{
    return m_strategy != StrategyValue::Strategy::NotSet;
}

bool StrategyValue::operator==(const StrategyValue::Strategy src) const
{
    return m_strategy == src;
}
