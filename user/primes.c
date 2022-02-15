#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int p_left[2]) {
    int p;
    read(p_left[0], &p, sizeof(p));
    if (p == -1) {      // end
        exit(0);
    }
    printf("prime %d\n", p);


    int p_right[2];
    pipe(p_right);


    if (fork() == 0) {  // child
        close(p_right[1]);  // close write end
        close(p_left[0]);   // close read end
        sieve(p_right);
    } else {    // parent
        close(p_right[0]);
        int buf;
        while (read(p_left[0], &buf, sizeof(buf)) && buf != -1) {
            if (buf % p != 0) {
                write(p_right[1], &buf, sizeof(buf));
            }
        }

        buf = -1;
        write(p_right[1], &buf, sizeof(buf));
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv)
{
    int p[2];
    pipe(p);

    if (fork() == 0) {  //first child
        close(p[1]);
        sieve(p);
        exit(0);
    } else {        // parent
        close(p[0]);
        for (int i = 2; i < 36; i++) {
            write(p[1], &i, sizeof(int));
        }
        int i = -1;     // end of file
        write(p[1], &i, sizeof(int));
    }
    wait(0);
    exit(0);
}