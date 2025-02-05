#include <abi/Syscalls.h>

#include <assert.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/process/Launchpad.h>

int process_this()
{
    return __plug_process_this();
}

const char *process_name()
{
    return __plug_process_name();
}

HjResult process_run(const char *command, int *pid, TaskFlags flags)
{
    // Q. Why not just pass `command` itself, as Launchpad also basically just
    // requires an executable ?
    // Ans. Because, `executable` is the executable path, not the command,
    // so we first create a process out of some known executable, ie. the shell,
    // then pass the command to run to shell
    Launchpad *launchpad = launchpad_create("shell", "/Applications/shell/shell");

    launchpad_flags(launchpad, flags);
    
    // An argument is already there, ie. the executable path, and after that, we
    // are passing "-c command" for the shell to handle (which in turn executes
    // that command)
    launchpad_argument(launchpad, "-c");
    launchpad_argument(launchpad, command);

    return launchpad_launch(launchpad, pid);
}

void NO_RETURN process_exit(int code)
{
    __plug_process_exit(code);
    __builtin_unreachable();
}

void NO_RETURN process_abort()
{
    __plug_process_exit(PROCESS_FAILURE);
    __builtin_unreachable();
}

HjResult process_cancel(int pid)
{
    return __plug_process_cancel(pid);
}

HjResult process_get_directory(char *buffer, size_t size)
{
    return __plug_process_get_directory(buffer, size);
}

HjResult process_set_directory(const char *directory)
{
    return __plug_process_set_directory(directory);
}

String process_resolve(String path)
{
    return __plug_process_resolve(path);
}

HjResult process_sleep(int time)
{
    return __plug_process_sleep(time);
}

HjResult process_wait(int pid, int *exit_value)
{
    return __plug_process_wait(pid, exit_value);
}
