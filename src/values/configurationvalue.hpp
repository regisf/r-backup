#ifndef CONFIGURATIONVALUE_HPP
#define CONFIGURATIONVALUE_HPP

#include <string>
#include <filesystem>

class ConfigurationValue
{
public:
    explicit ConfigurationValue(const std::string& configuration);

    std::filesystem::path  value() const;

private:
    std::filesystem::path m_value;

};

#endif // CONFIGURATIONVALUE_HPP
