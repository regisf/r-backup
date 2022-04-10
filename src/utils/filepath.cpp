#include "filepath.hpp"


std::filesystem::path resolve_path(const std::string &path)
{
    try
    {
        return std::filesystem::canonical(std::filesystem::path(path));
    }
    catch (std::filesystem::filesystem_error &err)
    {
        std::stringstream ss;
        ss << "Error : The file \"" << path << "\" doesn't exist";
        throw ss.str();
    }
}
