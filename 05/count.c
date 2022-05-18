#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_count(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        do_count(NULL);
    }
    for (i=1; i<argc; i++) {
        do_count(argv[i]);
    }
    exit(0);
}


#define BUFFER_SIZE 2048

static void do_count(const char *path) {
    int fd;
    unsigned char buf[BUFFER_SIZE];
    int n;
    int i;
    int cnt = 0;

    if (path==NULL) {
        fd = STDIN_FILENO;
    } else {
        fd = open(path, O_RDONLY);
    }
    if (fd < 0) die(path);
    for (;;) {
        n = read(fd, buf, sizeof buf);
        if (n<0) die(path);
        if (n==0) break;
        for (i=0; i<n; i++) {
            if (buf[i] == '\n') cnt++;
        }
    }
    printf("      %d %s\n", cnt, path);
    if (close(fd) < 0) die(path);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}