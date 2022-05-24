#include <stdio.h>

int countA() {
    static int cnt = 0; // 初期化したstatic変数
    cnt++;
    return cnt;
}

int countB() {
    int cnt = 0; // 初期化したローカル変数
    cnt++;
    return cnt;
}

int main(void) {
    int cntA = 0, cntB = 0;
    for (int i=0; i<3; i++) {
        cntA = countA();
        cntB = countB();
        printf("A: %d, B: %d\n", cntA, cntB);
    }
    return 0;
}

