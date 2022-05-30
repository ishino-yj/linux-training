#include <pwd.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>

int main(int argc, char *argv[]) {
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufsize;
    int s;


    if (argc != 2) {
        fprintf(stderr, "Usage: %s username\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)          /* 値を決定できなかった */
        bufsize = 16384;        /* 十分大きな値にすべき */


    buf = malloc(bufsize);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }


    s = getpwnam_r(argv[1], &pwd, buf, bufsize, &result);
    if (result == NULL) {
        if (s == 0)
            printf("Not found\n");
        else {
            errno = s;
            perror("getpwnam_r");
        }
        exit(EXIT_FAILURE);
    }


    printf("pw_name: %s\n", pwd.pw_name);
    printf("pw_passwd: %s\n", pwd.pw_passwd);
    printf("pw_gecos: %s\n", pwd.pw_gecos);
    printf("pw_shell: %s\n", pwd.pw_shell);
    exit(EXIT_SUCCESS); } 