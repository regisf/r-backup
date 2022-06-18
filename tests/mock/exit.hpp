//
// Created by regis on 18/06/2022.
//

#ifndef R_BACKUP_EXIT_HPP
#define R_BACKUP_EXIT_HPP

namespace mock
{
    void exit_tear_down();

    bool is_exit_called();

    void exit(int status);

    void exit_setup();

}

#endif //R_BACKUP_EXIT_HPP
