#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

static void do_ls(char *path);

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i=1; i < argc; i++) {
        do_ls(argv[i]);
    }
    exit(0);
}

static void do_ls(char *path) {
    DIR *d;
    struct dirent *ent;
    d = opendir(path);  /* ① */
    if (!d) {
        perror(path);
        exit(1);
    }
    while (ent = readdir(d)) { /* ② */
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        printf("%s\n", ent->d_name);
        int len = strlen(path) + strlen(ent->d_name) + 2; // 文字列の長さ + / + \0 
        char new_path[len];
        struct stat st;
        snprintf(new_path, len, "%s/%s", path, ent->d_name); // 文字列の連結（バッファオーバーフローをケア）dir//hoge みたいになることがあるが、問題なさそう。
        if (lstat(new_path, &st) < 0) {
            // perror(new_path);
            continue;
        }
        if (!S_ISDIR(st.st_mode)) { // ディレクトリじゃなかったら抜ける
            // printf("%s is not a directory.\n", new_path);
            continue;
        } 
        do_ls(new_path); // ここまでの審査をくぐり抜けた場合は do_ls を再帰呼び出し
    }
    closedir(d); 
}