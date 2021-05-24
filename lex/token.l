%{

%}
DIGIT		[-,+]?[0-9]+
DELIM		[\t\n]
WS          [!@#$%^&*()]+
ID          [a-zA-Z]+

%%
"while"|"if"|"else" {printf("\t keywords \n");}   
"int"|"float" {printf("\t keywords \n");}
{WS}	{ printf("\n special chracters\n"); }
{ID}	{ printf("\n Identifiers\n"); }
{DIGIT}	{ printf("\n Integers\n"); }
[,.'"]+ printf("Punctuation Chars");
"<="|"=="|"="|"++"|"-"|"*"|"+" { printf("\t operator\n");}

.  ;
%%
int yywrap(){
    return 0;
}
int main(){yylex();
}