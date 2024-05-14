#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define BUF_SIZE 3

int main(int argc, char *argv[]) {
    FILE *fp1;
    FILE *fp2;
    char buf[BUF_SIZE];

    fp1 = fopen("test.pdf", "r");
    fp2 = fopen("cpy.pdf", "w");

    time_t start;
    time(&start);
    while (fgets(buf, BUF_SIZE, fp1) != NULL) {
        fputs(buf, fp2);
    }
    time_t end;
    time(&end);
    printf("copy time: %ld", end - start);
    fclose(fp1);
    fclose(fp2);
    return 0;
}