#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char* path, char* target) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        exit(0);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    switch(st.type){
        case T_FILE:
            if (strcmp(path+strlen(path)-strlen(target), target) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0)
                {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }

                if (strcmp(buf+strlen(buf)-2, "/.") != 0 && strcmp(buf+strlen(buf)-3, "/..") != 0) {
                    find(buf, target);
                }
            }
            break;
    }
    close(fd);
}

int 
main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: find <file>\n");
        exit(1);
    }
    char target[512];
    target[0] = '/';
    strcpy(target + 1, argv[2]);
    find(argv[1], target);
    
    exit(0);
}