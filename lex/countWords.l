%{
    int capital=0,  words=1;
%}

%%
[A-Z] {
    capital++;
}
[ \t]+ {
    words++;
}
%%
int yywrap()  
{  
    return 1;  
}  
int main(int argc, char*argv[]){
    yyin = fopen(argv[1], "r");
    yylex();
    printf("\nNumber of Words: %d,\nCapital Letters: %d",words, capital);
} 