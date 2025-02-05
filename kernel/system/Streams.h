#pragma once

#include <libio/Streams.h>

#include "archs/Arch.h"
#include "system/graphics/EarlyConsole.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"

namespace Kernel
{

struct LogStream : public IO::Writer
{
    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        early_console_write(buffer, size);
        return Arch::debug_write(buffer, size);
    }
};

template <typename... Args>
static ResultOr<size_t> logln(const char *fmt, Args... args)
{
    // disables interrupts at construction and enable it back after scope over
    InterruptsRetainer retainer;

    LogStream log;
    IO::BufLine buf{log};

    size_t written = 0;

    // if another program called this logln, then it prints the running program's id, else it's called BEFORE the sheduler is working, or nothing is in running state in sheduler
    if (scheduler_running_id() >= 0)
    {
        written += TRY(IO::print(buf, "\e[{}m{}({}) \e[37m", (scheduler_running_id() % 6) + 91, scheduler_running()->name, scheduler_running_id()));
    }
    else
    {
        written += TRY(IO::write(buf, "early() "));
    }

    // actually print the message
    written += TRY(IO::print(buf, fmt, std::forward<Args>(args)...));
    written += TRY(IO::write(buf, "\e[m\n"));

    return written;
}

} // namespace Kernel
