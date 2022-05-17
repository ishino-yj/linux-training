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
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue; // ent->d_name が "." または ".." だったら、中断して次のentへ
        printf("%s\n", ent->d_name);

        // この後の結果格納用変数を用意
        int len = strlen(path) + strlen(ent->d_name) + 2; // 文字列の長さ + (/ と \0 の2文字分)
        char new_path[len];
        snprintf(new_path, len, "%s/%s", path, ent->d_name); // 文字列の連結 path/d_name という文字列を作って、new_path変数に入れる。path//d_name みたいになることがあるが、問題なさそう。
        
        struct stat st;
        if (lstat(new_path, &st) < 0) {
            // perror(new_path);
            continue; // lstat に失敗したら中断して次のentへ（これはexitしてしまっても良い）
        }
        if (!S_ISDIR(st.st_mode)) { // ディレクトリじゃなかったら、中断して次のentへ
            // printf("%s is not a directory.\n", new_path);
            continue;
        } 
        do_ls(new_path); // ここまできているということは、"." でも ".." でもないディレクトリなので、引数を new_path に変えて do_ls を再帰呼び出し
    }
    closedir(d); 
}