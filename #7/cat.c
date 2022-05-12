#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_cat(FILE *f, int flag); // プロトタイプ宣言の修正

static struct option longopts[] = {  // longoptsを変更。可視化するということでvisualize。引数は取らないので no_argument
    {"visualize", no_argument, NULL, 'v'}, 
    {"help", no_argument, NULL, 'h'}, 
    {0, 0, 0, 0}
};

int main (int argc, char *argv[]) {
    int opt;
    int VIS_FLAG = 0; // フラグを用意
    
    while ((opt = getopt_long(argc, argv, "v", longopts, NULL)) != -1) { // n: を v に変更
        switch (opt) {
            case 'v': // nをvに変更。処理もnlinesの設定からVIS_FLAGの設置に変更
                VIS_FLAG = 1;
                break;
            case 'h':
                fprintf(stdout, "Usage: %s [-v] [FILE ...]\n", argv[0]); // helpの文面修正
                exit(0);
            case '?': 
                fprintf(stderr, "Usage: %s [-v] [FILE ...]\n", argv[0]); // helpの文面修正
                exit(1);
        }
    }

    if (optind == argc) {
        do_cat(stdin, VIS_FLAG); // do_headをdo_catに変更
    } else {
        int i;
        for (i=optind; i<argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_cat(f, VIS_FLAG); // do_headをdo_catに変更
            fclose(f);
        }
    }
    exit(0);
}

static void do_cat(FILE *f, int flag) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (flag && c == '\t') {  // フラグが1、かつ c が対象の文字の時、決められた処理を行う
            if (putchar('\\') < 0) exit(1);
            if (putchar('t') < 0) exit(1);
            continue;
        }
        if (flag && c == '\n') {
            if (putchar('$') < 0) exit(1);
            if (putchar('\n') < 0) exit(1);
            continue;
        }
        if (putchar(c) < 0) exit(1);
    }
}