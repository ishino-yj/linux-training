#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

static void do_mkdir(char *path);

int main(int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i=1; i < argc; i++) {
        do_mkdir(argv[i]);
    }
    exit(0);
}

static void do_mkdir(char *path) {
    int e = 0;
    e = mkdir(path, 0777);
    if ( 0 <= e ) return; // 正常終了
    if (errno != ENOENT) { // エラー終了
        perror(path);
        exit(1);
    }

    /* 親ディレクトリがなくて失敗している場合 */ 
    // printf("path: %s\n", path);
    int len = strlen(path);
    char parent_path[1024] = "";
    for(int i=len-1; 0<i; i--) { // pathを後ろから見ていく
        if(path[i] == '/') {
            strncpy(parent_path, path, i); // pathのアドレスから i 文字分を new_pathにコピー
        }
    }
    // printf("parent_path: %s\n", parent_path);
    do_mkdir(parent_path); // 親ディレクトリを作成
    do_mkdir(path); // このディレクトリを作成
}
