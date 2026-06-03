#include <stdio.h>
int main() 
{
    char content[100];
    printf("Hello World!"); 
    printf("请输入任意内容：");
    scanf("%s", content);
    printf("你输入的内容是：%s\n", content);
    return 0;
}
