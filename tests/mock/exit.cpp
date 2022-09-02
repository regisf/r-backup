//
// Created by regis on 18/06/2022.
//


#include "exit.hpp"

namespace mock
{
    static bool exit_called = false;

    void exit_setup()
    {
        exit_called = false;
    }

    void exit_tear_down()
    {
        exit_called = false;
    }

    bool is_exit_called()
    {
        return exit_called;
    }

    void exit(int _status)
    {
        exit_called = true;
    }
}