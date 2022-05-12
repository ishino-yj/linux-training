#include <stdio.h>
#include <stdlib.h>
/* ① */
#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

/* ② */
static struct option longopts[] = {
    {"lines", required_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

int main (int argc, char *argv[]) {
    int opt;
    long nlines = DEFAULT_N_LINES;
    
    /* ③ */
    while ((opt = getopt_long(argc, argv, "n", longopts, NULL)) != -1) {
        switch (opt) {
            case 'n':  /* ④ nlinesをセットする。headの処理自体はこのwhileを抜けた後で行う */
                nlines = atol(optarg);
                break;
            case 'h':  /* ⑤ ./head -h でヘルプ表示 */
                fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
                exit(0);
            case '?':  /* ⑥ 知らんコマンドが指定されたらヘルプ表示 */
                fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
                exit(1);
        }
    }
    /* ⑦ */
    if (optind == argc) {
        do_head(stdin, nlines);
    } else {
        int i;

        /* ⑧ whileを抜けた時、optindは指定された最初のファイル名を指しているので、そこからループするとファイル名を順番に見ていける。 */
        for (i=optind; i<argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_head(f, nlines);
            fclose(f);
        }
    }
    exit(0);
}

static void do_head(FILE *f, long nlines) {
    int c;
    if (nlines <= 0) return;
    while ((c = getc(f)) != EOF) {
        if (putchar(c) < 0) exit(1);
        if (c == '\n') {
            nlines--;
            if (nlines == 0) return;
        }
    }
    
}