//
// Created by regis on 18/06/2022.
//

#ifndef R_BACKUP_SYM_LINK_HPP
#define R_BACKUP_SYM_LINK_HPP

#include <filesystem>

namespace mock
{
    bool is_sym_link_always_true(const std::filesystem::path &_)
    {
        return true;
    }

    bool is_symlink_always_false(const std::filesystem::path &_)
    {
        return false;
    }
}

#endif //R_BACKUP_SYM_LINK_HPP
