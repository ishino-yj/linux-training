#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_grep(regex_t *pat, FILE *f);

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
    int reg_flags = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    if (ignore_case) reg_flags = reg_flags | REG_ICASE; // -i が指定された時は REG_ICASE を指定して、大文字と小文字を区別しない
    err = regcomp(&pat, argv[optind], reg_flags);
    if (err != 0) {
        char buf[1024];
        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (optind+1 == argc) {
        do_grep(&pat, stdin);
    } else {
        for (int i=optind+1; i < argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src) {
    char buf[4096];
    while (fgets(buf, sizeof buf, src)) {
        // (-vが指定されていない かつ regexecに成功した場合) または （-vが指定されている かつ regexecが失敗した場合）に出力
        if ((!invert_match && regexec(pat, buf, 0, NULL, 0) == 0) || (invert_match && regexec(pat, buf, 0, NULL, 0) != 0)) {
            fputs(buf, stdout);
        }
    }
}