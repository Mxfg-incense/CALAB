#include <stdio.h>

int main(void) {
int (*x)[6];                 
/*pointer to an array of integers*/
int y[6] = {1,2,3,4,5,6};   
int *z;                      
int i;

z = y;
for(i = 0;i<6;i++)
    printf("%d ",z[i]);

x = &y;
printf("%p\n", (void *)y);

for(i = 0;i<6;i++)
    printf("%p \n",(void *)x[i]);
return 0;
}