#include <stdio.h>
#include <stdlib.h>


static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        do_cat(NULL);
    }
    for (i=1; i<argc; i++) {
        do_cat(argv[i]);
    }
    exit(0);
}


#define BUFFER_SIZE 2048

static void do_cat(const char *path) {
    FILE *f;
    unsigned char buf[BUFFER_SIZE];
    size_t n;

    if (path==NULL) {
        f = stdin;
    } else {
        f = fopen(path, "r");
    }
    if (!f) die(path);
    for (;;) {
        n = fread(buf, sizeof (unsigned char), sizeof buf, f);
        if (n<0) die(path);
        if (n==0) break;
        if (fwrite(buf, sizeof (unsigned char), n, stdout) < n) die(path);
    }
    fclose(f);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
