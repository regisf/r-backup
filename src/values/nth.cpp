#include "nth.hpp"

NthValue::NthValue(const std::string& value)
{
    m_value = std::atoi(value.c_str());
}

NthValue::NthValue(const NthValue& src)
{
    m_value = src.m_value;
    if (m_value < 1) 
    {
        throw NthValueException("Nth must be a positive integer");
    }
}

int NthValue::value() const
{
    return m_value;
}

bool NthValue::is_set() const
{
    return m_value != 0;
}
