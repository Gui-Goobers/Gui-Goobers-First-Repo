#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ProcessClient.h>
#include "Renice.h"

Renice::Renice(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Change the priority of a process");
    parser().registerFlag('n', "new", "Changes process priority");
    parser().registerPositional("PRIO", "The new priority for the process");
    parser().registerPositional("PID", "The ID of the process to change the priority of");
}

Renice::~Renice()
{
}

Renice::Result Renice::exec()
{
    if (arguments().get("new")){

        int prio = atoi(arguments().get("PRIO"));
        int pid = atoi(arguments().get("PID"));

        const ProcessClient process;
        ProcessClient::Info info;

        // Make sure the process is valid
        const ProcessClient::Result result = process.processInfo(pid, info);
        if (result == ProcessClient::Success)
        {
            // Valid process
            if(renice(pid, prio)) 
            {
                ERROR("failed to change priority: " << strerror(errno));
                return IOError;
            }
        }

        else
        {
            // Invalid process - return an error
            ERROR("invalid process id: " << arguments().get("PID") << "'");
            return IOError;
        }
}
