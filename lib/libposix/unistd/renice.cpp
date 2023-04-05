#include <FreeNOS/User.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Timer.h>
#include <FreeNOS/Kernel.h>
#include "unistd.h"
#include "errno.h"
#include <ProcessClient.h>
#include <ProcessManager.h>
#include <Kernel.h>
#include <Process.h>

unsigned int renice(unsigned int pid, unsigned int prio)
{
    if (ProcessCtl(pid, SetPriority, 0, prio))
    {
        errno = EIO;
        return pid;
    }
    printf("Changed the priority to %d\n", prio);
    return 0;
}