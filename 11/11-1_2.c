/*
    tail2 -- simple tail command with no line length limitation
*/

#include <stdio.h>
#include <stdlib.h>

static void tail(FILE *f, int nlines);
static unsigned char *readline(FILE *f);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }
    tail(stdin, atoi(argv[1]));
    exit(0);
}

static void tail(FILE *f, int nlines) {
    unsigned char **ringbuf; // リングバッファ。新しいものが来たら一番古いものを削除するようなバッファ。
    unsigned char **p;
    unsigned char *line;
    int n;

// Incremental. ポインタを一つ進める。
// ただし、ringbufの長さ（nlines）を超えてしまった場合は先頭に戻す
#define INC(ptrvar) do { \
    ptrvar++;                       \
    if (ptrvar >= ringbuf + nlines) \
        ptrvar = ringbuf;           \
} while (0) // while(0) なので、一回しか行われない

    if (nlines == 0) return;

    // nlines * (char型へのポインタ)サイズ バイト分のメモリをヒープ領域に割り当てて、その先頭へのポインタを返す
    // callocなので領域はゼロクリアされている
    ringbuf = calloc(nlines, sizeof(char*));
    if (ringbuf == NULL) exit(1);
    p = ringbuf;

    /* ringbufに入れながら一行ずつ読んでいく */
    while (line = readline(f)) {
        if (*p) free(*p); // pが指す領域に何か書いてあったら、解放する。
        *p = line; // pが指す領域にlineを書き込む
        INC(p); // ringbufの参照を1つ進める
    }
    if (*p == NULL) p = ringbuf;

    /* ringbufに入っている内容を書き出していく */
    for (n = nlines; n > 0 && *p; n--) {
        printf("%s", *p);
        free(*p);
        INC(p);
    }
    free(ringbuf);
}

/* mallocとポインタ使ってカッコよく書いてるけど、要はreadlineしている。 */
static unsigned char * readline(FILE *f) {
    unsigned char *buf, *p;
    size_t buflen = BUFSIZ;
    int c;

    buf = p = malloc(sizeof(char) * buflen);
    if (buf == NULL) exit(1);
    for (;;) {
        c = getc(f);
        if (c == EOF) {
            if (buf == p) {
                free(buf);
                return NULL;
            }
            break;
        }
        *p++ = c;
        if (p >= buf + buflen - 1) {
            buflen *= 2;
            buf = realloc(buf, buflen);
            if (buf == NULL) exit(1);
        }
        if (c == '\n') break;
    }
    *p++ = '\0';
    return buf;
}