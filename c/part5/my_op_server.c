#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void get_op_ret(char *ops, char *ret);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    clnt_adr_sz = sizeof(clnt_adr);
    for (i = 0; i < 5; ++i) {
        clnt_sock= accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1) {
            error_handling("accept() error");
        } else {
            printf("Connected client %d \n", i + 1);
        }
        str_len = read(clnt_sock, message, BUF_SIZE - 1);
        char ret[BUF_SIZE] = "\0";
        printf("client message: %s\n", message);
        get_op_ret(message, ret);
        write(clnt_sock, ret, sizeof(ret));

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void get_op_ret(char *ops, char *ret) {
    char *operand_count_s;
    char *tmp = ops;
    operand_count_s = strsep(&tmp, ",");
    int operand_count = atoi(operand_count_s);
    printf("operand_count = %d\n", operand_count);
    char operands[operand_count][BUF_SIZE];
    for (int i = 0; i < operand_count; ++i) {
        strcpy(operands[i], strsep(&tmp, ","));
        printf("operands[%d] = %s\n", i, operands[i]);
    }
    char *operator;
    char *endptr;
    double nums = strtod(operands[0], &endptr);;
    operator = strsep(&tmp, ",");
    printf("operator = %c\n", operator[0]);
    for (int i = 1; i < operand_count; ++i) {
        switch (operator[0]) {
            case '+':
                nums += strtod(operands[i], &endptr);
                break;
            case '-':
                nums -= strtod(operands[i], &endptr);
                break;
            case '*':
                nums *= strtod(operands[i], &endptr);
                break;
            case '/':
                nums /= strtod(operands[i], &endptr);
                break;
        }
    }
    printf("nums = %lf\n", nums);
    sprintf(ret,"%lf", nums);
}