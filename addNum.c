#include <stdio.h>
int main(void){
    int x, y; 
    if (scanf("%d %d", &x, &y) == 2) {
        printf("sum=%d\n", x+y);
    } else {
        fprintf(stderr, "bad input\n");
    }
    return 0;
}
