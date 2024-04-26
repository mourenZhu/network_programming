#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define NUMBER_SIZE 64

void error_handling(char *message);

int main(int argc, char *argv[]) {

    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in serv_adr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    } else {
        puts("Connected......");
    }

    fputs("Operand count: ", stdout);
    int operand_count = 0;
    scanf("%d", &operand_count);
    char operands[operand_count][NUMBER_SIZE];
    for (int i = 0; i < operand_count; ++i) {
        printf("Operand %d: ", i+1);
        scanf("%s", operands[i]);
    }
    printf("Operator: ");
    char operator[2] = "";
    scanf("%s", operator);
    sprintf(message, "%d", operand_count);
    strcat(message, ",");
    for (int i = 0; i < operand_count; ++i) {
        strncat(message, operands[i], strlen(operands[i]));
        strcat(message, ",");
    }
    strcat(message, operator);
//    printf("Message %s", message);
    write(sock, message, sizeof(message));

    char ret[NUMBER_SIZE] = "\0";
    read(sock, ret, NUMBER_SIZE - 1);
    printf("Operation result: %s\n", ret);

    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}