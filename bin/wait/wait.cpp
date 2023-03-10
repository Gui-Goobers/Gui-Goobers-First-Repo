#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "Wait.h"

Wait::Wait(int argc, char **argv) : POSIXApplication(argc, argv)
{
    parser().setDescription("wait");
    parser().registerPositional("PID", "PID is argument");
}

Wait::~Wait()
{

}

Wait::Result Wait::exec()
{
    int pid = 0;
    int status;

    // if PID is invalid (less than 0) return error
    if (((pid=atoi(arguments().get("PID")))<0)) {
        ERROR("PID '"<<arguments().get("PID")<<"' not found...");
        return NotFound;
    }

    // call waitpid
    waitpid(pid, &status, 0);

    // done
    return Success;
}