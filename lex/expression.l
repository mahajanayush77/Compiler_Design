%{
#include<stdio.h>
int a=0,s=0,m=0,d=0,mo=0,ob=0,cb=0,clb=0,crb=0;
int flaga=0, flags=0, flagm=0, flagd=0, flagmo=0;
%}
%%
[+] {a++;flaga=1;}
[-] {s++;flags=1;}
[*] {m++;flagm=1;}
[/] {d++;flagd=1;}
[%] {mo++; flagmo=1;}
[(] {ob++;}
[)] {cb++;}
[{] {clb++;}
[}] {crb++;}
%%
int yywrap()  
{  
    return 1;  
}  
int main()
{
printf("Enter the expression\n");
yylex();
if(ob-cb==0||clb-crb==0)
{
printf("Valid expression\n");
}
else
{
printf("Invalid expression");
}
printf("\nAdd=%d\nSub=%d\nMul=%d\nDiv=%d\nMod=%d\n",a,s,m,d,mo);
printf("Operators are: \n");
if(flaga)
printf("+\n");
if(flags)
printf("-\n");
if(flagm)
printf("*\n");
if(flagd)
printf("/\n");
if(flagmo)
printf("/\n");
return 0;
}
