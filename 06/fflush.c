#include<stdio.h>
#include<unistd.h>  // sleep()関数のためのライブラリ。

int main(void)
{
    for (int i=0; i<3; i++) {
        printf("%d", i);
        // fflush(stdout);
        sleep(1); 
    }
    printf("\n");
    return 0;
}