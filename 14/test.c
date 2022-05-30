#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

#define INIT_BUFSIZE 1024

   /* ポインタ型関数my_getcwdを定義 */
char *
my_getcwd(void)
{ 
    char *buf, *tmp;
    size_t size =  
    INIT_BUFSIZE;

    buf = malloc(size);
    if (!buf)return  NULL;
    
}

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