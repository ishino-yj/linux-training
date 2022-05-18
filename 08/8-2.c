#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_slice(regex_t *pat, FILE *f);

static struct option longopts[] = {
    {"ignore-case", no_argument, NULL, 'i'},
    {"invert-match", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

// -i, -vに対応するフラグを用意
int ignore_case = 0;
int invert_match = 0;

int main(int argc, char *argv[]) {
    int opt;
    
    while ((opt = getopt_long(argc, argv, "iv", longopts, NULL)) != -1) {
        switch (opt) {
            case 'i':
                ignore_case = 1;
                break;
            case 'v':
                invert_match = 1;
                break;
            case 'h':  
                fprintf(stdout, "Usage: %s [-iv] [FILE ...]\n", argv[0]);
                exit(0);
            case '?': 
                fprintf(stderr, "Usage: %s [-iv] [FILE ...]\n", argv[0]);
                exit(1);
        }
    }


    if (optind == argc) {
        fputs("no pattern\n", stderr);
        exit(1);
    } 

    regex_t pat;
    int err;
    int reg_flags = REG_EXTENDED | REG_NEWLINE;
    if (ignore_case) reg_flags = reg_flags | REG_ICASE;
    err = regcomp(&pat, argv[optind], reg_flags);
    if (err != 0) {
        char buf[1024];
        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (optind+1 == argc) { 
        do_slice(&pat, stdin);
    } else {
        for (int i=optind+1; i < argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_slice(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_slice(regex_t *pat, FILE *src) {
    char buf[4096];
    size_t nmatch = 1; // pmatchの個数なんだけど、増やすと何が嬉しいのかよく分かっていない。一行あたりに何箇所マッチをとるかとか？？？ とりあえ1でそれっぽく動く。
    regmatch_t pmatch[nmatch]; // pmatch構造体の配列。マッチした部分の最初と最後のオフセット（文字列内のインデクス）などがある
    while (fgets(buf, sizeof buf, src)) {
        if ((!invert_match && regexec(pat, buf, nmatch, pmatch, 0) == 0) || (invert_match && regexec(pat, buf, nmatch, pmatch, 0) != 0)) {
            // buf に入っている文字列を、マッチした箇所の先頭（pmatch[0].rm_so）から末尾（pmatch[0].rm_eo）まで出力する。
            // rm は多分 regex_match の略。so は start_offset、eo は end_offset っぽい。
            for (int i=0; i<sizeof buf; i++) {
                if (pmatch[0].rm_so <= i && i < pmatch[0].rm_eo) { 
                    if (putchar(buf[i]) < 0) exit(1);
                } else if (i == pmatch[0].rm_eo) { // マッチ箇所ごとに改行があると見やすいので改行する。
                    if (putchar('\n') < 0) exit(1);
                }
            }
        }
    }
}