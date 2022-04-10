#ifndef FILEPATH_HPP
#define FILEPATH_HPP

#include <filesystem>
#include <string>
#include <sstream>

std::filesystem::path resolve_path(const std::string &path);

#endif // FILEPATH_HPP
