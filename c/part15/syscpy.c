#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define BUF_SIZE 3

int main(int argc, char *argv[]) {
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    fd1 = open("test.pdf", O_RDONLY);
    fd2 = open("cpy.pdf", O_WRONLY | O_CREAT | O_TRUNC);

    time_t start;
    time(&start);
    while ((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }
    time_t end;
    time(&end);
    printf("copy time: %ld", end - start);
    close(fd1);
    close(fd2);
    return 0;
}