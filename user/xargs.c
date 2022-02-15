#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 带参数列表，执行某个程序
void run(char *program, char **args) {
	if(fork() == 0) { // child exec
		exec(program, args);
		exit(0);
	}
	return; // parent return
}

int 
main(int argc, char *argv[]) {
    char buf[512];
    char *p = buf, *last_p = buf;

    char *argsbuf[128];
    char **args = argsbuf;  // 参数列表

    for(int i=1;i<argc;i++) {
		// 将 argv 提供的参数加入到最终的参数列表中
		*args = argv[i];
		args++;
	}
    char **pa = args;

    while (read(0, p, 1) != 0) {
        if (*p == ' ' || *p == '\n') {  // 一个参数
            *p = '\0';  // 替换为 '\0'
            *pa = last_p;
            pa++;  
            last_p = p+1;

            if (*p == '\n') {
                *pa = 0;
                run(argv[1], argsbuf);
                pa = args;
            }
        }
        p++;
    }
    if(pa != args) { // 如果最后一行不是空行
		// 收尾最后一个参数
		*p = '\0';
		*(pa++) = last_p;
		// 收尾最后一行
		*pa = 0; // 参数列表末尾用 null 标识列表结束
		// 执行最后一行指令
		run(argv[1], argsbuf);
	}

    while (wait(0) != -1) {};
    exit(0);
}