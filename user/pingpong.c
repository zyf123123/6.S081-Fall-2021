#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv)
{
    int p[2];
    pipe(p);

    if (fork() == 0) {
        dup(p[0]);
        close(p[0]);
        int pid = getpid();
        char buf[10];
        printf("%d: received ping\n", pid);
        write(p[1], buf, sizeof(pid));
        close(p[1]);
        exit(0);
    } else {
        char buf[10];
        close(p[1]);
        read(p[0], &buf, sizeof(int));
        close(p[0]);
        int pid = getpid();
        printf("%d: received pong\n", pid);
    }

    exit(0);
}