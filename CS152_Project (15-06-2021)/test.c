#include<stdio.h>
#include<math.h>
int main(void)
{
//encryrption:

int num,num1,i,e=0,d=0;
int choice;
printf("enter 4 digit number:\n");
scanf("%d",&num);
num1=num;

printf( "enter (1) to encrypt (2) to decrypt\n");
scanf( "%d", &choice);

if ( choice == 1 )
{
//encryption:
for(i=3;i>=0;i--)
{
num1=(num)/(pow(10,i));
num1=(num1+7)%10;
e=(10*e)+num1;
}
printf("encryption = %d\n",e);
}
else
{

//decryption:
for(i=3;i>=0;i--)
{
num1=e/pow(10,i);
num1=((num1-7)%10);
d=(10*d)+num1;
}
printf("decryption = %d",d);
}

return 0;
}