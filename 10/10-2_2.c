#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s: wrong arguments\n", argv[0]);
        exit(1);
    }

    FILE *f;
    f = fopen(argv[1], "r+");
    if (!f) {
        perror(argv[1]);
        exit(1);
    }
    if (unlink(argv[1]) < 0) {
        perror(argv[1]);
        exit(1);
    }
    fclose(f);
    exit(0);
}