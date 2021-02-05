#include <fcntl.h>
#include <string.h>

#include <abi/Syscalls.h>

#include <libsystem/Logger.h>

OpenFlag fcntl_parse_mode(int mode)
{
    OpenFlag flags = OPEN_STREAM;

    if (mode & O_CREAT)
        flags |= OPEN_CREATE;
    if (mode & O_APPEND)
        flags |= OPEN_APPEND;
    if (mode & O_DIRECTORY)
        flags |= OPEN_DIRECTORY;

    return flags;
}

int open(const char *path, int mode, ...)
{
    logger_trace("open", path);
    int flags = fcntl_parse_mode(mode);
    int handle = 0;
    Result result = hj_handle_open(&handle, path, strlen(path), flags);

    if (result != Result::SUCCESS)
    {
        // TODO: set errno
        logger_error("Failed to open handle: %s", path);
        return -1;
    }

    return handle;
}