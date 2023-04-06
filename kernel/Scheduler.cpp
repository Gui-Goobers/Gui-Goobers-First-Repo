/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Log.h>
#include "Kernel.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
    DEBUG("");
}

Size Scheduler::count() const
{
    return m_queue_max.count() + m_queue_higher.count() + m_queue_default.count() + m_queue_lower.count() + m_queue_min.count();
}

Scheduler::Result Scheduler::enqueue(Process *proc, bool ignoreState)
{
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }

    switch(proc->getPriority())
    {
        case 1: 
            m_queue_min.push(proc);
            return Success; 
            break;
        case 2: 
            m_queue_lower.push(proc);   
            return Success; 
            break;
        case 3: 
            m_queue_default.push(proc); 
            return Success; 
            break;
        case 4: 
            m_queue_higher.push(proc);  
            return Success; 
            break;
        case 5: 
            m_queue_max.push(proc);     
            return Success; 
            break;
    }
}

Scheduler::Result Scheduler::dequeue(Process *proc, bool ignoreState)
{
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }

    Size count;
    switch(proc->getPriority())
    {
        case 1: 
            count = m_queue_min.count();
            break;
        case 2:   
            count = m_queue_lower.count(); 
            break;
        case 3:  
            count = m_queue_default.count();
            break;
        case 4:   
            count = m_queue_higher.count();
            break;
        case 5:     
            count = m_queue_max.count();
            break;

    }
    

    // Traverse the Queue to remove the Process
    for (Size i = 0; i < count; i++)
    {
        Process *p;
        switch(proc->getPriority())
        {
            case 1: 
                p = m_queue_min.pop();
                break;
            case 2: 
                p = m_queue_lower.pop();
                break;
            case 3: 
                p = m_queue_default.pop();
                break;
            case 4:  
                p = m_queue_higher.pop();
                break;
            case 5:    
                p = m_queue_max.pop();
                break;

        }

        if (p == proc)
            return Success;
        else
            switch(proc->getPriority()) 
            {
                case 1: 
                    m_queue_min.push(p);
                  
                    break;
                case 2: 
                    m_queue_lower.push(p);   
                
                    break;
                case 3: 
                    m_queue_default.push(p); 
                  
                    break;
                case 4: 
                    m_queue_higher.push(p);  
                  
                    break;
                case 5: 
                    m_queue_max.push(p);     
                 
                    break;

            }
    }

    FATAL("process ID " << proc->getID() << " is not in the schedule");
    return InvalidArgument;
}

Process * Scheduler::select()
{

    if (m_queue_max.count() > 0)
    {
        Process *p = m_queue_higher.pop();
        m_queue_higher.push(p);

        return p;
    }
    if (m_queue_higher.count() > 0)
    {
        Process *p = m_queue_higher.pop();
        m_queue_higher.push(p);

        return p;
    }
    
    if (m_queue_default.count() > 0)
    {
        Process *p = m_queue_default.pop();
        m_queue_default.push(p);

        return p;
    }

    if (m_queue_lower.count() > 0)
    {
        Process *p = m_queue_lower.pop();
        m_queue_lower.push(p);

        return p;
    }

    if (m_queue_min.count() > 0)
    {
        Process *p = m_queue_min.pop();
                m_queue_min.push(p);

        return p;
    }
       
    return (Process *) NULL;
}

