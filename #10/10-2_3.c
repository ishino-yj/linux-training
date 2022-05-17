#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s: wrong arguments\n", argv[0]);
        exit(1);
    }

    /* unlink するための ファイルを作成 */
    char *filename = "hoge";
    FILE *f;
    f = fopen(filename, "w+");
    if (!f) {
        perror(argv[1]);
        exit(1);
    }
    fclose(f);


    f = fopen(argv[1], "r+");
    if (!f) {
        perror(argv[1]);
        exit(1);
    }
    if (rename(filename, "hoge2") < 0) { // openしていない hoge を hoge2 に rename
        perror(argv[1]);
        exit(1);
    }
    fclose(f);
    exit(0);
}