#ifndef NTH_VALUE_HPP
#define NTH_VALUE_HPP

#include <string>
#include <exception>

class NthValueException : public std::exception 
{
public:
    NthValueException(const std::string& msg) : m_message(msg) {}
    NthValueException(const NthValueException& exp) { m_message = exp.m_message; }
    std::string message() const { return m_message; }

private:
    std::string m_message;
};

class NthValue 
{
public:
    explicit NthValue() = default;
    NthValue(const std::string& value);
    NthValue(const NthValue& src);

    int value() const;
    bool is_set() const;

private:
    int m_value{0};
};

#endif
