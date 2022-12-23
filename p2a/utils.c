#include "utils.h"

void error(const char* format, ...)
{
    va_list args;
    char buffer[kBufferSize] = { 0 };

    va_start(args, format);
    vsnprintf(buffer, kBufferSize, format, args);
    va_end(args);

    write(STDERR_FILENO, buffer, strlen(buffer));
}

void info(const char* format, ...)
{
    va_list args;
    char buffer[kBufferSize] = { 0 };

    va_start(args, format);
    vsnprintf(buffer, kBufferSize, format, args);
    va_end(args);

    write(STDOUT_FILENO, buffer, strlen(buffer));
}
