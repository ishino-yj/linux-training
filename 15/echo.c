#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static int open_connection(char *host, char *service);

#define BUF_SIZE 1024

int
main(int argc, char *argv[])
{
    int sock;
    FILE *f;
    char buf_stdin[BUF_SIZE];
    char buf[BUF_SIZE];

    sock = open_connection((argc>1 ? argv[1] : "localhost"), "echo");
    
    f = fdopen(sock, "w+"); // w+ を指定！！
    if (!f) {
        perror("fdopen(3)");
        exit(1);
    }

    fgets(buf_stdin, sizeof buf_stdin, stdin); // 標準入力から受け取り
    fputs(buf_stdin, f);  // サーバへ送信

    fgets(buf, sizeof buf, f); // サーバから受信（サーバはechoプロトコルに従って、受信したものをそのまま返す）
    fputs(buf, stdout); // 標準出力へ書き込む
    
    fclose(f);
    exit(0);
}

static int
open_connection(char *host, char *service)
{
    int sock;
    struct addrinfo hints, *res, *ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }
    for (ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock < 0) {
            continue;
        }
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        /* success */
        freeaddrinfo(res);
        return sock;
    }
    fprintf(stderr, "socket(2)/connect(2) failed");
    freeaddrinfo(res);
    exit(1);
}