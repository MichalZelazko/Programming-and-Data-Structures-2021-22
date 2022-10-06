#include <stdio.h>

void inc_ptr(int *value){
    *value += 1;
}

int inc(int value){
    int ret;
    if (value > 5)
        ret = value + 10;
    else
        ret = value + 1;
    ret++;
    return ret;
}

int main()
{
    int num = 1;
    inc_ptr(&num);
    printf("%d \r\n", num);
    num = inc(num);
    printf("%d \r\n", num);
    printf("%d \r\n", inc(num));

    int *ptr = &num;
    inc_ptr(ptr);
    printf("%d \r\n", *ptr);

    ptr = 0;
    inc_ptr(ptr);
    printf("%d \r\n", *ptr);

    return 0;
}
