#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void putstring(const char *s);

int main(int argc, char *argv[]) {
    int i;

    for (i=1; i<argc; i++) {
        FILE *f;
        int c;
        f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            exit(1);
        }
        while ((c = fgetc(f)) != EOF) {
            if (c == 9) {
                putstring("\\t");
                continue;
            }
            if (c == 10) {
                putstring("$\n");
                continue;
            }
            if (putchar(c) < 0) exit(1);
        }
        fclose(f);
    }
    exit(0);
}

static void putstring(const char *s) {
    int i;
    for (i=0; i<strlen(s); i++) {
        if (putchar(s[i]) < 0) exit(1);
    }
}