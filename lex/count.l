%{
int nwords,nchars;
%}
%%
\n {
	nchars++;
	}
[^ \n\t]+ {nwords++, nchars=nchars+yyleng;}
. {nchars++;}
%%
int yywrap(void)  
{
return 1;
}
int main(int argc, char*argv[]) 
{
	yyin=fopen(argv[1],"r");
yylex();
printf("Chars=%d\nWords=%d\n",nchars,nwords);
return 0;
} 