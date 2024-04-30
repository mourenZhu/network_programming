#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void is_exit(int sig) {
    if (sig == SIGINT)
        puts("是否退出程序,输入Y则终止程序");
        char e = fgetc(stdin);;
        if (e == 'Y') {
            exit(0);
        }
}

int main() {
    struct sigaction act;
    act.sa_handler=is_exit;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    while (1) {
        printf("x\n");
        sleep(1);
    }

    return 0;
}