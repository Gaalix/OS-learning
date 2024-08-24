#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

// $ = konstante
// % = registri

void check_error(int status, const char *msg)
{
    if (status == -1)
    {
        perror(msg);
        _exit(1);
    }
}

int create_dir(const char *path, mode_t mode)
{
    int status;
    __asm__ volatile(
        "mov $39, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(status)          // output
        : "r"(path), "r"(mode)  // input
        : "eax", "ebx", "ecx"); // clobbered
    return status;
}

int change_dir(const char *path)
{
    int status;
    __asm__ volatile(
        "mov $12, %%eax\n"
        "mov %1, %%ebx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(status)
        : "r"(path)
        : "eax", "ebx");
    return status;
}

int opensys(const char *path, int flags, mode_t mode)
{
    int fd;
    __asm__ volatile(
        "mov $5, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "mov %3, %%edx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=m"(fd)
        : "m"(path), "m"(flags), "m"(mode)
        : "eax", "ebx", "ecx", "edx");
    return fd;
}

int chmod(const char *path, mode_t mode)
{
    int status;
    __asm__ volatile(
        "mov $15, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(status)
        : "r"(path), "r"(mode)
        : "eax", "ebx", "ecx");
    return status;
}

int write(int fd, const void *buf, size_t count)
{
    int status;
    __asm__ volatile(
        "mov $4, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "mov %3, %%edx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(status)
        : "m"(fd), "m"(buf), "m"(count)
        : "eax", "ebx", "ecx", "edx");
    return status;
}

int getpid()
{
    int pid;
    __asm__ volatile(
        "mov $20, %%eax\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(pid)
        :
        : "eax");
    return pid;
}

time_t time(time_t *t)
{
    time_t result;
    __asm__ volatile(
        "mov $13, %%eax\n"
        "mov %1, %%ebx\n"
        "int $0x80\n"
        "mov %%eax, %0\n"
        : "=r"(result)
        : "r"(t)
        : "eax", "ebx");
    return result;
}

void sync()
{
    __asm__ volatile(
        "mov $162, %%eax\n"
        "int $0x80\n"
        :
        :
        : "eax");
}

int main()
{
    int status;

    // Ustvarjanje imenika
    status = create_dir("SysDir", 0755);
    check_error(status, "create_dir");

    // sync
    sync();

    // Sprememba imenika
    status = change_dir("SysDir");
    check_error(status, "change_dir");

    // Ustvarjanje zbirke
    int fd = opensys("PidTimeData.dat", O_WRONLY | O_CREAT, 0640);
    check_error(fd, "open");

    // Parametri zbirke
    status = chmod("PidTimeData.dat", 0640);
    check_error(status, "chmod");

    // Zapis v zbirko
    int pid = getpid();
    time_t t = time(NULL);
    struct tm *time = localtime(&t);
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "%d.%m.%Y %H:%M", time);

    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "PID: %d\nTime: %s\n", pid, timeStr);

    status = write(fd, buffer, len);
    check_error(status, "write");

    // sync
    sync();

    close(fd);
    return 0;
}