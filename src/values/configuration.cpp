#include "configuration.hpp"

ConfigurationValue::ConfigurationValue(const std::string& value) : m_value(value)
{}

std::filesystem::path ConfigurationValue::value() const
{
    return m_value;
}

