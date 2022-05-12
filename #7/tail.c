#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_tail(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

/* 今回の課題ではコマンドはなくてもいいが、いずれ必要になるかもしれないので、枠組みは用意しておく */
static struct option longopts[] = {
    // {"lines", required_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

int main (int argc, char *argv[]) {
    int opt;
    long nlines = DEFAULT_N_LINES;
    
    while ((opt = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (opt) {
            case 'h':  
                fprintf(stdout, "Usage: %s [FILE ...]\n", argv[0]);
                exit(0);
            case '?': 
                fprintf(stderr, "Usage: %s [FILE ...]\n", argv[0]);
                exit(1);
        }
    }

    if (optind == argc) {
        do_tail(stdin, nlines);
    } else {
        int i;
        for (i=optind; i<argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_tail(f, nlines);
            fclose(f);
        }
    }
    exit(0);
}

static void do_tail(FILE *f, long nlines) {
    int c;
    if (nlines <= 0) return; // 今回は nlines=DEFAULT_N_LINES で固定されているから必要ないが、いつか行数を指定できるようにするかもしれないので一応書いておく。
    
    // まず、ファイルの行数を数える（wc -l）
    int cnt = 0;
    while ((c = getc(f)) != EOF) {
        if (c == '\n') {
            cnt++;
        }
    }

    // 次に、ファイルオフセットの位置をファイル先頭に戻す
    // これでまた最初から読める
    rewind(f);

    // 取得した「ファイル全体の行数」をもとに、後ろからnlines行を書き出す。
    while ((c = getc(f)) != EOF) {
        if (cnt < nlines) { // cnt はカウントダウンされているので、残りの行数を表す。残りの行数がnlinesより小さいならputcharする。
            if (putchar(c) < 0) exit(1);
        }
        if (c == '\n') { // 今度は改行を見つけるたびにカウントダウンしていく。
            cnt--;
        }
    }
    
}