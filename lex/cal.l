%{ 
int op = 0;
int i=0,ob=0,cb=0,clb=0,crb=0;
float a, b; 
%} 

dig [0-9]+|([0-9]*)"."([0-9]+) 
add "+"
sub "-"
mul "*"
div "/"
pow "^"
ln \n 
%% 

{dig} {digi();} 
{add} {op=1;} 
{sub} {op=2;} 
{mul} {op=3;} 
{div} {op=4;} 
{pow} {op=5;} 
[(] {ob++;}
[)] {cb++;}
[{] {clb++;}
[}] {crb++;}
{ln} {if(op==0 || ob-cb!=0 || clb-crb!=0) printf("The Answer :%f\n\n",a);
else printf("Invalid expression\n");} 

%% 
digi() 
{ 
if(op==0|| ob-cb!=0 || clb-crb!=0) {
a = atof(yytext);
}
else
{ 
b=atof(yytext); 

switch(op) 
{ 
case 1:a=a+b; 
	break; 

case 2:a=a-b; 
break; 

case 3:a=a*b; 
break; 

case 4:a=a/b; 
break; 

case 5:for(i=a;b>1;b--) 
a=a*i; 
break; 
} 
op=0; 
} 
} 

int yywrap() 
{ 
return 1; 
}

int main() 
{ 
yylex(); 

return 0;
} 