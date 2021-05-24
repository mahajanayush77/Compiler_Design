%{ 
#include <stdio.h> 
%} 

DIGIT [0-9]

%% 
^[a-zA-Z_]+[a-zA-Z0-9_]{0,30} printf("Valid Identifier"); 
[+-]?{DIGIT}({DIGIT})* printf("Constant");
^[^a - z A - Z _] printf("Invalid Identifier"); 

. ; 
%% 
int yywrap()  
{  
    return 1;  
}  
int main() 
{ 
    yylex(); 
    return 0;
} 