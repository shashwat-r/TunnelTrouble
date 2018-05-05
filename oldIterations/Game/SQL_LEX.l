%{
#include "y.tab.h"
%}

%%
"select" | "from" | "where" | "group" | "order" | "by" | "having" | "like" | "in" | "update" | "delete" | "set" |
"insert" | "into" | "values" | "between" | "distinct" | "join" | "inner" | "outer" | "left" | "right" | "full" |
"count" | "sum" | "avg" | "min" | "max" | "union" | "primary" | "key" | "foreign" | "unique" | "not" | "null" |
"alter" | "drop" | "create" | "alter" | "table" {}






		{return IF;}
[a-zA-Z]+			   	{return ID;}
[0-9]+                 	{return NUM;}
[ \t\n]             	;
"=="	|
">="	|
[\>]		|
"<="	|
[\<]						{return COMP;}
[-\+=]           	  	{return SYM;}
.                    	{return yytext[0];}
%%
int yywrap (void) {return 1;}
